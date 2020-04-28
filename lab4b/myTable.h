#ifndef	MY_TABLE_H
#define	MY_TABLE_H

#define	MTB_ERROR_TABLE_FULL			1
#define	MTB_ERROR_CUCUSHKA_CYCLE		2
#define	MTB_ERROR_NO_ELM_WT_KEY			3
#define	MTB_ERROR_NO_ELM_WT_KEY_VER		4
#define	MTB_ERROR_NO_ELM_WT_KEY_FINDED	5
#define	MTB_FILE_NOT_EXIST				6
#define	MTB_NO_FILE_ACCESS				7
#define	MTB_FILE_FORMAT_WRONG			8
#define	MTB_NO_ERROR					0

//#define	nullptr					NULL

const char LINE_SEPARATOR[] = "----------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

inline int maximum(int, int);

struct Item
{
	int key1, key2, version, info_lenght, offset;
	Item(int, int, int, int);
	Item(Item*);
};

struct ItemList
{
	int key;
	Item* item;
	ItemList* next;
	ItemList* getend(int*);
	ItemList(int, Item*);
	void println(char*);
	int deleteByVersion(int);
	int count();
	void clear();
	~ItemList()
	{
		clear();
	}
};

inline int myHash1(int);
inline int myHash2(int);


class myTable
{
public:
	// SIZE - размер таблицы (количество элементов с ”Ќ» јЋ№Ќџћ» ключами)
	// step - шаг дл€ открытого перемешивани€ в первом пространстве ключей
	// factor1 и facto2 - параметры хеш функций дл€ кукушечного перемешивани€.
	// хеш функции используют арифметику остатков, ввиду этого необходимо, что бы образовыволось кольцо.
	// по этому числа должны быть взаимнопростыми 
	//(хот€, тем не менее, в таблицу всеравно можно включить не более 3х элементов с одинаковыми остатками по модулю SIZE)
	// это св€зано с недостатками хэш функций дл€ кукушкиного перемешивани€
	static const int SIZE = 11, step = 3, factor1 = 9, factor2 = 7;
	void println() const;
	ItemList* findByKey1(int) const;
	ItemList* findByKey2(int) const;
	int deleteByKey1(int);
	int deleteByKey1(int, int);
	int deleteByKey2(int);
	int optimize();
	int deleteByKey2(int, int);
	static myTable* createFromFile(char* filename);
	int add(int, int, const char*);
	void clear();
	~myTable() 
	{
		clear();
	}
	inline char* getFileName() const{ return filename; }
private:
	int add_f(int, int, int, int, Item**);
	myTable(char*);
	char* filename;
	int count1, count2, cur_off = 0;
	ItemList* keyspace1[SIZE], * keyspace2[SIZE];
};
#endif