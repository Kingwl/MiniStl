// Queue.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Queue.hpp"

int _tmain(int argc, _TCHAR* argv[])
{
	Queue<int> q;
	q.push(1);
	q.push(2);
	std::cout << q.front() << q.back() << std::endl;
	q.push(3);
	std::cout << q.front() << q.back() << std::endl;
	q.pop();
	std::cout << q.front() << q.back() << std::endl;
	q.pop();
	std::cout << q.front() << q.back() << std::endl;
	q.pop();
	std::cout << q.size();
	system("pause");
	return 0;
}

