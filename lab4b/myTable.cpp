#include "myTable.h"
#include <math.h>
#include <iostream>
#include <io.h>
#include <fstream>

myTable::myTable(char* file)
{
	count1 = 0;
	count2 = 0;
	for (int i = 0; i < SIZE; i++)
	{
		keyspace1[i] = nullptr;
		keyspace2[i] = nullptr;
	}
	filename = file;
}

int myTable::add_f(int ky1, int ky2, int data, int infolen, Item** itmp)
{
	if (count1 >= SIZE || count2 >= SIZE) {
		return MTB_ERROR_TABLE_FULL;
	}
	int  v1 = 0, v2 = 0;
	Item* itm = new Item(ky1, ky2, data, infolen);
	if (itmp != nullptr)
		*itmp = itm;
	int curpos = ky1 % SIZE;
	while (1)
	{
		if (keyspace1[curpos] == nullptr)
		{
			keyspace1[curpos] = new ItemList(ky1, itm);
			count1++;
			break;
		}
		else if (keyspace1[curpos]->key == ky1)
		{
			ItemList* it = keyspace1[curpos]->getend(&v1);
			it->next = new ItemList(ky1, itm);
			break;
		}
		else {
			curpos += step;
			curpos %= SIZE;
		}
	}
	int h1 = myHash1(itm->key2), h2 = myHash2(itm->key2);
	if (keyspace2[h1] == nullptr)
	{
		keyspace2[h1] = new ItemList(itm->key2, itm);
		count2++;
	}
	else if (keyspace2[h1]->key == itm->key2)
	{
		ItemList* it = keyspace2[h1]->getend(&v2);
		it->next = new ItemList(ky2, itm);
	}
	else if (keyspace2[h2] == nullptr)
	{
		keyspace2[h2] = new ItemList(itm->key2, itm);
		count2++;
	}
	else if (keyspace2[h2]->key == itm->key2)
	{
		ItemList* it = keyspace2[h2]->getend(&v2);
		it->next = new ItemList(ky2, itm);
	}
	else {
		ItemList* cur = new ItemList(ky2, itm);
		bool hsh = true;
		int* was = new int[myTable::SIZE]{};
		while (1) {
			h1 = 0;
			if (hsh)
				h1 = myHash1(cur->key);
			else
				h1 = myHash2(cur->key);

			if (keyspace2[h1] == nullptr)
			{
				keyspace2[h1] = cur;
				count2++;
				break;
			}
			else
			{
				ItemList* tmp = keyspace2[h1];
				keyspace2[h1] = cur;
				cur = tmp;
			}
			if (was[h1] > 4) {
				delete[] was;
				return MTB_ERROR_CUCUSHKA_CYCLE;
			}
			was[h1]++;
			hsh = !hsh;
		}
		delete[] was;
	}
	itm->version = maximum(v1, v2);
	return MTB_NO_ERROR;
}
int ItemList::count()
{
	int c = 0;
	ItemList* cur = this;
	while (cur != nullptr)
	{
		c++;
		cur = cur->next;
	}
	return c;
}
const char* tmpname = "tmp.bin";
int myTable::optimize()
{
	std::ifstream in(filename, std::ios::binary);
	if (!in)return MTB_NO_FILE_ACCESS;
	std::ofstream out(tmpname, std::ios::binary | std::ios::app);
	if (!out)return MTB_NO_FILE_ACCESS;
	for (int i = 0; i < SIZE; i++)
	{
		if (keyspace1[i] != nullptr)
		{
			ItemList* cur = keyspace1[i];
			while (cur != nullptr)
			{
				int off = out.tellp();
				Item* curi = cur->item;
				out.write((char*) & (curi->key1), sizeof(curi->key1));
				out.write((char*) & (curi->key2), sizeof(curi->key2));
				out.write((char*) & (curi->version), sizeof(curi->version));
				out.write((char*)&off, sizeof(off));
				out.write((char*) & (curi->info_lenght), sizeof(curi->info_lenght));

				in.seekg(curi->offset + 5 * sizeof(int));
				char* inform = new char[curi->info_lenght + 1];
				in.read(inform, curi->info_lenght * sizeof(char));
				inform[curi->info_lenght] = '\0';
				out.write(inform, curi->info_lenght);
				cur = cur->next;
				delete[] inform;
			}
		}
	}
	in.close();
	out.close();
	remove(filename);
	rename(tmpname, filename);
	return MTB_NO_ERROR;
}

