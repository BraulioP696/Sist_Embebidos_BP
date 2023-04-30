#include <stdio.h>
#include <alchemy/sem.h>
#include <alchemy/task.h>

#define MAX_COUNT 1000000

int global_var = 0;
RT_SEM sem;

void task1(void *arg)
{
    int i;
    for(i = 0; i < MAX_COUNT; i++)
    {
        rt_sem_p(&sem, TM_INFINITE);
        global_var++;
        printf("val1: %d \n",global_var);
        rt_sem_v(&sem);
    }
}

void task2(void *arg)
{
    int i;
    for(i = 0; i < MAX_COUNT; i++)
    {
        rt_sem_p(&sem, TM_INFINITE);
        global_var--;
        printf("val2: %d \n",global_var);
        rt_sem_v(&sem);
    }
}

int main(int argc, char **argv)
{
    rt_sem_create(&sem, "Semaphore", 1, S_FIFO);
    
    RT_TASK taskA, taskB;
    rt_task_create(&taskA, "Task1", 0, 50, 0);
    rt_task_create(&taskB, "Task2", 0, 50, 0);
    rt_task_start(&taskA, &task1, 0);
    rt_task_start(&taskB, &task2, 0);
    rt_task_join(&taskA);
    rt_task_join(&taskB);

    rt_sem_delete(&sem);
    return 0;
}
