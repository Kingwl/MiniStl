// Vector.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Vector.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	Vector<int> v(10);
	v.push_back(1);
	Vector<int> v1(v);
	v.pop_back();
	for (auto &i : v1)
	{
		std::cout << i;
	}
	system("pause");
	return 0;
}

