// Algo.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Algo.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	const int size = 100;
	int n[size];
	for (int i = 0; i < size; ++i)
	{
		n[i] = rand() % 0xffff;
	}
	sort(n, n + size);
	for (int i = 0; i < size; ++i)
	{
		std::cout << n[i] << ",";
	}
	system("pause");
	return 0;
}

