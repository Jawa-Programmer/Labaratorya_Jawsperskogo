// lab5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <conio.h>
#include <windows.h>
#include "LLRBTree.h"

using namespace std;

template <typename T> void safeGet(T& variable)
{
	cin >> variable;
	while (!cin)
	{
		cin.clear();
		cin.ignore(cin.rdbuf()->in_avail());
		cout << "incorrect input, try again: ";
		cin >> variable;
	}
	return;
}

int main()
{
    std::cout << "Hello World!\n";
	LLRBTree tree;
	LLRB_ERRORS error = tree.add(11, "LOL");
}
