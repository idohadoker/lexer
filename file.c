
// Modular Programming in C
// ========================
//
// see https://www.embedded.com/design/prototyping-and-development/4023876/Modular-Programming-in-C
//

/*
    How do you organize medium-sized or larger C programs? Few C textbooks give any insight; 
    they concentrate on exposition of C's features using small examples.
    The examples usually fit in a single source code file. 
    Without some guiding principle of organization, larger C programs can become difficult to 
    understand and impossible to maintain. 
    Modular programming is one way of managing the complexity. 
*/

// The two most important elements of a module are the division of the module into an interface and an implementation and the ability to hide information in the implementation. A syntax for creating a module (similar to that of Modula) would be:

// DEFINITION MODULE foo
//    declarations of exported functions
//    EXPORT list of functions and data
//        and data
// END foo
//
// IMPLEMENTATION MODULE foo
//   IMPORT list of modules used
//    ... code ...
// END foo 

#include "list.h"
#include "smt.h"

int main(int argc, char **argv)
{
    int a = 1;
    int b = 2;

    pri(b);
    
    printf("\n add(%i,%i)=%i \n", a, b, add(a,b));
    printf("\n sub(%i,%i)=%i \n", a, b, sub(a,b));
}

// see https://bellard.org/tcc/tcc-doc.html
//
// run with: "tcc mod_math.c -run mod_test.c"

