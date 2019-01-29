

CC = g++

LINK = -lSDL2 -lGL -lGLEW

TARGET = poly

OBJS = main.o

all:${TARGET}

main.o: main.cpp
	${CC} -c main.cpp

${TARGET}: main.o
	${CC} ${LINK} -o ${TARGET} ${OBJS}



