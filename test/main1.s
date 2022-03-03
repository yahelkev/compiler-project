.intel_syntax noprefix
_main:
    PUSH ebp
	MOV ebp, esp

	MOV eax, 17
	MOV DWORD PTR [rbp-4], eax

	MOV eax, 16
	MOV DWORD PTR [rbp-8], eax

	MOV eax, DWORD PTR [rbp-4]
	PUSH eax
	MOV eax, DWORD PTR [rbp-8]
	PUSH eax
	CALL ADD
	SUB ebp, 24
	MOV DWORD PTR [rbp-12], eax


	MOV eax, 0
	POP ebp
	RET