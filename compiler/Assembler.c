#include "Assembler.h"

void newAssembler(Assembler* asm, char* path, Table* table)
{
    asm->_obj = malloc(sizeof(ObjectFile));
    asm->table = table;
    newObjectFile(asm->_obj, path);
    setHeaders(asm->_obj, COMPUTER, 3, 0, 0, 0, 0, 0x0104);
}
void freeAssembler(Assembler* asm) {
    freeObjectFile(asm->_obj);
    free(asm->_obj);
}
void runAssembler(Assembler* asm) {
    int offsetHeadersEnd = sizeof(fileHeader) + 3 * sizeof(sectionHeadrer);
    asm->_obj->fileHeaders->symbolTablePtr =
     offsetHeadersEnd + asm->_obj->relocarionTableSize * sizeof(relocationTableSection);
    setSectionHeaders(asm->_obj->_textHeaders, 0, 0, strlen(asm->_obj->_textSection), offsetHeadersEnd,
        offsetHeadersEnd + strlen(asm->_obj->_textSection), 0, 0, 0, 0x60300020);
    setSectionHeaders(asm->_obj->_dataHeaders, 0, 0, 0, 0, 0, 0, 0, 0, 0xc0300040);
    setSectionHeaders(asm->_obj->_bssHeaders, 0, 0, 0, 0, 0, 0, 0, 0, 0xc0300080);
    
    writeFile(asm->_obj);
}