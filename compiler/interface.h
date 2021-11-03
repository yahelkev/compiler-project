#pragma once
#include <stdio.h>

#define MIN_ARGC 2
#define MAX_ARGC 3
#define MAX_STRING 3
#define FILE_ENDING "ourLanguage"


//the 'main' of the handler
void handleInput(int argc, char* argv[]);

void handleFlags(char* flag);
void checkSrcFile(char* srcfileName);

void error(char* msg);