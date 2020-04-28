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
	system("color 1E");
	LLRBTree tree;
	LLRB_ERRORS error = tree.add(11, "LOL");
	error = tree.add(13, "KEK");
	error = tree.add(10, "PEK");
	error = tree.add(12, "POOP");
	error = tree.add(14, "FOO");
	error = tree.add(15, "FOO2");
	error = tree.add(5, "FOO3");
	error = tree.add(16, "FOO4");
	error = tree.add(9, "FOO5");
	cout << (error == LLRB_KEY_IS_TAKEN) << endl;
	error = tree.printAsTable();
	const char* fnd;
	error = tree.find(0, &fnd);
	if (error == LLRB_NO_ERROR)cout << fnd << endl;
}
