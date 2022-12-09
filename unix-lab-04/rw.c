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

#define COUNT_WRITERS 2
#define COUNT_READERS 5
#define SLEEP 2

#define P -1
#define V 1

#define WAITING_READERS 0
#define ACTIVE_READERS 1
#define ACTIVE_WRITER 2
#define WAITING_WRITERS 3
#define BIN_WRITER 4

const int PERMS =  S_IRWXU | S_IRWXG | S_IRWXO;

int *shm_buf;
int sem_id;

struct sembuf start_read[] = {{WAITING_READERS, V, 0}, {BIN_WRITER, 0, 0}, /*{ACTIVE_WRITER, 0, 0},*/
{WAITING_WRITERS, 0, 0}, {ACTIVE_READERS , V, 0}, {WAITING_READERS, P, 0}};
struct sembuf stop_read[] = {{ACTIVE_READERS, P, 0}};
struct sembuf start_write[] = {{WAITING_WRITERS, V, 0}, {ACTIVE_READERS, 0, 0},
/*{ACTIVE_WRITER, 0, 0},*/ {BIN_WRITER, P, 0}, /*{ACTIVE_WRITER, V, 0},*/ {WAITING_WRITERS, P, 0}};
struct sembuf stop_write[] = {/*{ACTIVE_WRITER, P, 0}, */{BIN_WRITER, V, 0}};

void reader(int semid, int* shm)
{
    while (1)
    {
        semop(semid, start_read, 4);
		printf("Reader %d - %d\n", getpid(), *shm);
        semop(semid, stop_read, 1);
        sleep(rand() % SLEEP);
    }
}

void writer(int semid, int* shm)
{
    while (1)
    {
        semop(semid, start_write, 4);
        (*shm)++;
        printf("Writer %d - %d\n", getpid(), *shm);
        sleep(rand() % SLEEP);
        semop(semid, stop_write, 1);
    }
}

void create_writers()
{
    pid_t pid = -1;

	for (int i = 0; i < COUNT_WRITERS && pid != 0; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork error\n");
            exit(1);
        }

        if (pid == 0)
        {
            writer(sem_id, shm_buf);
            return;
        }
	}
}

void create_readers()
{
    pid_t pid = -1;

    for (int i = 0; i < COUNT_READERS && pid != 0; i++)
    {
        pid = fork();
        if (pid == -1)
        {
            perror("fork error\n");
            exit(1);
        }

        if (pid == 0)
        {
            reader(sem_id, shm_buf);
            return;
        }
	}
}

int main()
{
    int shm_id;
    key_t key = ftok("key_file",0);
    if (key == -1)
    {
        printf("ftok error\n");
        return 1;
    }
    if ((shm_id = shmget(key, sizeof(int), IPC_CREAT | PERMS)) == -1)
	{
		perror("shmget error\n");
		exit(1);
	}

    srand(time(NULL));
    shm_buf = shmat(shm_id, NULL, 0);
    if (shm_buf == (void*) -1)
    {
        perror("shmat error\n");
        exit(1);
    }

    (*shm_buf) = 0;

    if ((sem_id = semget(key, 5, IPC_CREAT | PERMS)) == -1)
	{
		perror("semget error\n");
		exit(1);
	}

	if ((semctl(sem_id, BIN_WRITER, SETVAL, 1)) == -1)
	{
		perror("semctl error\n");
		exit(1);
	}

    create_writers();
    create_readers();

    for (int i = 0; i < COUNT_WRITERS + COUNT_READERS; ++i)
        wait(NULL);

    if (shmdt(shm_buf) == -1)
    {
        perror("shm_buf error\n");
        exit(1);
    }

    return 0;
}