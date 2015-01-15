#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _SHARED_PTR_
#define _SHARED_PTR_

template<class T>
class Ref
{
public:
	typedef Ref<T> self;

	Ref(T *ptr)
		:_count(1), _elemt(ptr)
	{
	
	}

	void retain()
	{
		++_count;
	}

	void release()
	{
		if (--_count == 0)
		{
			delete _elemt;
			_elemt = nullptr;
		}
	}

	T* get()
	{
		return _elemt;
	}

	std::size_t count()
	{
		return _count;
	}

	bool empty() const
	{
		return _elemt == nullptr;
	}
private:

	std::size_t _count;
	T *_elemt;
};

template<class T>
class shared_ptr
{
	typedef Ref<T> RefCount;
	typedef shared_ptr<T> self;

public:
	shared_ptr(T *ptr)
		:_count(new RefCount(ptr))
	{

	}

	shared_ptr(const self &x)
		:_count(x._count)
	{
		_count->retain();
	}

	self& operator= (const self& x)
	{
		if (this != &x)
		{
			x._count->retain();
			_count->release();
			_count = x._count;
		}
	}

	~shared_ptr()
	{
		_count->release();
		_count = nullptr;
	}

	std::size_t GetRefCount()
	{
		return _count->count();
	}

	T* get()
	{
		return _count->get();
	}

	T& operator*()
	{
		return *_count->get();
	}

	T* operator->()
	{
		return &operator*();
	}

private:

	RefCount *_count;
};
#endif