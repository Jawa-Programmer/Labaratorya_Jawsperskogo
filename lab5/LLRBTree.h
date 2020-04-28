
#ifndef LLRBTree_h
#define LLRBTree_h

#include <stdint.h>

enum LLRB_ERRORS { LLRB_NO_ERROR, LLRB_KEY_IS_TAKEN, LLRB_NO_KEY };
enum Color { BLACK, RED };
/// <summary>
/// ���� ������. ������ ����, �������� � ��������� ���������� (�����, ��������, ��������)
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
/// ����� ������-������� ������. �������� ���������, ����������� ���������, ������, ������� �������� � �������� ���������� � ����������� ����� � ���� �������
/// </summary>
/// <remarks>
/// ������ ����� �������� ���������, ����������� ���������, ������, ������� �������� � �������� ���������� � ����������� ����� � ���� �������
/// </remarks>
class LLRBTree
{
public:
	LLRBTree();
	/// <summary>
	/// ������ ����� ��������� �������� ����� ������� � ������ key � ����������� *info � ������
	/// </summary>
	/// <returns>
	/// ����� ���������� <see cref="NO_ERROR"/>, ���� ���������� ������ ������� � <see cref="KEY_IS_TAKEN"/>, ���� ������� � ����� ������ ��� ���� � ������
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

