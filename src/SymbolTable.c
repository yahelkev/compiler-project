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
        for (size_t i = 0; i < table->size; i++)
            if(!strcmp(table->keys[i], key)) 
                return table->values[i];
    else {
        TABLE_VALUE val;
        struct error err = makeError("no such key");
        newValue(&val, ERROR_TAG, &err, -1, -1);
        return val;
    }
         // Switch for Error type TABLE_VALUE
}

/* Return :
    1 - Insertion succesded
    0 - Insertion Failed
*/
bool insertValue(Table* table, char* key, TABLE_VALUE value) {
    if (!isDefined(table,key)) {
            table->keys = (char**)realloc(table->keys, sizeof(char*) * ( table->size + 1 ));
            table->keys[table->size] = (char*)malloc(sizeof(char) * ( strlen(key) + 1 ));
            strcpy(table->keys[table->size], key);
            table->values = (TABLE_VALUE*)realloc(table->values, sizeof(TABLE_VALUE) * ( table->size + 1 ));
            table->values[table->size] = value;
            table->size++;

            return true;
    }

    return false;
}


struct function makeFunction(struct variable* args, int amount, char* returnType) {
    struct function func;
    func.args = args;
    func.amount = amount;
    func.returnType = (char*)malloc(sizeof(char) * ( strlen(returnType) + 1 ));
    strcpy(func.returnType, returnType);
    return func;
}

struct variable makeVariable(char* type) {
    struct variable var;
    var.type = (char*)malloc(sizeof(char) * ( strlen(type) + 1 ));
    strcpy(var.type, type);
    return var;
}

struct error makeError(char* msg) {
    struct error err;
    err.msg = (char*)malloc(sizeof(char) * ( strlen(msg) + 1 ));
    strcpy(err.msg, msg);
    return err;
}

void newValue(TABLE_VALUE* value, ValueTag tag, void* structValuePointer, int line, int column) {
    value->line = line;
    value->column = column;
    value->tag = tag;
    printf("Heyyyy %d %d", tag, VARIABLE_TAG);
    switch(tag) {
        FUNCTION_TAG: value->function = (struct function*)structValuePointer; printf("YEPPP"); return;
        VARIABLE_TAG: value->variable = (struct variable*)structValuePointer; printf("YEPPP1111111111") ;printf("[ %s ]\n", value->variable->type); return;
        ERROR_TAG: value->error = (struct error*)structValuePointer; return;
        default: return;
    }
}
