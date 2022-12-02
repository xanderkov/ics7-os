#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_BUF 128
#define PRODUCERS_AMOUNT 3
#define CONSUMERS_AMOUNT 3
#define PERMS S_IRWXU | S_IRWXG | S_IRWXO

int shmid, semid;
char* addr3;

char* addr;
char* addr1;
char* addr2;

char** ptr_prod;
char** ptr_cons;
char*  alfa;


#define SB 0
#define SE 1
#define SF 2

#define P -1
#define V 1

struct sembuf start_produce[2] = { {SE, P, 0}, {SB, P, 0} };
struct sembuf stop_produce[2] =  { {SB, V, 0}, {SF, V, 0} };
struct sembuf start_consume[2] = { {SF, P, 0}, {SB, P, 0} };
struct sembuf stop_consume[2] =  { {SB, V, 0}, {SE, V, 0} };

void producer(const int semid)
{
	while (1)
	{
        
        int sem_op_p = semop(semid, start_produce, 2);
        if (sem_op_p == -1)
        {
            perror("semop error\n");
            exit(1);
        }

        **ptr_prod = *alfa;
        printf("Producer [%d] >>>> [%c]\n", getpid(), **ptr_prod);

        (*ptr_prod)++;
        (*alfa)++;


        sleep(rand() % 3);

        int sem_op_v = semop(semid, stop_produce, 2);
        if (sem_op_v == -1)
        {
            perror("semop error\n");
            exit(1);
        }
    }
}

void consumer(const int semid)
{
	while (1)
	{
        int sem_op_p = semop(semid, start_consume, 2);
        if (sem_op_p == -1)
        {
            perror("semop error\n");
            exit(1);
        }

        printf("Consumer [%d] <<<< [%c]\n", getpid(), **ptr_cons);

        (**ptr_cons)++;

        sleep(rand() % 3);

        int sem_op_v = semop(semid, stop_consume, 2);
        if (sem_op_v == -1)
        {
            perror("semop error\n");
            exit(1);
        }
    }
}

void create_producers() {
    int pid = -1;

	for (int i = 0; i < PRODUCERS_AMOUNT; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("can't fork\n");
            exit(1);
        }

        if (pid == 0)
        {
            producer(semid);
            return;
        }
	}
}

void create_consumers() {
    int pid = -1;

    for (int i = 0; i < CONSUMERS_AMOUNT; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("can't fork\n");
            exit(1);
        }

        if (pid == 0)
        {
            consumer(semid);
            return;
        }
	}
}

int main()
{
    key_t sem_key = ftok("key_file", 0);
    key_t mem_key;
	if ((shmid = shmget(mem_key, SIZE_BUF * sizeof(char), IPC_CREAT | PERMS)) == -1)
	{
		perror("shmget error\n");
		exit(1);
	}

	addr = shmat(shmid, NULL, 0);
	if (addr == (void*) -1)
	{
		perror("shmat error\n");
		exit(1);
	}

    ptr_prod = addr;
    ptr_cons = addr + sizeof(char*);
    alfa = ptr_cons + sizeof(char*);

	addr3 = alfa + sizeof(char);
    
    *ptr_prod = addr3;
    *ptr_cons = addr3;

	*addr = addr3;
	*ptr_cons = addr3;
    *alfa = 'a';

	if ((semid = semget(sem_key, 3, IPC_CREAT | PERMS)) == -1)
	{
		perror("semget error\n");
		exit(1);
	}

	int c_sb = semctl(semid, SB, SETVAL, 1);
	int c_se = semctl(semid, SE, SETVAL, SIZE_BUF);
	int c_sf = semctl(semid, SF, SETVAL, 0);

	if (c_se == -1 || c_sf == -1 || c_sb == -1)
	{
		perror("semctl error\n");
		exit(1);
	}

    create_producers();
    create_consumers();

	for (int i = 0; i < (CONSUMERS_AMOUNT + PRODUCERS_AMOUNT); i++)
		wait(NULL);

	if (shmdt(addr) == -1)
	{
		perror("shmdt error\n");
		exit(1);
	}
}