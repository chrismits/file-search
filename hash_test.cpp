/*
 * @brief A short program that demonstrates use of the STL hash facility.
 * @details
 *      This program shows two ways to use the C++ STL hash facility,
 *      and also illustrates I/O manipulation tools for columnar
 *      output.
 *
 *      The program hashes all the command-line parameters after the
 *      program name using two different methods of invoking a hash.
 *
 *      Usage:  hash_test key1 ...
 *
 *      The I/O stuff is secondary, but, if you haven't seen it you
 *      might find it helpful.
 *
 *      Crucial things to understand:
 *           - You must #include <functional> to use the hash facility.
 *           - std::hash is a templated struct/class, not a function
 *           - It overrides operator(), which allows you to use
 *             instances as if they were functions.
 *           - The template can only be instantiated for particular,
 *             supported types.  It isn't a panacea.
 *           - The standard library includes a specialization for C++
 *             string values, but NOT C strings/pointers to char
 *             (pointers to char are hashed on the pointer value, not
 *             the string contents).
 *           - Because std::hash is a struct/class, you must make
 *             instance before you can use it, just like any
 *             struct/class. 
 *           - There are two ways to make an instance:
 *             . You can declare a variable that holds an instance,
 *               which will run the default constructor:
 *
 *                       hash<string> hash_function;
 *             
 *               Then you can call the instance as if it were a
 *               function that takes a key (string here) and returns a
 *               size_t.
 *             . You can make an instance and use it immediately:
 *
 *                       size_t slot = hash<string>{}(key) % tablesize
 *               or
 *                       size_t slot = hash<string>()(key) % tablesize
 *
 *               In both cases, the empty braces/parens are there to
 *               indicate that an instance should be made just for
 *               right here, and the default constructor should run.
 *               Once a hash object/instance has been constructed, it
 *               can be applied using normal function call syntax
 *               "(key)" after the object --- this cases C++ to use
 *               the operator() defined for hashes.
 *           - Many students create trouble by having something else
 *             named "hash" in their program (a data member, local
 *             variable, function, etc.).  This just creates trouble.
 *             Don't name anythin in your program "hash" when using
 *             this facility.
 *
 * @author Mark A. Sheldon
 * @copyright Mark A. Sheldon, Tufts University, Fall 2016
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <functional>

using namespace std;

/*
 * Constants for output formatting.
 * The string being hashed will be left justified in a field of 
 * STRING_WIDTH columns.  (We could make a pass over the command line
 * parameters and find the widest one, which would be better.)  The
 * hash code is formatted in a field that depends on the size of a
 * size_t in the current C++ implementation.  I'm allowing 3 decimal
 * digits for each byte in a size_t, and then 2 spaces as a column
 * separator.
 */
const int STRING_WIDTH = 10;
const int HASH_WIDTH   = 3 * sizeof(size_t) + 2;

int main(int argc, char *argv[])
{
        hash<string> hash_function;

        for (int i = 1; i < argc; ++i)
		cout << setw(STRING_WIDTH) << left
                     << argv[i]
                     << " hashes to:  "
                     << setw(HASH_WIDTH)   << right
                     << hash_function(string(argv[i]))
                     << setw(HASH_WIDTH)   << right
		     << hash<string>{}(string(argv[i]))
                     /* could  use  () rather than {} here */
                     << endl;
        return 0; 
}
