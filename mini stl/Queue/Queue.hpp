#ifndef _QUEUE_
#define _QUEUE_
#include "Deque.hpp"
#include "Vector.hpp"
#include "Heap.hpp"
template<class T, class Sequeue = Deque<T>>
class Queue
{

protected:

	Sequeue c;

public:

	typedef typename Sequeue::value_type value_type;
	typedef typename Sequeue::size_type size_type;
	typedef typename Sequeue::reference reference;
	typedef typename Sequeue::const_reference const_reference;

	bool empty() const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	reference front()
	{
		return c.front();
	}

	const_reference front() const
	{
		return c.front();
	}

	reference back()
	{
		return c.back();
	}

	const_reference back() const
	{
		return c.back();
	}

	void push(const T &x)
	{
		c.push_back(x);
	}

	void pop()
	{
		c.pop_front();
	}

};

template<class T, class Sequeue = Vector<T>, class Compare = std::less<typename Sequeue::value_type>>
class Priority_queue
{
public:

	typedef Priority_queue<T, Sequeue, Compare> self;
	typedef typename Sequeue::value_type value_type;
	typedef typename Sequeue::size_type size_type;
	typedef typename Sequeue::reference reference;
	typedef typename Sequeue::const_reference const_reference;

protected:

	Sequeue c;
	Compare comp;

public:
	Priority_queue()
		:c()
	{

	}

	Priority_queue(const self &x)
		:c(x.c), comp(x.comp)
	{

	}
	template<class RandomAccessIterator>
	Priority_queue(RandomAccessIterator first, RandomAccessIterator last)
		: c(first,last)
	{
		make_heap(c.begin(), c.end(),comp);
	}

	template<class RandomAccessIterator>
	Priority_queue(RandomAccessIterator first, RandomAccessIterator last, const Compare &x)
		: c(first, last), comp(x)
	{
		make_heap(c.begin(), c.end(),comp);
	}

	bool empty() const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	const_reference top() const
	{
		return c.front();
	}

	void push(const T &x)
	{
		c.push_back(x);
		push_heap(c.begin(), c.end(), comp);
	}

	void pop()
	{
		pop_heap(c.begin(), c.end(), comp);
		c.pop_back();
	}
};

#endif