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
    addSymbol(asm->_obj, ".file", 0, MAGE_SYM_DEBUG, 0, IMAGE_SYM_CLASS_FILE, 0);

    for (size_t i = 0; i < asm->table->size; i++) {
        if (FUNCTION_TAG == asm->table->values[i]->tag) {
            printf("%s\n", asm->table->keys[i]);
            addSymbol(asm->_obj, asm->table->keys[i], 0, TEXT_SECTION_NUM, DT_FUNCTION, IMAGE_SYM_CLASS_EXTERNAL, 0);
        }
    }

    addSymbol(asm->_obj, ".text", 0, TEXT_SECTION_NUM, 0, IMAGE_SYM_CLASS_STATIC, 0);
    addSymbol(asm->_obj, ".data", 0, DATA_SECTION_NUM, 0, IMAGE_SYM_CLASS_STATIC, 0);
    addSymbol(asm->_obj, ".bss", 0, BSS_SECTION_NUM, 0, IMAGE_SYM_CLASS_STATIC, 0);
    writeFile(asm->_obj);
}