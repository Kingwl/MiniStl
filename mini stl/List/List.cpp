// List.cpp : �������̨Ӧ�ó������ڵ㡣
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
	system("pause");
	return 0;
}

