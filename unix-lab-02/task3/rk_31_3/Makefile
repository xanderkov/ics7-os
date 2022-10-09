INCLUDE = defines.h 

APP = app.exe

OBJS = main.o io.o sort_matrix.o 

SRCS = main.c io.c sort_matrix.c

CFLAGS = -Wall -pedantic -Werror 

CC = gcc

${APP}: ${OBJS}
	${CC} $^ -o $@

%.o: %.c *.h
	${CC} ${CFLAGS} -c  $< 
