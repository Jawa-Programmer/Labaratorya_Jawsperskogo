#include "LLRBTree.h"
#include <iostream>

LLRBTree::LLRBTree()
{
	root = nullptr;
}

LLRB_ERRORS LLRBTree::find(int key, const char** data)
{
	Node* prev = getPrevEl(key);
	if (prev->key == key) {
		(*data) = prev->info;
		return LLRB_NO_ERROR;
	}

	return LLRB_NO_KEY;
}

LLRB_ERRORS LLRBTree::printAsTable()
{
	if (root)root->print();
	return LLRB_NO_ERROR;
}
LLRB_ERRORS LLRBTree::add(int k, const char* d)
{
	Node* add = insert(k, d);
	if (add) {
		correct_case1(add);
		root = add->getRoot();
		return LLRB_NO_ERROR;
	}
	return LLRB_KEY_IS_TAKEN;
}
Node* LLRBTree::insert(int key, const char* info)
{
	if (root == nullptr)
	{
		root = new Node(key, info);
		return root;
	}
	Node* prev = getPrevEl(key);
	if (prev->key == key)
		return nullptr;
	if (prev->key < key)
		if (prev->right)return nullptr;
		else {
			prev->right = new Node(key, info);
			prev->right->parent = prev;
			return prev->right;
		}
	else
	{
		if (prev->left)return nullptr;
		else {
			prev->left = new Node(key, info);
			prev->left->parent = prev;
			return prev->left;
		}
	}
}

Node* LLRBTree::getPrevEl(int key)
{
	Node* prev = root;
	while (1)
	{
		if (key > prev->key&& prev->right)
			prev = prev->right;
		else if (key < prev->key && prev->left)
			prev = prev->left;
		else break;
	}
	return prev;
}

void LLRBTree::correct_case1(Node* n)
{
	if (n->parent == nullptr)
		n->color = BLACK;
	else
		correct_case2(n);
}

void LLRBTree::correct_case2(Node* n)
{
	if (n->parent->color == BLACK)
		return;
	else
		correct_case3(n);
}

void LLRBTree::correct_case3(Node* n)
{
	struct Node* u = n->getUncle(), * g;

	if ((u != nullptr) && (u->color == RED)) {
		n->parent->color = BLACK;
		u->color = BLACK;
		g = n->getGrandparent();
		g->color = RED;
		correct_case1(g);
	}
	else {
		correct_case4(n);
	}
}

void LLRBTree::correct_case4(Node* n)
{
	struct Node* g = n->getGrandparent();

	if ((n == n->parent->right) && (n->parent == g->left)) {
		n->parent->rotate_left();
		n = n->left;
	}
	else if ((n == n->parent->left) && (n->parent == g->right)) {
		n->parent->rotate_right();
		n = n->right;
	}
	correct_case5(n);
}

void LLRBTree::correct_case5(Node* n)
{
	struct Node* g = n->getGrandparent();

	n->parent->color = BLACK;
	g->color = RED;
	if ((n == n->parent->left) && (n->parent == g->left)) {
		g->rotate_right();
	}
	else { /* (n == n->parent->right) && (n->parent == g->right) */
		g->rotate_left();
	}
}

void Node::rotate_left()
{
	struct Node* pivot = this->right;

	pivot->parent = this->parent; /* ��� ����, ��������, pivot ���������� ������ ������ */
	if (this->parent != NULL) {
		if (this->parent->left == this)
			this->parent->left = pivot;
		else
			this->parent->right = pivot;
	}

	this->right = pivot->left;
	if (pivot->left != NULL)
		pivot->left->parent = this;

	this->parent = pivot;
	pivot->left = this;
}

void Node::rotate_right()
{
	struct Node* pivot = this->left;

	pivot->parent = this->parent; /* ��� ����, ��������, pivot ���������� ������ ������ */
	if (this->parent != NULL) {
		if (this->parent->left == this)
			this->parent->left = pivot;
		else
			this->parent->right = pivot;
	}

	this->left = pivot->right;
	if (pivot->right != NULL)
		pivot->right->parent = this;

	this->parent = pivot;
	pivot->right = this;
}

void Node::print()
{
	if (left != nullptr)left->print();
	std::cout << key << " : " << info << (color == RED ? " (R)" : " (B)");
	if (parent)
		std::cout << " parent: " << parent->key;
	else std::cout << "parent: NULL";
	std::cout << '\n';
	if (right != nullptr)right->print();
}

Node* Node::getRoot()
{
	Node* rot = this;
	while (rot->parent != nullptr)
		rot = rot->parent;
	return rot;
}

