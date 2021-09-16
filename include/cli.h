#ifndef CLI_H
#define CLI_H
#include "variable.h"
#include "tooling.h"
#include "SIZES.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct CLI_ {
    VARIABLE** variableList;
    int varAmount;

    char currentCommand[MAX_COMMAND_SIZE];

    void (*addVar)(struct CLI_*, VARIABLE*);
    void (*run)(struct CLI_*);
    void (*free)(struct CLI_*);
}CLI;

CLI* cli();
void __ADDVARIABLE__(CLI* cli_, VARIABLE* var);
void __RUNCLI__(CLI* cli_);
void __FREECLI__(CLI* cli);
#endif // !CLI_H