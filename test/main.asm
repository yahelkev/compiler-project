	.file "D:\Magshimim\Year 3\Project\compiler-project\test\main.txt"
	.intel_syntax noprefix
	.text
	.def	___main;	.scl  2;	.type  32;	.endef
	.section .rdata,"dr"

ADD:
	PUSH rbp 
	MOV rbp, rsp
 
	POP rbp 
	RET 

main:
 
	PUSH rbp 
	MOV rbp, rsp
 

	MOV eax, 0 
	POP rbp 
	RET 
