CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic -g3
SRC_PATH := src/
SRC := $(SRC_PATH)io.c $(SRC_PATH)matrix_operation.c $(SRC_PATH)main.c $(UNIT_PATH)unit_test.c
UNIT :=  $(UNIT_PATH)unit_test.c
MAIN := $(SRC_PATH)main.c
OBJ :=  io.o array_operations.o list_operations.o time.o

app.exe : $(OBJ) main.o
	$(CC) -o app.exe  $(OBJ) main.o

%.o : %.c *.h
	$(CC) $(CFLAGS) -c $<


.PHONY : clean
clean :
	rm *.o *.exe