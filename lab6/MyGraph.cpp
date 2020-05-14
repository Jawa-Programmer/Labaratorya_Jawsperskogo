#include "MyGraph.h"

MyNodesList* MyNodesList::find(int x, int y)
{
	MyNodesList* tmp = this;
	while (tmp->next != nullptr)
	{
		if (tmp->info->x == x && tmp->info->y == y)return tmp;
	}
	return nullptr;
}
MyGraphErrors MyNodesList::add(MyGraphNode* toAdd)
{
	MyNodesList* tmp = this;
	while (tmp->next != nullptr)
	{
		if (*tmp->info == *toAdd)return MG_COORDS_BUSY;
		tmp = tmp->next;
	}
	MyNodesList* nw = new MyNodesList;
	nw->info = toAdd;
	nw->next = tmp->next;
	tmp->next = nw;
	return MG_NO_ERROR;
}
MyGraphErrors MyNodesList::remove(MyGraphNode* toDel)
{
	MyNodesList* tmp = this;
	while (tmp)
	{
		if (*tmp->info == *toDel)
		{
			MyNodesList* tmp2 = tmp->next;
			if (tmp2)
			{
				tmp->info = tmp2->info;
				tmp->next = tmp2->next;
				delete tmp2;
			}
		}
		tmp = tmp->next;
	}
	return MG_NO_ERROR;
}

void MyGraphNode::addIcident(MyGraphNode* toAdd)
{

	MyNodesList* tmp = new MyNodesList;
	tmp->info = toAdd;
	tmp->next = incidents;
	incidents = tmp;

}

void MyGraphNode::removeIncident(MyGraphNode* toRemove)
{
	if (incidents)
		if (incidents->info == toRemove) { MyNodesList* tmp = incidents; incidents = incidents->next; delete tmp; }
		else incidents->remove(toRemove);
}

bool MyGraphNode::operator==(MyGraphNode b)
{
	return x == b.x && y == b.y;
}

MyGraphErrors MyGraph::addNode(int x, int y)
{
	if (nodes) {
		if (nodes->find(x, y))return MG_COORDS_BUSY;
		MyGraphNode* nw = new MyGraphNode;
		nw->x = x;
		nw->y = y;
		nodes->add(nw);
	}
	else {
		nodes = new MyNodesList;
		nodes->info = new MyGraphNode;
		nodes->info->x = x;
		nodes->info->y = y;
	}
	return MG_NO_ERROR;
}
