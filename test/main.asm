	.file "D:\Magshimim\Year 3\Project\compiler-project\test\main.txt"
	.intel_syntax noprefix
	.text
	.def	___main;	.scl  2;	.type  32;	.endef
	.section .rdata,"dr"

ADD:
	PUSH rbp 
	MOV rbp, rsp
 
	MOV eax, 5 
	MOV DWORD PTR [rbp-4], eax
 
	MOV eax, DWORD PTR [rbp+4] 
	ADD eax, DWORD PTR [rbp+8] 
	IMUL eax, DWORD PTR [rbp-4] 
	POP rbp 
	RET 

main:
 
	PUSH rbp 
	MOV rbp, rsp
 
	MOV eax, 1 
	PUSH eax 
	MOV eax, 2 
	PUSH eax 
	CALL ADD 
	ADD eax, 1 
	MOV DWORD PTR [rbp-8], eax
 

	MOV eax, 0 
	POP rbp 
	RET 
