#include "LLRBTree.h"
#include <iostream>
#include <fstream>

LLRBTree::LLRBTree()
{
	root = nullptr;
}

LLRB_ERRORS LLRBTree::find(int key, char** data)
{
	Node* prev = getPrevEl(key);
	if (prev->key == key) {
		strcpy_s(*data, sizeof(char) * 20, prev->info);
		return LLRB_NO_ERROR;
	}
	return LLRB_NO_KEY;
}

LLRB_ERRORS LLRBTree::printAsTable()
{
	fout.open("tableout.txt");
	if (root)root->print(&fout);
	fout.close();
	return LLRB_NO_ERROR;
}
LLRB_ERRORS LLRBTree::printAsTree()
{
	fout.open("treeout.txt");
	PrintTree(root, 0);
	fout.close();
	return LLRB_NO_ERROR;
}
LLRB_ERRORS LLRBTree::readFromFile(const char* filepath)
{
	if (root)
		root->clear();
	root = nullptr;
	std::ifstream fin(filepath);
	if (!fin)return LLRB_INCORRECT_FILEPATH;
	while (!fin.eof())
	{
		int key; char* data = new char[20];
		fin >> key >> data;
		add(key, data);
	}

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
LLRB_ERRORS LLRBTree::remove(int key)
{
	Node* todel = getPrevEl(key);
	if (todel->key != key)return LLRB_NO_KEY;

	delete[] todel->info;
	todel->info = nullptr;
	Node* next = todel->getNext();
	if (next != todel)
	{
		todel->info = next->info;
		todel->key = next->key;
	}
	todel = next;
	if (todel == root)
	{
		delete todel;
		root = nullptr;
		return LLRB_NO_ERROR;
	}
	Node* N = nullptr;
	if (todel->left)
	{
		N = todel->left;
		N->parent = todel->parent;
		if (todel->parent)	todel->parent->right = N;
		else root = N;
	}
	else if (todel->right)
	{
		N = todel->right;
		N->parent = todel->parent;
		if (todel->parent) todel->parent->left = N;
		else root = N;
	}
	else
	{
		if (todel->parent)
		{
			if (todel->parent->left == todel) todel->parent->left = nullptr;
			else todel->parent->right = nullptr;
		}
	}
	delete todel;
	delete_case1(N);

	return LLRB_NO_ERROR;
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

void LLRBTree::delete_case1(Node* n)
{
	if (n && n->parent)delete_case2(n);
}

void LLRBTree::delete_case2(Node* n)
{
	Node* s = n->getBro();
	if (s->color == RED) {
		n->parent->color = RED;
		s->color = BLACK;
		if (n == n->parent->left)
			n->parent->rotate_left();
		else
			n->parent->rotate_right();
	}
	delete_case3(n);
}

void LLRBTree::delete_case3(Node* n)
{
	Node* s = n->getBro();

	if ((n->parent->color == BLACK) &&
		(s->color == BLACK) &&
		(s->left->color == BLACK) &&
		(s->right->color == BLACK)) {
		s->color = RED;
		delete_case1(n->parent);
	}
	else
		delete_case4(n);
}
void LLRBTree::delete_case4(Node* n)
{
	Node* s = n->getBro();

	if ((n->parent->color == RED) &&
		(s->color == BLACK) &&
		(s->left->color == BLACK) &&
		(s->right->color == BLACK)) {
		s->color = RED;
		n->parent->color = BLACK;
	}
	else
		delete_case5(n);
}
void LLRBTree::delete_case5(Node* n)
{
	Node* s = n->getBro();

	if (s->color == BLACK) {
		if ((n == n->parent->left) &&
			(s->right->color == BLACK) &&
			(s->left->color == RED)) {
			s->color = RED;
			s->left->color = BLACK;
			s->rotate_right();
		}
		else if ((n == n->parent->right) &&
			(s->left->color == BLACK) &&
			(s->right->color == RED)) {
			s->color = RED;
			s->right->color = BLACK;
			s->rotate_left();
		}
	}
	delete_case6(n);
}
void LLRBTree::delete_case6(Node* n)
{
	Node* s = n->getBro();

	s->color = n->parent->color;
	n->parent->color = BLACK;

	if (n == n->parent->left) {
		s->right->color = BLACK;
		n->parent->rotate_left();
	}
	else {
		s->left->color = BLACK;
		n->parent->rotate_right();
	}
}
void LLRBTree::PrintTree(Node* q, long n)
{
	if (q)
	{
		PrintTree(q->right, n + 5);
		for (int i = 0; i < n - 5; i++)
			if (i % 5 == 0) {
				//		std::cout << "|";
				fout << "|";
			}
			else {
				//		std::cout << " ";
				fout << " ";
			}
		int a = n - 4;
		if (a < 0)a = 0;
		//	std::cout << "|";
		fout << "|";
		for (int i = a; i < n; i++) {
			//		std::cout << "-";
			fout << "-";
		}
		//	std::cout << q->key /*<< " " << q->info*/ << std::endl;
		fout << q->key << " " << (q->color == RED ? "R" : "B") /*<< " " << q->info*/ << std::endl;
		PrintTree(q->left, n + 5);
	}
}
void Node::rotate_left()
{
	struct Node* pivot = this->right;

	pivot->parent = this->parent;
	if (this->parent != nullptr) {
		if (this->parent->left == this)
			this->parent->left = pivot;
		else
			this->parent->right = pivot;
	}

	this->right = pivot->left;
	if (pivot->left != nullptr)
		pivot->left->parent = this;

	this->parent = pivot;
	pivot->left = this;
}

void Node::rotate_right()
{
	struct Node* pivot = this->left;

	pivot->parent = this->parent;
	if (this->parent != nullptr) {
		if (this->parent->left == this)
			this->parent->left = pivot;
		else
			this->parent->right = pivot;
	}

	this->left = pivot->right;
	if (pivot->right != nullptr)
		pivot->right->parent = this;

	this->parent = pivot;
	pivot->right = this;
}

void Node::print(std::ofstream* fout)
{
	if (left != nullptr)left->print(fout);
	//	std::cout << key << " : " << info << ".\tdebug info: color = " << (color == RED ? " Red" : " Black");
	(*fout) << key << " : " << info << ".\tdebug info: color = " << (color == RED ? " Red" : " Black");
	if (parent) {
		//std::cout << ". parent: " << parent->key;
		(*fout) << ". parent: " << parent->key;
	}
	else {
		//	std::cout << ". parent: NULL"; 
		*fout << ". parent: NULL";
	}
	//	std::cout << '\n';
	*fout << '\n';
	if (right != nullptr)right->print(fout);
}

void Node::clear()
{
	if (left)left->clear();
	if (right)right->clear();
	left = nullptr;
	right = nullptr;
	//delete[] info;
	delete this;
}

Node* Node::getRoot()
{
	Node* rot = this;
	while (rot->parent != nullptr)
		rot = rot->parent;
	return rot;
}

