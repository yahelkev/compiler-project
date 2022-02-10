#include "Assembler.h"

void newAssembler(Assembler* asm, char* path, Table* table)
{
    asm->_obj = malloc(sizeof(ObjectFile));
    asm->table = table;
    newObjectFile(asm->_obj, path);
    setHeaders(asm->_obj, 0x1c4, 0, 0, 0, 0, 0, 0x0104);
}
void freeAssembler(Assembler* asm) {
    freeObjectFile(asm->_obj);
    free(asm->_obj);
}
void runAssembler(Assembler* asm) {

    writeFile(asm->_obj);
}