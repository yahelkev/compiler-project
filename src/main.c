#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cli.h"
int main(int argc, char** argv, char** env) {
    if (argc < 2) {
        CLI* cli_ = cli();
        cli_->run(cli_);
        // Run cli
    } else {
        // Get flags
        // Compile src file to .exe
    }
    
    return 0;
}