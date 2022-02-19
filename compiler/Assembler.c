#include "Assembler.h"

void newAssembler(Assembler* asm, char* path, Table* table)
{
    asm->_obj = malloc(sizeof(ObjectFile));
    asm->table = table;
    newObjectFile(asm->_obj, path);
    setHeaders(asm->_obj, COMPUTER_TYPE, 3, 0, 0, 0, 0, 0x0104);
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
        offsetHeadersEnd + strlen(asm->_obj->_textSection), 0, 0, 0, TEXT_CHARACTERISTICS);
    setSectionHeaders(asm->_obj->_dataHeaders, 0, 0, 0, 0, 0, 0, 0, 0, DATA_CHARACTERISTICS);
    setSectionHeaders(asm->_obj->_bssHeaders, 0, 0, 0, 0, 0, 0, 0, 0, BSS_CHARACTERISTICS);
    addSymbol(asm->_obj, "tryme", 123, 0, 0, 0x0, 0x1);
    addSymbol(asm->_obj, "secondd", 123, 0, 0, 0x0, 0x0);
    writeFile(asm->_obj);
}