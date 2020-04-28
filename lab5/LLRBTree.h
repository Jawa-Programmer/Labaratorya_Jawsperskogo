
#ifndef LLRBTree_h
#define LLRBTree_h

#include <stdint.h>

enum LLRB_ERRORS { LLRB_NO_ERROR, LLRB_KEY_IS_TAKEN, LLRB_NO_KEY };
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
	void print();
	Node* getRoot();
	Node(int k, const char* inf) {
		key = k;
		info = inf;
		color = RED;
		left = nullptr;
		right = nullptr;
		parent = nullptr;
	}
	inline Node* getGrandparent()
	{
		if (parent != nullptr)return parent->parent;
	}
	inline Node* getUncle()
	{
		Node* gp = getGrandparent();
		if (gp != nullptr)
			return gp->left == parent ? gp->right : gp->left;
		return nullptr;
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
	LLRB_ERRORS find(int,const char**);
	LLRB_ERRORS printAsTable();
	LLRB_ERRORS printAsTree();
private:
	Node* root;
	Node* getPrevEl(int);
	Node* insert(int, const char*);
	void correct_case1(Node* n);
	void correct_case2(Node* n);
	void correct_case3(Node* n);
	void correct_case4(Node* n);
	void correct_case5(Node* n);
};
#endif 

