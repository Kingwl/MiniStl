// Deque.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "iostream"
#include "Deque.hpp"
int _tmain(int argc, _TCHAR* argv[])
{
	const int siz = 1000;
	Deque<int> d(10,0);
	for (int i = 0; i < siz; ++i)
	{
		d.push_back(i);
	}
	d.erase(d.begin());
	d.erase(d.begin(), d.begin() + 3);
	d.insert(d.begin() + 2, 1);
	d.clear();
	std::cout << d.size();
	d.back();
	d.begin();
	d.cbegin();
	d.cend();
	d.empty();
	d.end();
	d.front();
	d.clear();
	//d.clear();

	system("pause");

	return 0;
}

