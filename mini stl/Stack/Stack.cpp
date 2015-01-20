// Stack.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Stack.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	Stack<int> s;
	s.push(1);
	s.push(2);
	std::cout << s.top();
	s.pop();
	std::cout << s.top();
	s.pop();
	std::cout << s.size();
	system("pause");
	return 0;
}

