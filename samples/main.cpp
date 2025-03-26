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
	cout << (*it).second<<endl;

	
	SortedTable<int> B(&le_operator<int>);
	B.insert(1, 1);
	B.insert(2, 2);
	B.insert(3, 3);
	B.insert(4, 3);
	B.erase(1);
	SortedTable<int>::iterator it2 = B.begin();
	cout << it2.value();
	return 0;
}
