#include <stdio.h>
#include <stdlib.h>
#include <alchemy/task.h>
#include <alchemy/sem.h>

#define NUM_THREADS 2

int global_var = 0;
RT_SEM sem;

void taskOne(void *arg) {
    int i;
    for(i=0; i<1000000; i++) {
        rt_sem_p(&sem, TM_INFINITE);
        global_var++;
        printf("val: %d",global_var);
        rt_sem_v(&sem);
    }
}

void taskTwo(void *arg) {
    int i;
    for(i=0; i<1000000; i++) {
        rt_sem_p(&sem, TM_INFINITE);
        global_var--;
        printf("va2: %d",global_var);
        rt_sem_v(&sem);
    }
}

int main(int argc, char* argv[]) {
    RT_TASK tasks[NUM_THREADS];
    int i;

    rt_sem_create(&sem, "my_semaphore", 1, S_FIFO);

    for(i=0; i < NUM_THREADS; i++) {
        if(i==0)
            rt_task_create(&tasks[i], "taskOne", 0, 50, 0);
        else
            rt_task_create(&tasks[i], "taskTwo", 0, 50, 0);
    }

    for(i=0; i < NUM_THREADS; i++) {
        if(i==0)
            rt_task_start(&tasks[i], &taskOne, NULL);
        else
            rt_task_start(&tasks[i], &taskTwo, NULL);
    }

    for(i=0; i < NUM_THREADS; i++) {
        rt_task_join(&tasks[i]);
    }

    printf("global_var: %d\n", global_var);

    rt_sem_delete(&sem);

    return 0;
}
