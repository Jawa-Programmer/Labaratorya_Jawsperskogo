#include "LLRBTree.h"

LLRB_ERRORS LLRBTree::add(int key, const char* info)
{
	if (root == nullptr)
	{
		root = new Node(key, info);
		root->red = 0;
		root->black = 1;
		return NO_ERROR;
	}
	Node* prev = getPrevEl(key);
	if (prev->key == key)
		return KEY_IS_TAKEN;
	if (prev->black)
	{
		if (prev->key < key)
			if (prev->right)return KEY_IS_TAKEN;
			else prev->right = new Node(key, info);
		else
		{
			if (prev->left)return KEY_IS_TAKEN;
			else prev->left = new Node(key, info);
		}
	}
	return NO_ERROR;
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

Node* Node::rotate_left()
{
	struct Node* pivot = this->right;

	pivot->parent = this->parent; /* при этом, возможно, pivot становится корнем дерева */
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
	return this->getRoot();
}

Node* Node::rotate_right()
{
	struct Node* pivot = this->left;

	pivot->parent = this->parent; /* при этом, возможно, pivot становится корнем дерева */
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
	return this->getRoot();
}
