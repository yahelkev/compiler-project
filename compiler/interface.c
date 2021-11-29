#include "interface.h"
#define _CRT_SECURE_NO_WARNINGS


int handleInput(int argc, char* argv[], char** fileNamePtr)
{
	int flag = 0;
	switch (argc)
	{
	case MIN_ARGC:
		flag = handleFlags(argv[1]);
		if (Unknown_Flag == flag){
			flag = None_Flag;
			checkSrcFile(argv[1]);
			*fileNamePtr = argv[1];
		}
		break;
	case MAX_ARGC:
		flag = handleFlags(argv[1]);
		if (Unknown_Flag == flag)
		{
			errorInterface("invalid flag! try -h for help\n");
		}
		checkSrcFile(argv[2]);
		*fileNamePtr = argv[2];
		break;
	default:
		errorInterface("make sure you use the form of <flag(optional)> <srcCode.ourLanguage>");
		break;
	}
	return flag;
}

void errorInterface(char* msg)
{
	printf("%s", msg);
	exit(0);
}

int handleFlags(char* flag)
{
	if (!strcmp(flag, "-h") || !strcmp(flag, "--help"))
	{
		printf("flag options are:\n");
		printf("	-h/--help - for help\n");
		errorInterface("	you can enter nothing to use no flag\n");
		//no need to return a thing because the program stops
	}
	else { return Unknown_Flag; };
}

void checkSrcFile(char* srcfileName)
{
	FILE* file;
	if (file = fopen(srcfileName, "r"))
	{
		int test = getc(file);
		if (test != EOF) {
			fclose(file);
			return;
		}
		fclose(file);
	}
	errorInterface("make sure src file exists!\n");
}


char* getFileContent(char* fileName)
{
	int i = 0;
	char ch = 0;
	char* buffer = NULL;
	long length = 0;
	FILE* file = fopen(fileName, "r");
	if (file)
	{
		fseek(file, 0, SEEK_END);
		length = ftell(file);
		fseek(file, 0, SEEK_SET);
		buffer = (char*)malloc(length);
		ch = fgetc(file);
		while (ch != EOF) {
			buffer[i] = ch;
			i++;
			ch = fgetc(file);
		}
		buffer[i] = '\0';
		fclose(file);
	}
	return buffer;
}