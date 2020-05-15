#include "MyGraph.h"

MyGraphNode* MyNodesList::find(int x, int y)
{
	MyNodesList* tmp = this;
	while (tmp->next != nullptr)
	{
		if (tmp->info->x == x && tmp->info->y == y)return tmp->info;
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
	MyNodesList* tmp = this, * prev = nullptr;
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
			else if (prev) {
				delete tmp;
				prev->next = nullptr;
			}
			return MG_NO_ERROR;
		}
		prev = tmp;
		tmp = tmp->next;
	}
	return MG_COORDS_NOT_FOUNDED;
}

void MyGraphNode::addIncident(MyGraphNode* toAdd)
{
	if (incidents->find(toAdd->x, toAdd->y))return;
	MyNodesList* tmp = new MyNodesList;
	tmp->info = toAdd;
	tmp->next = incidents;
	incidents = tmp;
	toAdd->addIncoming(this);
}

void MyGraphNode::removeIncident(MyGraphNode* toRemove)
{

	if (incidents)
		if (incidents->info == toRemove) { MyNodesList* tmp = incidents; incidents = incidents->next; delete tmp; }
		else incidents->remove(toRemove);
	toRemove->removeIncoming(this);
}

void MyGraphNode::addIncoming(MyGraphNode* toAdd)
{
	if (incoming)incoming->add(toAdd);
	else
	{
		incoming = new MyNodesList;
		incoming->info = toAdd;
	}
}

void MyGraphNode::removeIncoming(MyGraphNode* toRemove)
{
	if (incoming)
	{
		if (*incoming->info == *toRemove) {
			MyNodesList* tmp = incoming->next;
			delete incoming;
			incoming = tmp;
		}
		else {
			incoming->remove(toRemove);
		}
	}
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

MyGraphErrors MyGraph::addArc(int x1, int y1, int x2, int y2)
{
	MyGraphNode* n1 = nodes->find(x1, y1);
	MyGraphNode* n2 = nodes->find(x2, y2);
	if (n1 && n2) {
		n1->addIncident(n2);
		return MG_NO_ERROR;
	}
	return MG_COORDS_NOT_FOUNDED;
}

MyGraphErrors MyGraph::removeArc(int x1, int y1, int x2, int y2)
{
	MyGraphNode* n1 = nodes->find(x1, y1);
	MyGraphNode* n2 = nodes->find(x2, y2);
	if (n1 && n2) {
		n1->removeIncident(n2);
		return MG_NO_ERROR;
	}
	return MG_COORDS_NOT_FOUNDED;
}
