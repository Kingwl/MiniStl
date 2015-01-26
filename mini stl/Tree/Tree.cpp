// Tree.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "Tree.hpp"
#include "string"
int _tmain(int argc, _TCHAR* argv[])
{
    binary_search_tree<int> bst;
   /* std::string str; 
    int i;
    while (std::cin >> str >> i)
    {
        if (str == "in")
        {
            bst.insert(i);
        }
        else if (str == "out")
        {
            bst.remove(i);
        }
       bst.print();
       std::cout << std::endl;
    }*/
    bst.insert(3);
    bst.insert(1);
    bst.insert(6);
    bst.insert(2);
    bst.insert(7);
    bst.insert(42);
    bst.insert(9);
    bst.insert(233);

    std::cout << std::endl;

    for (auto iter = bst.begin(); iter != bst.end(); ++iter)
    {
        std::cout << *iter << " ";
    }
    std::cout << std::endl;

    system("pause");
	return 0;
}

