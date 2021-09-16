#include "cli.h"

CLI* cli() {
    CLI* new_ = (CLI*)malloc(sizeof(CLI));
    new_->varAmount = 0;
    new_->addVar = &__ADDVARIABLE__;
    new_->run = &__RUNCLI__;
    new_->free = &__FREECLI__;
    new_->variableList = (VARIABLE**)malloc(sizeof(VARIABLE*));
    new_->addVar(new_, variable("x", "10", NUMBER));
    return new_;
}
void __ADDVARIABLE__(CLI* cli_, VARIABLE* var) {
    cli_->variableList == (VARIABLE**)realloc(cli_->variableList, sizeof(VARIABLE*)*(cli_->varAmount + 1));
    cli_->variableList[cli_->varAmount] = var;
    cli_->varAmount++;
    return;
}

void __RUNCLI__(CLI* cli_) {
    char* sub = (char*)malloc(sizeof(char));
     do {
        printf("\n=> ");
        fgets(cli_->currentCommand, MAX_COMMAND_SIZE, stdin);
        cli_->currentCommand[strlen(cli_->currentCommand)-1] = '\0';
        remove_spaces(cli_->currentCommand);

        if (!strcmp(cli_->currentCommand, "hlt")) {
            printf("__hlt__\n");
        } else {
            printf("%s\n", cli_->currentCommand);
            sub = substr(cli_->currentCommand, 0, 2);
            if (!strcmp(sub, "::")) {
                char flag = *substr(cli_->currentCommand, 2, 1);
                switch (flag) {
                case 'v': {
                    char* tostring;
                    for (size_t i = 0; i < cli_->varAmount; i++) {
                        tostring = cli_->variableList[i]->str(cli_->variableList[i]);
                        printf("%s", tostring);
                        free(tostring);
                    }
                }
                }
                    
            } else
                // Parse command
                printf("----%s----", sub);
        }
        
    } while (strcmp(cli_->currentCommand, "hlt"));
    free(sub);
}


void __FREECLI__(CLI* cli) {
    free(cli->currentCommand);
    // Free variables
    free(cli);
    return;
}