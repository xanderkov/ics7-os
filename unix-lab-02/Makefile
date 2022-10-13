CC := gcc
CFLAGS := -std=c99 -Wall -Werror -pedantic
OBJ := io.o main.o 

all: task1, task2, task4, task3 task5
	$(CC) task-1.c -o app1.out

task1: task-1.c
	$(CC) task-1.c -o app1.out

task2: task-2.c
	$(CC) task-2.c -o app2.out
	
task3: task-3.c
	$(CC) task-3.c -o app3.out

task4: task-1.c
	$(CC) task-4.c -o app4.out

task5: task-2.c
	$(CC) task-5.c -o app5.out