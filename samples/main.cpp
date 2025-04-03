//UNN
//Litvyakov D. D.
//
#define _USE_MATH_DEFINES
#include "table.h"
#include <iostream>
#include <cmath>


size_t BradysHash(size_t key, double val) {
	return key;
}

using namespace std;
int main()
{
	HashTable<double> BradysTableSin(361, &BradysHash);
	for (size_t i = 0; i < 361; ++i) {
		BradysTableSin.insert(i, sin(double(i) * M_PI / 180));
	}
	cout << "Bradys Table for sin: \n";
	for (size_t i = 0; i < 361; ++i) {
		cout << (BradysTableSin.find(i))->first<<"\370 = "<< (BradysTableSin.find(i))->second << endl;
	}

	return 0;
}

