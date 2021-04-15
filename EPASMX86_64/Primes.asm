extern malloc: PROC

.code

generate_primes PROC
	; Backup n and pSize
	push rdx
	push rcx

	; Allocate boolean array
	sub rsp, 20h
	call malloc
	add rsp, 20h

	pop rcx ; n

	mov r8, 0
booleanFillLoop:
	mov byte ptr [rax + r8], 1
	inc r8
	cmp r8, rcx
	jne booleanFillLoop

	; Performs r11 = ceil(sqrt(rcx)) for max factor
	; Assumes sse2 support, todo check first and use fpu otherwise? 
	; sse2 support is in x86_64 baseline though, it might be fine to assume support
	cvtsi2sd xmm0, rcx
	sqrtsd xmm0, xmm0
	; Assumes sse4.1 support, this requires x86_64-v3 and so cannot realistically be assumed
	roundsd xmm0, xmm0, 00000010b
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
	jge primeCheckLoopIterate
	; inner loop counter
	mov r10, r8
	shl r10, 1
primeCheckLoopInner:
	; todo confirm this loop cannot write to unallocated memory if r10 >= rcx on the first iteration
	mov byte ptr [rax + r10], 0
	add r10, r8
	cmp r10, rcx
	jl primeCheckLoopInner
primeCheckLoopIterate:
	inc r8
	cmp r8, rcx
	jl primeCheckLoop
	
	pop rdx ; pSize
	push rax ; bool array
	push rcx ; n

	mov dword ptr [rdx], r9d

	; Allocate integer array
	shl r9, 2
	mov rcx, r9
	sub rsp, 20h
	call malloc
	add rsp, 20h

	pop rcx ; n
	pop rdx ; bool array

	mov r8, 2; loop counter/bool array indexer
		mov r9, 0; int array indexer
primeFillLoop:
	cmp byte ptr [rdx + r8], 1
	jne primeFillLoopIterate
	mov dword ptr [rax + r9], r8d
	add r9, 4
primeFillLoopIterate:
	inc r8
	cmp r8, rcx
	jng primeFillLoop

	ret
generate_primes ENDP

END