#ifndef MY_GRAPH_H
#define MY_GRAPH_H


enum MyGraphErrors { MG_NO_ERROR, MG_COORDS_BUSY, MG_COORDS_NOT_FOUNDED, MG_ALREADY_INCIDENT };

struct MyGraphNode;
struct MyNodesList;
struct MyNodesStack;
template<class T> struct MyList;
class MyGraph;

//��������� ������ ����� �����
struct MyNodesList
{
	//����������� ������, ������ ��� �������� ������ ����� �����
	void clear();
	//������� ������, �� ������ ����������
	void clean();
	//��������������� ����� � ������� ���������� ����� ������
	void print();
	//������ �� ������� ������� �����
	MyGraphNode* info = nullptr;
	//������ �� ��������� ������� ������
	MyNodesList* next = nullptr;
	//����� ������� ����� � ������ �� ��� �����������
	MyGraphNode* find(int x, int y);
	//���������� �������� ������� ����� � ����� ������ (��� ���� ����������� ���������� �� �����������)
	MyGraphErrors add(MyGraphNode* toAdd);
	//�������� �������� ������� ����� �� ������
	MyGraphErrors remove(MyGraphNode* toDel);
};

//��������� ������ ���� (�������, ������ ���������). ���������� ���������� ����� ��������� �������� ������������ ������ ������� ������ � �������.
struct MyNodesStack {
	MyGraphNode* node;
	MyNodesList* incident;
	MyNodesStack* next;
};

//��������� ���� �����. �������� ����, ������ �������� � ��������� ���
struct MyGraphNode
{
public:
	//���������� �����
	int x = 0, y = 0;
	//"�����" ������ �� ������� (����� ��� ������ � �������) 
	int N = -1;
	//������� � ��������� � ������� ��� ������ ������
	bool forvis = false;
	//������� � ��������� ������� ��� ������ � �������� �����������
	bool Visited = false;
	//����������, ������������ ������� �������� ������ (������� ������� �� ���������, �� � ���� ����������� �����)
	~MyGraphNode();
	//��������� ����
	MyNodesList* incidents = nullptr;
	//�������� ����
	MyNodesList* incoming = nullptr;
	//������� ��������� ����� ��������� ����� �� ������� ���� � ���� toAdd
	MyGraphErrors addIncident(MyGraphNode* toAdd);
	//������� ��������� ��������� ����� �� ������� ���� � ���� toRemove
	MyGraphErrors removeIncident(MyGraphNode* toRemove);
	//�������� ���������. ���� � ������ ���������� ����������, �� true
	bool operator==(MyGraphNode b);
	//��������� �������, � ������� ����� �� ����� ������ �����
private:
	//������� ��������� �������� ����� �� ToAdd � ������� ����
	void addIncoming(MyGraphNode* toAdd);
	//������� ��������� �������� ����� �� ToAdd � ������� ����
	void removeIncoming(MyGraphNode* toRemove);
};

//��������� ��������� �������� ������ ��������� ������ ������������� ����.
template<class T> struct MyList
{
	//��������� �� ����������
	T* info = nullptr;
	//��������� �� ��������� ������� � ������.
	MyList* next = nullptr;
};

//���� ����� ���������� ��������� ��� ������ � ������ � ������� ��� ������������ ����
class MyGraph
{
public:
	//�����������. ������� ���� �� �����. ���� ����� �� ����������, �� ���������.
	MyGraph(const char* filepath);
	//����������, ��������� ����������� ������� ������ ������
	~MyGraph();
	//����� ��������� � ���� ������� � ������������ x � y. ���� ���������� ��� ������, �� ������ COORDS_BUSY, ����� NO_ERROR
	MyGraphErrors addNode(int x, int y, bool WF = false);
	//����� ������� �� ����� ������� � ������������ x � y. ���� ���������� �� ������, �� ������ COORDS_NOT_FOUNDED, ����� NO_ERROR
	MyGraphErrors removeNode(int x, int y, bool WF = false);
	//����� ��������� ���� �� ������� � ������������ (x1,y1) � ������� � ����������� � (x2,y2). ���� ���� �� ��� �� ��������� ����������� �� ������ COORDS_NOT_FOUNDED, ����� NO_ERROR
	MyGraphErrors addArc(int x1, int y1, int x2, int y2, bool WF = false);
	//����� ������� ���� �� ������� � ������������ (x1,y1) � ������� � ����������� � (x2,y2). ���� ���� �� ��� �� ��������� ����������� �� ������ COORDS_NOT_FOUNDED, ����� NO_ERROR
	MyGraphErrors removeArc(int x1, int y1, int x2, int y2, bool WF = false);
	//��������� ������� ���� � ���� ������� ��������� � ���� log.txt �, �� �������, � �������
	void printToConsole(bool toConsol = false);
	//����� ���������� ��������� �� ������ ���������� �� ������ ������, ������������� �������� ������� ��������� ��������� �����. (������ ������� ������, ������� �� ������ ������ ������������ ����� ������� ������� ���������) �������� ����� �� ��������� �������:)
	MyList<MyNodesList>* StronglyConnected();
	//������ ��������� ���������� ������ ������ � ����� ����� (������� ������, ������� �������� �����������������, ���������� ��� ������)
	void GC();
	//��������� ���������� �������� ���� ����� ����� (����� �������� � ���, ��� ����� ���� �� �������� �����)
	void randomArc(bool WF = false);
	inline int getArcsCount() { return countArc; }
	inline int getNodesCount() { return countNode; }
private:
	const char* filename;
	//�������, ����������� ��� �������� ������� ������ �� ������� �����.
	int timer = 0;
	//������ ���� ������ �����
	MyNodesList* nodes = nullptr;
	int countNode = 0;
	int countArc = 0;
	//������� ��������� ��� �������� � ��������� �����
	void clearArcs(MyGraphNode* node, bool WF = false);

	//������� ������� �� �������� ������ � �������
	void cleanN();
	//����� � ������� �� ���������������� �����. �� ���������� ������ � �������� ����� ����� ������
	void DFB();
	//����� � ������� �� ������� �����. ���� � ���� �������� ��������������� �� ������� ��������� ������ ������ ������, �� ������������ ��������� - ������ ������� ������� ���������.
	MyList<MyNodesList>* iDFB(MyNodesList* sorted);
	//���������� ������ ������, ��������������� �� ����������� ������� ������
	MyNodesList* sort(MyNodesList*);
	//���������� � ����� ����� ����� ����
	void wrirteArc(int x1, int y1, int x2, int y2);

	//�������� ���� ��� ���������
	void clearArc(int x1, int y1, int x2, int y2);

};

#endif