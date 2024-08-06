#include <tasker.h>
#include <sched.h>
#include <semaphores.h>

#define MAX_TASKS 256
#define NEXT(x) ((x + 1) % MAX_TASKS)

typedef struct
{
    Callback task;
    void *data;
} Task;

static Task shedule[MAX_TASKS];

static sem_t mutex;
static sem_t tasks;

static uint16_t next_task = 0;
static uint16_t last_task = 0;
static uint16_t pend_task = 0;

void tasker_daemon()
{
    sem_init(&tasks, 1, 0);
    sem_init(&mutex, 1, 1);

    while (1)
    {
        sem_wait(&tasks);

        if (pend_task)
        {
            int reschedule = shedule[next_task].task(shedule[next_task].data);
            next_task = NEXT(next_task);

            if (!reschedule)
            {
                pend_task--;
                continue;
            }

            sem_wait(&mutex);

            if (pend_task < MAX_TASKS)
            {
                shedule[last_task].task = shedule[next_task].task;
                shedule[last_task].data = shedule[next_task].data;
            }

            last_task = NEXT(last_task);
            sem_post(&tasks);

            sem_post(&mutex);
        }
    }
}

int add_task(Callback task, void *data)
{
    if (!task)
        return -1;

    sem_wait(&mutex);

    if (pend_task >= MAX_TASKS)
        return -1;

    shedule[last_task].task = task;
    shedule[last_task].data = data;

    last_task = NEXT(last_task);
    sem_post(&tasks);
    pend_task++;

    sem_post(&mutex);

    return 0;
}
