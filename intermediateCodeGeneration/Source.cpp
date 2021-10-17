#include <iostream>
#include <fstream>

#define INT_VAR_SIZE 4
#define ASM_FILE "../out.asm"

void initIntAsm(std::string name, std::string value);

int main()
{
	std::string AST[4] = { "var", "int", "=", "30" };
	if (AST[1] == "int")
	{
		initIntAsm(AST[0], AST[3]);
	}
	return 0;
}

void initIntAsm(std::string name, std::string value)
{
	std::ofstream outfile;
	outfile.open(ASM_FILE, std::ios_base::app);
	outfile << "mov     dword ptr[rbp - " << INT_VAR_SIZE << "], " << value <<std::endl;
}