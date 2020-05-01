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
	system("mode con cols=160 lines=30");
	LLRBTree tree;
	while (true)
	{
		LLRB_ERRORS status = LLRB_NO_OPERATIONS;
		system("cls");
		cout << "Contro: a - add new element, r - remove element by key, f - search element by key, Esc - exit" << endl;
		tree.printAsTable();
		char key = _getch();
		switch (key)
		{
		default:
			break;
		case 27: goto end;
			break;
		case 'a':
		case 'A':
		case 'ф':
		case 'Ф':
		{
			cout << "Adding new element" << endl;
			cout << "Type key: " << endl;
			int ky;
			char* data = new char[20];
			safeGet(ky);
			cout << "Type data: " << endl;
			safeGet(data);
			status = tree.add(ky, data);
		}
		break;
		case 'r':
		case 'R':
		case 'к':
		case 'К':
		{
			cout << "Removing element" << endl;
			cout << "Type key: " << endl;
			int ky;
			safeGet(ky);
			status = tree.remove(ky);
		}
		break;
		case 'f':
		case 'F':
		case 'а':
		case 'А':
		{
			cout << "Searching element" << endl;
			cout << "Type key: " << endl;
			int ky;
			safeGet(ky);
			char* data = new char[20];
			status = tree.find(ky, &data);
			if (status == LLRB_NO_ERROR) cout << "Founded data: " << data << endl;
			delete[] data;
		}
		break;
		}

		switch (status)
		{
		case LLRB_NO_ERROR:
			cout << "Operation finished without errors" << endl;
			break;
		case LLRB_KEY_IS_TAKEN:
			cout << "This key is taken, tree wasn't changed" << endl;
			break;
		case LLRB_NO_KEY:
			cout << "This key is not founded in tree" << endl;
			break;
		case LLRB_NO_OPERATIONS:
			break;
		}
		cout << "Press eny key to continue" << endl;
		_getch();
	}
end:
	return 0;
}
