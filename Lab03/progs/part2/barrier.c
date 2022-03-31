#include <stdio.h>
#include <semaphore.h>
#include <sys/shm.h>
#include <sys/wait.h>

int nproc = 0, shmid, shmid_count;
int *count;
sem_t *barrier, *mutex;
int* counter;
// Initializes the barrier
void init_barrier(int num_proc)
{
    nproc = num_proc;
    shmid = shmget(IPC_PRIVATE, 2*sizeof(sem_t), IPC_CREAT | 0600);
    barrier = (sem_t *)shmat(shmid, NULL, 0);
    mutex = barrier + 1;
    shmid_count = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0600);
    count = (int *)shmat(shmid_count, NULL, 0);
    *count = 0;
    counter = count;

    sem_init(barrier, 1, 0);
    sem_init(mutex, 1, 1);
}

void reach_barrier()
{
    sem_wait(mutex);
    *counter = *counter + 1;
    sem_post(mutex);
    if (*counter == nproc)
    {
        // Release one process
        sem_post(barrier);
    }
    else
    {
        // We are not the last process. So we wait at the
        // semaphore until we are freed.
        sem_wait(barrier);
        // Now that we are freed, we free the next process
        sem_post(barrier);
    }
}

void destroy_barrier(int my_pid)
{
    if (my_pid != 0)
    {
        // Destroy the semaphores and detach
        // and free any shared memory. Notice
        // that we explicity check that it is
        // the parent doing it.
        sem_destroy(barrier);
        sem_destroy(mutex);
        shmctl(shmid, IPC_RMID, 0);
        shmctl(shmid_count, IPC_RMID, 0);
    }
}
