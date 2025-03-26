//UNN
//Litvyakov D. D.
//

#include "table.h"
#include <iostream>

using namespace std;
int main()
{
	//таблица брадиса хэш таблица
	UnsortedTable<int> A;
	A.insert(1, 5);
	UnsortedTable<int>::iterator it = A.find(1);
	
	cout << it.key() << (*it).second;
	return 0;
}
