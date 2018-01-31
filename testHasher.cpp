/*
 *  COMP 15 PROJ 2 - GERP
 *  Christos-Nikolaos Mitsopoulos 
 *
 *  testHasher.cpp
 *  This file contains a functionality test for WordHasher.cpp
 *
 * How it works:
 * Compares output of reference implementation with that of
 * my implementation by redirecting output to two different
 * files.
 *  Modified By (UTLN): cmitso01    
 *           On       : 11/30/2017
 *
 */

#include <iostream>
#include <cstdlib>
#include <fstream>
using namespace std;

/*
To select commands, open commands.txt and add the commands
you want the query to run
*/
int main()
{
	//Directory to index -> Change if you want
	string directory = "/comp/15/files/proj2tests/smallGutenberg";
	string command_one = "./the_gerp " + directory + 
	" < commands.txt > ref_output.txt";
	//run test with reference implementation
	system(command_one.c_str());
	string command_two = "./gerp " + directory +
	" < commands.txt > my_output.txt";
	//run test with my implementation
	system(command_two.c_str());
	string sort_one = "sort ref_output.txt > ref_output_sorted.txt"; 
	string sort_two = "sort my_output.txt > my_output_sorted.txt"; 
	//sort files
	system(sort_one.c_str());
	system(sort_two.c_str());
	string diff ="diff ref_output_sorted.txt my_output_sorted.txt"
	 " > diffOutput.txt";
	system(diff.c_str());
	ifstream input;
	input.open("diffOutput.txt");
	string s;
	while (input >> s)
	{
		cout << "S = " << s << endl;
		cerr << "Sorry, File is not empty!" << endl;
		return 1;
	}
	cerr << "Query worked! No difference in output" << endl;
	return 0;
}
