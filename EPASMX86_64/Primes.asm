extern malloc: PROC
extern free: PROC

.code

generate_primes PROC
	; Rather than continually pushing and poping n and pSize to get around malloc overridding them since they are volatile
	; some non volatile registers can be backed up and then used to house n and pSize
	; Backup n and pSize
	push r12
	push r13
	mov r12, rcx
	mov r13, rdx

	; Allocate boolean array
	sub rsp, 20h
	call malloc
	add rsp, 20h

	mov r8, 2
booleanFillLoop:
	mov byte ptr [rax + r8], 1
	inc r8
	cmp r8, r12
	jne booleanFillLoop

	; Performs r11 = ceil(sqrt(r12)) for max factor, sqrt is done here and ceil is done implicitly using comparisons
	; Assumes sse2 support, todo check first and use fpu otherwise? 
	; sse2 support is in x86_64 baseline though, it might be fine to assume support
	cvtsi2sd xmm0, r12
	sqrtsd xmm0, xmm0
	cvttsd2si r11, xmm0

	mov r8, 2 ; outer loop counter
	mov r9, 0 ; prime count
primeCheckLoop:
    ; ensure prime status
	cmp byte ptr [rax + r8], 1
	jne primeCheckLoopIterate
	inc r9
	; ensure r8 < r11/max factor for performance since only going that far will give the same result
	cmp r8, r11
	jg primeCheckLoopIterate
	; inner loop counter
	mov r10, r8
	shl r10, 1
primeCheckLoopInner:
	cmp r10, r12
	jnl primeCheckLoopIterate
	mov byte ptr [rax + r10], 0
	add r10, r8
	jmp primeCheckLoopInner
primeCheckLoopIterate:
	inc r8
	cmp r8, r12
	jl primeCheckLoop
	
	mov dword ptr [r13], r9d

	mov r13, rax ; bool array

	; Allocate integer array
	shl r9, 2
	mov rcx, r9
	sub rsp, 20h
	call malloc
	add rsp, 20h

	mov r8, 2; loop counter/bool array indexer
	mov r9, 0; int array indexer
primeFillLoop:
	cmp byte ptr [r13 + r8], 1
	jne primeFillLoopIterate
	mov dword ptr [rax + r9], r8d
	add r9, 4
primeFillLoopIterate:
	inc r8
	cmp r8, r12
	jng primeFillLoop

	mov rcx, r13
	mov r13, rax
	sub rsp, 20h
	call free
	add rsp, 20h

	mov rax, r13

	pop r13
	pop r12

	ret
generate_primes ENDP

END