myTable* myTable::createFromFile(char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in) {
		myTable* mtb = new myTable(filename);
		while (true)
		{
			int k1, k2, inflen, vers, off = in.tellg(), offf;
			in.read((char*)&k1, sizeof(k1));
			if (in.eof())break;
			in.read((char*)&k2, sizeof(k2));
			in.read((char*)&vers, sizeof(vers));
			in.read((char*)&offf, sizeof(offf));
			in.read((char*)&inflen, sizeof(inflen));
			in.seekg(inflen, std::ios::cur);
			mtb->add_f(k1, k2, off, inflen, nullptr);

		}
		in.close();
		return mtb;
	}
	else
	{
		std::ofstream out(filename, std::ios::binary);
		if (!out)return nullptr;
		out.close();
		return new myTable(filename);
	}
}

int myTable::add(int k1, int k2, const char* inf)
{
	std::ofstream out(filename, std::ios::binary | std::ios::app);
	if (!out)return MTB_NO_FILE_ACCESS;
	out.seekp(cur_off, std::ios::beg);
	int ln = strlen(inf);
	Item* itm = nullptr;
	int status = add_f(k1, k2, out.tellp(), ln, &itm);
	if (!itm) return status;
	out.write((char*) & (itm->key1), sizeof(itm->key1));
	out.write((char*) & (itm->key2), sizeof(itm->key2));
	out.write((char*) & (itm->version), sizeof(itm->version));
	out.write((char*) & (itm->offset), sizeof(itm->offset));
	out.write((char*) & (itm->info_lenght), sizeof(itm->info_lenght));
	out.write(inf, ln);
	out.close();
	cur_off += 20 + ln;
	return MTB_NO_ERROR;
}

void myTable::println() const
{
	std::cout << LINE_SEPARATOR << "keyspace 1:\nkey:\tdata\n";
	for (int i = 0; i < SIZE; i++)
	{
		if (keyspace1[i] != nullptr)
		{
			keyspace1[i]->println(filename);
			std::cout << std::endl;
		}
	}
	std::cout << "keyspace 2:\nkey:\tdata\n";
	for (int i = 0; i < SIZE; i++)
	{
		if (keyspace2[i] != nullptr)
		{
			keyspace2[i]->println(filename);
			std::cout << std::endl;
		}
	}
	std::cout << LINE_SEPARATOR << "count1 = " << count1 << " count2 = " << count2 << ". Table size = " << SIZE << "." << std::endl;
}

ItemList* myTable::findByKey1(int k1) const
{
	int pos = k1 % myTable::SIZE, bg = pos;
	ItemList* orig = keyspace1[pos], * copy = nullptr, * beg = nullptr;
	while (1)
	{
		if (orig != nullptr && orig->key == k1) break;
		pos += step;
		pos %= SIZE;
		if (pos == bg) { orig = nullptr; break; }
		orig = keyspace1[pos];
	}
	if (orig != nullptr)
	{
		beg = new ItemList(k1, new Item(orig->item));
		copy = beg;
		orig = orig->next;
	}
	while (orig != nullptr) {
		copy->next = new ItemList(k1, new Item(orig->item));
		copy = copy->next;
		orig = orig->next;
	}
	return beg;
}

ItemList* myTable::findByKey2(int k2) const
{
	int h1 = myHash1(k2), h2 = myHash2(k2);
	if (keyspace2[h1] != nullptr && keyspace2[h1]->key == k2)
	{
		ItemList* orig = keyspace2[h1], * copy = nullptr, * beg = nullptr;
		if (orig != nullptr)
		{
			beg = new ItemList(k2, new Item(orig->item));
			copy = beg;
			orig = orig->next;
		}
		while (orig != nullptr) {
			copy->next = new ItemList(k2, new Item(orig->item));
			copy = copy->next;
			orig = orig->next;
		}
		return beg;

	}
	else if (keyspace2[h2] != nullptr && keyspace2[h2]->key == k2)
	{

		ItemList* orig = keyspace2[h2], * copy = nullptr, * beg = nullptr;
		if (orig != nullptr)
		{
			beg = new ItemList(k2, new Item(orig->item));
			copy = beg;
			orig = orig->next;
		}
		while (orig != nullptr) {
			copy->next = new ItemList(k2, new Item(orig->item));
			copy = copy->next;
			orig = orig->next;
		}
		return beg;
	}
	return nullptr;
}

