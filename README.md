Christos-Nikolaos Mitsopoulos
11/30/2017


## Purpose of the program

This program indexes and searches a directory for a specified string (basically like the grep unix program). It then returns a list of all matches
in files of current directory. Each match contains the full filepath, line number and full line where it was found.

## Files provided

- WordHasher.h -> The WordHasher class header and function definitions
- WordHasher.cpp -> The WordHasher class implementation that
					contains declarations of functions
- main.cpp -> The script for the final executable
- entryStructs.h -> The structs used for word indexing
- stringProcessing.h -> Function declaration for stringProcessing function
- stringProcessing.cpp -> Function definition for above
- Makefile -> The instructions for "make" to compile the program

## How to use
To start, clone this project and its contents to your local machine. Then, open a command line utility program and do the following:

To compile the program, simply type "make" in the command line window.
To run the program, type the following in the command line:

./gerp [DirectoryToIndex]

where DirectoryToIndex determines the path of the directory which will be traversedand indexed.

After indexing the directory, the program will prompt you for a query where you can enter any of the following:

	- A string
	- @i and a string (or @insensitive) will do a case insensitive search.
	- @q or @quit will quit the program.


## Architectural Overview

The WordHasher class utilizes the FSTree implementation and represents the input directory as a tree with a pointer to its DirNode root. The tree is then traversed using a pointer to the root of the subtree each time. The hashtable implemented using the WordHasher class is based on the structs found in entryStructs.h, as the sequence that represents the hashtable consists of Entry node pointers. Pointers were chosen so that when an index is empty, the node in that index points to null.

### Outline of Data Structures

The indexing part of this project was completed using a hashtable implemented in the WordHasher class. The hashtable is represented by a dynamic array
of Entry struct pointers. My implementation uses the built in STL function std::hash and generates an index by inputting a string. Collisions are 
then handled using the chaining method. That is, every index in the sequence contains a linked list. In order to better understand the way information
about words is stored in the hashtable and how it is added, it is prudent to understand what information is needed for every word in the query. 
When printing query results we need:

	- the full file path
	- the word itself
	- the line number
	- the full line where the word is found
	- In the case of insensitive search, all variations of the word found.

### How is all this data stored in the table?

The hashtable stores entry struct pointers. An entry struct pointer
consists of:
	
	- All lowercase version of the word(basically a string that represents the key and this is what is hashed)
	- A vector of WordInfo nodes which consist of:
		1. A string that represents the original word found in file
		2. A vector of LocInfo nodes which consist of:
			- The full filepath in a string
			- The line number of the word in the file.
		3. A pointer to the next entry node.

To sum up, each index in the hashtable contains an all lowercase versionof a word, a vector of all the different variations (capitalizations) of
this word found in the directory and a vector of all the different locations of all the different variations found.

### Why a hashtable?

A hashtable was chosen for very fast retrieval of information once the key (lowercase version of word) is known (sequential access of array).

In terms of what is stored in the table:

Think about the case where two words are the same in an all lowercase version, but have different capitalizations. If you are talking about
large directories with a large amount of files, this happens really often. Now imagine if each different word variation was in an entirely
different index. This would lead into an extremely large array and a large amount of indexes which is memory inefficient. By storing
different variations in the same struct, this problem is eliminated.

### How are collisions handled?

Collisions are handled by implementing a linked list structure at every index. The way this is done is, each Entry node contains a pointer to 
another entry node (or NULL) as in a linked list. Therefore, multiple different keys can co-exist in the same index (however performance
decreases if linked list has to be traversed to find a query because there is no sequential access).

### How do I make the querying faster?

Every 10 files, my program checks whether the hashtable needs to be expanded and rehashed in order to reduce the length of linked lists
in indexes (which leads to faster querying in the end, although indexing slows down a bit). The algorithm, chooses that an expansion
is required, if the load factor (ratio of entries to array size) is greater than 0.7.

### What could I improve in this algorithm?

Right now, even if two words are exactly the same, a new location entryis created everytime. I chose this instead of looping through the location
vector and seeing if the same location is already added because looping through each location vector was time and resource consuming.

### Memory vs time trade-off

For my algorithm, I chose faster runtime instead of low memory usage. I think this was an appropriate choice regarding the deliverables of 
this project, as I was below the maximum RAM usage and my indexing was really fast. For comparison, here are my results for the largeGutenberg directory:

	- User time (seconds): 46.39
	- Maximum resident set size (kbytes): 7953164 -> 8gb

## Testing

The testing for this project is mostly based on trial and error. Here are some of the testing methods I used:

For file traversal:

	- Create many directories with subdirectories inside them and empty files.
	- Print and open all filenames and check results against expected ls command.

For addition functionality:
I created printTable() that prints the entries currently in the hashtable along with other useful information such as line number in which word is found,
along with all its different variations. Using this function I tested all the different cases of addition and checked the results. After this I checked
edge cases such as when a string is full of nonalphanumerical characters.

I then created a sample Directory with a single file in it and filled the file with sample tests such as:
- Same word - different variations
- Same word - same line

