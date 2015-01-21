#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _BITSET_
#define _BITSET_

template<bool,class T1, class T2>
struct _If{
	typedef T2 value;
};

template<class T1, class T2>
struct _If<true, T1, T2>
{
	typedef T1 value;
};

//SFINAE



template<std::size_t N>
class BitSet
{
	template<std::size_t I>
	class bitset_reference
	{
		typedef bitset_reference<I> self;
	public:
		bitset_reference(BitSet<I> *ptr,std::size_t pos)
			:_ptr(ptr), _pos(pos)
		{

		}
		bitset_reference(const self &x)
			:_ptr(x._ptr), _pos(x._pos)
		{

		}

		self operator= (bool flag)
		{
			_ptr->set_value(_pos, flag);
			return *this;
		}

		self operator=(const self &x)
		{
			_ptr = x._ptr;
			_pos = x._pos;
			return *this;
		}

		operator bool()
		{
			return _ptr->get_value(_pos);
		}

	private:

		BitSet<I> *_ptr;
		std::size_t _pos;
	};
	friend class reference;
public:

	typedef BitSet<N> self;
	typedef typename _If < N <= 32, unsigned, unsigned long long > ::value data_value;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef bitset_reference<N> reference;

	BitSet()
	{
		std::memset(_elemts, 0, sizeof(_elemts));
	}

	reference operator[](size_type index)
	{
		return reference(this,index);
	}

protected:
	bool get_value(std::size_t n)
	{
		std::size_t pos = n / block_size;
		std::size_t index = n % block_size;
		data_value offset = 1 << index;
		data_value result = offset & _elemts[pos];
		return !(result == 0);
	}
	void set_value(std::size_t n, bool x)
	{
		data_value x_copy = (data_value)x;
		std::size_t pos = n / block_size;
		std::size_t index = n % block_size;
		data_value offset = ~(1 << index);
		data_value result = offset & _elemts[pos];
		data_value value = (x_copy << index);
		_elemts[pos] = result | value;
	}

private:
	enum :size_type
	{
		char_size = 8,
		int_size = char_size * 4,
		long_long_size = int_size * 2,
	};
	enum : difference_type
	{
		block_size = difference_type(char_size * sizeof(data_value)),
		block_num = difference_type(N == 0 ? 0 : (N - 1)/block_size),
	};
	data_value _elemts[block_num + 1];
};

#endif