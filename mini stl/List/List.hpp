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
		std::_Construct(&ptr->data, x);
		return ptr;
	}

	void destroy_node(list_node_ptr p)
	{
		std::_Destroy(&p->data);
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
#endif
