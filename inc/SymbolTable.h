#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <stdlib.h>

#define bool int
#define false 0
#define true 1

enum {
        FUNCTION_TAG, VARIABLE_TAG, ERROR_TAG
}ValueTag;
// C Style
typedef struct TABLE_VALUE {
    ValueTag tag;
    int line;
    int column;

    union {
        
        struct varibale {
            char* type;
        }* variable_;

        struct function {
            struct variable* args;
            char* returnType;
        }* function_;

        struct error {
            char* msg;
        }* error_;
    };
}TABLE_VALUE;


typedef struct Table {
    int size;
    char** keys;
    TABLE_VALUE* values;
}Table;

void newTable(Table* table);
bool isDefined(Table* table, char* key);
TABLE_VALUE getValue(Table* table, char* key);
/* Return :
    1 - Insertion succesded
    0 - Insertion Failed
*/
bool insertValue(Table* table, char* key, TABLE_VALUE value);

struct function makeFunction(struct variables* args, char* returnType);
struct variable makeVariable(char* type);
void newValue(TABLE_VALUE* value, ValueTag tag, void* structValuePointer, int line, int column);




#endif // !SYMBOL_TABLE_H