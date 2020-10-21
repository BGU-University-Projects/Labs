all: exec

libs: asm-lib

asm-lib: skeleton.s
	nasm -g -f elf -o skeleton.o skeleton.s

exec: libs
	ld -m elf_i386 skeleton.o -o infect
	rm skeleton.o

.PHONY: clean
clean:
	rm -f *.o