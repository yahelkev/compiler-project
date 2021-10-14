#include <iostream>

#define INT_VAR_SIZE 4

void initIntAsm(std::string name, std::string value);

int main()
{
	std::string AST[4] = { "var", "int", "=", "29" };
	if (AST[1] == "int")
	{
		initIntAsm(AST[0], AST[3]);
	}
	return 0;
}

void initIntAsm(std::string name, std::string value)
{
	std::cout<< "sub esp," << INT_VAR_SIZE <<std::endl;
	std::cout<< "mov DWORD PTR _" + name + "$[ebp]," << value <<std::endl;
}