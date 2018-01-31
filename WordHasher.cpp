/*
 *  COMP 15 PROJ 2 - GERP
 *  Christos-Nikolaos Mitsopoulos 
 *
 *  WordHasher.cpp
 *  This file contains the function definitions (implementation) for
 *  the WordHasher class
 *  Modified By (UTLN): cmitso01    
 *           On       : 11/30/2017
 */

#include "WordHasher.h"

/*
Default Constructor. Initializes a wordhasher object.
    -Parameters -> none
    -Return type -> void
*/
WordHasher::WordHasher()
{
    currTableSize = INITIAL_TABLE_SIZE;
    entryCount = 0;

    hashTable = new Entry *[currTableSize];
    for (int i = 0; i < currTableSize; ++i)
    {
        hashTable[i] = NULL;
    }
}

/*
Destructor. Recycles all heap allocated data.
    -Parameters -> none
    -Return type -> void
*/
WordHasher::~WordHasher()
{
    removeTable(currTableSize);
}

/*
Removes an entry node from the hashtable.
    -Parameters -> Entry *node : The entry struct
    -Return type -> void
*/
void WordHasher::removeEntry(Entry *node)
{
    if (node -> next != NULL)
        removeEntry(node -> next);
    delete node;
}

/*
Deallocates memory for the hashtable.
    -Parameters -> none
    -Return type -> void
*/
void WordHasher::removeTable(int tableSize)
{
    for (int i = 0; i < tableSize; ++i)
        if (hashTable[i] != NULL)
            removeEntry(hashTable[i]);
    delete [] hashTable;
}

/*
Starts the process of indexing all the words in the directory.
    -Parameters -> string directory : The directory to index
    -Return type -> void
*/
void WordHasher::run(std::string directory)
{
    FSTree dirTree(directory);
    DirNode *root = dirTree.getRoot();

    //call function to loop through every file
    getTreeFiles(root, directory);
}

/******************** FILE PROCESSING *******************************/

/*
Gets all the files in the FSTree for further manipulation.
    -Parameters -> DirNode *node : The node being looked at
                -> string directory : The current directory path
    -Return type -> void
*/
void WordHasher::getTreeFiles(DirNode *node, std::string directory)
{
    if (node -> isEmpty())
        return;
    
    int file_count = node -> numFiles();
    int dir_count = node -> numSubDirs();

    std::string tempDirName;
    for (int i = 0; i < file_count; ++i)
    {
        //std::cout << directory + (node -> getFile(i)) << std::endl;
        tempDirName = directory + (node -> getFile(i));
        processFile(tempDirName);
        if (i % 10 == 0)
            expand();
    }

    //recurse in sub-directories:
    for (int i = 0; i < dir_count; ++i)
    {
        DirNode *tempNode = node -> getSubDir(i);
        std::string dir = directory + (tempNode -> getName()) + "/";
        getTreeFiles(tempNode, dir);
    }
}

/*
Starts indexing procedure for a file. Reads file and calls necessary
functions to complete indexing.
    -Parameters -> string filepath : The path to the file being processed
    -Return type -> void
*/
void WordHasher::processFile(std::string filepath)
{
    std::ifstream input;
    input.open(filepath);

    //helpful variables
    std::string line;
    int lineCount = 1;

    //Get each line separately
    while (getline(input, line))
    {
        std::istringstream line_stream(line);
        processLine(filepath, line_stream, lineCount);
        lineCount++;
    }
    input.close();
}

/*
Continuation of the indexing procedure for a single line. Reads each word
in line and calls necessary functions for indexing.
    -Parameters -> string filepath : The path to the file
                -> istream line : Variable used to read input
                -> int count : The line number
    -Return type -> void
*/
void WordHasher::processLine(std::string filepath,std::istream& line,int count)
{
    std::string s;
    std::string key;

    while (line >> s)
    {
        s = stripNonAlphaNum(s);

        if (s != "")
        {
            key = lowerCase(s);
            WordInfo info(s, LocInfo(filepath, count));
            size_t index = getIndex(key);
            addEntry(hashTable[index], key, info);
        }
    }
}

