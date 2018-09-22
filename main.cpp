/*
 *  Christos-Nikolaos Mitsopoulos 
 *
 *  main.cpp
 *  This file contains the script for the final executable.
 *  Modified  
 *           On       : 11/30/2017
 *
 */

#include "WordHasher.h"
using namespace std;

void readInput(WordHasher &);

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cerr << "Usage:  gerp directory" << endl;
		cerr << "            where:  directory is a valid directory" << endl;
		return 1;
	}

	std::string directory = argv[1];
	directory += "/";
	
	//Run hasher.run to index the directory
	WordHasher hasher;
	hasher.run(directory);

	//start query process
	readInput(hasher);

	return 0;
}

/*
This function starts the word query input process after the directory
has been fully indexed.
	-Parameters -> WordHasher &hasher : A WordHasher object
	-Return type -> void
*/
void readInput(WordHasher &hasher)
{
	string input;
	bool insensitive = false;

	cout << "Query? ";
	while (cin >> input && input != "@q" && input != "@quit")
	{
		insensitive = false;
		if (input == "@i" || input == "@insensitive")
		{
			cin >> input;
			insensitive = true;
		}
		hasher.find(input, insensitive);
		cout << "Query? ";
	}

	cout << "\nGoodbye! Thank you and have a nice day." << endl;
}
