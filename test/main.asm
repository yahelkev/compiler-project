	.file "D:\Magshimim\Year 3\Project\compiler-project\test\main.txt"
	.intel_syntax noprefix
	.text
	.def	___main;	.scl  2;	.type  32;	.endef
	.section .rdata,"dr"
main:
 
	PUSH rbp 
	MOV rbp, rsp
 
	MOV eax, 5 
	MOV DWORD PTR [rbp-4], eax
 
	MOV eax, DWORD PTR [rbp-4] 
	ADD eax, 17 
	MOV DWORD PTR [rbp-8], eax
 
START_LOOP0: 
	MOV eax, DWORD PTR [rbp-4] 
	CMP eax, DWORD PTR [rbp-8] 
	jge END_LOOP0 
	MOV eax, DWORD PTR [rbp-4] 
	ADD eax, 1 
	MOV	DWORD PTR [rbp-4], eax 
	JMP START_LOOP0
 
END_LOOP0: 

	MOV eax, 0 
	POP rbp 
	RET 
