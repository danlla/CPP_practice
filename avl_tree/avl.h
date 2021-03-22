#include <algorithm>


template<typename TKey, typename TCompare = std::less<TKey>>
class set
{
	struct node
	{
		const TKey key;
		uint8_t height;
		node* left;
		node* right;
		node* parent;

		node(): key(TKey()), height(0), left(nullptr), right(nullptr), parent(nullptr){}

		node(const int key, node* parent) : key(key), height(1), left(nullptr), right(nullptr), parent(parent) {}

		node(const int key,uint8_t height, node* parent) : key(key), height(height), left(nullptr), right(nullptr), parent(parent) { }

		//node(const node&) = delete;

		node& operator=(const node&) = delete;
	};

	node* _root;
	size_t _size;
	node* _end;

	bool less(const TKey& lhs,const node* rhs)
	{
		if (rhs == _end)
			return true;
		return TCompare()(lhs, rhs->key);
	}

	bool less(const node* lhs, const TKey& rhs)
	{
		if (lhs == _end)
			return false;
		return TCompare()(lhs->key, rhs);
	}

	static void clear(node* const root)
	{
		if (root == nullptr)
		{
			return;
		}
		clear(root->left);
		clear(root->right);
		delete root;
	}

	static void debug(const node* const root, const size_t level = 0)
	{
		static const size_t OFFSET_SIZE = 2;
		const std::string offset(level * OFFSET_SIZE, ' ');
		if (root == nullptr)
		{
			printf("%sNULL\n", offset.c_str());
			return;
		}
		printf("%s%d\n", offset.c_str(), root->key);
		if (root->left || root->right)
		{
			debug(root->left, level + 1);
			debug(root->right, level + 1);
		}
	}

	static uint8_t height(node* p)
	{
		if (p == nullptr)
		{
			return 0;
		}
		return p->height;
	}

	static void update_height(node* const p)
	{
		p->height = std::max(height(p->left), height(p->right)) + 1;
	}

	static node* rotate_right(node* const p)
	{
		auto q = p->left;

		q->parent = p->parent;
		p->parent = q;
		if(q->right!=nullptr)
			q->right->parent = p;

		p->left = q->right;
		q->right = p;
		update_height(p);
		update_height(q);
		return q;
	}

	static node* rotate_left(node* const q)
	{
		auto p = q->right;

		
		p->parent = q->parent;
		q->parent = p;
		if(p->left!=nullptr)
			p->left->parent = q;

		q->right = p->left;
		p->left = q;
		update_height(q);
		update_height(p);
		return p;
	}

	static node* balance(node* p)
	{
		update_height(p);
		auto dh = height(p->left) - height(p->right);
		if (dh == 2)
		{
			// if h(s) > h(A)
			if (height(p->left->right) > height(p->left->left))
			{
				p->left = rotate_left(p->left);
			}
			p = rotate_right(p);
		}
		else if (dh == -2)
		{
			// !!!
			if (height(p->right->left) > height(p->right->right))
			{
				p->right = rotate_right(p->right);
			}
			p = rotate_left(p);
		}
		return p;
	}

	node* insert(node* p, const TKey& key,node** tmp, node* parent=nullptr)
	{
		if (p == nullptr)
		{
			*tmp = new node(key, parent);
			return *tmp;
		}
		if (less(key, p))
		{
			p->left = insert(p->left, key, tmp, p);
		}
		else if (less(p, key))
		{
			p->right = insert(p->right, key, tmp, p);
		}
		return balance(p);
	}

	bool contains(node* current, const TKey& key)
	{
		if (current == nullptr)
			return false;
		if (!(less(current, key) || less(key, current)))
			return true;
		if (less(key, current))
			return contains(current->left, key);
		else 
			return contains(current->right, key);
	}

public:
	class iterator
	{
		node* _node;
		iterator(node* _node) : _node(_node){}
	public:
		friend class set<TKey, TCompare>;
		iterator& operator++()
		{
			if (_node->right != nullptr)
			{
				_node = _node->right;
				while (_node->left != nullptr)
					_node = _node->left;
			}
			else
			{
				while (_node->parent->left != _node)
					_node = _node->parent;
				_node = _node->parent;
			}

			return *this;
		}

		iterator& operator--()
		{
			if (_node->left != nullptr)
			{
				_node = _node->left;
				while (_node->right != nullptr)
					_node = _node->right;
			}
			else
			{
				while (_node->parent->right != _node)
					_node = _node->parent;
				_node = _node->parent;
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
		auto tmp = _root;
		while (tmp->left != nullptr)
			tmp = tmp->left;
		return iterator(tmp);
	}

	iterator end()
	{
		return iterator(_end);
	}

	set() //: _root(nullptr), _size(0)
	{
		_root = new node(TKey(), nullptr);
		_end = _root;
		_size = 0;
	}

	set(set<TKey, TCompare>& obj) :_root(new node(TKey(), nullptr)), _size(0), _end(nullptr)
	{
		_end = _root;
		for (auto i : obj)
			insert(i);
	}

	set<TKey, TCompare>& operator=(const set<TKey, TCompare>& obj)
	{
		auto tmp(obj);
		swap(tmp);
		return *this;
	}

	void swap(set<TKey, TCompare>& obj)
	{
		std::swap(_root, obj._root);
		std::swap(_size, obj._size);
	}

	size_t size() const
	{
		return _size;
	}

	iterator& insert(const TKey& key)
	{
		node* tmp;
		_root = insert(_root, key, &tmp);
		++_size;
		return iterator(tmp);
	}

	bool contains(const TKey& key)
	{
		return contains(_root, key);
	}

	void clear()
	{
		clear(_root);
		_root = nullptr;
	}

	void debug()
	{
		debug(_root);
	}

	~set()
	{
		clear();
	}
};
