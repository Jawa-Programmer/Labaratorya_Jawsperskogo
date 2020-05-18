#include "MyGraph.h"
#include <fstream>
#include <iostream>


void MyNodesList::clear()
{
	MyNodesList* tmp1 = next;
	delete info;
	while (tmp1)
	{
		MyNodesList* tmp = tmp1->next;
		delete tmp1->info;
		delete tmp1;
		tmp1 = tmp;
	}
}

void MyNodesList::clean()
{
	MyNodesList* tmp1 = next;
	while (tmp1)
	{
		MyNodesList* tmp = tmp1->next;
		delete tmp1;
		tmp1 = tmp;
	}
}

void MyNodesList::print()
{
	MyNodesList* tmp = this;
	std::cout << "{";
	while (tmp)
	{
		std::cout << "(" << tmp->info->x << "," << tmp->info->y << "), ";
		tmp = tmp->next;
	}
	std::cout << "}\n";
}

MyGraphNode* MyNodesList::find(int x, int y)
{
	MyNodesList* tmp = this;
	while (tmp != nullptr)
	{
		if (tmp->info->x == x && tmp->info->y == y)return tmp->info;
		tmp = tmp->next;
	}
	return nullptr;
}
MyGraphErrors MyNodesList::add(MyGraphNode* toAdd)
{
	MyNodesList* tmp = this;
	if (*tmp->info == *toAdd)return MG_COORDS_BUSY;
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

MyGraphNode::~MyGraphNode()
{
	//	clearArcs();
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

void MyGraph::clearArcs(MyGraphNode* node, bool WF)
{
	while (node->incoming)
	{
		if (WF)clearArc(node->incoming->info->x, node->incoming->info->y, node->x, node->y);
		node->incoming->info->removeIncident(node);
	}
	while (node->incidents) {
		if (WF)clearArc(node->x, node->y, node->incidents->info->x, node->incidents->info->y);
		node->removeIncident(node->incidents->info);
	}
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

MyGraph::MyGraph(const char* filepath)
{
	filename = filepath;
	std::ifstream in(filepath);
	if (in.is_open()) {
		while (!in.eof()) {
			bool correct = false;
			int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
			in.read((char*)&correct, sizeof(correct));
			in.read((char*)&x1, sizeof(x1));
			in.read((char*)&y1, sizeof(y1));
			in.read((char*)&x2, sizeof(x2));
			in.read((char*)&y2, sizeof(y1));
			if (correct)
			{
				addNode(x1, y1);
				addNode(x2, y2);
				addArc(x1, y1, x2, y2);
			}
		}
		in.close();
	}
	else
	{
		std::ofstream out(filepath, std::ios::app);
		out.close();
	}
}

MyGraph::~MyGraph()
{
	nodes->clear();
	delete nodes;
	nodes = nullptr;
}

MyGraphErrors MyGraph::addNode(int x, int y, bool WF)
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

MyGraphErrors MyGraph::removeNode(int x, int y, bool WF)
{
	if (nodes) {
		if (nodes->info->x == x && nodes->info->y == y)
		{
			MyNodesList* tmp = nodes->next;
			clearArcs(nodes->info, WF);
			delete nodes;
			nodes = tmp;
			return MG_NO_ERROR;
		}
		else
		{
			MyGraphNode* toDel = nodes->find(x, y);
			if (toDel)
			{
				clearArcs(toDel, WF);
				return nodes->remove(toDel);
			}
		}
	}
	return MG_COORDS_NOT_FOUNDED;
}

MyGraphErrors MyGraph::addArc(int x1, int y1, int x2, int y2, bool WF)
{
	MyGraphNode* n1 = nodes->find(x1, y1);
	MyGraphNode* n2 = nodes->find(x2, y2);
	if (n1 && n2) {
		n1->addIncident(n2);
		if (WF)wrirteArc(x1, y1, x2, y2);
		return MG_NO_ERROR;
	}
	return MG_COORDS_NOT_FOUNDED;
}

MyGraphErrors MyGraph::removeArc(int x1, int y1, int x2, int y2, bool WF)
{
	MyGraphNode* n1 = nodes->find(x1, y1);
	MyGraphNode* n2 = nodes->find(x2, y2);
	if (n1 && n2) {
		n1->removeIncident(n2);
		if (WF)clearArc(x1, y1, x2, y2);
		return MG_NO_ERROR;
	}
	return MG_COORDS_NOT_FOUNDED;
}

void MyGraph::printToConsole()
{
	{
		MyNodesList* tmp = nodes;
		while (tmp)
		{
			std::cout << "\t" << tmp->info->x;
			tmp = tmp->next;
		}
		std::cout << "\n";
		tmp = nodes;
		while (tmp)
		{
			std::cout << "\t" << tmp->info->y;
			tmp = tmp->next;
		}
		std::cout << "\n";
		tmp = nodes;
		while (tmp)
		{
			std::cout << tmp->info->x << " " << tmp->info->y;
			{
				MyNodesList* tmp2 = nodes;
				while (tmp2)
				{
					if (tmp->info->incoming->find(tmp2->info->x, tmp2->info->y))std::cout << "\t+";
					else std::cout << "\t-";
					tmp2 = tmp2->next;
				}
			}
			std::cout << "\n";
			tmp = tmp->next;
		}
	}

}

void MyGraph::cleanN()
{
	timer = 0;
	MyNodesList* tmp = nodes;
	while (tmp)
	{
		tmp->info->N = -1;
		tmp->info->Visited = false;
		tmp->info->forvis = false;
		tmp = tmp->next;
	}
}

void MyGraph::DFB()
{
	MyNodesList* tmp = nodes;
	while (tmp)
	{
		if (tmp->info->N == -1)DFBr(tmp->info);
		tmp = tmp->next;
	}
}

void MyGraph::DFBr(MyGraphNode* node)
{
	timer++;
	node->forvis = true;
	MyNodesList* tmp = node->incoming;
	while (tmp)
	{
		if (!tmp->info->forvis)DFBr(tmp->info);
		tmp = tmp->next;
	}
	node->N = timer;
	timer++;
}

void MyGraph::iDFB(MyGraphNode* node, MyNodesList* graph)
{
	node->Visited = true;
	graph->add(node);
	MyNodesList* tmp = sort(node->incidents);
	while (tmp)
	{
		if (!tmp->info->Visited)iDFB(tmp->info, graph);
		tmp = tmp->next;
	}
}

MyNodesList* MyGraph::sort(MyNodesList* mndl)
{
	if (!mndl)return nullptr;

	MyGraphNode* minG = nullptr;

	MyNodesList* tmp = mndl, * uns = nullptr, * unsEnd = nullptr;
	int max = tmp->info->N;
	minG = tmp->info;
	while (tmp)
	{
		if (tmp->info->N > max) { max = tmp->info->N; minG = tmp->info; }
		tmp = tmp->next;
	}

	uns = new MyNodesList;
	uns->info = minG;
	unsEnd = uns;
	while (1)
	{
		int prevMax = max;
		max = -1;
		tmp = mndl;
		while (tmp)
		{
			if (tmp->info->N < prevMax && tmp->info->N > max) { max = tmp->info->N; minG = tmp->info; }
			tmp = tmp->next;
		}
		if (max == -1)break;
		unsEnd->next = new MyNodesList();
		unsEnd = unsEnd->next;
		unsEnd->info = minG;
	}
	return uns;
}

void MyGraph::wrirteArc(int x1, int y1, int x2, int y2)
{
	std::ofstream out(filename, std::ios::app);
	if (out.is_open())
	{
		bool correct = true;
		out.write((char*)&correct, sizeof(bool));
		out.write((char*)&x1, sizeof(int));
		out.write((char*)&y1, sizeof(int));
		out.write((char*)&x2, sizeof(int));
		out.write((char*)&y2, sizeof(int));
	}
	out.close();
}

void MyGraph::clearArc(int dx1, int dy1, int dx2, int dy2)
{
	std::ifstream in(filename);
	int pos = -1;
	if (in.is_open())
	{
		while (!in.eof()) {
			bool correct = false;
			int x1 = -1, y1 = -1, x2 = -1, y2 = -1;
			in.read((char*)&correct, sizeof(correct));
			in.read((char*)&x1, sizeof(x1));
			in.read((char*)&y1, sizeof(y1));
			in.read((char*)&x2, sizeof(x2));
			in.read((char*)&y2, sizeof(y1));
			if (correct && x1 == dx1 && x2 == dx2 && y1 == dy1 && y2 == dy2) { pos = (int)(in.tellg()) - (4 * sizeof(int) + sizeof(bool)); break; }
		}
		in.close();
	}
	if (pos == -1)return;
	std::ofstream out(filename, std::ios::binary | std::ios::in | std::ios::out);
	if (out.is_open())
	{
		bool fls = false;
		out.seekp(pos, std::ios::beg);
		out.write((char*)&fls, sizeof(bool));
		out.close();
	}
}

MyList<MyNodesList>* MyGraph::StronglyConnected()
{
	cleanN();
	DFB();
	MyNodesList* sorted = sort(nodes), * tmp = sorted;
	MyList<MyNodesList>* uns = nullptr, * unsHead = nullptr;
	while (tmp)
	{
		if (!tmp->info->Visited) {
			MyNodesList* pr = new MyNodesList;
			pr->info = tmp->info;
			iDFB(tmp->info, pr);

			if (uns) {
				uns->next = new MyList<MyNodesList>;
				uns = uns->next;
				uns->info = pr;
			}
			else
			{
				uns = new MyList<MyNodesList>;
				uns->info = pr;
				unsHead = uns;
			}
		}
		tmp = tmp->next;
	}
	sorted->clean();
	delete sorted;
	return unsHead;
}