int ItemList::deleteByVersion(int ver)
{
	ItemList* it = this;
	if (it->item->version == ver)
	{
		return 1;
	}
	int retcode = -1;
	while (it != nullptr)
	{
		if (it->next != nullptr && it->next->item->version == ver)
		{
			ItemList* tmp = it->next->next;
			delete it->next;
			it->next = tmp;
			retcode = 0;
			break;
		}
		it = it->next;
	}
	return retcode;
}
int myTable::deleteByKey1(int k1, int ver)
{
	int pos = k1 % myTable::SIZE;
	ItemList* it = keyspace1[pos];
	while (true)
	{
		if (it != nullptr && it->key == k1)break;
		pos += step;
		pos %= myTable::SIZE;
		if (pos == k1 % myTable::SIZE)
			return MTB_ERROR_NO_ELM_WT_KEY_VER;
		it = keyspace1[pos];
	}
	if (it == nullptr)
		return MTB_ERROR_NO_ELM_WT_KEY_VER;
	ItemList* prew = nullptr;
	while (it != nullptr)
	{
		if (it->item->version == ver)
		{
			break;
		}
		prew = it;
		it = it->next;
	}
	if (it == nullptr)
		return MTB_ERROR_NO_ELM_WT_KEY_VER;

	int hash = myHash1(it->item->key2);
	if (keyspace2[hash] != nullptr && keyspace2[hash]->key == it->item->key2) {
		if (keyspace2[hash]->deleteByVersion(it->item->version) == 1)
		{
			ItemList* tmp = keyspace2[hash]->next;
			delete keyspace2[hash];
			keyspace2[hash] = tmp;
		}
		if (keyspace2[hash] == nullptr)count2--;
	}
	else
	{
		hash = myHash2(it->item->key2);
		if (keyspace2[hash] != nullptr && keyspace2[hash]->key == it->item->key2)
		{
			if (keyspace2[hash]->deleteByVersion(it->item->version) == 1)
			{
				ItemList* tmp = keyspace2[hash]->next;
				delete keyspace2[hash];
				keyspace2[hash] = tmp;
			}
		}
		if (keyspace2[hash] == nullptr)count2--;
	}
	if (prew == nullptr)
	{
		keyspace1[pos] = it->next;
		delete it->item;
		delete it;
	}
	else
	{
		prew->next = it->next; delete it->item;
		delete it;
	}
	if (keyspace1[pos] == nullptr) { count1--; }
	return MTB_NO_ERROR;
}

