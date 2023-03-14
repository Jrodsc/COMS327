#include <stdio.h>

/* Some declaration modifiers that we can use in C and C++
 * const - We are not allwed to change the value of a variable
 *          Enforced by de compiler
 * static - Static variables reside in the data segment
 *          static variables have a lifetime equal to the lifetime
 *          equal to the lifetime of the program, retains value across calls
 * auto - Default for variables on the stack. Doesn't need to be used
 *          explicity
 * register - hint to the compuler that we think it woould be a good performance choice
 *              to place this variable in a register (do not allocate space in memory)
 * volatile - tells the ocmpiler that the variable may be changed outside of the 
 *              contained code
 */

int main(int argc, char * argv[]){
    
    const int i = 0;

    *((int *) &i) = 1;

    printf("%d\n", i);

    return 0;
}
