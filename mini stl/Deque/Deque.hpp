#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif

#ifndef _DEQUE_
#define _DEQUE_
#include "Allocator.hpp"

template <class T>
 T max(T a, T b)
{
	return a < b ? b : a;
}

inline std::size_t deque_buff_size(std::size_t n)
{
	return n < 512 ? std::size_t(512 / n) : std::size_t(1);
}

template<class T, class Ref, class Ptr>
struct deque_iterator
{
	typedef deque_iterator<T, T&, T*> iterator;
	typedef deque_iterator<T, const T&, const T*> const_iterator;
	typedef iterator self;

	static std::size_t buffer_size()
	{
		return deque_buff_size(sizeof(T));
	}

	typedef std::random_access_iterator_tag iterator_category;

	typedef T value_type;
	typedef Ptr pointer;
	typedef Ref reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T** map_pointer;

	pointer cur;
	pointer first;
	pointer last;
	map_pointer node;

	void set_node(map_pointer new_node)
	{
		node = new_node;
		first = *new_node;
		last = first + buffer_size();
	}

	deque_iterator():cur(nullptr),first(nullptr),last(nullptr),node(nullptr)
	{
	
	}
	
	deque_iterator(pointer p, map_pointer m)
		:cur(p), first(*m), last(*m + buffer_size()), node(m)
	{

	}
	deque_iterator(const iterator &x)
		:cur(x.cur), first(x.first), last(x.last), node(x.node)
	{

	}

	reference operator*() const
	{
		return *cur;
	}

	pointer operator->() const
	{
		return &(operator*());
	}
	
	difference_type operator- (const iterator &x) const
	{
		return difference_type((node - x.node - 1) * buffer_size() + (cur - first) + (x.last - x.cur));
	}

	self& operator++()
	{
		++cur;
		if (cur == last)
		{
			set_node(node + 1);
			cur = first;
		}
		return *this;
	}

	self& operator++(int)
	{
		self tmp = *this;
		++*this;
		return tmp;
	}

	self& operator--()
	{
		if (cur == first)
		{
			set_node(node - 1);
			cur = last;
		}
		--cur;
		return *this;
	}

	self& operator--(int)
	{
		self tmp = *this;
		--*this;
		return tmp;
	}

	self& operator+= (difference_type n)
	{
		difference_type offset = n + (cur - first);
		if (offset >= 0 && offset < (difference_type)buffer_size())
		{
			cur += n;
		}
		else
		{
			difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
				: -difference_type((-offset - 1) / buffer_size()) - 1;
			set_node(node + node_offset);
			cur = first + (offset - node_offset * buffer_size());
		}
		return *this;
	}

	self& operator-= (difference_type n)
	{
		return *this += -n;
	}

	self operator+ (difference_type n) const
	{
		self tmp = *this;
		return tmp += n;
	}

	self operator- (difference_type n)const
	{
		self tmp = *this;
		reutrn tmp -= n;
	}

	reference operator[](difference_type n) const
	{
		return *(*this + n);
	}

	bool operator == (const self &x) const
	{
		return cur == x.cur;
	}

	bool operator!= (const self &x) const
	{
		return cur != x.cur;
	}

	bool operator< (const self &x) const
	{
		return node == x.node ? cur < x.cur : node < x.node;
	}

	bool operator>(const self &x) const
	{
		return x < *this;
	}

	bool operator<= (const self &x) const
	{
		return !(x < *this);
	}

	bool operator>= (const self &x) const
	{
		return !(*this < x);
	}

};

template<class T, class Ref, class Ptr>
inline deque_iterator<T, Ref, Ptr> operator+(ptrdiff_t n, const deque_iterator<T, Ref, Ptr> &x)
{
	return x + n;
}

template<class T, class Alloc = alloc>
class Deque_alloc_base
{
public:

	typedef T** map_pointer;
	typedef simple_alloc<T, Alloc> node_alloc;
	typedef simple_alloc<T*, Alloc> map_alloc;

	Deque_alloc_base()
		:_map(nullptr),_map_size(0)
	{
	
	}

	~Deque_alloc_base()
	{

	}

protected:

	std::size_t buff_size()
	{
		return deque_buff_size(sizeof(T));
	}

	T* allocate_node()
	{
		return node_alloc::allocate(buff_size());
	}

	void deallocate_node(T *p)
	{
		node_alloc::deallocate(p, buff_size());
	}
	
	map_pointer allocate_map(std::size_t n)
	{
		return map_alloc::allocate(n);
	}

	void deallocate_map(map_pointer p, std::size_t n)
	{
		map_alloc::deallocate(p, n);
	}

	map_pointer _map;
	std::size_t _map_size;
};

template<class T, class Alloc = alloc>
class Deque_base : public Deque_alloc_base<T,Alloc>
{
public:

	typedef Deque_alloc_base<T, Alloc> _Base;
	typedef typename _Base::map_pointer map_pointer;
	typedef deque_iterator<T, T&, T*> iterator;
	typedef deque_iterator<T, const T&, const T*> const_iterator;

	Deque_base()
		:start(), finish()
	{
		init_map(0);
	}

	Deque_base(std::size_t n)
		:start(), finish()
	{
		init_map(n);
	}

	~Deque_base()
	{
		if (_map != nullptr)
		{
			deallocate_map(_map, _map_size);
		}
	}

protected:

	enum : std::size_t
	{
		init_map_size = 8
	};

