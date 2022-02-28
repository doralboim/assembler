startAssembler: startAssembler.o instructions.o handleSymbolTable.o
	gcc -Wall -pedantic startAssembler.o instructions.o handleSymbolTable.o -o startAssembler

startAssembler.o: startAssembler.c constants.h instructions.h assembler.h handleSymbolTable.h
	gcc -c -ansi -Wall -pedantic startAssembler.c -o startAssembler.o
	
instructions.o: instructions.c instructions.h constants.h assembler.h
	gcc -c -ansi -Wall -pedantic instructions.c -o instructions.o

handleSymbolTable.o: handleSymbolTable.c constants.h instructions.h assembler.h
	gcc -c -ansi -Wall -pedantic handleSymbolTable.c -o handleSymbolTable.o