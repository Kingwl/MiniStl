#define DEBUG
#ifdef DEBUG
#include "iostream"
#endif
#ifndef _ALGORITHM_
#define _ALGORITHM_
#include "functional"
enum : std::size_t
{
	threshold = 40
};
template<class Iterator>
void insert_sort(Iterator first, Iterator last)
{
	if (first == last) return;
	for (Iterator cur_i = first; cur_i != last; ++cur_i)
	{
		for (Iterator cur_j = cur_i + 1; cur_j != last; ++cur_j)
		{
			if (*cur_j < *cur_i)
			{
				std::swap(*cur_j, *cur_i);
			}
		}
	}
}

template<class Iterator>
void quick_sort(Iterator first, Iterator last)
{
	if (first == last) return;

	Iterator cur_s = first;
	Iterator cur_l = last;
	--cur_l;
	Iterator key = cur_s;

	while (cur_s < cur_l)
	{
		while (cur_s < cur_l && *key < *cur_l)
			--cur_l;
		while (cur_s < cur_l && *cur_s < *key)
			++cur_s;
		std::swap(*cur_s, *cur_l);
	}
	std::swap(*key, *cur_s);

	quick_sort(first, cur_s);
	quick_sort(cur_l + 1, last);

}

template<class Iterator>
void sort(Iterator first, Iterator last)
{
	if (first == last) return;
	std::size_t size = std::distance(first, last);
	if (size < threshold)
	{
		insert_sort(first, last);
	}
	else
	{
		quick_sort(first, last);
	}
}

#endif