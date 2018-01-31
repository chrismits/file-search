/*
 *  COMP 15 PROJ 2 - GERP
 *  Christos-Nikolaos Mitsopoulos 
 *
 *  entryStructs.h
 *	This file contains the structs used in the indexing purpose.
 *  They are in a separate file for separation of concerns purposes.
 *
 *  Information about structs:
 *      -> Entry : The main struct which is entered directly into the
 *                  hashtable, it contains the all-lowercase version of the 
 *                  word (key) and a vector of WordInfo structs.
 *      -> WordInfo : This struct contains the original word (case sensitive)
 *                    as well as a vector of LocInfo structs.
 *      -> LocInfo : This struct contains the full filepath of the file in
 *                   which the word can be found as well as an integer that 
 *                   represents the line in which the word is found.
 *
 *  Modified By (UTLN): cmitso01    
 *           On       : 11/30/2017
 *
*/

#ifndef ENTRYSTRUCTS_H
#define ENTRYSTRUCTS_H

//STRUCT 3
struct LocInfo {
	std::string path;
	int line;
    LocInfo(std::string s, int count) {
        path = s;
        line = count;
    };
};

//STRUCT 2
struct WordInfo {
	std::string ogWord; //original word
	std::vector<LocInfo> locations;
    WordInfo(std::string name, LocInfo loc) {
        ogWord = name;
        locations.push_back(loc);
    };
};

//MAIN STRUCT
struct Entry {
	std::string keyword;
	std::vector<WordInfo> words;
    Entry *next;
    Entry(){};
    Entry(std::string key, WordInfo word) {
        keyword = key;
        words.push_back(word);
        next = NULL;
    };
    Entry(Entry *node) {
        keyword = node -> keyword;
        words = node -> words;
        next = NULL;
    };
};

#endif
