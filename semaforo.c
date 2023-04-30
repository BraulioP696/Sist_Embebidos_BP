#include <stdio.h>
#include <alchemy/sem.h>
#include <alchemy/task.h>

RT_SEM sem; // semáforo
int var_compartida = 0; // variable compartida

void hilo_1(void *arg) {
  int i;
  for (i = 0; i < 10000; i++) {
    rt_sem_p(&sem, TM_INFINITE); // bloquea el semáforo
    var_compartida++; // incrementa la variable compartida
    printf("val: %d \n" ,var_compartida);
    rt_sem_v(&sem); // libera el semáforo
  }
}

void hilo_2(void *arg) {
  int i;
  for (i = 0; i < 10000; i++) {
    rt_sem_p(&sem, TM_INFINITE); // bloquea el semáforo
    var_compartida--; // decrementa la variable compartida
    printf("val2: %d \n" ,var_compartida);
    rt_sem_v(&sem); // libera el semáforo
  }
}

int main(int argc, char* argv[]) {
  rt_sem_create(&sem, "mi_sem", 1, S_FIFO); // crea el semáforo con valor inicial 1

  RT_TASK tarea_1, tarea_2;
  rt_task_create(&tarea_1, "hilo_1", 0, 50, T_JOINABLE);
  rt_task_create(&tarea_2, "hilo_2", 0, 50, T_JOINABLE);

  rt_task_start(&tarea_1, &hilo_1, NULL);
  rt_task_start(&tarea_2, &hilo_2, NULL);

  rt_task_join(&tarea_1);
  rt_task_join(&tarea_2);

  printf("La variable compartida tiene un valor de %d\n", var_compartida);

  rt_sem_delete(&sem); // elimina el semáforo

  return 0;
}
