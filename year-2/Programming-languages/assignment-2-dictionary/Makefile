all: main
	
main: main.o dict.o lib.o 
	ld -o main main.o dict.o lib.o

main.o: main.asm
	nasm -f elf64 -o main.o main.asm

dict.o: dict.asm
	nasm -f elf64 -o dict.o dict.asm

lib.o: lib.asm
	nasm -f elf64 -o lib.o lib.asm

clean:
	rm -rf *.o main
