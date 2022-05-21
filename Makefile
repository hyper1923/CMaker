
CC = g++
INCLUDE = -I
OUTPUT = -o CMaker.exec
FILES = main.cpp

build:
	${CC} ${FILES} ${OUTPUT}

run:
	./CMaker.exec --build ./test.maker