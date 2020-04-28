#include "myTable.h"
#include <iostream>

myTable::myTable()
{
	for (int i = 0; i < SIZE; i++)
	{
		ky1[i] = nullptr;
		ky2[i] = nullptr;
	}
}

bool myTable::add(int k1, int k2, const char* ch)
{
	if (count >= SIZE)return false;
	Item* it = new Item;
	it->key1 = k1;
	it->key2 = k2;
	it->data = ch;
	int pk1 = k1 % SIZE;


	if (ky1[pk1] == nullptr) {
		ky1[pk1] = it;
	}
	else if (ky1[pk1]->key1 == k1)return false;
	else
		for (int i = pk1 + step; i != pk1; i += step)
		{
			if (i >= SIZE)i -= SIZE;
			if (ky1[i] == nullptr)
			{
				ky1[i] = it;
				break;
			}
			else if (ky1[i]->key1 == k1)return false;
		}
	int h1 = myHash1(k2), h2 = myHash2(k2);
	//std::cout << h1 << "\t" << h2 << "\t:" << ch << std::endl;
	if (ky2[h1] == nullptr) {
		ky2[h1] = it;
		count++;
		return true;
	}
	else if (ky2[h1]->key2 == k2)return false;
	if (ky2[h2] == nullptr) {

		ky2[h2] = it;
		count++;
		return true;
	}
	else if (ky2[h2]->key2 == k2)return false;
	Item* tmp = ky2[h1];
	ky2[h1] = it;
	int nextHash = myHash2(tmp->key2);
	bool hashh = false;
	std::cout << "collision" << std::endl;
	while (1)
	{
		if (it == tmp) return false;
		if (ky2[nextHash] == nullptr)
		{
			ky2[nextHash] = tmp;
			count++;
			return true;
		}
		else
		{
			if (ky2[nextHash]->key2 == k2)return false;
			Item* tmpp = ky2[nextHash];
			ky2[nextHash] = tmp;
			tmp = tmpp;
			if (hashh)
				nextHash = myHash2(tmp->key2);
			else
				nextHash = myHash1(tmp->key2);
			hashh = !hashh;
		}
	}

	count++;
	return true;
}

bool myTable::print()
{
	for (int i = 0; i < SIZE; i++)
		std::cout << i << ";\t";
	std::cout << std::endl;
	for (int i = 0; i < SIZE; i++)
		if (ky1[i] == nullptr)
			std::cout << ";\t";
		else
			std::cout << ky1[i]->data << ";\t";
	std::cout << std::endl;
	for (int i = 0; i < SIZE; i++)
		if (ky2[i] == nullptr)
			std::cout << ";\t";
		else
			std::cout << ky2[i]->data << ";\t";
	std::cout << std::endl;
	return true;
}

inline int myHash1(int a)
{
	a = a * FACTOR1 + 1;
	return a % myTable::SIZE;
}
inline int myHash2(int a)
{
	a = a + FACTOR2;
	return a % myTable::SIZE;
}
/*
inline int summOfDigits(int a)
{
	int sum = 0;
	while (a)
	{
		sum += a % 10;
		a /= 10;
	}
	return sum;
}
inline int alternatingsummOfDigits(int a)
{
	int sum = 0;
	bool sign = true;
	while (a)
	{
		if (sign)
			sum += a % 10;
		else
			sum -= a % 10;
		sign = !sign;
		a /= 10;
	}
	return abs(sum);
}*/