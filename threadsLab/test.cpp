#include "Bitset.h"
#include <iostream>
#include <string>
#include <time.h>
#include <random>


using namespace std;



int main(int argc, char ** argv)
{
	srand((unsigned)(time(NULL)));
	
	Bitset* set1 = new Bitset(25);
	
	for (int i = 0; i < 25; i++)
	{
		set1->setBit(i,(rand()%2));
	}
	
	set1->writeSetToFile("set1.bin");
	
	Bitset* set2 = new Bitset("set1.bin");
	
	for(int i = 0; i < 25; i++)
	{
		cout << "Bit " << i << " has the value " << (*set1)[i] << endl;
		cout << "Bit " << i << " has the value " << (*set2)[i] << endl;
	}
	
	
	for(int i = 0; i < 5; i++)
	{
		for(int j = 0; j < 5; j++)
		{
			cout << "Bit at (" << i << ", " << j << " has the value " << (*set1)(i,j) << endl;
		}
	}
	
	
	
	string c;
	
	getline(cin,c);
	
	return 0;
}
