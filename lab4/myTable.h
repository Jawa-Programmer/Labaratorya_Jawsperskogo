#ifndef MY_TABLE_H
#define MY_TABLE_H

#include <vector>

/// лючи дл€ хешировани€ с перемешиванием по кукушке
///¬ажно, что бы в разложении данных факторов на простые множители не встречались делители числа SIZE
#define FACTOR1	7
#define FACTOR2	9

inline int myHash1(int);
inline int myHash2(int);

inline int summOfDigits(int);
inline int alternatingsummOfDigits(int);

struct Item
{
	int key1, key2;
	const char* data;
};
struct ItemList
{
	int key, version;
	Item* item;
	ItemList* next;
};
class myTable
{
public:
	///Size - размер таблицы (количество уникальных элементов, которые в нее помещаютс€)
	///step - шаг дл€ перемешивани€ сложением. Ќе может быть составным или делителем SIZE
	static const int SIZE = 13, step = 3;

	myTable();
	bool add(int, int, const char*);
	bool removeByKey1(int);
	bool removeByKey2(int);
	bool find(int, std::vector<Item>);
	bool print();
private:
	int count = 0;
	Item* ky1[SIZE], * ky2[SIZE];
};


#endif // !MY_TABLE_H