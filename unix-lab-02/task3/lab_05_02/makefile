CC := gcc
CFLAGS := -std=gnu99 -Wall -Werror -I inc/ -pedantic -g3
SRC_PATH := src/
INC := inc/
OUT := out/
UNIT_PATH := unit_tests/
OBJ := $(OUT)io.o $(OUT)array.o $(OUT)list.o $(OUT)time.o

app.exe : $(OBJ) $(OUT)main.o
	$(CC) -o app.exe  $(OBJ) $(OUT)main.o


$(OUT)%.o : $(SRC_PATH)%.c $(INC)*.h
	$(CC) $(CFLAGS) -c $< -o $@



.PHONY : clean
clean :
	rm $(OUT)*.o *.exe