int myTable::deleteByKey2(int k2, int vers)
{
	int hash1 = myHash1(k2), hash2 = myHash2(k2);

	if (keyspace2[hash1] != nullptr && keyspace2[hash1]->key == k2)
	{
		ItemList* it = keyspace2[hash1];
		if (it->item->version == vers)
		{
			int pos = it->item->key1 % SIZE;
			ItemList* k1 = keyspace1[pos];
			while (1)
			{
				if (keyspace1[pos] != nullptr && keyspace1[pos]->key == it->item->key1)break;
				pos += step;
				pos %= SIZE;
				k1 = keyspace1[pos];
				if (pos == it->item->key1 % SIZE) { k1 = nullptr; break; }
			}
			if (k1 != nullptr)
			{
				if (k1->deleteByVersion(it->item->version) == 1)
				{
					keyspace1[pos] = k1->next;
					delete k1;
				}
			}
			if (keyspace1[pos] == nullptr)count1--;
			delete it->item;
			ItemList* tmp = it->next;
			delete it;
			it->next = tmp;
			keyspace2[hash1] = nullptr;
			count2--;
		}
		else {
			while (it->next != nullptr)
			{
				if (it->next->item->version == vers) break;
				it = it->next;
			}
			int pos = it->next->item->key1 % SIZE;
			ItemList* k1 = keyspace1[pos];
			while (1)
			{
				if (keyspace1[pos] != nullptr && keyspace1[pos]->key == it->next->item->key1)break;
				pos += step;
				pos %= SIZE;
				k1 = keyspace1[pos];
				if (pos == it->next->item->key1 % SIZE) { k1 = nullptr; break; }
			}
			if (k1 != nullptr)
			{
				if (k1->deleteByVersion(it->next->item->version) == 1)
				{
					keyspace1[pos] = k1->next;
					delete k1;
				}
			}
			if (keyspace1[pos] == nullptr)count1--;
			delete it->next->item;
			ItemList* tmp = it->next->next;
			delete it->next;
			it->next = tmp;
			if (keyspace2[hash1] == nullptr)count2--;
		}
	}
	else if (keyspace2[hash2] != nullptr && keyspace2[hash2]->key == k2)
	{
		ItemList* it = keyspace2[hash2];
		if (it->item->version == vers)
		{
			int pos = it->item->key1 % SIZE;
			ItemList* k1 = keyspace1[pos];
			while (1)
			{
				if (keyspace1[pos] != nullptr && keyspace1[pos]->key == it->item->key1)break;
				pos += step;
				pos %= SIZE;
				k1 = keyspace1[pos];
				if (pos == it->item->key1 % SIZE) { k1 = nullptr; break; }
			}
			if (k1 != nullptr)
			{
				if (k1->deleteByVersion(it->item->version) == 1)
				{
					keyspace1[pos] = k1->next;
					delete k1;
				}
			}
			if (keyspace1[pos] == nullptr)count1--;
			delete it->item;
			ItemList* tmp = it->next;
			delete it;
			it->next = tmp;
			keyspace2[hash2] = nullptr;
			count2--;
		}
		else {
			while (it->next != nullptr)
			{
				if (it->next->item->version == vers) break;
				it = it->next;
			}
			int pos = it->next->item->key1 % SIZE;
			ItemList* k1 = keyspace1[pos];
			while (1)
			{
				if (keyspace1[pos] != nullptr && keyspace1[pos]->key == it->next->item->key1)break;
				pos += step;
				pos %= SIZE;
				k1 = keyspace1[pos];
				if (pos == it->next->item->key1 % SIZE) { k1 = nullptr; break; }
			}
			if (k1 != nullptr)
			{
				if (k1->deleteByVersion(it->next->item->version) == 1)
				{
					keyspace1[pos] = k1->next;
					delete k1;
				}
			}
			if (keyspace1[pos] == nullptr)count1--;
			delete it->next->item;
			ItemList* tmp = it->next->next;
			delete it->next;
			it->next = tmp;
			if (keyspace2[hash2] == nullptr)count2--;
		}
	}
	else return MTB_ERROR_NO_ELM_WT_KEY;
	return MTB_NO_ERROR;
}
void myTable::clear()
{
	for (int i = 0; i < SIZE; i++)
	{
		if (keyspace1[i] != nullptr)
		{
			deleteByKey1(keyspace1[i]->key);
		}
		if (keyspace2[i] != nullptr)
		{
			deleteByKey2(keyspace2[i]->key);
		}
	}
}
int myTable::deleteByKey2(int k2)
{
	int hash1 = myHash1(k2), hash2 = myHash2(k2);

	if (keyspace2[hash1] != nullptr && keyspace2[hash1]->key == k2)
	{
		ItemList* it = keyspace2[hash1];
		while (it != nullptr)
		{
			int pos = it->item->key1 % SIZE;
			ItemList* k1 = keyspace1[pos];
			while (1)
			{
				if (keyspace1[pos] != nullptr && keyspace1[pos]->key == it->item->key1)break;
				pos += step;
				pos %= SIZE;
				k1 = keyspace1[pos];
				if (pos == it->item->key1 % SIZE) { k1 = nullptr; break; }
			}
			if (k1 != nullptr)
			{
				if (k1->deleteByVersion(it->item->version) == 1)
				{
					keyspace1[pos] = k1->next;
					delete k1;
				}
			}
			if (keyspace1[pos] == nullptr)count1--;
			ItemList* tmp = it->next;
			delete it->item;
			delete it;
			it = tmp;
		}
		keyspace2[hash1] = nullptr;
		count2--;
	}
	else if (keyspace2[hash2] != nullptr && keyspace2[hash2]->key == k2)
	{
		ItemList* it = keyspace2[hash2];
		while (it != nullptr)
		{
			int pos = it->item->key1 % SIZE;
			ItemList* k1 = keyspace1[pos];
			while (1)
			{
				if (keyspace1[pos] != nullptr && keyspace1[pos]->key == it->item->key1)break;
				pos += step;
				pos %= SIZE;
				k1 = keyspace1[pos];
				if (pos == it->item->key1 % SIZE) { k1 = nullptr; break; }
			}
			if (k1 != nullptr)
			{
				if (k1->deleteByVersion(it->item->version) == 1)
				{
					keyspace1[pos] = k1->next;
					delete k1;
				}
			}
			if (keyspace1[pos] == nullptr)count1--;
			ItemList* tmp = it->next;
			delete it->item;
			delete it;
			it = tmp;
		}
		keyspace2[hash2] = nullptr;
		count2--;
	}
	else return MTB_ERROR_NO_ELM_WT_KEY;
	return MTB_NO_ERROR;
}


