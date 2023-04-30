#include <stdio.h>
#include <alchemy/sem.h>
#include <alchemy/task.h>

#define N 10000

int shared_variable = 0;
RT_SEM semaphore;

void increment_thread(void *arg) {
    int i;
    for (i = 0; i < N; i++) {
        rt_sem_p(&semaphore, TM_INFINITE);
        shared_variable++;
        printf("val1: %d \n",shared_variable);
        rt_sem_v(&semaphore);
    }
}

void decrement_thread(void *arg) {
    int i;
    for (i = 0; i < N; i++) {
        rt_sem_p(&semaphore, TM_INFINITE);
        shared_variable--;
        printf("val2: %d \n",shared_variable);
        rt_sem_v(&semaphore);
    }
}

int main(int argc, char *argv[]) {
    RT_TASK increment_task, decrement_task;

    rt_sem_create(&semaphore, "my_semaphore", 1, S_FIFO);

    rt_task_create(&increment_task, "increment_task", 0, 50, T_JOINABLE);
    rt_task_create(&decrement_task, "decrement_task", 0, 50, T_JOINABLE);

    rt_task_start(&increment_task, &increment_thread, NULL);
    rt_task_start(&decrement_task, &decrement_thread, NULL);

    rt_task_join(&increment_task);
    rt_task_join(&decrement_task);

    printf("The final value of shared_variable is %d\n", shared_variable);

    rt_sem_delete(&semaphore);

    return 0;
}
