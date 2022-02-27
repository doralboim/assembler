startAssembler: startAssembler.o instructions.o assemblerUtils.o
	gcc -Wall -pedantic startAssembler.o instructions.o assemblerUtils.o -o startAssembler

startAssembler.o: startAssembler.c constants.h instructions.h assembler.h utils.h
	gcc -c -ansi -Wall -pedantic startAssembler.c -o startAssembler.o
	
instructions.o: instructions.c instructions.h constants.h assembler.h
	gcc -c -ansi -Wall -pedantic instructions.c -o instructions.o

assemblerUtils.o: assemblerUtils.c constants.h instructions.h assembler.h
	gcc -c -ansi -Wall -pedantic assemblerUtils.c -o assemblerUtils.o