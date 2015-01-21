// Heap.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Heap.hpp"
#include "iostream"
template<class T>
bool big(const T &a, const T &b)
{
	return a > b;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int r[10] = { 3, 2, 1, 4, 5, 6, 9, 8, 7, 0 };
	int n[10];

	for (int i = 0; i < 10; ++i)
	{
		n[i] = r[i];
		push_heap(n, n + i + 1, big<int>);
		std::cout << n[0] << " ";

	}
	std::cout << std::endl;

	for (int i = 10; i > 0; --i)
	{
		std::cout << n[0] << " ";
		pop_heap(n,n+ i,big<int>);
	}

	std::cout << std::endl;
	system("pause");
	return 0;
}

