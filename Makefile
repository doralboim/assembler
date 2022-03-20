# startAssembler: startAssembler.o instructions.o symbolTableHandler.o
# 	gcc -Wall -pedantic startAssembler.o instructions.o symbolTableHandler.o -o startAssembler

# startAssembler.o: startAssembler.c constants.h instructions.h assembler.h symbolTableHandler.h
# 	gcc -c -ansi -Wall -pedantic startAssembler.c -o startAssembler.o
	
# instructions.o: instructions.c instructions.h constants.h assembler.h
# 	gcc -ansi -Wall -pedantic instructions.c -o instructions

# symbolTableHandler.o: symbolTableHandler.c constants.h instructions.h assembler.h
# 	gcc -c -ansi -Wall -pedantic symbolTableHandler.c -o symbolTableHandler.o

# utils: utils.o
# 	gcc -Wall -pedantic utils.o -o utils

# firstIter: firstIter.o utils.o dataHandler.o symbolTableHandler.o constants.o
# 	gcc -Wall -pedantic firstIter.o utils.o dataHandler.o symbolTableHandler.o -o firstIter

# firstIter.o: firstIter.c assembler.h constants.h utils.h
# 	gcc -c -ansi -Wall -pedantic firstIter.c -o firstIter.o

all: main.o firstIter.o utils.o instructions.o constants.o dataHandler.o symbolTableHandler.o secondIteration.o encoder.o
	gcc -Wall -pedantic -g main.o firstIter.o secondIteration.o encoder.o constants.o utils.o instructions.o dataHandler.o symbolTableHandler.o -o all

instructions.o: instructions.c instructions.h assembler.h
	gcc -c -g -ansi -Wall -pedantic instructions.c -o instructions.o

utils.o: utils.c utils.h constants.h assembler.h instructions.h
	gcc -c -g -ansi -Wall -pedantic utils.c -o utils.o

constants.o: constants.c constants.h
	gcc -c -g -ansi -Wall -pedantic constants.c -o constants.o

dataHandler.o: dataHandler.c constants.h
	gcc -c -g -ansi -Wall -pedantic dataHandler.c -o dataHandler.o

symbolTableHandler.o: symbolTableHandler.c constants.h assembler.h utils.h
	gcc -c -g -ansi -Wall -pedantic symbolTableHandler.c -o symbolTableHandler.o

firstIter.o: firstIter.c assembler.h constants.h dataHandler.h utils.h instructions.h
	gcc -c -g -ansi -Wall -pedantic firstIter.c -o firstIter.o

secondIteration: secondIteration.c assembler.h constants.h encoder.h
	gcc -c -g -ansi -Wall -pedantic secondIteration.c -o secondIteration.o

encoder: encoder.c constants.h assembler.h
	gcc -c -g -ansi -Wall -pedantic encoder.c -o encoder.o

main: main.c utils.h assembler.h constants.h
	gcc -c -g -ansi -Wall -pedantic main.c -o main.o
