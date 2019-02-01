

CC = g++

LINK = -lSDL2 -lGL -lGLEW

TARGET = poly

OBJS = main.o Engine.o

all:${TARGET}

${TARGET}: main.o Engine.o
	${CC} ${LINK} ${OBJS} -o ${TARGET} 


main.o: main.cpp Engine.o
	${CC} -c main.cpp

Engine.o: Engine.cpp Engine.hpp Shaders.hpp
	${CC} -c Engine.cpp


clean:
	rm *.o

cleanEmacs:
	rm *.*~
