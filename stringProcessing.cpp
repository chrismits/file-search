/*
 *  Christos-Nikolaos Mitsopoulos 
 *
 *  stringProcessing.cpp
 *  This file contains the function definitions (implementation) for
 *  the string processing algorithm
 *  Modified    
 *           On       : 11/16/2017
 *
 */

#include "stringProcessing.h"
using std::string;

/*
Removes all leading and trailing non-alphanumeric characters.
	-Parameters -> string s : The input string
	-Return type -> stirng : The modified string
*/
string stripNonAlphaNum(string s)
{
	size_t len = s.length();

	//strip leading
	while(!isalnum(s[0]))
	{
		if (len <= 0)
			return "";
		s.erase(0, 1);
		len--;
	}

	//strip trailing
	while(!isalnum(s[len]))
	{
		if (len <= 0)
			return "";
		s.erase(len, 1);
		len--;
	}

	return s;
}
