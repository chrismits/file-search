#include <iostream>
#include "stringProcessing.h"
#include <fstream>
#include <functional>
#include <vector>
using namespace std;

int main()
{
	string filename = "testFile";
	ifstream input;
	input.open(filename);

	string s;

	//create hash table
	vector<string> table;
	hash<string> hashFunction;
	while(input >> s)
	{
		s = stripNonAlphaNum(s);
		size_t index = hashFunction(s);
		cout << "Key = " << s << " ";
		cout << "Index = " << index << endl;
	}

	return 0;
}