/*
Converts string to all lowercase.
    -Parameters -> string word : The word to convert
    -Return type -> string : The all-lowercase version
*/
std::string WordHasher::lowerCase(std::string word)
{
    int length = word.length();
    for (int i = 0; i < length; ++i)
    {
        word[i] = tolower(word[i]);
    }
    return word;
}

/***************** HASHTABLE PROCESSING AND ADDITION ***************/

/*
Generates array index using built-in hash function.
    -Parameters -> string key : The key to hash
    -Return type -> size_t : The resulting index
*/
size_t WordHasher::getIndex(std::string key)
{
    size_t idx = std::hash<std::string>{}(key);
    idx = idx % currTableSize;
    return idx;
}

/*
Starts the entry process to add a new entry to the hashtable. Checks all the
different cases of addition(entirely new struct, or addition to struct vector)
    -Parameters -> Entry *&tableEntry : Entry node struct (by reference)
                -> string key : The key of the word (all lowercase)
                -> WordInfo info : WordInfo struct with original 
                                    word and locations
    -Return type -> void
*/
void WordHasher::addEntry(Entry *&tableEntry, std::string key, WordInfo info)
{
    if (tableEntry == NULL)
    {
        //case 1 : Entirely new entry added to hashtable
        tableEntry = new Entry(key, info);
        entryCount++;
    }
    else
    {
        //case 2: Not NULL, search for word in word vector(1) or in list(2)
        if (tableEntry -> keyword == key)
            appendToEntry(tableEntry, info); //in word vector
        else
            addEntry(tableEntry -> next, key, info);
    }
}

/*
Addition subfunction. This functions is called when the same key is found in
the hashtable and the original word needs to be searched in the Word vector.
The new word information is then appended to this vector.
    -Parameters -> Entry *&tableEntry : Entry node struct (by reference)
                -> WordInfo info : WordInfo struct with original word
                                    and location
    -Return type -> void
*/
void WordHasher::appendToEntry(Entry *&tableEntry, WordInfo info)
{   
    size_t i; 
    size_t len = tableEntry -> words.size();

    //search for original word in vector
    for (i = 0; i < len; ++i)
        if (tableEntry->words[i].ogWord == info.ogWord)
            break;

    if (i == len)
    {
        //did not find exact match
        tableEntry -> words.push_back(info);
    }
    else
    {
        //add to vector
        std::string file = info.locations.back().path;
        int line = info.locations.back().line;
        tableEntry -> words[i].locations.push_back(LocInfo(file, line));
    }
}

/*
Sub-function of add functions, this function adds a full already created
Entry node and does not create one from individual components (used in expand)
    -Parameters -> Entry *node : Pointer to entry struct
    -Return type -> void
*/
void WordHasher::addCompleteEntry(Entry *node, Entry **&tempTable)
{
    if (node == NULL)
        return;

    Entry *newEntry = new Entry(node);
    size_t index = getIndex(node -> keyword);
    if (tempTable[index] == NULL)
        tempTable[index] = newEntry;
    else
    {
        //Add at the end of linked list at specified index
        Entry *temp = tempTable[index];
        while(temp -> next != NULL)
            temp = temp -> next;

        temp -> next = newEntry;
    }
}

/*
Expands the array if load factor reaches a specific limit.
    -Parameters -> none
    -Return type -> void
*/
void WordHasher::expand()
{
    float loadFactor = (float)entryCount / currTableSize;
    if (loadFactor < 0.7)
        return;
    int tempCap = currTableSize;
    currTableSize *= 2;

    //Create and initialize temporary array.
    Entry **tempTable = new Entry *[currTableSize];
    for (int i = 0; i < currTableSize; ++i)
        tempTable[i] = NULL;

    //Loop through old array
    for (int i = 0; i < tempCap; ++i)
    {
        Entry *currEntry = hashTable[i];
        while (currEntry != NULL)
        {
            //Add to new array
            addCompleteEntry(currEntry, tempTable);
            currEntry = currEntry -> next; 
        }
    }

    //clear heap allocated data
    removeTable(tempCap);
    hashTable = tempTable;
}

/******************** DEBUGGING FUNCTIONS ***************************/

