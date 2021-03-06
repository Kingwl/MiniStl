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
		return tmp -= n;
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
	typedef std::size_t size_type;
	typedef typename _Base::map_pointer map_pointer;
	typedef deque_iterator<T, T&, T*> iterator;
	typedef deque_iterator<T, const T&, const T*> const_iterator;

	Deque_base()
		:start(), finish()
	{
		init_map(0);
	}

	Deque_base(size_type n)
		:start(), finish()
	{
		fill_init(n);
	}

	Deque_base(size_type n, const T &x)
	{
		fill_init(n, x);
	}

	~Deque_base()
	{
		if (_map != nullptr)
		{
			deallocate_map(_map, _map_size);
		}
	}

protected:

	enum : size_type
	{
		init_map_size = 8
	};

	void init_map(size_type n)
	{
		size_type nodes = n / buff_size() + 1;

		_map_size = max<size_type>(init_map_size, nodes + 2);
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

	void fill_init(size_type n, const T &x = T())
	{
		create_map_and_nodes(n);
		for (map_pointer cur = start.node; cur != finish.node; ++cur)
		{
			std::fill(*cur, *cur + buff_size(), x);
		}
		std::fill(finish.first, finish.cur, x);;
	}

	void create_map_and_nodes(size_type nodes)
	{
		size_type nodes_size = nodes / buff_size() + 1;
		_map_size = max<size_type>(init_map_size, nodes_size + 2);
		_map = allocate_map(_map_size);

		map_pointer nstart = _map + (_map_size - nodes_size) / 2;
		map_pointer nfinish = nstart + nodes_size - 1;

		map_pointer cur = nullptr;

		for (cur = nstart; cur <= nfinish; ++cur)
		{
			*cur = allocate_node();
		}

		start.set_node(nstart);
		finish.set_node(nfinish);
		start.cur = start.first;
		finish.cur = finish.first + nodes % buff_size();

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

	typedef _Base::size_type size_type;
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

	Deque(size_type n, const T &x)
		:_Base(n,x)
	{

	}

	Deque(size_type n)
		:_Base(n)
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

	iterator erase(iterator pos)
	{
		if (pos.cur == start.cur)
		{
			pop_front();
			return start;
		}

		iterator next = pos;
		++next;
		difference_type index = pos - start;

		if (size_type(index) < size() / 2)
		{
			std::copy_backward(start, pos, next);
			pop_front();
		}
		else
		{
			std::copy(next, finish, pos);
			pop_back();
		}
		return start + index;
	}

	iterator erase(iterator first, iterator last)
	{
		if (first == start && last == finish)
		{
			clear();
			return finish;
		}
		else
		{
			difference_type n = last - first;
			difference_type elems_before = first - start;

			if (size_type(elems_before) < (size() - n / 2))
			{
				std::copy_backward(start, first, last);
				iterator n_start = start + n;
				destroy(start, n_start);
				destroy_nodes(start.node, n_start.node);
				start = n_start;
			}
			else
			{
				std::copy(last, finish, first);
				iterator n_finish = finish - n;
				destroy(n_finish, finish);
				destroy_nodes(n_finish.node, finish.node);
				finish = n_finish;
			}

			return start + elems_before;
		}
	}

	iterator insert(iterator pos, const T &x)
	{
		if (pos.cur == start.cur)
		{
			push_front(x);
			return start;
		}
		else if (pos.cur == finish.cur)
		{
			push_back(x);
			iterator tmp = finish;
			--tmp;
			return tmp;
		}
		else
		{
			return insert_aux(pos, x);
		}
	}

protected:

	iterator insert_aux(iterator pos, const T &x)
	{
		difference_type index = pos - start;
		value_type x_copy(x);
		if (size_type(index) < size() / 2)
		{
			push_front(front());
			iterator front1 = start;
			++front1;
			iterator front2 = front1;
			++front2;
			pos = start + index;
			iterator pos1 = pos;
			++pos1;

			std::copy(front2, pos1, front1);
		}
		else
		{
			push_back(back());
			iterator back1 = finish;
			--back1;
			iterator back2 = back1;
			--back2;
			pos = start + index;

			std::copy_backward(pos, back2, back1);
		}

		*pos = x_copy;
		return pos;
	}

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
		if (nodes < start.node - _map)
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