	void init_map(std::size_t n)
	{
		std::size_t nodes = n / buff_size() + 1;

		_map_size = max<std::size_t>(init_map_size, nodes + 2);
		_map = allocate_map(_map_size);

		map_pointer n_start = _map + (_map_size - nodes) / 2;
		map_pointer n_finish = n_start + nodes;

		create_nodes(n_start, n_finish);

		start.set_node(n_start);
		finish.set_node(n_finish - 1);
		start.cur = start.first;
		finish.cur = finish.first + n % buff_size();
	}

	void create_nodes(map_pointer first, map_pointer last)
	{
		 for (map_pointer ptr = first; ptr != last; ++ptr)
		{
			*ptr = allocate_node();
		}
	}

	void destroy_nodes(map_pointer first, map_pointer last)
	{
		for (map_pointer ptr = first; ptr != last; ++ptr)
		{
			deallocate_node(*ptr);
		}
	}

	iterator start;
	iterator finish;
};
template<class T, class Alloc = alloc>
class Deque : public Deque_base<T,Alloc>
{
public:
	typedef Deque_base<T,Alloc> _Base;
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;

	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

	typedef typename _Base::map_pointer map_pointer;
	typedef typename _Base::iterator iterator;
	typedef typename _Base::const_iterator const_iterator;
	
	iterator begin()
	{
		return start;
	}

	const_iterator cbegin() const
	{
		return start;
	}

	iterator end()
	{
		return finish;
	}

	const_iterator cend() const
	{
		return finish;
	}

	reference operator[](size_type n)
	{
		return start[difference_type(n)];
	}

	const_reference operator[](size_type n) const
	{
		return start[difference_type(n)];
	}

	size_type size() const
	{
		return finish - start;
	}

	reference front()
	{
		return *start;
	}

	const_reference front() const
	{
		return *start;
	}

	reference back()
	{
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}

	const_reference back() const
	{
		iterator tmp = finish;
		--tmp;
		return *tmp;
	}

	size_type max_size() const
	{
		return size_type(-1) / sizeof(T);
	}

	bool empty() const
	{
		return finish == start;
	}

	Deque()
	{

	}

	~Deque()
	{
		clear();
	}

	void push_back(const T &x)
	{
		if (finish.cur != finish.last - 1)
		{
			construct(finish.cur, x);
			++finish.cur;
		}
		else
		{
			push_back_aux(x);
		}
	}

	void push_front(const T &x)
	{
		if (start.cur != start.first)
		{
			construct(start.cur, x);
			--start.cur;
		}
		else
		{
			push_front_aux(x);
		}
	}

	void pop_back()
	{
		if (finish.cur != finish.first)
		{
			--finish.cur;
			destroy(finish.cur);
		}
		else
		{
			pop_back_aux();
		}
	}

	void pop_front()
	{
		if (start.cur != start.last - 1)
		{
			destroy(start.cur);
			++start.cur;
		}
		else
		{
			pop_front_aux();
		}
	}

	void clear()
	{
		for (map_pointer node = start.node; node != finish.node; ++node)
		{
			destroy(*node, *node + buff_size());
			deallocate_node(*node);

		}

		if (start.node != finish.node)
		{
			destroy(start.cur, start.last);
			destroy(finish.first, finish.cur);
			deallocate_node(finish.first);
		}
		else
		{
			destroy(start.cur, finish.cur);
		}

		finish = start;
	}

protected:

	void reallocate_map(size_type node_to_add, bool add_at_front)
	{
		size_type old_size = finish.node - start.node + 1;
		size_type new_size = old_size + node_to_add;

		map_pointer new_start = nullptr;
		if (_map_size > new_size * 2)
		{
			new_start = _map + (_map_size - new_size) / 2 + (add_at_front ? node_to_add : 0);
			if (new_start < _map)
			{
				std::copy(start.node, finish.node + 1, new_start);
			}
			else
			{
				std::copy_backward(start.node, finish.node + 1, new_start + old_size);
			}
		}
		else
		{
			size_type new_map_size = _map_size + max<size_type>(node_to_add, _map_size) + 2;
			map_pointer new_map = allocate_map(new_map_size);

			new_start = new_map + (new_map_size - new_size) / 2 + (add_at_front ? node_to_add : 0);

			std::copy(start.node, finish.node + 1, new_start);
			deallocate_map(_map, _map_size);

			_map = new_map;
			_map_size = new_map_size;
		}

		start.set_node(new_start);
		finish.set_node(new_start + old_size - 1);
	}

	void reserve_map_at_back(size_type nodes = 1)
	{
		auto i = _map_size - (finish.node - _map);
		if (nodes + 1 > _map_size - (finish.node - _map))
		{
			reallocate_map(nodes,false);
		}
	}

	void reserve_map_at_front(size_type nodes = 1)
	{
		if (nodes < start.node - map)
		{
			reallocate_map(nodes, true);
		}
	}

	void push_back_aux(const T &x)
	{
		value_type x_copy = x;
		reserve_map_at_back();

		*(finish.node + 1) = allocate_node();
		construct(finish.cur, x_copy);
		finish.set_node(finish.node + 1);
		finish.cur = finish.first;
	}

	void push_front_aux(const T &x)
	{
		value_type x_copy = x;
		reserve_map_at_front();

		*(start.node - 1) = allocate_node();
		start.set_node(start.node - 1);
		start.cur = start.last - 1;
		construct(start.cur, x_copy);
	}
	
	void pop_back_aux()
	{
		deallocate_node(finish.first);
		finish.set_node(finish.node - 1);
		finish.cur = finish.last - 1;
		destroy(finish.cur);
	}

	void pop_front_aux()
	{
		deallocate_node(start.first);
		start.set_node(start.node + 1);
		start.cur = start.first;
	}
	
};
#endif