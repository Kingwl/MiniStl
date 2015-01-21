#ifndef _HEAP_
#define _HEAP_
#include "type_traits"

template<class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	typedef std::iterator_traits<RandomAccessIterator>::difference_type difference_type;;
	typedef std::iterator_traits<RandomAccessIterator>::value_type value_type;
	if (first == last) return;
	push_heap_aux(first, last, (difference_type*)0, (value_type*)0);
}

template<class RandomAccessIterator, class Distance, class T>
inline void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

template<class RandomAccessIterator, class Distance, class T>
inline void _push_heap(RandomAccessIterator first, Distance holeindex, Distance topindex, T value)
{
	Distance parent = (holeindex - 1) / 2;
	while (holeindex > topindex && *(first + parent) < value)
	{
		*(first + holeindex) = *(first + parent);
		holeindex = parent;
		parent = (holeindex - 1) / 2;
	}
	*(first + holeindex) = value;
}

template<class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	if (first == last)  return;
	pop_heap_aux(first, last, (value_type*)0);
}

template<class RandomAccessIterator, class T>
inline void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
	_pop_heap(first, last - 1, last - 1, T(*(last - 1)), (difference_type*)0);
}

template<class RandomAccessIterator, class T, class Distance>
inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*)
{
	*result = *first;
	adjust_heap(first, Distance(0), (last - first), value);
}

template<class RandomAccessIterator, class T, class Distance>
inline void adjust_heap(RandomAccessIterator first, Distance holeindex, Distance len, T value)
{
	Distance topindex = holeindex;
	Distance rightchild = holeindex * 2 + 2;

	while (rightchild < len)
	{
		if (*(first + rightchild) < *( first + (rightchild - 1)))
		{
			--rightchild;
		}
		*(first + holeindex) = *(first + rightchild);
		holeindex = rightchild;
		rightchild = (rightchild + 1) * 2;
	}

	if (rightchild == len)
	{
		*(first + holeindex) = *(first + (rightchild - 1));
		holeindex = rightchild - 1;
	}
	_push_heap(first, holeindex, topindex, value);
}

template<class RandomAccessIterator,class Compare>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	typedef std::iterator_traits<RandomAccessIterator>::difference_type difference_type;;
	typedef std::iterator_traits<RandomAccessIterator>::value_type value_type;
	if (first == last) return;
	push_heap_aux(first, last, (difference_type*)0, (value_type*)0,comp);
}

template<class RandomAccessIterator, class Distance, class T, class Compare>
inline void push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp)
{
	_push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)),comp);
}

template<class RandomAccessIterator, class Distance, class T, class Compare>
inline void _push_heap(RandomAccessIterator first, Distance holeindex, Distance topindex, T value, Compare comp)
{
	Distance parent = (holeindex - 1) / 2;
	while (holeindex > topindex && comp( *(first + parent) , value))
	{
		*(first + holeindex) = *(first + parent);
		holeindex = parent;
		parent = (holeindex - 1) / 2;
	}
	*(first + holeindex) = value;
}

template<class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	if (first == last)  return;
	pop_heap_aux(first, last, (value_type*)0, comp);
}

template<class RandomAccessIterator, class T, class Compare>
inline void pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*, Compare comp)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
	_pop_heap(first, last - 1, last - 1, T(*(last - 1)), (difference_type*)0, comp);
}

template<class RandomAccessIterator, class T, class Distance, class Compare>
inline void _pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*, Compare comp)
{
	*result = *first;
	adjust_heap(first, Distance(0), (last - first), value, comp);
}

template<class RandomAccessIterator, class T, class Distance, class Compare>
inline void adjust_heap(RandomAccessIterator first, Distance holeindex, Distance len, T value, Compare comp)
{
	Distance topindex = holeindex;
	Distance rightchild = holeindex * 2 + 2;

	while (rightchild < len)
	{
		if (comp(*(first + rightchild) , *(first + (rightchild - 1))))
		{
			--rightchild;
		}
		*(first + holeindex) = *(first + rightchild);
		holeindex = rightchild;
		rightchild = (rightchild + 1) * 2;
	}

	if (rightchild == len)
	{
		*(first + holeindex) = *(first + (rightchild - 1));
		holeindex = rightchild - 1;
	}
	_push_heap(first, holeindex, topindex, value, comp);
}
#endif