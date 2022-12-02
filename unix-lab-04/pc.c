#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SIZE_BUF 5
#define COUNT 3	
#define PERM S_IRWXU | S_IRWXG | S_IRWXO
#define SLEEP 2

int* shared_buffer;
int* sh_pos_prod;
int* sh_pos_cons;

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
		perror("Error operation on semaphors\n");
		exit(1);
	}
	
	shared_buffer[*sh_pos_prod] = *sh_pos_prod;
	printf("Producer %d >>>>> %d\n", getpid(), shared_buffer[*sh_pos_prod]);
	(*sh_pos_prod)++;
	sleep(rand() % 3);

	int sem_op_v = semop(semid, stop_produce, 2);
	if (sem_op_v == -1)
	{
		perror("Error operation on semaphors\n");
		exit(1);
	}
}
}

void consumer(const int semid)
{	

	while(1)
	{
	int sem_op_p = semop(semid, start_consume, 2);
	if (sem_op_p == -1)
	{
		perror("Error operation on semaphor\n");
		exit(1);
	}
		
	printf("Consumer %d <<<<< %d\n", getpid(), shared_buffer[*sh_pos_cons]);
	(*sh_pos_cons)++;
	sleep(rand() % 3);

	int sem_op_v = semop(semid, stop_consume, 2);
	if (sem_op_v == -1)
	{
		perror("Error operation on semaphor\n");
		exit(1);
	}
}
}

int main()
{
	int shmid, semid; 

	if ((shmid = shmget(IPC_PRIVATE, (SIZE_BUF+2) * sizeof(int), IPC_CREAT | PERM)) == -1) 
	{
		perror("Error create a shared area\n");
		exit( 1 );
	}

	sh_pos_prod = shmat(shmid, NULL, 0); 
	if (sh_pos_prod == (void*) -1)
	{
		perror("Error attach memory\n");
		exit(1);
	}

	shared_buffer = sh_pos_prod + 2 * sizeof(int); 
	sh_pos_cons = sh_pos_prod + sizeof(int);

	(*sh_pos_prod) = 0;
	(*sh_pos_cons) = 0;

	if ((semid = semget(IPC_PRIVATE, 3, IPC_CREAT | PERM)) == -1)
	{
		perror("Error create a semaphore\n");
		exit( 1 );
	}

	int c_sb = semctl(semid, SB, SETVAL, 1);
	int c_se = semctl(semid, SE, SETVAL, SIZE_BUF); 
	int c_sf = semctl(semid, SF, SETVAL, 0); 

	if (c_se == -1 || c_sf == -1 || c_sb == -1)
	{
		perror("Error set semaphors\n");
		exit(1);
	}	

	int pid = -1;

	for (int i = 0; i < 2*COUNT && pid != 0; i++) 
    {
        pid = fork();
        if (pid == -1) 
        {
            perror("Fork error\n"); 
            exit(1);
        }

        if (pid == 0) 
        {
            producer(semid);
            return 0;
        }
	}

	for (int i = 0; i < COUNT && pid != 0; i++) 
    {
        pid = fork();
        if (pid == -1) 
        {
            perror("Fork error\n"); 
            exit(1);
        }

        if (pid == 0) 
        {
            consumer(semid);
            return 0;
        }
	}

/*
	for (int i = 0; i < 2*COUNT && pid != 0; i++)
	{
		if ((pid = fork()) == -1)
		{
			perror("Fork error\n");
			exit(1);
		}

		if (pid == 0)
		{
			if (i < COUNT)
			{
				consumer(semid);
				return 0;
			}
			else
			{
				producer(semid);
				return 0;
			}
		}
	}
*/		
	for (int i = 0; i < 2*COUNT; i++)
		wait(NULL);

	if (shmdt(sh_pos_prod) == -1) 
	{
		perror("Error detach shared memory");
		exit(1);
	}
}
