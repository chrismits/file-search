/*
 *  Christos-Nikolaos Mitsopoulos 
 *
 *  FSTreeTraversal.cpp
 *  This file contains the FSTreeTraversal algorithm
 *  Modified  
 *           On       : 11/16/2017
 *
 */
#include "FSTree.h"
#include <iostream>

void listFiles(DirNode*, std::string);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: gerp directory" << std::endl;
        std::cerr << "          where:  directory is a valid directory" << std::endl;
        return 1;
    }

    std::string directory = argv[1];
    directory += "/";

    FSTree tree(directory);
    DirNode *root = tree.getRoot();
    listFiles(root, directory);

    return 0;
}

/*
Prints the full path of all files in the directory.
    -Parameters -> DirNode *node : The current node (directory)
                -> string directory : Name of directory
*/
void listFiles(DirNode *node, std::string directory)
{
    if (node -> isEmpty())
        return;
    
    int file_count = node -> numFiles();
    int dir_count = node -> numSubDirs();

    //print files:
    for (int i = 0; i < file_count; ++i)
        std::cout << directory + (node -> getFile(i)) << std::endl;

    //recurse in sub-directories:
    for (int i = 0; i < dir_count; ++i)
    {
        DirNode *tempNode = node -> getSubDir(i);
        std::string dir = directory + (tempNode -> getName()) + "/";
        listFiles(tempNode, dir);
    }
}