int myTable::deleteByKey1(int k1)
{
	int pos = k1 % myTable::SIZE;
	ItemList* it = keyspace1[pos];
	while (true)
	{
		if (it != nullptr && it->key == k1)break;
		pos += step;
		pos %= myTable::SIZE;
		if (pos == k1 % myTable::SIZE)
			return MTB_ERROR_NO_ELM_WT_KEY;
		it = keyspace1[pos];
	}
	if (it == nullptr)
		return MTB_ERROR_NO_ELM_WT_KEY;
	while (it != nullptr)
	{
		int hash = myHash1(it->item->key2);
		if (keyspace2[hash] != nullptr && keyspace2[hash]->key == it->item->key2) {
			if (keyspace2[hash]->deleteByVersion(it->item->version) == 1)
			{
				ItemList* tmp = keyspace2[hash]->next;
				delete keyspace2[hash];
				keyspace2[hash] = tmp;
			}
			if (keyspace2[hash] == nullptr)count2--;
		}
		else
		{
			hash = myHash2(it->item->key2);
			if (keyspace2[hash] != nullptr && keyspace2[hash]->key == it->item->key2)
			{
				if (keyspace2[hash]->deleteByVersion(it->item->version) == 1)
				{
					ItemList* tmp = keyspace2[hash]->next;
					delete keyspace2[hash];
					keyspace2[hash] = tmp;
				}
				if (keyspace2[hash] == nullptr)count2--;
			}
		}
		ItemList* tmp = it->next;
		delete it->item;
		delete it;
		it = tmp;
	}
	keyspace1[pos] = nullptr;
	count1--;
	return MTB_NO_ERROR;
}

ItemList* ItemList::getend(int* vrs)
{
	ItemList* ls = this;
	*vrs = maximum(0, ls->item->version) + 1;
	while (ls->next != nullptr)
	{
		(*vrs) = maximum((*vrs), ls->item->version) + 1;
		ls = ls->next;
	}
	return ls;
}

ItemList::ItemList(int k, Item* a)
{
	item = a;
	key = k;
	next = nullptr;
}

void ItemList::println(char* filename)
{
	ItemList* it = this;
	std::cout << it->key << ":\t";

	std::ifstream in(filename, std::ios::binary);
	if (!in)return;
	while (it != nullptr)
	{
		in.seekg(it->item->offset + 5 * sizeof(int));
		char* inform = new char[it->item->info_lenght + 1];
		in.read(inform, it->item->info_lenght * sizeof(char));
		inform[it->item->info_lenght] = '\0';
		std::cout << "(k1=" << it->item->key1 << ", k2=" << it->item->key2 << ", inf=" << inform << ", ver=" << it->item->version << "), ";
		it = it->next;
		delete[] inform;
	}
	in.close();
}

inline int maximum(int a, int b)
{
	return a > b ? a : b;
}

inline int myHash1(int a)
{

	int tmp = a * myTable::factor1 * myTable::factor1 / myTable::factor2;
	return abs(((tmp >> 8) & 0xff) + (tmp & 0xff)) % myTable::SIZE;
}
inline int myHash2(int a)
{
	int tmp = a * myTable::factor2 * myTable::factor2 / myTable::factor1;
	return abs(((tmp >> 8) & 0xff) - (tmp & 0xff)) % myTable::SIZE;
}

Item::Item(int k1, int k2, int off, int inflen)
{
	key1 = k1;
	key2 = k2;
	version = 0;
	offset = off;
	info_lenght = inflen;
}

Item::Item(Item* it)
{
	key1 = it->key1;
	key2 = it->key2;
	version = it->version;
	info_lenght = it->info_lenght;
	offset = it->offset;
}

void ItemList::clear()
{
	ItemList* pos = this;
	while (pos != nullptr)
	{
		ItemList* nx = pos->next;
		delete pos->item;
		delete pos;
		pos = nx;
	}
}
