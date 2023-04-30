#include <alchemy/sem.h>
#include <alchemy/task.h>
#include <stdio.h>

#define MAX_COUNT 10000

RT_SEM sem;

int shared_variable = 0;

void increment_thread(void *arg) {
    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        rt_sem_p(&sem, TM_INFINITE);
        shared_variable++;
        print("val: %d",shared_variable);
        rt_sem_v(&sem);
    }
}

void decrement_thread(void *arg) {
    int i;
    for (i = 0; i < MAX_COUNT; i++) {
        rt_sem_p(&sem, TM_INFINITE);
        shared_variable--;
        print("val2: %d",shared_variable);
        rt_sem_v(&sem);
    }
}

int main(int argc, char *argv[]) {
    rt_sem_create(&sem, "mysem", 1, S_FIFO);

    RT_TASK task1, task2;

    rt_task_create(&task1, "increment", 0, 50, 0);
    rt_task_create(&task2, "decrement", 0, 50, 0);

    rt_task_start(&task1, &increment_thread, 0);
    rt_task_start(&task2, &decrement_thread, 0);

    rt_task_join(&task1);
    rt_task_join(&task2);

    rt_sem_delete(&sem);

    printf("The final value of the shared variable is: %d\n", shared_variable);

    return 0;
}
