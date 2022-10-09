CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic
OBJ := io.o main.o 

app.exe : $(OBJ)
	$(CC) -o app.exe $(OBJ)


%.o : %.c *.h
	$(CC) $(CFLAGS) -c $< -o $@



.PHONY : clean
clean :
	rm *.o *.exe
