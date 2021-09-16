#include "../include/cli.h"

CLI* cli() {
    CLI* new = (CLI*)malloc(sizeof(CLI));
    new->varAmount = 0;
    new->addVar = &__ADDVARIABLE__;
    new->run = &__RUNCLI__;
}
void __ADDVARIABLE__(CLI* cli_, VARIABLE* var) {
    cli_->variableList == (VARIABLE**)realloc(cli_->variableList, sizeof(VARIABLE*)*(cli_->varAmount + 1));
    cli_->variableList[cli_->varAmount] = var;
    cli_->varAmount++;
    return;
}

void __RUNCLI__(CLI* cli_) {
    while(1) {
        printf("=> ");
        
        fgets(cli_->currentCommand, MAX_COMMAND_SIZE, stdin);
        cli_->currentCommand[strlen(cli_->currentCommand)-1] = '\0';
        remove_spaces(cli_->currentCommand);
        // if(!strcmp())
        printf("%s\n", cli_->currentCommand);
        char* retu = substr(cli_->currentCommand, 0, 2);
        printf("----%s----", retu);
        free(retu);
    }
}