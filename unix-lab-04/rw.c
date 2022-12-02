#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define COUNT_WRITERS 3
#define COUNT_READERS 5
#define SLEEP 5

#define P -1
#define V 1

#define ACTIVE_READERS 0
#define ACTIVE_WRITER 1
#define WAIT_WRITERS 2

const int PERM =  S_IRWXU | S_IRWXG | S_IRWXO;

struct sembuf start_read[] = { {ACTIVE_WRITER, 0, 0}, {WAIT_WRITERS, 0, 0}, {ACTIVE_READERS, V, 0} };
struct sembuf stop_read[] = { {ACTIVE_READERS, P, 0} };
struct sembuf start_write[] = { {WAIT_WRITERS, V, 0}, {ACTIVE_READERS, 0, 0}, {ACTIVE_WRITER, 0, 0}, {ACTIVE_WRITER, V, 0}, {WAIT_WRITERS, P, 0} };
struct sembuf stop_write[] = { {ACTIVE_WRITER, P, 0} };

void reader(int semid, int* shm) 
{
    while (1)
    {
        semop(semid, start_read, 3);
		printf("\tReader %d <<<<< %d\n", getpid(), *shm);        
        semop(semid, stop_read, 1);
        sleep(rand() % SLEEP);
    }
}

void writer(int semid, int* shm) 
{
    while (1)
    {
        semop(semid, start_write, 5);
        (*shm)++;
        printf("Writer %d >>>>> %d\n", getpid(), *shm);    
        sleep(rand() % SLEEP);    
        semop(semid, stop_write, 1);    
        
    }
}

int main() 
{
    srand(time(NULL));

    int shm_id;
    if ((shm_id = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | PERM)) == -1) 
	{
		perror("Error create shared memory\n");
		exit( 1 );
	}

    int *shm_buf = shmat(shm_id, NULL, 0); 
    if (shm_buf == (void*) -1) 
    {
        perror("Error attach shared memory\n");
        exit( 1 );
    }

    (*shm_buf) = 0;

    int sem_id;
    if ((sem_id = semget(IPC_PRIVATE, 3, IPC_CREAT | PERM)) == -1) 
	{
		perror("Error create semaphore\n");
		exit( 1 );
	}

	pid_t pid = -1;

	for (int i = 0; i < COUNT_WRITERS && pid != 0; i++) 
    {
        pid = fork();
        if (pid == -1) 
        {
            perror("Fork error\n"); 
            exit(1);
        }

        if (pid == 0) 
        {
            writer(sem_id, shm_buf);
            return 0;
        }
	}

    for (int i = 0; i < COUNT_READERS && pid != 0; i++) 
    {
        pid = fork();
        if (pid == -1) 
        {
            perror("Fork error\n"); 
            exit(1);
        }

        if (pid == 0) 
        {
            reader(sem_id, shm_buf);
            return 0;
        }
	}

    for (int i = 0; i < COUNT_WRITERS + COUNT_READERS; ++i)         
        wait(NULL);

    if (shmdt(shm_buf) == -1) 
    {
        perror("Error detach shared memory\n");
        exit(1);
    }

    return 0;
}