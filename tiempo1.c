#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <math>
#include <alchemy/task.h> #


RT_TASK tarea_hola;

void mensaje (void *arg){
    RT_TASK_INFO infoTarea;
    printf("Hola mundo\n");

    rt_task_inquire(NULL, &infoTarea);
    printf("Tarea: %s \n", infoTarea.name);
}

int main(int argc, char* argv[]){
    char str[10];
    sprintf(str,"hola");
    rt_task_create(&tarea_hola, str,0,50,0);
    rt_task_start(&tarea_hola, &mensaje,0);
}