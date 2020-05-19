#include <iostream>
#include <conio.h>
#include <ctime>
#include <fstream>
#include "MyGraph.h"
using namespace std;

///Функция безопасного чтения из консоли
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
}

void randomGraph(MyGraph* gr, int N, int NN)
{
	int max = (int)floor(sqrt(N + 1));
	for (int i = 1; i <= max; i++)
	{
		for (int j = 1; j <= max; j++)
			gr->addNode(i, j, true);
	}
	//int amx = rand() % ((N) * (N - 1));
	for (int i = 0; i < NN; i++)
	{
		gr->randomArc(true);
	}
}

int main()
{
	srand(time(0));
	system("color 1E");
	system("mode con cols=160 lines=40");
	//remove("graph.bin");
	remove("log.txt");
	MyGraph graph("graph.bin");
	//cout << "Generating random graph" << endl;
	//randomGraph(&graph, 20000,1000);
	MyList<MyNodesList>* scc = nullptr;
	while (1)
	{
		MyGraphErrors status = MG_NO_ERROR;
		system("cls");
		cout << "a - add new node, c - add new arc (connect nodes), r - remove node, d - remove arc (disconnect), s - find strongly conncted components, p - print graph to log.txt, P - print to consol and log.txt" << endl;
		if (graph.getNodesCount() < 20) 
		{
			graph.printToConsole(true);
			cout << "a - add new node, c - add new arc (connect nodes), r - remove node, d - remove arc (disconnect), s - find strongly conncted components, p - print graph to log.txt, P - print to consol and log.txt" << endl;
		}
		cout << "Nodes: " << graph.getNodesCount() << ". Arcs: " << graph.getArcsCount() << endl;
		char key = _getch();
		switch (key)
		{
		case 27: goto end;

		case 'p':
		{
			cout << "printing" << endl;
			graph.printToConsole();
			cout << "printing finished" << endl;
		}
		break;	
		case 'P':
		{
			system("cls");
			cout << "printing" << endl;
			graph.printToConsole(true);
			cout << "printing finished" << endl;
		}
		break;
		case 'a':
		{
			cout << "Type x and y:" << endl;
			int x1, y1;
			safeGet(x1);
			safeGet(y1);
			status = graph.addNode(x1, y1, true); }
		break;
		case 'c':
		{
			cout << "Type x1, y1, x2 and y2 to connect:" << endl;
			int x1, y1, y2, x2;
			safeGet(x1);
			safeGet(y1);
			safeGet(x2);
			safeGet(y2);
			status = graph.addArc(x1, y1, x2, y2, true); }
		break;
		case 'r':
		{
			cout << "Type x and y to remove: " << endl;
			int x1, y1;
			safeGet(x1);
			safeGet(y1);
			status = graph.removeNode(x1, y1, true); }
		break;
		case 'd':
		{
			cout << "Type x1, y1, x2 and y2 to disconnect:" << endl;
			int x1, y1, y2, x2;
			safeGet(x1);
			safeGet(y1);
			safeGet(x2);
			safeGet(y2);
			status = graph.removeArc(x1, y1, x2, y2, true); }
		break;
		case 's':
		{
			//очищаем список от прошлого разбиения
			MyList<MyNodesList>* scctmp = scc;
			while (scctmp)
			{
				scctmp->info->clean();
				delete scctmp->info;
				MyList<MyNodesList>* scctmp2 = scctmp->next;
				delete scctmp;
				scctmp = scctmp2;
			}
			double time = clock();
			scc = graph.StronglyConnected();
			time = clock() - time;
			scctmp = scc;
			cout << "Strongly conected component" << endl;
			while (scctmp)
			{
				scctmp->info->print();
				scctmp = scctmp->next;
			}
			cout << "seach time: " << time / 1000 << endl;
			cout << "For timing: " << graph.getNodesCount() << ';' << graph.getArcsCount() << ';' << time / 1000 << endl;
		}
		break;
		}
		switch (status)
		{
		case MG_NO_ERROR:
			cout << "Operation finished without errors" << endl;
			break;
		case MG_COORDS_NOT_FOUNDED:
			cout << "Coordinats wasn't founded in graph. It wasn't changed" << endl;
			break;
		case MG_COORDS_BUSY:
			cout << "Coordinats is busy. Graph wasn't changed" << endl;
			break;
		}
		cout << "Type any key to continue" << endl;
		_getch();
	}
end:
	graph.GC();
	return 0;
}
