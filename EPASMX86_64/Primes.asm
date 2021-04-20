extern malloc: PROC
extern free: PROC

.code

generate_primes PROC
	; Rather than continually pushing and poping n and pSize to get around malloc overridding them since they are volatile
	; some non volatile registers can be backed up and then used to house n and pSize
	push r12
	push r13
	mov r12, rcx ; n
	mov r13, rdx ; pSize

	; Allocate boolean array
	shr rcx, 1
	sub rsp, 20h
	call malloc
	add rsp, 20h

	mov r8, 1
booleanFillLoop:
	mov rcx, r8
	shr rcx, 1
	mov byte ptr [rax + rcx], 1
	add r8, 2
	cmp r8, r12
	jl booleanFillLoop

	; Performs r11 = ceil(sqrt(r12)) for max factor, sqrt is done here and ceil is done implicitly using comparisons
	; Assumes sse2 support, todo check first and use fpu otherwise? 
	; sse2 support is in x86_64 baseline though, it might be fine to assume support
	cvtsi2sd xmm0, r12
	sqrtsd xmm0, xmm0
	cvttsd2si r11, xmm0

	mov r8, 3 ; outer loop counter
	mov r9, 1 ; prime count
primeCheckLoop:
    ; ensure prime status
	; TODO: See if any performance can be gained by moving this check, 3 will always pass
	mov rcx, r8
	shr rcx, 1
	cmp byte ptr [rax + rcx], 1
	jne primeCheckLoopIterate
	inc r9
	; ensure r8 <= r11/max factor for performance since only going that far will give the same result
	cmp r8, r11
	jg primeCheckLoopIterate
	; inner loop counter
	mov r10, r8
	shl r10, 1
	add r10, r8
primeCheckLoopInner:
	cmp r10, r12
	jnl primeCheckLoopIterate
	mov rcx, r10
	shr rcx, 1
	mov byte ptr [rax + rcx], 0
	add r10, r8
	add r10, r8
	jmp primeCheckLoopInner
primeCheckLoopIterate:
	add r8, 2
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

	mov dword ptr [rax], 2

	mov r8, 3; loop counter/bool array indexer
	mov r9, 4; int array indexer
primeFillLoop:
	mov rcx, r8
	shr rcx, 1
	cmp byte ptr [r13 + rcx], 1
	jne primeFillLoopIterate
	mov dword ptr [rax + r9], r8d
	add r9, 4
primeFillLoopIterate:
	add r8, 2
	cmp r8, r12
	jl primeFillLoop

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