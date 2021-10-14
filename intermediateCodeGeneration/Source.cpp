#include <iostream>

#define INT_VAR_SIZE 4

void initIntAsm(std::string name, int value);

int main()
{
	initIntAsm("var", 23);
	return 0;
}

void initIntAsm(std::string name, int value)
{
	std::cout<< "sub esp," << INT_VAR_SIZE <<std::endl;
	std::cout<< "mov DWORD PTR _" + name + "$[ebp]," << value <<std::endl;
}