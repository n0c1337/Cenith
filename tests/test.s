section .text
global test
cooltest:
push rbp
mov rbp, rsp
sub rsp, 12
mov r10d, DWORD [rsp-12]
mov r10d, 2
mov DWORD [rsp-12], r10d

mov r10d, DWORD [rsp-12]
imul r10d, 6
mov DWORD [rsp-12], r10d

mov r10d, DWORD [rsp-8]
mov r10d, 0
mov DWORD [rsp-8], r10d

jmp .L11

.L1:
mov r10d, DWORD [rsp-8]
add r10d, 1
mov DWORD [rsp-8], r10d

mov r10d, DWORD [rsp-12]
mov r10d, 0
mov DWORD [rsp-12], r10d

mov r10d, DWORD [rsp-12]
add r10d, DWORD [rsp-8]
mov eax, r10d
mov DWORD [rsp-12], r10d

.L11:
mov r10d, DWORD [rsp-8]
cmp r10d, 9
mov DWORD [rsp-8], r10d

jle .L1

add rsp, 12
mov rsp, rbp
pop rbp
ret