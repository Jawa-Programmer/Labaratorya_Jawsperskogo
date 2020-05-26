#ifndef MY_GRAPH_H
#define MY_GRAPH_H


enum MyGraphErrors { MG_NO_ERROR, MG_COORDS_BUSY, MG_COORDS_NOT_FOUNDED, MG_ALREADY_INCIDENT };

struct MyGraphNode;
struct MyNodesList;
struct MyNodesStack;
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

//стурктура хранит пару (вершина, список смежности). Програмная реализация стека позволяет избежать рекурсивного вызова функции поиска в глубину.
struct MyNodesStack {
	MyGraphNode* node;
	MyNodesList* incident;
	MyNodesStack* next;
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
	MyGraphErrors addIncident(MyGraphNode* toAdd);
	//функция разрывает исходящую связь из данного узла в узел toRemove
	MyGraphErrors removeIncident(MyGraphNode* toRemove);
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
	//метод добавляет в граф вершину с координатами x и y. Если координаты уже заняты, то вернет COORDS_BUSY, иначе NO_ERROR
	MyGraphErrors addNode(int x, int y, bool WF = false);
	//метод удаляет из графа вершину с координатами x и y. Если координаты не заняты, то вернет COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors removeNode(int x, int y, bool WF = false);
	//метод добавляет дугу из вершины с координатами (x1,y1) в вершину с коодинатами с (x2,y2). Если хотя бы оди из координат некорректны то веренёт COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors addArc(int x1, int y1, int x2, int y2, bool WF = false);
	//метод удаляет дугу из вершины с координатами (x1,y1) в вершину с коодинатами с (x2,y2). Если хотя бы оди из координат некорректны то веренёт COORDS_NOT_FOUNDED, иначе NO_ERROR
	MyGraphErrors removeArc(int x1, int y1, int x2, int y2, bool WF = false);
	//процедура выводит граф в виде матрицы смежности в файл log.txt и, по желанию, в консоль
	void printToConsole(bool toConsol = false);
	//метод возвращает указатель на список указателей на списки вершин, принадлежащих облостям сильной связности исходного графа. (список списков вершин, вершины из одного списка пренадлежать одной облости сильной связности) Алгоритм вроде бы полностью рабочий:)
	MyList<MyNodesList>* StronglyConnected();
	//данная процедура производит сборку мусора в файле графа (удаляет записи, которые помечены недействительными, освобождая тем память)
	void GC();
	//процедура генерирует случаную дугу между двумя (может случится и так, что новая дуга не появится вовсе)
	void randomArc(bool WF = false);
	inline int getArcsCount() { return countArc; }
	inline int getNodesCount() { return countNode; }
private:
	const char* filename;
	//счетчик, используемы при подсчете времени выхода из вершины графа.
	int timer = 0;
	//список всех вершин графа
	MyNodesList* nodes = nullptr;
	int countNode = 0;
	int countArc = 0;
	//Функция разрывает вся входящие и исходящие связи
	void clearArcs(MyGraphNode* node, bool WF = false);

	//Очищает порядок от прошлого обхода в глубину
	void cleanN();
	//обход в глубину по инвертированному графу. По завершении обхода в вершинах будет время выхода
	void DFB();
	//обход в глубину по прямому графу. Если в него передать отсортированный по времени обратного обхода список вершин, то возвращаемое множество - список списков сильной связности.
	MyList<MyNodesList>* iDFB(MyNodesList* sorted);
	//возвращает список вершин, отсортированный по возрастанию времени выхода
	MyNodesList* sort(MyNodesList*);
	//записывает в конец файла новую дугу
	void wrirteArc(int x1, int y1, int x2, int y2);

	//помечает дугу как удаленную
	void clearArc(int x1, int y1, int x2, int y2);

};

#endif