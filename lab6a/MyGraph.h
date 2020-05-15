#ifndef MY_GRAPH_H
#define MY_GRAPH_H


enum MyGraphErrors { MG_NO_ERROR, MG_COORDS_BUSY, MG_COORDS_NOT_FOUNDED };

struct MyGraphNode;
struct MyNodesList;
class MyGraph;

///��������� ������ ����� �����
struct MyNodesList
{
	///������ �� ������� ������� �����
	MyGraphNode* info = nullptr;
	///������ �� ��������� ������� ������
	MyNodesList* next = nullptr;
	///����� ������� ����� � ������ �� ��� �����������
	MyGraphNode* find(int x, int y);
	///���������� �������� ������� ����� � ����� ������ (��� ���� ����������� ���������� �� �����������)
	MyGraphErrors add(MyGraphNode* toAdd);
	///�������� �������� ������� ����� �� ������
	MyGraphErrors remove(MyGraphNode* toDel);
};
///��������� ���� �����. �������� ����, ������ �������� � ��������� ���
struct MyGraphNode
{
public:
	///���������� �����
	int x = 0, y = 0;
	///��������� ����
	MyNodesList* incidents = nullptr;
	///�������� ����
	MyNodesList* incoming = nullptr;
	///������� ��������� ����� ��������� ����� �� ������� ���� � ���� toAdd
	void addIncident(MyGraphNode* toAdd);
	///������� ��������� ��������� ����� �� ������� ���� � ���� toRemove
	void removeIncident(MyGraphNode* toRemove);
	///�������� ���������. ���� � ������ ���������� ����������, �� true
	bool operator==(MyGraphNode b);
	///��������� �������, � ������� ����� �� ����� ������ �����
private:
	///������� ��������� �������� ����� �� ToAdd � ������� ����
	void addIncoming(MyGraphNode* toAdd);
	///������� ��������� �������� ����� �� ToAdd � ������� ����
	void removeIncoming(MyGraphNode* toRemove);
};
///���� ����� ���������� ��������� ��� ������ � ������ � ������� ��� ������������ ����
class MyGraph
{
public:
	///������� ��������� � ���� ������� � ������������ x � y. ���� ���������� ��� ������, �� ������ COORDS_BUSY, ����� NO_ERROR
	MyGraphErrors addNode(int x, int y);
	///������� ��������� ���� �� ������� � ������������ (x1,y1) � ������� � ����������� � (x2,y2). ���� ���� �� ��� �� ��������� ����������� �� ������ COORDS_NOT_FOUNDED, ����� NO_ERROR
	MyGraphErrors addArc(int x1, int y1, int x2, int y2);
	///������� ������� ���� �� ������� � ������������ (x1,y1) � ������� � ����������� � (x2,y2). ���� ���� �� ��� �� ��������� ����������� �� ������ COORDS_NOT_FOUNDED, ����� NO_ERROR
	MyGraphErrors removeArc(int x1, int y1, int x2, int y2);
private:
	///������ ���� ������ �����
	MyNodesList* nodes = nullptr;

};

#endif