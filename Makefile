startAssembler: startAssembler.o
	gcc -Wall -pedantic startAssembler.o -o startAssembler
startAssembler.o: startAssembler.c preprocessor.h
	gcc -c -ansi -Wall -pedantic startAssembler.c -o startAssembler.o