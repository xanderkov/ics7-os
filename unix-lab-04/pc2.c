#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

const int N = 5;
const int REPETITIONS = 10;
const int PERM = S_IRWXU;

const int consumersCount = 2;
const int producersCount = 2;

int *sharedMemoryBuffer;
int *sharedMemoryConsBuffer;
int *sharedMemoryProdBuffer;

#define BUFFER_FULL 0
#define BUFFER_EMPTY 1
#define RESOURCE_CONTROL 2

struct sembuf producerStart[2] = { 
									{BUFFER_EMPTY, -1, 0}, // ждем появление свободной записи
									{RESOURCE_CONTROL, -1, 0}
								 };
struct sembuf producerStop[2] =  { 
									{BUFFER_FULL, 1, 0}, // сигнализируем о появлении заполненной записи
									{RESOURCE_CONTROL, 1, 0} 
								 };

struct sembuf consumerStart[2] = { 
									{BUFFER_FULL, -1, 0}, // ждем появление заполненной записи
									{RESOURCE_CONTROL, -1, 0} 
								 };
struct sembuf consumerStop[2] =  { 
									{BUFFER_EMPTY, 1, 0}, // сигнализируем о появлении свободной записи
									{RESOURCE_CONTROL, 1, 0} 
								 };

void producer(const int semID, const int i, const int value)
{
	if (semop(semID, producerStart, 2) == -1)
	{
		perror("Can't make operation on semaphors\n");
		exit(1);
	}
		
	sharedMemoryBuffer[*sharedMemoryProdBuffer] = *sharedMemoryProdBuffer;
	printf("Producer %d operation %d >>>>> %d\n", getpid(), value, sharedMemoryBuffer[*sharedMemoryProdBuffer]);
	++(*sharedMemoryProdBuffer);
	
	if (semop(semID, producerStop, 2) == -1)
	{
		perror("Can't make operation on semaphors\n");
		exit(1);
	}
    sleep(rand() % 5);
}

void consumer(const int semID, const int i, const int value)
{
	if (semop(semID, consumerStart, 2) == -1)
	{
		perror("Can't make operation on semaphors\n");
		exit(1);
	}
	
	printf("Consumer #%d operation %d <---- %d\n", i, value, sharedMemoryBuffer[*sharedMemoryConsBuffer]);
	++(*sharedMemoryConsBuffer);
	if (semop(semID, consumerStop, 2) == -1)
	{
		perror("Can't make operation on semaphors\n");
		exit(1);
	}
	sleep(rand() % 2);
}

int main()
{
  	printf("Parent pid: %d\n", getpid());

    int sharedMemoryID, semID;
	int ctrlBufferFull, ctrlBufferEmpty, ctrlResourceControl;
	pid_t pid = -1;
	int value;

	if ((sharedMemoryID = shmget(IPC_PRIVATE, (N + 1) * sizeof(int), IPC_CREAT | PERM)) == -1) 
	{
		perror("Can't create shared memory\n");
		exit(1);
	}

	sharedMemoryProdBuffer = shmat(sharedMemoryID, 0, 0); 
	if (*sharedMemoryProdBuffer == -1)
	{
		perror("Can't attach shared memory\n");
		exit(1);
	}
	sharedMemoryConsBuffer = sharedMemoryProdBuffer + sizeof(int);
    sharedMemoryBuffer = sharedMemoryProdBuffer + 2 * sizeof(int); 

	(*sharedMemoryProdBuffer) = 0;
	(*sharedMemoryConsBuffer) = 0;

	if ((semID = semget(IPC_PRIVATE, 3, IPC_CREAT | PERM)) == -1) 
	{
		perror("Can't create a semaphore\n");
		exit(1);
	}

	ctrlBufferFull = semctl(semID, BUFFER_FULL, SETVAL, 0);
	ctrlBufferEmpty = semctl(semID, BUFFER_EMPTY, SETVAL, N); 
	ctrlResourceControl = semctl(semID, RESOURCE_CONTROL, SETVAL, 1); 

	if (ctrlBufferEmpty == -1 || ctrlResourceControl == -1 || ctrlBufferFull == -1)
	{
		perror("Can't set control semaphors\n");
		exit(1);
	}

	for (size_t i = 0; i < producersCount && pid != 0; ++i)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Can't fork writer\n"); 
            exit(1);
        }
        if (pid == 0)
        {
			value = 0;
			while (value < REPETITIONS)
			{
				producer(semID, i, value);
				value++;
			}
        }
	}

    for (size_t i = 0; i < consumersCount && pid != 0; ++i)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("Can't fork reader\n"); 
            exit(1);
        }
        if (pid == 0)
        {
            value = 0;
			while (value < REPETITIONS) 
			{
				consumer(semID, i, value);
				value++;
			}	
        }
	}

    if (shmdt(sharedMemoryProdBuffer) == -1)
    {
        perror("Can't detach shared memory\n");
        exit(1);
    }

	if (pid != 0) 
	{
		for (int i = 0; i < producersCount + consumersCount; ++i)
        {
	        wait(NULL);
        }
		if (shmctl(sharedMemoryID, IPC_RMID, NULL) == -1)
        {
            perror("Can't free shared memory");
            exit(1);
        }
	}
}