/*
Prints useful information about the hashtable
    -Parameters -> none
    -Return type -> void
*/
void WordHasher::printTable()
{
    std::cout << "\n--------------TABLE---------------\n";
    std::cout << "CAPACITY = " << currTableSize << std::endl;
    std::cout << "COUNT = " << entryCount << std::endl;
    for (int i = 0; i < currTableSize; ++i)
    {
        Entry *temp = hashTable[i];
        if (temp != NULL)
        {
            std::cout << "\nList Length = " << numListItems(temp) << std::endl;
            while (temp != NULL)
            {
                std::cout << "idx= " << i;
                std::cout << ", key= " << temp -> keyword << ", words= "; 
                size_t numWords = (temp -> words).size();
                for (size_t j = 0; j < numWords; ++j)
                {
                    std::cout << (temp -> words)[j].ogWord << ", lnum= ";
                    size_t numLocs = (temp -> words)[j].locations.size();
                    for (size_t k = 0; k < numLocs; ++k)
                        std::cout << (temp -> words)[j].locations[k].line<<" ";
                }
                std::cout << "\n";
                temp = temp -> next;
            }
        }
    }
}

/*
Returns the number of nodes in a linked
list structure(useful to check how many entries in one index).
    -Parameters -> Entry *node : Pointer to an Entry node
    -Return type -> int (number of items)

*/
int WordHasher::numListItems(Entry *node)
{
    if (node == NULL)
        return 0;
    else
        return 1 + numListItems(node -> next);
}


/******************** QUERYING FUNCTIONS ******************************/

/*
Starts the query process by finding the node containing the string being
looked at, in the hashtable
    -Parameters -> string word : The query
                -> bool insensitive : Search type (true if @i)
    -Return type -> void
*/
void WordHasher::find(std::string word, bool insensitive)
{
    word = stripNonAlphaNum(word);
    std::string lowWord = lowerCase(word);
    size_t index = getIndex(lowWord);
    Entry *node = hashTable[index];
    bool found = false;

    while (node != NULL)
    {
        //same key -> search for exact word and filepath
        if (node -> keyword == lowWord)
        {
            found = wordSearch(node, word, insensitive);
            break;
        }

        node = node -> next;
    }

    //Word not found, print appropriate message
    if (!found)
    {
        if (word == "")
            word = "\"\"";

        if (insensitive)
            std::cout << word << error_one << std::endl;
        else
            std::cout << word << error_two << std::endl;
    }
}

/*
Continues the search process by searching for an exact match(if insensitive),
or all matches(if sensitive) in the current node.
    -Parameters -> Entry *node : The hashtable node containing the key
                -> string ogword : The case sensitive version of the key
                -> bool insensitive : true if searching with insensitive
    -Return type -> bool : True if query found

*/
bool WordHasher::wordSearch(Entry *node, std::string ogword, bool insensitive)
{
    std::vector<WordInfo> results = node -> words;
    size_t vecSize = results.size();
    
    if (insensitive)
    {
        //print all different word variations, locations and complete lines
        for (size_t i = 0; i < vecSize; ++i)
            printQuery(results[i].locations);

        return true;
    }

    //case sensitive, print location and complete line of exact match
    for (size_t i = 0; i < vecSize; ++i)
    {
        if (results[i].ogWord == ogword)
        {
            printQuery(results[i].locations);
            return true;
        }
    }
    return false;
}

/*
Reads files and prints queries with specified format.
    -Parameters -> vector<LocInfo> &locs : Vector with location info
    -Return type -> void
*/
void WordHasher::printQuery(std::vector<LocInfo> &locs)
{
    std::ifstream input;
    std::string s;
    size_t locSize = locs.size();
    bool flag = true;

    for (size_t j = 0; j < locSize; j++)
    {
        std::string filepath = locs[j].path;
        int lineCount = locs[j].line;

        input.open(filepath);

        //get line we are searching for
        for (int i = 1; i <= lineCount; ++i)
            getline(input,s);

        input.close();

        //Handle same query results
        if (j != 0)
            if (filepath == locs[j - 1].path && lineCount == locs[j - 1].line)
                flag = false;
        if (flag)    
            std::cout << filepath << ":" << lineCount << ": " << s <<std::endl;
        flag = true;
    }
}
