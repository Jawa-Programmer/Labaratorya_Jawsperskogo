#include "LLRBTree.h"

LLRB_ERRORS LLRBTree::add(int key,const char* info)
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
