// lab4b.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <conio.h>
#include <windows.h>
#include "myTable.h"
using namespace std;
///Тестовый пример (вставить в main)
	/*cout << ((tb.add(34, 12, "4") != 0) ? "error" : "no error") << endl;
	cout << ((tb.add(23, 23, "1") != 0) ? "error" : "no error") << endl;
	cout << ((tb.add(23, 12, "1a") != 0) ? "error" : "no error") << endl;
	cout << ((tb.add(24, 12, "2") != 0) ? "error" : "no error") << endl;
	cout << ((tb.add(25, 12, "3") != 0) ? "error" : "no error") << endl;
	tb.println();
	tb.deleteByKey2(23);
	tb.println();*/
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
	system("color 0A");
	system("mode con cols=160 lines=30");
	myTable* mtmp = nullptr;

	while (mtmp == nullptr) 
	{
		cout << "Type filename: ";
		char filename[10]; 
		cin >> filename;
		mtmp = myTable::createFromFile(filename);
		if(mtmp == nullptr)
			cout << "\nIncorrect filename\n";
	}

	myTable tb = *mtmp;
	char* word;
	while (1)
	{
		system("cls");
		cout << "Contols: a - add new item, f - find items by key, d - delete items, esc - exit" << endl;
		tb.println();
		char key = _getch();

		int status;
		switch (key) {
		default:
			status = -1;
			break;
		case 27: goto end;
			break;
		break;
		case 'a':
			cout << "Type key1, key2 and word" << endl;
			int k1, k2;
			word = new char[10];
			safeGet(k1);
			safeGet(k2);
			safeGet(word);
			status = tb.add(k1, k2, word);
			break;
		case 'f':
		{
			cout << "Choose keyspace (1 or 2)" << endl;
			char keyspace = _getch();
			cout << "Type key" << endl;
			int key;
			safeGet(key);
			ItemList* rslt = (keyspace == '1') ? tb.findByKey1(key) : tb.findByKey2(key);
			status = MTB_NO_ERROR;
			if (rslt == nullptr) status = MTB_ERROR_NO_ELM_WT_KEY_FINDED;
			else { rslt->println(tb.getFileName()); rslt->clear(); }
		}
		break;
		case 'd': {
			cout << "Choose keyspace (1 or 2)" << endl;
			char keyspace = _getch();
			cout << "By version? (y/N)" << endl;
			char v = _getch();
			if (v == 'y' || v == 'Y')
			{
				cout << "Type key and version" << endl;
				int key, vers;
				safeGet(key);
				safeGet(vers);
				if (keyspace == '1')
					status = tb.deleteByKey1(key, vers);
				else
					status = tb.deleteByKey2(key, vers);
			}
			else
			{
				cout << "Type key" << endl;
				int key;
				safeGet(key);
				if (keyspace == '1')
					status = tb.deleteByKey1(key);
				else
					status = tb.deleteByKey2(key);
			}
		}
				break;
		}
		switch (status)
		{
		case -1:
			continue;
			break;
		case MTB_ERROR_CUCUSHKA_CYCLE:
			cerr << "Critical error: cucushka cycled. Try change Hash1 and Hash2 to better function. Program will be terminated." << endl;
			goto end;
			break;
		case MTB_ERROR_TABLE_FULL:
			cerr << "Table is full, element wasn't added" << endl;
			break;
		case MTB_ERROR_NO_ELM_WT_KEY:
			cerr << "Element with this key doesn't exist. Table wasn't chanded" << endl;
			break;
		case MTB_ERROR_NO_ELM_WT_KEY_FINDED:
			cerr << "Element with this key wasn't finded." << endl;
			break;
		case MTB_ERROR_NO_ELM_WT_KEY_VER:
			cerr << "Element with this key and version doesn't exist. Table wasn't chanded" << endl;
			break;
		case MTB_NO_ERROR:
			cout << "Operation finished without errors." << endl;
			break;
		case MTB_FILE_NOT_EXIST:
			cerr << "File not founded." << endl;
			break;
		case MTB_NO_FILE_ACCESS:
			cerr << "Filesystem cant get access to file with this path." << endl;
			break;
		case MTB_FILE_FORMAT_WRONG:
			cerr << "File finded by this path is not table." << endl;
			break;
		}
		cin.ignore(cin.rdbuf()->in_avail());
		cout << "Press any key to continue." << endl;
		_getch();
	}
end:
	tb.optimize();
	tb.clear();
	system("PAUSE");
	return 0;
}
