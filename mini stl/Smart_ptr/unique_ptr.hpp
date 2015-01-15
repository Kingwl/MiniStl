#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _UNIQUE_PTR_
#define _UNIQUE_PTR_
template<class T>
class unique_ptr
{
public:
	typedef unique_ptr<T> self;

	unique_ptr(T *ptr)
		:_elemt(ptr)
	{

	}

	unique_ptr(const self &) = delete;

	self& operator=(const self &) = delete;

	self& operator=(self &&x)
	{
		_elemt = x._elemt;
		x._elemt = nullptr;
	}

	unique_ptr(self &&x)
		:_elemt(x._elemt)
	{
		x._elemt = nullptr;
	}

	~unique_ptr()
	{
		delete _elemt;
	}

	T& operator*()
	{
		return *_elemt;
	}

	T* operator->()
	{
		return &operator*();
	}

private:

	T* _elemt;
};
#endif