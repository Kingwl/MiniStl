// simple_alloc.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "Allocator.hpp"
#include "ctime"

struct  Test
{
	Test(int i, int b){ std::cout << "args con" << std::endl; }
	Test(){ std::cout << "con" << std::endl; }
	~Test(){std::cout << "decon" << std::endl;}
};
int _tmain(int argc, _TCHAR* argv[])
{
	simple_alloc<int> alloc;
	const int SIZE = 100000;
	int *pool[SIZE];
	int *heap[SIZE];
	int *number = new int[SIZE];
	unsigned timePool = 0;
	unsigned timeHeap = 0;
	for (int i = 0; i < SIZE; i++)
	{
		number[i] = rand() % 10000;
	}
	timePool = (unsigned)time(0);
	for (int i = 0; i < SIZE; i++)
	{
		pool[i] = alloc.allocate();
		*pool[i] = number[i];
	}
	timePool = (unsigned)time(0) - timePool;

	timeHeap = (unsigned)time(0);
	for (int i = 0; i < SIZE; i++)
	{
		heap[i] = new int;
		*heap[i] = number[i];
	}
	timeHeap = (unsigned)time(0) - timeHeap;

	std::cout << "pool alloc time : " << timePool << std::endl;
	std::cout << "heap alloc time : " << timeHeap << std::endl;
	
	bool flag = true;
	for (int i = 0; i < SIZE; ++i)
	{
		if (*pool[i] != *heap[i])
		{
			flag = false;
			break;
		}
	}
	if (flag) std::cout << "check over" << std::endl;

	timePool = 0;
	timeHeap = 0;
	timePool = (unsigned)time(0);
	for (int i = 0; i < SIZE; i++)
	{
		alloc.deallocate(pool[i]);
	}
	timePool = (unsigned)time(0) - timePool;

	timeHeap = (unsigned)time(0);
	for (int i = 0; i < SIZE; i++)
	{
		delete heap[i];
	}
	timeHeap = (unsigned)time(0) - timeHeap;

	std::cout << "pool dealloc time : " << timePool << std::endl;
	std::cout << "heap dealloc time : " << timeHeap << std::endl;
	system("pause");
	return 0;
}

