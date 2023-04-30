#include <stdio.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>

#define NLOOP 100

int shared_var = 0;
RT_SEM sem1, sem2;

void increment(void *arg)
{
    int i;
    for (i = 0; i < NLOOP; i++) {
        rt_sem_p(&sem1, TM_INFINITE);
        shared_var++;
        printf("Hilo 1: %d\n", shared_var);
        rt_sem_v(&sem2);
    }
}

void decrement(void *arg)
{
    int i;
    for (i = 0; i < NLOOP; i++) {
        rt_sem_p(&sem2, TM_INFINITE);
        shared_var--;
        printf("Hilo 2: %d\n", shared_var);
        rt_sem_v(&sem1);
    }
}

int main(int argc, char *argv[])
{
    rt_task_create(NULL, "increment", 0, 50, 0);
    rt_task_create(NULL, "decrement", 0, 50, 0);

    rt_sem_create(&sem1, "sem1", 0, S_PRIO);
    rt_sem_create(&sem2, "sem2", 1, S_PRIO);

    rt_task_start(&increment, NULL);
    rt_task_start(&decrement, NULL);

    rt_task_join(&increment);
    rt_task_join(&decrement);

    rt_sem_delete(&sem1);
    rt_sem_delete(&sem2);

    return 0;
}
