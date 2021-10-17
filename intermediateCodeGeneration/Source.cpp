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
	std::cout<< "mov     dword ptr[rbp - " << INT_VAR_SIZE << "], " << value <<std::endl;
}