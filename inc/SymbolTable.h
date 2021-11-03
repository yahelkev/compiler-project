#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <string.h>
#include <stdlib.h>

#define bool int
#define false 0
#define true 1


// C Style
typedef struct TABLE_VALUE {
    enum {
        FUNCTION_TAG, VARIABLE_TAG
    }tag;
    int line;
    int column;

    union {
        
        struct varibale {
            char* type;
        }* varialble_;

        struct function {
            struct variable* args;
            char* returntype;
        }* function_;
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



#endif // !SYMBOL_TABLE_H