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
	
	(it++).value() = 6;
	(it--).value() = 12;

	
	SortedTable<int> B;
	B.insert(1,1);
	B.insert(4, 4);
	B.insert(5,5);
	B.insert(2, 2);
	B.insert(3, 3);
	B.erase(1);

	SortedTable<int>::iterator it2 = B.begin();
	cout << A<<endl;
	cout << (B);

	HashTable<int> C(16);
	C.insert(1, 1);
	return 0;
}
