#include <stdio.h>
#include "interface.h"

void  main(int argc, char* argv[])
{
    char* srcFileName = NULL;
    int flag = handleInput(argc, argv, &srcFileName);
    printf("flag: %d\n", flag);
    printf("file: %s\n", srcFileName);

    char* fileContent = getFileContent(srcFileName);
    printf("\n%s\n", fileContent);
    if (fileContent) {
        free(fileContent);
    }

}