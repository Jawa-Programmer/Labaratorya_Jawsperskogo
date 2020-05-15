#ifndef MY_GRAPH_H
#define MY_GRAPH_H


enum MyGraphErrors { MG_NO_ERROR, MG_COORDS_BUSY, MG_COORDS_NOT_FOUNDED };

struct MyGraphNode;
struct MyNodesList;
class MyGraph;

///структура списка узлов графа
struct MyNodesList
{
	///ссылка на текущую вершину графа
	MyGraphNode* info = nullptr;
	///ссылка на следующий элемент списка
	MyNodesList* next = nullptr;
	///поиск вершины графа в списке по его координатам
	MyGraphNode* find(int x, int y);
	///добавления элемента вершины графа в конец списка (при этом совподающие координаты не допускаются)
	MyGraphErrors add(MyGraphNode* toAdd);
	///удаление элемента вершины графа из списка
	MyGraphErrors remove(MyGraphNode* toDel);
};
///структура узла графа. Содержит ключ, список входящих и исходящих дуг
struct MyGraphNode
{
public:
	///координаты графа
	int x = 0, y = 0;
	///исходящие дуги
	MyNodesList* incidents = nullptr;
	///входящие дуги
	MyNodesList* incoming = nullptr;
	///функция добавляет новую исходящую связь из данного узла в узел toAdd
	void addIncident(MyGraphNode* toAdd);
	///функция разрывает исходящую связь из данного узла в узел toRemove
	void removeIncident(MyGraphNode* toRemove);
	///Оператор сравнения. Если у вершин одинаковые координаты, то true
	bool operator==(MyGraphNode b);
	///служебные функции, к которым лучше не иметь доступ извне
private:
	///Функция добавляет входящую связь из ToAdd в текущий узел
	void addIncoming(MyGraphNode* toAdd);
	///Функция разрывает входящую связь из ToAdd в текущий узел
	void removeIncoming(MyGraphNode* toRemove);
};
///Этот класс организует интерфейс для работы с графом в удобном для пользователя виде
class MyGraph
{
public:
	///функция добавляет в граф вершину с координатами x и y. Если координаты уже заняты, то вернет COORDS_BUSY, иначе NO_ERROR
	MyGraphErrors addNode(int x, int y);
	///Функция добавляет дугу из вершины с координатами (x1,y1) в вершину с коодинатами с (x2,y2). Если хотя бы оди из координат некорректны то веренёт COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors addArc(int x1, int y1, int x2, int y2);
	///Функция удаляет дугу из вершины с координатами (x1,y1) в вершину с коодинатами с (x2,y2). Если хотя бы оди из координат некорректны то веренёт COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors removeArc(int x1, int y1, int x2, int y2);
private:
	///список всех вершин графа
	MyNodesList* nodes = nullptr;

};

#endif