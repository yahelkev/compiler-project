#pragma once
#include <stdio.h>

#define MIN_ARGC 2
#define MAX_ARGC 3
#define MAX_STRING 3
#define FILE_ENDING "ourLanguage"

enum Flags
{
	Unknown_Flag,
	None_Flag,
	Help_Flag
};

//the 'main' of the handler. returns the flag.
int handleInput(int argc, char* argv[]);

int handleFlags(char* flag);
void checkSrcFile(char* srcfileName);

void error(char* msg);