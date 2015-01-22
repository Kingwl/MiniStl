// Heap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Heap.hpp"
#include "iostream"
#include "vector"
#include "list"
template<class T>
bool big(const T &a, const T &b)
{
	return a < b;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int r[10] = { 3, 2, 1, 4, 5, 6, 9, 8, 7, 0 };
	int n[10];

	for (int i = 0; i < 10; ++i)
	{
		n[i] = r[i];
		push_heap(n, n + i + 1, big<int>);

	}

	sort_heap(n, n + 10, big<int>);
	for (auto p : n)
	{
		std::cout << p << " ";
	}
	std::cout << std::endl;

	for (int i = 0; i < 10; ++i)
	{
		r[i] = rand() % 100;
	}
	make_heap(r, r + 10,big<int>);
	sort_heap(r, r + 10, big<int>);
	for (auto p : r)
	{
		std::cout << p << " ";
	}
	std::cout << std::endl;
	std::vector<int> v;
	make_heap(v,big<int>);


	system("pause");
	return 0;
}

