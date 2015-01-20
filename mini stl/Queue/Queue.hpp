#ifndef _QUEUE_
#define _QUEUE_
#include "Deque.hpp"

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

#endif