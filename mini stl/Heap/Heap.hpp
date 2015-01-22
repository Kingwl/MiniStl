#ifndef _HEAP_
#define _HEAP_
#include "type_traits"
#include "iterator"
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
	if (last - first < 2)  return;
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
	if (last - first < 2)  return;
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

template<class RandomAccessIterator>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	while (first < last)
	{
		pop_heap(first, last--);
	}
}

template<class RandomAccessIterator, class Compare>
inline void sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	while (first < last)
	{
		pop_heap(first, last--, comp);
	}
}

template<class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
	if (last - first < 2) return;
	_make_heap(first, last, (difference_type*)0, (value_type*)0);
}

template<class RandomAccessIterator, class Distance, class T>
inline void _make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*)
{
	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (true)
	{
		adjust_heap(first, parent, len, T(*(first + parent)));
		if (parent == 0) return;
		--parent;
	}
}

template<class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
{
	typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
	typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
	if (last - first < 2) return;
	_make_heap(first, last, (difference_type*)0, (value_type*)0, comp);
}

template<class RandomAccessIterator, class Distance, class T, class Compare>
inline void _make_heap(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp)
{
	Distance len = last - first;
	Distance parent = (len - 2) / 2;
	while (true)
	{
		adjust_heap(first, parent, len, T(*(first + parent)), comp);
		if (parent == 0) return;
		--parent;
	}
}

template<class Iterator>
struct is_random
{
	static const bool result = false;
};
template<>
struct is_random<std::random_access_iterator_tag>
{
	static const bool result = true;
};

template<class Container>
inline void make_heap(Container &c)
{
	typedef typename std::iterator_traits<typename Container::iterator>::iterator_category iterator_category;
	static_assert(is_random<iterator_category>::result, "iterator is not random access iterator");
	make_heap(std::begin(c), std::end(c));
}

template<class Container, class Compare>
inline void make_heap(Container &c, Compare comp)
{
	typedef typename std::iterator_traits<typename Container::iterator>::iterator_category iterator_category;
	static_assert(is_random<iterator_category>::result, "iterator is not random access iterator");
	make_heap(std::begin(c), std::end(c),comp);
}

#endif