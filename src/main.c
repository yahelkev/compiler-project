#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/visitor.h"

int main(int argc, char** argv) {
    // i16 x = 10;
    ast* assigntree = make_variableExp("x", i16, make_integerExp(10));
    // i16 y = 3 + 2;
    ast* binarytree = make_variableExp("y", i16, 
                            make_binaryExp("+", make_integerExp(3), make_integerExp(2)));
    return 0;
}