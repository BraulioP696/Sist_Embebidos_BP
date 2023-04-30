#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h> //Libreria xenomai para generar tareas RT
#include <alchemy/timer.h>
#include <alchemy/sem.h>

//DEFINIMOS EL NUMERO DE ITERACIONES POR TAREA
#define ITER 10

//DEFINIMOS LAS TAREAS
static RT_TASK tarea1;
static RT_TASK tarea2;

int global = 0; //RECURSO COMPARTIDO

void tareaUno(void *arg){
    int i;
    for(i=0;i<ITER; i++);{
        printf("Tarea 1 la variable global es: %d ",++global);
    }
}

void tareaDos(void *arg){
    int i;
    for(i=0;i<ITER; i++);{
        printf("Tarea 2 la variable global es: %d ",--global);
    }
}

int main(int argc, char* argv[]){ //ARGUMENTOS DE LA FUNCIÓN PRINCIPAL
    char str1[10]; char str2[10]; //CREAMOS EL STRING (ARREGLO DE CARACTERES)
    sprintf(str1,"task_1"); sprintf(str2,"task_2");//COLOCAMOS NOMBRE A UN STRING
    rt_task_create(&tarea1, str1,0,1,0); //CREAMOS LA TAREA, DIRECCIÓN, NOMBRE DEL HILO, PRIORIDADES EN XENOMAI
    rt_task_create(&tarea2, str2,0,1,0); //CREAMOS LA TAREA, DIRECCIÓN, NOMBRE DEL HILO, PRIORIDADES EN XENOMAI
    rt_task_start(&tarea1, &tareaUno,0); //INICIAMOS LA TAREA, DIRECCIÓN TAREA, FUNCIÓN A EJECUTAR, 0
    rt_task_start(&tarea2, &tareaDos,0); //INICIAMOS LA TAREA, DIRECCIÓN TAREA, FUNCIÓN A EJECUTAR, 0
    return 0;
}