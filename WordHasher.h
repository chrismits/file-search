/*
 *	Christos-Nikolaos Mitsopoulos
 *
 *	WordHasher.h
 *  This file contains the function declaration (interface) for 
 *	the WordHasher class.
 * -------------------
 * General Information
 * -------------------
 * This is the class definition of WordHasher. This class basically
 * implements a hashtable that stores information about each word in
 * a directory (the different variations of the word, its location in
 * each file it is found). Indexes are based on the hashing of an all
 * lowercase version of each word(the key). The hashtable then stores
 * an Entry struct(further information in entryStructs.h).
 *
 * Info on private variables:
 * Entry **hashTable -> a sequence of Entry Structs
 * int currTableSize -> Current capacity of the above sequence
 * int entryCount -> Current amount of entry structs in the sequence
 *
 * Info on functions:
 * -A small amount of info above each function definition.
 * -For full documentation, see WordHasher.cpp comments
 *
 *  Modified By (UTLN): cmitso01    
 *           On       : 11/23/2017
*/

#ifndef WORDHASHER_H
#define WORDHASHER_H

#include <vector>
#include <functional>
#include <sstream>
#include <fstream>
#include "FSTree.h"
#include "stringProcessing.h"
#include "entryStructs.h"

class WordHasher
{
private:
    /*** HashTable variables ***/

	Entry **hashTable;
	int currTableSize;
	int entryCount;
    static const int INITIAL_TABLE_SIZE = 199;

    /*** Addition functions ***/

    //start Entry addition process (or create completely new entry)
    void addEntry(Entry *&tableEntry, std::string key, WordInfo info);
    //Add new word variation to already created entry
    void appendToEntry(Entry *&tableEntry, WordInfo info);
    //Add complete entry and all the information it contains. Used in expand
    void addCompleteEntry(Entry *node, Entry **&tempTable);
    //Removes an entry from the hashtable
    void removeEntry(Entry *node);
    //Clears heap allocated data for table
    void removeTable(int tableSize);

    /*** hash functions ***/

    //expands array
    void expand();
    //hashes a string and generates an index for the specific tableSize
    size_t getIndex(std::string key);

    /*** Index building and file processing ***/

    //Get all filepaths in a directory to use in later functions
	void getTreeFiles(DirNode *node, std::string directory);
    //Open and read file, separate each line and call function to process it
	void processFile(std::string filepath);
    //Reads all words in the line and passes them for indexing
	void processLine(std::string filepath, std::istream& line, int count);
    //converts a string to all lowercase
	std::string lowerCase(std::string word);

    /*** Query processing functions and variables ***/
    
    std::string error_one = " Not Found.";
    std::string error_two = " Not Found. Try with @insensitive or @i.";
    //Searches for a word in the index
    bool wordSearch(Entry *node, std::string ogword, bool insensitive);
    //Prints the search results in the appropriate format
    void printQuery(std::vector<LocInfo> &locs);

    /*** debugging functions ***/
    void printTable();
    int numListItems(Entry *node);
public:
	//Constructors and Destructors
	WordHasher();
	~WordHasher();

	void run(std::string directory);
    void find(std::string word, bool insensitive);
};

#endif
