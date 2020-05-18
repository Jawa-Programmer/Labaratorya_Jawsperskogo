#ifndef MY_GRAPH_H
#define MY_GRAPH_H


enum MyGraphErrors { MG_NO_ERROR, MG_COORDS_BUSY, MG_COORDS_NOT_FOUNDED };

struct MyGraphNode;
struct MyNodesList;
template<class T> struct MyList;
class MyGraph;

//структура списка узлов графа
struct MyNodesList
{
	//освобождает память, удаляя все элементы списка далее этого
	void clear();
	//удаляет список, не удаляя информации
	void clean();
	//форматированный вывод в консоль содержания всего списка
	void print();
	//ссылка на текущую вершину графа
	MyGraphNode* info = nullptr;
	//ссылка на следующий элемент списка
	MyNodesList* next = nullptr;
	//поиск вершины графа в списке по его координатам
	MyGraphNode* find(int x, int y);
	//добавления элемента вершины графа в конец списка (при этом совподающие координаты не допускаются)
	MyGraphErrors add(MyGraphNode* toAdd);
	//удаление элемента вершины графа из списка
	MyGraphErrors remove(MyGraphNode* toDel);
};
//структура узла графа. Содержит ключ, список входящих и исходящих дуг
struct MyGraphNode
{
public:
	//координаты графа
	int x = 0, y = 0;
	//"время" выхода из вершины (нужно при обходе в глубину) 
	int N = -1;
	//отметка о вхождении в вершину при прямом обходе
	bool forvis = false;
	//отметка о посещении вершины при обходе в обратном направлении
	bool Visited = false;
	//деструктор, высвобождает занятую вершиной память (смежные вершины не удоляются, но с ними разрыватеся связь)
	~MyGraphNode();
	//исходящие дуги
	MyNodesList* incidents = nullptr;
	//входящие дуги
	MyNodesList* incoming = nullptr;
	//функция добавляет новую исходящую связь из данного узла в узел toAdd
	void addIncident(MyGraphNode* toAdd);
	//функция разрывает исходящую связь из данного узла в узел toRemove
	void removeIncident(MyGraphNode* toRemove);
	//Оператор сравнения. Если у вершин одинаковые координаты, то true
	bool operator==(MyGraphNode b);
	//служебные функции, к которым лучше не иметь доступ извне
private:
	//Функция добавляет входящую связь из ToAdd в текущий узел
	void addIncoming(MyGraphNode* toAdd);
	//Функция разрывает входящую связь из ToAdd в текущий узел
	void removeIncoming(MyGraphNode* toRemove);
};

//шаблонная структура содержит список элементов одного произвольного типа.
template<class T> struct MyList
{
	//указатель на информацию
	T* info = nullptr;
	//указатель на следующий элемент в списке.
	MyList* next = nullptr;
};

//Этот класс организует интерфейс для работы с графом в удобном для пользователя виде
class MyGraph
{
public:
	//конструктор. создает граф из файла. Если файла не существует, он создается.
	MyGraph(const char* filepath);
	//деструктор, полностью освобождает занятую графом память
	~MyGraph();
	//функция добавляет в граф вершину с координатами x и y. Если координаты уже заняты, то вернет COORDS_BUSY, иначе NO_ERROR
	MyGraphErrors addNode(int x, int y, bool WF = false);
	//функция удаляет из графа вершину с координатами x и y. Если координаты не заняты, то вернет COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors removeNode(int x, int y, bool WF = false);
	//Функция добавляет дугу из вершины с координатами (x1,y1) в вершину с коодинатами с (x2,y2). Если хотя бы оди из координат некорректны то веренёт COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors addArc(int x1, int y1, int x2, int y2, bool WF = false);
	//Функция удаляет дугу из вершины с координатами (x1,y1) в вершину с коодинатами с (x2,y2). Если хотя бы оди из координат некорректны то веренёт COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors removeArc(int x1, int y1, int x2, int y2, bool WF = false);
	//Выводит граф в консоль в виде матрицы смежности
	void printToConsole();
	//Возвращает указатель на список указателей на вершины из сильносвязных компонентов исходного графа. Алгоритм вроде бы полностью рабочий) Единственный минус: из вершин, невошедших ни в одну компоненту связности, создает отдельные списки из одного элемента
	MyList<MyNodesList>* StronglyConnected();
private:
	const char* filename;
	//счетчик, используемы при подсчете времени выхода из вершины графа.
	int timer = 0;
	//список всех вершин графа
	MyNodesList* nodes = nullptr;


	//Функция разрывает вся входящие и исходящие связи
	void clearArcs(MyGraphNode* node, bool WF = false);

	//Очищает порядок от прошлого обхода в глубину
	void cleanN();
	//обход в глубину по прямому графу. По завершении обхода в вершинах будет время выхода
	void DFB();
	//рекурсивная функция обхода. Служебная, вызывается из DFB.
	void DFBr(MyGraphNode* node);
	//обход в глубину по инвертированному графу. По завершении обхода в графе будут пометки о том, что вершины уже пройдены, пройденные вершины будут добавленны в переданный граф
	void iDFB(MyGraphNode* node, MyNodesList* graph);
	//возвращает список вершин, отсортированный по возрастанию времени выхода
	MyNodesList* sort(MyNodesList*);
	//записывает в конец файла новую дугу
	void wrirteArc(int x1, int y1, int x2, int y2);

	//помечает дугу как удаленную
	void clearArc(int x1, int y1, int x2, int y2);

};

#endif