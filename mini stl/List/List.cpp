// List.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "list.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	list_iterator<int,int&,int*> a;
	list_iterator<int, int&, int*> b(a);
	list<int> l;
	l.push_back(1);
	std::cout << *l.begin();
	l.erase(l.begin());
	std::cout << l.empty();

	sList<int> sl;
	sl.push_front(1);
	sl.push_front(2);
	sl.push_front(3);
	std::cout << sl.size();
	sl.pop_front();
	std::cout << sl.size();
	for (auto p : sl)
	{
		std::cout << p;
	}
	sl.clear();
	std::cout << sl.size();

	system("pause");
	return 0;
}

