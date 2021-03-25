#pragma once
#include <vector>
#include <list>
#include <memory>

template <typename T>
class collection
{
public:
	class iterator
	{
		friend class  collection<T>;
	public:
		virtual T& operator*() = 0;
		virtual const T& operator*() const = 0;
		virtual T* operator->() = 0;
		virtual iterator& operator++() = 0;
		virtual bool operator==(const iterator& rhs) const = 0;
		virtual bool operator!=(const iterator& rhs) const = 0;
	};;
	virtual iterator& insert(iterator& pos, const T& value) = 0;
	virtual iterator& erase(iterator& pos) = 0;
	virtual void clear() = 0;
	virtual size_t size() = 0;
	virtual bool empty() = 0;
	virtual std::unique_ptr<iterator> begin() = 0;
	virtual std::unique_ptr<iterator> end() = 0;
	virtual ~collection() = default;
};

template <typename T>
class vector : public collection<T> 
{
	std::vector<T> v;

public:
	class vector_iterator : public  collection<T>::iterator
	{
		using raw_iterator = typename std::vector<T>::iterator;
		raw_iterator  it;
		vector_iterator(raw_iterator it) : it(it) {}

		friend class vector<T>;
	public:
		T& operator*()
		{
			return *it;
		}
		const T& operator*() const
		{
			return *it;
		}
		vector_iterator& operator++()
		{
			++it;
			return *this;
		}

		T* operator->()
		{
			return &*it;
		}
		bool operator==(const iterator& rhs) const
		{
			auto vector_it = dynamic_cast<vector_iterator&>(const_cast<iterator&>(rhs));
			return it == vector_it.it;
		}
		bool operator!=(const iterator& rhs) const
		{
			auto vector_it = dynamic_cast<vector_iterator&>(const_cast<iterator&>(rhs));
			return it != vector_it.it;
		}
	};
	vector() : v(std::vector<T>()) {}

	iterator& insert(iterator& pos, const T& value)
	{
		auto vector_it = dynamic_cast<vector_iterator&>(pos);
		return vector_iterator(v.insert(vector_it.it, value));
	}

	iterator& erase(iterator& pos)
	{
		auto vector_it = dynamic_cast<vector_iterator&>(pos);
		return vector_iterator(v.erase(vector_it.it));
	}

	void clear()
	{
		v.clear();
	}

	size_t size()
	{
		return v.size();
	}

	bool empty()
	{
		return v.empty();
	}

	std::unique_ptr<iterator> begin()
	{
		return std::unique_ptr<vector_iterator>(new vector_iterator(v.begin()));
	}

	std::unique_ptr<iterator> end()
	{
		return std::unique_ptr<vector_iterator>(new vector_iterator(v.end()));
	}

};

template <typename T>
class list : public collection<T>
{
	std::list<T> l;
public:
	class list_iterator : public  collection<T>::iterator
	{
		using raw_iterator = typename std::list<T>::iterator;
		raw_iterator  it;
		list_iterator(raw_iterator it) : it(it) {}

		friend class list<T>;
	public:
		T& operator*()
		{
			return *it;
		}
		const T& operator*() const
		{
			return *it;
		}
		list_iterator& operator++()
		{
			++it;
			return *this;
		}

		T* operator->()
		{
			return &*it;
		}
		bool operator==(const iterator& rhs) const
		{
			auto list_it = dynamic_cast<list_iterator&>(const_cast<iterator&>(rhs));
			return it == list_it.it;
		}
		bool operator!=(const iterator& rhs) const
		{
			auto list_it = dynamic_cast<list_iterator&>(const_cast<iterator&>(rhs));
			return it != list_it.it;
		}
	};

	list() : l(std::list<T>()) {}

	iterator& insert(iterator& pos, const T& value)
	{
		auto list_it = dynamic_cast<list_iterator&>(pos);
		return list_iterator(l.insert(list_it.it, value));
	}

	iterator& erase(iterator& pos)
	{
		auto list_it = dynamic_cast<list_iterator&>(pos);
		return list_iterator(l.erase(list_it.it));
	}

	void clear()
	{
		l.clear();
	}

	size_t size()
	{
		return l.size();
	}

	bool empty()
	{
		return l.empty();
	}

	std::unique_ptr<iterator> begin()
	{
		return std::unique_ptr<list_iterator>(new list_iterator(l.begin()));
	}

	std::unique_ptr<iterator> end()
	{
		return std::unique_ptr<list_iterator>(new list_iterator(l.end()));
	}
};