
#ifndef LLRBTree_h
#define LLRBTree_h

#include <stdint.h>
#include <fstream>

enum LLRB_ERRORS { LLRB_NO_ERROR, LLRB_KEY_IS_TAKEN, LLRB_NO_KEY, LLRB_NO_OPERATIONS, LLRB_INCORRECT_FILEPATH };
enum Color { BLACK, RED };
/// <summary>
/// Узел дерева. Хранит ключ, значение и служебную информацию (цвета, родителя, потомков)
/// </summary>
struct Node {
	int key;
	const char* info;
	Color color;
	Node* parent, * left, * right;
	void rotate_left();
	void rotate_right();
	void print(std::ofstream*);
	void clear();
	Node* getRoot();
	Node(int k, const char* inf) {
		key = k;
		info = inf;
		color = RED;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}
	/// возвращает дедушку
	inline Node* getGrandparent()
	{
		if (parent != nullptr)return parent->parent;
	}
	/// возвращает дядю
	inline Node* getUncle()
	{
		Node* gp = getGrandparent();
		if (gp != nullptr)
			return gp->left == parent ? gp->right : gp->left;
		return nullptr;
	}
	/// возвращает братика
	inline Node* getBro()
	{
		return (parent->left == this) ? parent->right : parent->left;
	}
	/// возвращает элемент, с наименьшим ключем, большим чем у текущего элемента
	inline Node* getNext()
	{
		if (right == nullptr)return this;
		Node* nxt = right;
		while (nxt->left)
		{
			nxt = nxt->left;
		}
		return nxt;
	}
};

/// <summary>
/// Класс Красно-черного дерева. Содержит интерфейс, позволяющий добавлять, искать, удалять элементы и выводить содержимое в стандартный поток в виде таблицы
/// </summary>
/// <remarks>
/// Данный класс содержит интерфейс, позволяющий добавлять, искать, удалять элементы и выводить содержимое в стандартный поток в виде таблицы
/// </remarks>
class LLRBTree
{
public:
	LLRBTree();
	/// <summary>
	/// Данный метод позваляет добавить новый элемент с ключем key и информацией *info в дерево
	/// </summary>
	/// <returns>
	/// Метод возвращает <see cref="NO_ERROR"/>, если добавление прошло успешно и <see cref="KEY_IS_TAKEN"/>, если элемент с таким ключем уже есть в дереве
	/// </returns>
	LLRB_ERRORS add(int, const char*);
	LLRB_ERRORS remove(int);
	LLRB_ERRORS find(int, char**);
	LLRB_ERRORS printAsTable();
	LLRB_ERRORS printAsTree();
	LLRB_ERRORS readFromFile(const char*);
	~LLRBTree()
	{
		if (root)
			root->clear();
		root = nullptr;
	}
private:
	Node* root;
	/// Ищет наибольший элемент, чей ключ непревосходит заданный
	Node* getPrevEl(int);
	/// вставка как в обычное дерево
	Node* insert(int, const char*);
	/// коррекция вставленного (случай вставки в корень дерева - перекрашиваем красное в черное)
	void correct_case1(Node* n);
	/// коррекция вставленного (проверка, если родитель красный, то мы нарушили свойство "потомки красного - черные")
	void correct_case2(Node* n);
	/// коррекция вставленного (родитель и дядя красные. Что бы дедушка случайно не нарушил свойство "корень черный" мы применяем к нему случай 1)
	void correct_case3(Node* n);
	/// коррекция вставленного (родитель красный, а дядя черный. Лечение: выполнить поворот и продолжить к случаю 5  (сторона поворота зависит от текущего положения вставляемого элемента))
	void correct_case4(Node* n);
	/// коррекция вставленного (Родитель красный, дядя черный. Если нарушено какое-либо свойство, просто повернуть вокруг дедушки (сторона поворота зависит от текущего положения вставляемого элемента))
	void correct_case5(Node* n);

	/// "безопастное" удаление. случай нахождения в корне. Просто удаляем единственный элемент и не паримся
	void delete_case1(Node* n);
	/// "безопастное" удаление. случай красного брата: меняем цвета родителя и брата, делаем поворот
	void delete_case2(Node* n);
	/// "безопастное" удаление. так получилось, что отец черный, брат черный, племянники тоже черные, о̶д̶и̶н̶ ̶я̶ ̶б̶е̶л̶ы̶й̶  тогда красим брата в красный. Мы нарушили баланс (черная высота) и рекурсивно исравляем его, применяя эти же методы к родителю
	void delete_case3(Node* n);
	/// "безопастное" удаление. красный папа, но брат и племянники ̶н̶и̶г̶е̶р̶ы̶  меняем цвета брата и родителя. так как был удален черный узел и мы уменьшили черную высоту нового брата, баланс сохранен
	void delete_case4(Node* n);
	/// "безопастное" удаление. бро черный, но дети у него разноцветные, тогда вращаем дерево вокруг брата, и идем дальше
	void delete_case5(Node* n);
	/// "безопастное" удаление. 
	void delete_case6(Node* n);
	std::ofstream fout;

	void PrintTree(Node* q, long n);
};
#endif 

