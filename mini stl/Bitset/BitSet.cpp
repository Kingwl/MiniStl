// BitSet.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "BitSet.hpp"
#include "string"

int _tmain(int argc, _TCHAR* argv[])
{

	const int  size = 256;
	bool bb[size];
	BitSet<size> b;
	b[128] = true;
	bool flag = b[128];
	std::cout << flag;
	b[128] = false;
	flag = b[128];
	std::cout << flag;

	system("pause");
	return 0;
}

