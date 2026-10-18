#include <tasker.h>
#include <lib.h>
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

static sem_t tasks;

static uint16_t next_task = 0;
static uint16_t last_task = 0;
// Includes the running task, such that it can always be re-queued
static uint16_t pend_task = 0;

void init_tasker()
{
    sem_init(&tasks, 1, 0);
}

void tasker_daemon()
{
    while (1)
    {
        // add_task may run with interrupts off, never hold tasks with interrupts on
        uint64_t flags = interrupts_disable();

        if (sem_wait(&tasks) || !pend_task)
        {
            interrupts_restore(flags);
            continue;
        }

        Task task = shedule[next_task];
        next_task = NEXT(next_task);

        interrupts_restore(flags);

        int reschedule = task.task(task.data);

        flags = interrupts_disable();

        if (reschedule)
        {
            shedule[last_task] = task;
            last_task = NEXT(last_task);
            sem_post(&tasks);
        }
        else
        {
            pend_task--;
        }

        interrupts_restore(flags);
    }
}

int add_task(Callback task, void *data)
{
    if (!task)
        return -1;

    uint64_t flags = interrupts_disable();

    if (pend_task >= MAX_TASKS)
    {
        interrupts_restore(flags);
        return -1;
    }

    shedule[last_task].task = task;
    shedule[last_task].data = data;

    last_task = NEXT(last_task);
    pend_task++;
    sem_post(&tasks);

    interrupts_restore(flags);

    return 0;
}
