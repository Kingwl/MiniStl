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
	typedef BitSet<N> self;
	typedef typename _If < N <= 32, unsigned, unsigned long long > ::value data_value;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;

public:

private:
	enum : difference_type
	{
		block_size = difference_type(sizeof(char)* sizeof(data_value)),
		block_num = difference_type(N == 0 ? 0 : (N - 1)/block_size),
	};
	data_value _elemts[block_num + 1];
	std::size_t _pos;
};

#endif