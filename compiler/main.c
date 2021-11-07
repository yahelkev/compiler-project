#include <stdio.h>
#include "interface.h"

void  main(int argc, char* argv[])
{
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    printf("flag: %d\n", flag);
    printf("file: %s\n", srcFileName);

}