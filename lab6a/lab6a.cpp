#include <iostream>
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

int main()
{
	system("color 1E");
	system("mode con cols=160 lines=30");
	MyGraph graph;
	graph.addNode(1, 1);
	graph.addNode(1, 2);
	graph.addNode(1, 3);

	graph.addNode(2, 1);
	graph.addNode(2, 2);
	graph.addNode(2, 3);

	graph.addArc(1, 1, 1, 2);
	graph.addArc(1, 2, 1, 1);
	graph.addArc(1, 2, 2, 1);

	graph.addArc(1, 3, 2, 1);
	graph.addArc(2, 1, 2, 2);
	graph.addArc(2, 2, 2, 3);
	graph.addArc(2, 3, 1, 3);

	graph.printToConsole();
	cout << "----------------------------------------------------" << endl;
	{
		MyList<MyNodesList>* sc = graph.StronglyConnected(), * sct = sc;
		while (sct) {
			sct->info->print();
			sct = sct->next;
		}
	}
	cout << "----------------------------------------------------" << endl;
	graph.removeNode(1, 1);
	graph.printToConsole();
}
