// SmartPtr.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "smart_ptr.h"

class Test
{
public:
	void fuck()
	{

	}
};
int _tmain(int argc, _TCHAR* argv[])
{
	shared_ptr<Test> p(new Test);
	unique_ptr<Test> u(new Test);
	auto a(std::move(u));
	a = std::move(u);
	p->fuck();
	system("pause");
	return 0;
}

