#define DEBU
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _LIST_
#define _LIST_
#include "Allocator.hpp"
#include "memory"

template <class T>
struct _list_node
{
	typedef _list_node<T> self;
	typedef self* node_ptr;
	_list_node(const self &x)
		:data(x.data), prev(x.prev), next(x.next)
	{
	
	}

	T data;
	node_ptr prev;
	node_ptr next;
};

template<class T, class Ref, class Ptr>
struct list_iterator
{
	typedef list_iterator<T, T&, T*> iterator;
	typedef list_iterator<T, Ref, Ptr> self;

	typedef std::bidirectional_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef _list_node<T>* list_node_ptr;

	list_node_ptr node;

	// construct
	list_iterator(const list_node_ptr p = nullptr)
		:node(p)
	{
	
	}

	list_iterator(const iterator &i)
		:node(i.node)
	{

	}

	bool operator== (const self &i) const
	{
		return node == i.node;
	}

	bool operator!=(const self &i)
	{
		return node != i.node;
	}

	reference operator*() const 
	{
		return (*node).data;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	self& operator++()
	{
		node = node->next;
		return *this;
	}

	self& operator++(int)
	{
		self tmp = *this;
		node = node->next;
		return tmp;
	}

	self& operator--()
	{
		node = node->prev;
		reutrn *this;
	}

	self& operator--(int)
	{
		self tmp = *this;
		node = node->prev;
		return tmp;
	}
};

template<class T, class Alloc = alloc>
class list
{
protected :
	typedef list<T, Alloc> self;
	typedef _list_node<T> list_node;
	typedef typename simple_alloc<list_node,alloc> list_node_alloc;

	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

public:

	typedef list_node* list_node_ptr;

	typedef list_iterator<T, T&, T*> iterator;
	typedef const list_iterator<T, const T&, const T*> const_iterator;

	list()
	{
		empty_init();
	}

	list(const self &x)
		:node(x.node)
	{

	}

	list(const self &&x)
		:node(x.node)
	{
		x.node = nullptr;
	}

	iterator begin()
	{
		return node->next;
	}

	const_iterator begin() const 
	{
		return node->next; 
	}

	iterator end()
	{
		return node;
	}

	const_iterator end() const
	{
		return node;
	}

	bool empty() const
	{
		return node->next == node;
	}

	size_type size() const
	{
		size_type result = std::distance(begin(), end());
		return result;
	}

	size_type max_size() const
	{
		return size_type(-1) / sizeof(T)
	}

	reference front()
	{
		return *begin();
	}

	const_reference front() const
	{
		return *begin();
	}

	reference back()
	{
		return *(--end());
	}

	const_reference back() const
	{
		return *(--end());
	}

	iterator insert(iterator pos, const T &x)
	{
		list_node_ptr tmp = create_node(x);
		tmp->next = pos.node;
		tmp->prev = pos.node->prev;
		pos.node->prev->next = tmp;
		pos.node->prev = tmp;
		return tmp;
	}

	iterator erase(iterator pos)
	{
		list_node_ptr next_ptr = pos.node->next;
		list_node_ptr prev_ptr = pos.node->prev;
		prev_ptr->next = next_ptr;
		next_ptr->prev = prev_ptr;
		destroy_node(pos.node);
		return next_ptr;
	}

	void push_back(const T &x)
	{
		insert(end(), x);
	}

	void push_front(const T &x)
	{
		insert(begin(), x);
	}

	void pop_back()
	{
		iterator tmp = end();
		erase(--tmp);
	}

	void pop_front()
	{
		erase(begin());
	}

	void clear()
	{
		list_node_ptr cur = node->next;
		while (cur != node)
		{
			list_node_ptr tmp = cur;
			cur = cur->next;
			destroy_node(tmp);
		}
		node->next = node;
		node->prev = node;
	}

	void remove(const T &x)
	{
		iterator first = begin();
		iterator last = end();

		while (first != last)
		{
			iterator next = first;
			++next;
			if (*first == x)
			{
				erase(first);
			}
			first = next;
		}
	}

	void swap(const self &x)
	{
		std::swap(node, x.node);
	}

	void splice(iterator pos, self &x)
	{
		if (&x != this && x.empty() == false)
		{
			transfer(pos, x.begin(), x.end());
		}
	}

	void splice(iterator pos, self&, iterator i)
	{
		iterator j = i;
		++j;
		if (pos != j && pos != i)
		{
			transfer(pos, i, j);
		}
	}

	void splice(iterator pos, self&, iterator first, iterator last)
	{
		if (first != last)
		{
			transfer(pos, first, last);
		}
	}

	void marge(self &x)
	{
		iterator first1 = begin();
		iterator last1 = end();
		iterator first2 = x.begin();
		iterator last2 = x.end();

		while (first1 != last1 && first2 != last2)
		{
			if (*first2 < first1)
			{
				iterator next = first2;
				transfer(first1, first2, ++next);
				first2 = next;
			}
			else
			{
				++first1;
			}
		}
		if (first2 != last2)
		{
			transfer(last1, first2, last2);
		}
	}

protected:

