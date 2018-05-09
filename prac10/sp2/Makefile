20111586.out : 20111586.o shell.o assembler.o memory.o opcode.o
	gcc -o 20111586.out 20111586.o shell.o assembler.o memory.o opcode.o

20111586.o : 20111586.c 20111586.h
	gcc -c 20111586.c -Wall

shell.o : shell.c shell.h
	gcc -c shell.c -Wall

assembler.o : assembler.c assembler.h
	gcc -c assembler.c -Wall

memory.o : memory.c memory.h
	gcc -c memory.c -Wall

opcode.o : opcode.c opcode.h
	gcc -c opcode.c -Wall

clean :
	rm 20111586.o shell.o assembler.o memory.o opcode.o
