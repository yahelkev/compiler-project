#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/visitor.h"

int main(int argc, char** argv) {
    // i16 x = 10;
    ast* assigntree = make_creationvariableExp("x", i16, make_integerExp(10));
    // i16 y = 3 + 2;
    ast* binarytree = make_creationvariableExp("y", i16, 
                            make_binaryExp("+", make_integerExp(3), make_integerExp(2)));

    printf("Parsing first tree ... %d\n", parse(assigntree));
    printf("Parsing second tree ... %d\n", parse(binarytree));
    return 0;
}