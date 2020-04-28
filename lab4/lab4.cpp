
#include <iostream>
#include "myTable.h"
//#include <conio.h>
//#include <windows.h>

using namespace std;


int main()
{
	/*COORD position = { 10,0 }; //позиция x и y
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole,position);*/
	for (int N = 0; N < myTable::SIZE; N++) {
		myTable tbl;
		char* ch;
		for (int i = 0; i <= myTable::SIZE; i++) {
			ch = new char[4];
			_itoa_s(i, ch, 3, 10);
			tbl.add(20 + N + i, 30 + N + i, ch);
		}
		ch = new char[4];
		_itoa_s(myTable::SIZE + 1, ch, 3, 10);
		tbl.add(20 + N + myTable::SIZE + 2, 30 + N + myTable::SIZE + 2, ch);
		tbl.print();
		cout << "------------------------------------------" << endl;
	}
	system("PAUSE");
	return 0;
}
