#include "interface.h"
#define _CRT_SECURE_NO_WARNINGS


void handleInput(int argc, char* argv[])
{
	switch (argc)
	{
	case MIN_ARGC:
		handleFlags(argv[1]);
		checkSrcFile(argv[1]);
		break;
	case MAX_ARGC:
		handleFlags(argv[1]);
		error("invalid flag! try -h for help\n");
		break;
	default:
		error("make sure you use the form of <flag(optional)> <srcCode.ourLanguage>");
		break;
	}
}

void error(char* msg)
{
	printf("%s", msg);
	exit(0);
}

void handleFlags(char* flag)
{
	if (!strcmp(flag, "-h") || !strcmp(flag, "--help"))
	{
		printf("flag options are:\n");
		printf("	-h/--help - for help\n");
		error("	you can enter nothing to use no flag");
	}
	else { return; };
}

void checkSrcFile(char* srcfileName)
{
	FILE* file;
	if (file = fopen(srcfileName, "r"))
	{
		fclose(file);
		printf("start compiling...");
		return;
	}
	printf("make sure src file exists!\n");
}
