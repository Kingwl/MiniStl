// Vector.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Vector.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	Vector<int> v(10);
	v.push_back(1);
	v.pop_back();
	system("pause");
	return 0;
}

