# startAssembler: startAssembler.o instructions.o symbolTableHandler.o
# 	gcc -Wall -pedantic startAssembler.o instructions.o symbolTableHandler.o -o startAssembler

# startAssembler.o: startAssembler.c constants.h instructions.h assembler.h symbolTableHandler.h
# 	gcc -c -ansi -Wall -pedantic startAssembler.c -o startAssembler.o
	
instructions.o: instructions.c instructions.h constants.h assembler.h
	gcc -ansi -Wall -pedantic instructions.c -o instructions

# symbolTableHandler.o: symbolTableHandler.c constants.h instructions.h assembler.h
# 	gcc -c -ansi -Wall -pedantic symbolTableHandler.c -o symbolTableHandler.o