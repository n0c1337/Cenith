section .text
global test
test:
push rbp
mov rbp, rsp
sub rsp, 12
mov r10d, 2
mov DWORD [rsp-4], r10d

mov r10d, DWORD [rsp-4]
imul r10d, 6
mov DWORD [rsp-4], r10d

mov r10d, 0
mov DWORD [rsp-8], r10d

mov r10d, 0
mov DWORD [rsp-12], r10d

jmp .L11

.L1:
mov r10d, DWORD [rsp-12]
add r10d, 1
mov DWORD [rsp-12], r10d

mov r10d, DWORD [rsp-12]
mov DWORD [rsp-8], r10d

mov r10d, DWORD [rsp-8]
imul r10d, 2
mov DWORD [rsp-8], r10d
mov eax, DWORD [rsp-8]

.L11:
mov r10d, DWORD [rsp-12]
cmp r10d, 9
mov DWORD [rsp-12], r10d

jne .L1

add rsp, 12
mov rsp, rbp
pop rbp

;main:
;        push    rbp
;        mov     rbp, rsp
;        mov     DWORD PTR [rbp-12], 32
;        lea     rax, [rbp-12]
;        mov     QWORD PTR [rbp-8], rax
;        nop
;        mov     rax, QWORD PTR [rbp-8] <------- qword cause pointer is 64
;        mov     DWORD PTR [rax], 2     <------- dword cause pointer to i32 
;        mov     eax, 0
;        pop     rbp
; ret