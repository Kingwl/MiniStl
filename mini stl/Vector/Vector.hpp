#include "Allocator.hpp"
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _VECTOR_
#define _VECTOR_

template<class T, class Alloc = alloc>
class Vector
{
public:
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef const T* const_pointer;
	typedef const T& const_reference;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef T* iterator;
	typedef const T* const_iterator;

protected:

	typedef simple_alloc<T, alloc> data_allocator;

	iterator _start;
	iterator _finish;
	iterator _end_of_stroage;

	void _insert_aux(iterator pos, const T& x = T())
	{
		if (_finish != _end_of_stroage)
		{
			construct(_finish, *(_finish - 1));
			++_finish;
			T x_copy(x);
			std::copy_backward(pos, _finish - 2, _finish - 1);
			*pos = x_copy;
		}
		else
		{
			const size_type old_size = size();
			const size_type new_size = old_size == 0 ? 1 : old_size * 2 / 3;

			iterator new_start = data_allocator::allocate(new_size);
			iterator new_finish = new_start;

			new_finish = std::copy(_start, pos, new_start);
			construct(new_finish, x);
			++new_finish;
			new_finish = std::copy(pos, _finish, new_finish);

			destroy(begin(), end());
			_deallocate();

			_start = new_start;
			_finish = new_finish;
			_end_of_stroage = _start + new_size;
		}
	}

	
	iterator _allocate_and_fill(size_type n, const T& x)
	{
		iterator result = data_allocator::allocate(n);
		std::fill_n(result, n, x);
		return result;
	}

	void _deallocate()
	{
		if (_start != nullptr)
		{
			data_allocator::deallocate(_start, _end_of_stroage - _start);
		}
	}

	void _fill_init(size_type n, const T& x)
	{
		_start = _allocate_and_fill(n, x);
		_finish = _start + n;
		_end_of_stroage = _finish;
	}
	
public:

	iterator begin()
	{
		return _start;
	}

	const_iterator cbegin() const
	{
		return _start;
	}

	iterator end()
	{
		return _finish;
	}

	const_iterator cend() const
	{
		return _finish;
	}

	size_type size() const
	{
		return size_type(_finish - _start);
	}

	bool empty() const
	{
		return size() == 0;
	}

	size_type capacity() const
	{
		return size_type(_end_of_stroage - begin());
	}

	reference operator[](std::size_t n)
	{
		return *(begin() + n);
	}

	const_reference operator[](std::size_t n) const
	{
		return *(begin() + n);
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
		return *(end() - 1);
	}

	const_reference back() const
	{
		return *(end() - 1);
	}

/*-----------------------------------------------------------------------------------*/
	Vector()
		:_start(nullptr), _finish(nullptr), _end_of_stroage(nullptr)
	{
	
	}
	Vector(const Vector<T, Alloc> &v)
	{

	}
	Vector(size_type n, const T &x)
	{
		_fill_init(n, x);
	}

	explicit Vector(size_type n)
	{
		_fill_init(n, T());
	}

	~Vector(){
		destroy(begin(), end());
		_deallocate();
	};
/*-----------------------------------------------------------------------------------*/

	void push_back(const T &x)
	{
		if (_finish != _end_of_stroage)
		{
			construct(_finish, x);
			++_finish;
		}
		else
		{
			_insert_aux(end(), x);
		}
	}

	void pop_back()
	{
		--_finish;
		destroy(_finish);
	}

	void insert(iterator pos, size_type n, const T &x)
	{
		if (n != 0)
		{
			if (_end_of_stroage - _finish >= n)
			{
				T x_copy(x);
				const size_type elemt_after = _finish - pos;
				iterator old_finish = _finish;

				if (elemt_after > n)
				{
					std::copy(_finish - n, _finish, _finish);
					_finish += n;
					std::copy_backward(pos, old_finish - n, old_finish);
					std::fill(pos, old_finish, x_copy);
				}
				else
				{
					std::fill_n(_finish, n - elemt_after, x_copy);
					_finish += n - elemt_after;
					std::copy(pos, old_finish, _finish);
					_finish += elemt_after;
					std::fill(pos, old_finish, x_copy);
				}
			}
			else
			{
				const size_type old_size = size();
				const size_type new_size = old_size + ((old_size / 2) > n ? (old_size / 2) : n);

				iterator new_start = data_allocator::allocate(new_size);
				iterator new_finish = new_start;

				new_finish = std::copy(_start, pos, new_start);
				new_finish = std::fill_n(new_finish, n, x);
				new_finish = std::copy(pos, _finish, new_finish);

				destroy(begin(), end());
				_deallocate();

				_start = new_start;
				_finish = new_finish;
				_end_of_stroage = _start + new_size;
			}
		}
	}

	iterator erase(iterator pos)
	{
		if (pos + 1 != _finish)
		{
			std::copy(pos + 1, _finish, pos);
		}
		--_finish;
		destroy(_finish);
		return pos;
	}

	iterator erase(iterator first, iterator last)
	{
		iterator i = std::copy(last, _finish, first);
		destroy(i, _finish);
		_finish = _finish - (last - first);
		return first;
	}

	void resize(size_type new_size, const T &x = T())
	{
		if (new_size < size())
		{
			erase(_start + new_size, end());
		}
		else
		{
			insert(end(), new_size - size(), x);
		}
	}

	void clear()
	{
		erase(begin(), end());
	}
};
#endif