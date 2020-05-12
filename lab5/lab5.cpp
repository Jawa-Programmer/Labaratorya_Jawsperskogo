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
/*
int main()
{
	system("color 1E");
	system("mode con cols=160 lines=30");
	LLRBTree tree;
	if (tree.readFromFile("debug.txt") == LLRB_INCORRECT_FILEPATH) {
		cout << "debug file not founded" << endl << "press any key to continue" << endl; _getch();
	}
	while (true)
	{
		LLRB_ERRORS status = LLRB_NO_OPERATIONS;
		system("cls");
		cout << "Contro: a - add new element, r - remove element by key, f - search element by key, Esc - exit" << endl;
		tree.printAsTree();
		cout << "---------------------------------------------------------------------------------------------" << endl;
		tree.printAsTable();
		cout << "Contro: a - add new element, r - remove element by key, f - search element by key, Esc - exit" << endl;
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
*/

#include <random>
#include <ctime>
#include <math.h>
#include <windows.h>

void gotoxy(int column, int line)
{
	COORD coord;
	coord.X = column;
	coord.Y = line;
	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE),
		coord
	);
}



struct sample
{
	int totalEl, totalFin;
	long totalSearchTime;
};
int main()
{
	system("color 1E");
	system("mode con cols=170 lines=40");
	std::random_device rd;
	std::mt19937 mersenne(rd());
	sample arr[100] = {};
	int N = 0;
	for (int i = 1; i <= 100; i++) {
		N += 20000;
		LLRBTree tree;

		int cnt = 0;
		cout << "started creation of random tree " << N << endl;
		for (int count = 0; count < N; ++count)
		{
			if ((tree.add((mersenne() % N), "lol")) == LLRB_NO_ERROR) cnt++;

		}
		/*cout << "printing tree to file, state 1/2" << endl;
		tree.printAsTree();
		cout << "printing tree to file, state 2/2" << endl;

		//cout << "Skiped" << endl;
		tree.printAsTable();*/

		cout << "timing " << endl;

		int finded = 0;
		char* tmp = new char[20];


		long starttime = clock();
		for (int count = 0; count < N; ++count)
		{
			if (tree.find((mersenne() % N), &tmp) == LLRB_NO_ERROR)finded++;
		}
		long dt = clock() - starttime;
		dt *= 1000;

		cout << "Total elements: " << cnt << endl;
		cout << "Total finded elements: " << finded << endl;
		cout << "Total search time: " << dt << endl;
		cout << "Avage search time: " << (float)dt / cnt << endl;
		cout << "lb(count): " << log2(cnt) << endl;
		arr[i - 1].totalEl = cnt;
		arr[i - 1].totalFin = finded;
		arr[i - 1].totalSearchTime = dt;
		delete[] tmp;
	}
	system("cls");
	for (int i = 0; i < 100; i++)
	{
		int x = (int)(arr[i].totalEl * 160.0 / 2000000.0);
		int y = 30 - (int)(arr[i].totalSearchTime*20 / arr[i].totalEl);

		gotoxy(x, y);
		cout << '*';
	}
	_getch();
	return 0;
}