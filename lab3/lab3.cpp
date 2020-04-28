
#include <iostream>

#define STD_STR_LEN	255

#define EOFn		"e"

using namespace std;

#define STRING_AS_LIST
#define PRINT_SEPARATOR

#ifdef STRING_AS_LIST

struct mstr
{
	char letter;
	mstr* next;
};

typedef mstr* mstrp;
void trim(mstrp);
void refactor(mstrp*);
void deleteStr(mstrp*);
void deleteStr(mstrp*, mstrp);
void printstr(mstrp);

#else

void trim(char*);
void refactor(char*);

#endif 



int main()
{
	cout << "Type random count of strings" << endl;
#ifndef STRING_AS_LIST
	char str[STD_STR_LEN + 1];
	bool endFiled = true;
	while (endFiled)
	{
		cin.getline(str, STD_STR_LEN);
		char* tmp;
		if ((tmp = strstr(str, EOFn)) >= str) {
			endFiled = false;
			*tmp = '\0';
		}
		trim(str);
		refactor(str);
#ifdef PRINT_SEPARATOR
		cout << str << ";" << endl;
#else
		cout << str << endl;
#endif 
	}
#else
	bool endFiled = true;
	while (endFiled)
	{
		mstrp beg = nullptr, str = nullptr;
		char c;
		while ((c = (char)cin.get()) != '\n')
		{
			if (c == 'e')
			{
				endFiled = false;
				break;
			}
			if (str != nullptr) {
				str->next = new mstr{ c,nullptr };
				str = str->next;
			}
			else {
				str = new mstr{ c,nullptr };
				beg = str;
			};
		}
		trim(beg);
		refactor(&beg);
		printstr(beg);
		deleteStr(&beg);
	}

#endif


	system("PAUSE");
	return 0;
}
#ifndef STRING_AS_LIST
void refactor(char* str)
{
	char* tmp = str, * tmpp = str;
	while ((tmp = strstr(tmpp, "01")) >= str) {
		char* tmpsp;
		if ((tmpsp = strstr(tmpp, " ")) <= 0 || tmpsp > tmp) {
			memmove(tmpp, tmp, STD_STR_LEN - (tmp - str));
			tmpp += 2;
		}
		else
		{
			memmove(tmpp + 1, tmp, STD_STR_LEN - (tmp + 1 - str));
			*tmpp = ' ';
			tmpp += 3;
		}
		tmp = tmpp;
	}
	*tmpp = '\0';
}
void trim(char* str)
{
	char* tmp = str;
	while ((tmp = strstr(tmp, "\t")) >= str)
		*tmp = ' ';
	tmp = str;
	while ((tmp = strstr(tmp, "  ")) >= str)
		memmove(tmp, tmp + 1, STD_STR_LEN - (tmp + 1 - str));
}
#else
void printstr(mstrp str)
{
	while (str != nullptr)
	{
		cout << str->letter;
		str = str->next;
	}
#ifdef PRINT_SEPARATOR

	cout << ";" << endl;
#else
	cout << endl;
#endif // 


}

void deleteStr(mstrp* str)
{
	mstrp cur = *str;
	while (cur != nullptr)
	{
		mstrp tmp = cur->next;
		delete cur;
		cur = tmp;
	}
	*str = nullptr;
}
void deleteStr(mstrp* str, mstrp sep)
{
	mstrp cur = *str;
	while (cur != nullptr && cur != sep)
	{
		mstrp tmp = cur->next;
		delete cur;
		cur = tmp;
	}
	*str = nullptr;
}
void trim(mstrp str) {
	while (str != nullptr)
	{
		if (str->letter == '\t')str->letter = ' ';
		if (str->next != nullptr && str->next->letter == '\t')str->next->letter = ' ';
		if (str->next != nullptr && str->next->letter == ' ' && str->letter == ' ')
		{
			mstrp tmp = str->next->next;
			delete str->next;
			str->next = tmp;
		}
		else
			str = str->next;
	}
}
void refactor(mstrp* strn) {
	mstrp str = *strn, strr = str;
	bool first = true;
	while (str != nullptr)
	{
		switch (str->letter)
		{
		case '0':
			if (str->next != nullptr && str->next->letter == '1')
			{
				if (first) {
					first = false;
					deleteStr(strn, str);
					*strn = str;
				}
				else {
					deleteStr(&(strr->next), str);
					strr->next = str;
				}
				str = str->next;
				strr = str;
			}
			else
			{
				strr->next = str->next;
				str = str->next;
			}
			break;
		case ' ':
			if (strr->letter == ' ')
			{
				strr->next = str->next;
				str = str->next;
			}
			else if (str->next != nullptr)
			{
				strr->next = str;
				strr = str;
				str = str->next;
			}
			else
			{
				strr->next = nullptr;
				delete str->next;
				str = nullptr;
			}
			break;
		default:
			strr->next = str->next;
			str = str->next;
		}
	}
}
#endif