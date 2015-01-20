#ifndef _STACK_
#define _STACK_
#include "Deque.hpp"
template<class T, class Sequene = Deque<T>>
class Stack
{
public:
	typedef typename Sequene::value_type value_type;
	typedef typename Sequene::size_type size_type;
	typedef typename Sequene::reference reference;
	typedef typename Sequene::const_reference const_reference;

protected:

	Sequene c;

public:

	bool empty() const
	{
		return c.empty();
	}

	size_type size() const
	{
		return c.size();
	}

	reference top()
	{
		return c.back();
	}

	const_reference top() const
	{
		return c.back();
	}

	void push(const T &x)
	{
		c.push_back(x);
	}

	void pop()
	{
		c.pop_back();
	}

};
#endif