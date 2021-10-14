#include <iostream>

void initIntAsm(std::string name, int value);

int main()
{
	initIntAsm("var", 23);
	return 0;
}

void initIntAsm(std::string name, int value)
{
	std::cout<< "*asm code*" <<std::endl;
}