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
	A.insert(2, 10);
	UnsortedTable<int>::iterator it = A.find(1);
	
	(++it).value() = 6;
	cout << (*it).second;
	--it;
	cout << (*it).second;
	return 0;
}
