#include <unordered_set>


template<typename TKey, typename THash = std::hash<TKey>, typename TEqualComparer = std::equal_to<TKey>>
class unordered_set {
	struct node {
		TKey key;
		node* next;

		node(const TKey& key, node* next) : key(key), next(next) { }
	};

	node** table;
	const size_t table_size;

public:
	class iterator {
		// TODO Поля
		node** table;
		const size_t table_size;
		node* _node;
		size_t table_index;
		// TODO Конструктор
		iterator(node** table,size_t table_size, node* _node, size_t table_index):table(table), table_size(table_size), _node(_node), table_index(table_index){}

	public:
		friend class unordered_set<TKey, THash, TEqualComparer>;
		iterator& operator++()
		{
			if (_node->next != nullptr)
				_node = _node->next;
			else 
			{
				++table_index;
				while (table_index < table_size && table[table_index] == nullptr)
					++table_index;
				if (table_index == table_size)
					_node = nullptr;
				else
					_node = table[table_index];
			}
			return *this;
		}

		const TKey& operator *()
		{
			return _node->key;
		}

		bool operator == (iterator rhs)
		{
			return _node == rhs._node;
		}

		bool operator != (iterator rhs)
		{
			return _node != rhs._node;
		}
	};

	iterator begin()
	{
		size_t table_index = 0;
		node* node = nullptr;
		while (table_index < table_size && table[table_index] == nullptr)
			++table_index;
		if (table_index == table_size)
			node = nullptr;
		else
			node = table[table_index];
		return iterator(table,table_size, node, table_index);
	}

	iterator end()
	{
		return iterator(table, table_size, nullptr, table_size);
	}

	unordered_set(size_t table_size = 20) : table(new node*[table_size]), table_size(table_size)
	{
		for (int i = 0; i < table_size; ++i)
			table[i] = nullptr;
	}

	unordered_set(const unordered_set<TKey, THash, TEqualComparer>& obj) : table(new node*[obj.table_size]), table_size(obj.table_size)
	{
		for (size_t i = 0; i < table_size; ++i)
		{
			if (obj.table[i] != nullptr)
			{
				table[i] = new node(obj.table[i]->key, nullptr);
				auto cur = obj.table[i];
				cur = cur->next;
				while (cur != nullptr)
				{
					table[i]->next = new node(cur->key, nullptr);
					cur = cur->next;
				}
			}
			else
				table[i] = nullptr;
		}
	}

	unordered_set<TKey, THash, TEqualComparer>& operator = (const unordered_set<TKey, THash, TEqualComparer>& obj)
	{
		auto tmp(obj);
		swap(tmp);
		return *this;
	}

	void swap(unordered_set<TKey, THash, TEqualComparer>& obj)
	{
		std::swap(table, obj.table);
		std::swap(table_size, obj.table_size);
	}

	iterator insert(TKey key)
	{
		const auto index = THash()(key)%table_size;
		if (table[index] == nullptr)
		{
			table[index] = new node(key, nullptr);
			return iterator(table, table_size, table[index], index);
		}
		auto cur = table[index]; //сравнение
		while (cur->next != nullptr)
			cur = cur->next;
		cur->next = new node(key, nullptr);
		return iterator(table, table_size, cur->next, index);
	}

	bool contains(const TKey& key) {
		const auto index = THash()(key) % table_size;
		if (table[index] == nullptr) {
			return false;
		}
		auto cur = table[index];
		while (cur != nullptr)
		{
			if (cur->key == key)
				return true;
			cur = cur->next;
		}
		return false;
	}

	void clear()
	{
		/*for (size_t i = 0; i < table_size; ++i)
		{
			if (table[i] == nullptr)
				continue;

		}*/
		delete[] table;
	}
	~unordered_set()
	{
		clear();
	}
};