#include "../inc/SymbolTable.h"

void newTable(Table* table) {
    table->size = 0;
    table->keys = (char**)malloc(sizeof(char*) * ( table->size + 1 ));
    table->values = (TABLE_VALUE*)malloc(sizeof(TABLE_VALUE) * ( table->size + 1 ));
    return;
}

bool isDefined(Table* table, char* key) {
    for (size_t i = 0; i < table->size; i++)
        if(!strcmp(table->keys[i], key)) return true;
    
    return false;   
}

TABLE_VALUE getValue(Table* table, char* key) {
    if(isDefined(table, key))
        for (size_t i = 0; i < count; i++)
            if(!strcmp(table->keys[i], key)) 
                return table->values[i];
    else
        return // Switch for Error type TABLE_VALUE
}

/* Return :
    1 - Insertion succesded
    0 - Insertion Failed
*/
bool insertValue(Table* table, char* key, TABLE_VALUE value) {
    if (!isDefined(table,key)) {
            table->keys = (char**)realloc(sizeof(char*) * ( table->size + 1 ));
            table->keys[table->size] = (char*)malloc(sizeof(char) * ( strlen(key) + 1 ));
            strcpy(table->keys[i], key);
            table->values = (TABLE_VALUE*)malloc(sizeof(TABLE_VALUE) * ( table->size + 1 ));
            table->values[table->size] = value;
            table->size++;

            return true;
    }
    
    return false;
}
