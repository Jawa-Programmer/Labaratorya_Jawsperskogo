#ifndef MY_GRAPH_H
#define MY_GRAPH_H

enum MyGraphErrors { MG_NO_ERROR, MG_COORDS_BUSY };

struct MyNodesList
{
	MyGraphNode* info = nullptr;
	MyNodesList* next = nullptr;
	MyNodesList* find(int x, int y);
	MyGraphErrors add(MyGraphNode* toAdd);
	MyGraphErrors remove(MyGraphNode* toDel);
};

struct MyGraphNode
{
	int x, y;
	MyNodesList* incidents;
	void addIcident(MyGraphNode* toAdd);
	void removeIncident(MyGraphNode* toRemove);
	bool operator==(MyGraphNode b);
};

class MyGraph
{
public:
	MyGraphErrors addNode(int x, int y);
private:
	MyNodesList* nodes;

};

#endif