#include <stdio.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>

#define NLOOP 10000

int shared_var = 0;
RT_SEM sem;

void inc_task(void *arg)
{
    int i;
    for (i = 0; i < NLOOP; i++) {
        rt_sem_p(&sem, TM_INFINITE);
        shared_var++;
        printf("valor actual: %d \n", shared_var);
        rt_sem_v(&sem);
    }
}

void dec_task(void *arg)
{
    int i;
    for (i = 0; i < NLOOP; i++) {
        rt_sem_p(&sem, TM_INFINITE);
        shared_var--;
        printf("valor actual: %d \n", shared_var);
        rt_sem_v(&sem);
    }
}

int main(int argc, char **argv)
{
    rt_sem_create(&sem, "my_sem", 1, S_FIFO);
    RT_TASK t1, t2;

    rt_task_create(&t1, "inc_task", 0, 50, 0);
    rt_task_create(&t2, "dec_task", 0, 50, 0);

    rt_task_start(&t1, &inc_task, 0);
    rt_task_start(&t2, &dec_task, 0);

    rt_task_join(&t1);
    rt_task_join(&t2);

    rt_sem_delete(&sem);

    printf("Valor final de la variable compartida: %d\n", shared_var);

    return 0;
}
