all:
	nasm -f elf64 test.s -o test.o
	gcc -no-pie main.c test.o -o main.out