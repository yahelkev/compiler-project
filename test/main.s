	.file "D:\Magshimim\Year 3\Project\compiler-project\test\main.txt"
	.intel_syntax noprefix
	.text
	.def	___main;	.scl  2;	.type  32;	.endef
	.section .rdata,"dr"

printi:
 
	RET 


printf:
 
	RET 


prints:
 
	RET 


add:
	PUSH ebp 
	MOV ebp, esp
 
	MOV eax, DWORD PTR [ebp+4] 
	ADD eax, DWORD PTR [ebp+8] 
	POP ebp 
	RET 

_main:
 
	PUSH ebp 
	MOV ebp, esp
 
	MOV eax, 17 
	MOV DWORD PTR [ebp-4], eax
 
	MOV eax, 16 
	MOV DWORD PTR [ebp-8], eax
 
	MOV eax, DWORD PTR [ebp-4] 
	PUSH eax 
	MOV eax, DWORD PTR [ebp-8] 
	PUSH eax 
	CALL add 
	SUB ebp, 24 
	MOV DWORD PTR [ebp-12], eax
 

	MOV eax, 0 
	POP ebp 
	RET 