	list_node_ptr get_node()
	{
		return list_node_alloc::allocate();
	}

	void put_node(list_node_ptr p)
	{
		list_node_alloc::deallocate(p); 
	}

	list_node_ptr create_node(const T &x)
	{
		list_node_ptr ptr = get_node();
		construct(&ptr->data, x);
		return ptr;
	}

	void destroy_node(list_node_ptr p)
	{
		destroy(&p->data);
		put_node(p);
	}

	void empty_init()
	{
		node = get_node();
		node->next = node;
		node->prev = node;
	}

	void transfer(iterator pos, iterator first, iterator last)
	{
		if (pos != last)
		{
			last.node->prev->next = pos.node;
			first.node->prev->next = last.node;
			pos.node->prev->next = first.node;
			list_node_ptr = pos.node->prev;
			pos.node->prev = last.node->prev;
			last.node->prev = first.node->prev;
			first.node->prev = tmp;
		}
	}

protected:

	list_node_ptr node;
};


struct s_list_base
{
	s_list_base *next;
};

template<class T>
struct s_list_node : public s_list_base
{
	T data;

	s_list_node(const T &x = T())
		:data(x)
	{

	}
};

inline s_list_base* insert_node(s_list_base *prev, s_list_base *news)
{
	news->next = prev->next;
	prev->next = news;
	return news;
}

inline std::size_t list_size(s_list_base *node)
{
	std::size_t result = 0;
	for (; node != nullptr; node = node->next)
	{
		++result;
	}
	return result;
}

struct s_list_iterator_base
{
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef std::forward_iterator_tag iterator_category;

	s_list_base *node;
	s_list_iterator_base(s_list_base *x)
		:node(x)
	{

	}

	void incr()
	{
		node = node->next;
	}

	bool operator==(const s_list_iterator_base &x) const
	{
		return node == x.node;
	}

	bool operator!=(const s_list_iterator_base &x) const
	{
		return node != x.node;
	}

};

template<class T, class Ref, class Ptr>
struct s_list_iterator : public s_list_iterator_base
{
	typedef s_list_iterator<T, T&, T*> iterator;
	typedef s_list_iterator<T, const T &, const T *> const_iterator;
	typedef s_list_iterator<T, Ref, Ptr> self;

	typedef T value_type;
	typedef Ref reference;
	typedef Ptr pointer;
	typedef s_list_node<T> list_node;

	s_list_iterator(list_node *x = nullptr)
		:s_list_iterator_base(x)
	{

	}

	s_list_iterator(const s_list_iterator &x)
		:s_list_iterator_base(x.node)
	{

	}

	reference operator*() const
	{
		return ((list_node*)node)->data;
	}

	pointer operator->() const
	{
		return &(operator*());
	}

	self& operator++()
	{
		incr();
		return *this;
	}

	self& operator++(int)
	{
		self tmp = *this;
		incr();
		return tmp;
	}

};

template<class T, class Alloc = alloc>
class sList
{

public:

	typedef T value_type;
	typedef T& reference;
	typedef T* pointer;
	typedef const T& const_reference;
	typedef const T* const_pointer;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef s_list_iterator<T, reference, pointer> iterator;
	typedef s_list_iterator<T, const_reference, const_pointer> const_iterator;

private:
	typedef s_list_node<T> slist_node;
	typedef s_list_base list_node_base;
	typedef s_list_iterator_base iterator_base;
	typedef simple_alloc<slist_node, Alloc> node_alloc;

	slist_node _head;

	static slist_node* create_node(const T &x)
	{
		slist_node *node = node_alloc::allocate();
		construct(node, x);
		node->next = nullptr;
		return node;
	}

	static void destory_node(slist_node *node)
	{
		destroy(node);
		node_alloc::deallocate(node);
	}

public:

	sList()
	{
		_head.next = nullptr;
	}

	~sList()
	{
		clear();
	}

	iterator begin()
	{
		return iterator((slist_node*)_head.next);
	}

	iterator end()
	{
		return iterator(nullptr);
	}

	size_type size() const
	{
		return list_size(_head.next);
	}

	bool empty() const
	{
		return _head.next == nullptr;
	}

	void swap(sList<T,Alloc> &l)
	{
		std::swap(_head.next, l._head.next);
	}

	reference front()
	{
		return ((slist_node*)_head.next)->data;
	}

	void push_front(const T &x)
	{
		insert_node(&_head, create_node(x));
	}

	void pop_front()
	{
		slist_node* node = (slist_node*)_head.next;
		_head.next = node->next;
		destory_node(node);
	}

	void insert(iterator pos, const T &x)
	{
		insert_node(pos.node, create_node(x));
	}

	void erase_after(iterator pos)
	{
		slist_node *ptr = pos.node->next;
		pos.node->next = ptr->next;
		destory_node(ptr);
	}

	void clear()
	{
		slist_node *ptr = (slist_node*)_head.next;;
		while (ptr != nullptr)
		{
			slist_node *tmp = ptr;
			ptr = (slist_node*)ptr->next;
			destory_node(tmp);
		}
		_head.next = nullptr;
	}

};

#endif
