all: start.o firstIter.o utils.o produceOutput.o instructions.o constants.o dataHandler.o symbolTableHandler.o secondIteration.o encoder.o
	gcc -g -Wall -pedantic produceOutput.o secondIteration.o firstIter.o encoder.o start.o constants.o utils.o instructions.o dataHandler.o symbolTableHandler.o -o all

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

produceOutput: produceOutput.c assembler.h constants.h
	gcc -c -g -ansi -Wall -pedantic produceOutput.c -o produceOutput.o

main: start.c utils.h assembler.h constants.h
	gcc -c -g -ansi -Wall -pedantic start.c -o start.o

clean:
	rm *.o
