
#ifndef LLRBTree_h
#define LLRBTree_h

#include <stdint.h>

enum LLRB_ERRORS
{
	NO_ERROR, KEY_IS_TAKEN, NO_KEY
};
/// <summary>
/// ���� ������. ������ ����, �������� � ��������� ���������� (�����, ��������, ��������)
/// </summary>
struct Node {
	int key;
	const char* info;
	uint8_t black, red;
	int black_height = 1;
	Node* parent, * left, * right;
	Node(int k, const char* inf) {
		key = k;
		info = inf;
		red = 1;
		black = 0;
		black_height = 1;
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
	LLRB_ERRORS find(int, char*&);
	LLRB_ERRORS printAsTable();
	LLRB_ERRORS printAsTree();
	int getMax();
private:
	Node* root;
	Node* getPrevEl(int);

};
#endif 

