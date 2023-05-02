#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <alchemy/task.h> //Libreria xenomai para generar tareas RT
#include <alchemy/timer.h>
#include <alchemy/sem.h>

//DEFINIMOS EL NUMERO DE ITERACIONES POR TAREA
#define ITER 15

//DEFINIMOS LAS TAREAS
static RT_TASK tarea1, tarea2;

int global = 0; //RECURSO COMPARTIDO
int result =0;
//DEFINIMOS SEMAFORO
RT_SEM sem;
int sem_id;

void tareaUno(void *arg){
    int i;
    for(i=0; i <ITER; i++){
        // BLOQUEO DE SEMAFORO
        rt_sem_v(&sem);
        rt_sem_p(&sem, TM_INFINITE);
        global ++;   //SECCIÓN CRITICA
        printf("Resultado de suma: %d \n",global);
        //LIBERAMOS SEMAFORO
        
    }rt_sem_p(&sem, TM_INFINITE);
}


void tareaDos(void *arg){
    int i;
    for(i=0; i  <ITER; i++){
        // ESPERAMOS EL SEMAFORO 
        rt_sem_v(&sem);
        //IMPRESION VARIABLE INCREMENTADA
        global --;
        printf("Resultado de resta: %d \n",global);
        //LIBERAMOS SEMAFORO
        rt_sem_p(&sem, TM_INFINITE);
    }
}

int main(int argc, char* argv[]){ //ARGUMENTOS DE LA FUNCIÓN PRINCIPAL

    //CREACIÓN DE SEMAFORO
    rt_sem_create(&sem, "SemaforoP3",1,S_FIFO);

    // MENSAJE EN CASO DE ERROR CREANDO SEMAFORO
    if(sem_id < 0) {
        printf("Error creando semáforo\n");
        exit(EXIT_FAILURE);}
    else{printf("Semaforo creado con exito\n");}
    
    char str1[10]; char str2[10]; //CREAMOS EL STRING (ARREGLO DE CARACTERES)
    sprintf(str1,"task_1"); sprintf(str2,"task_2");//COLOCAMOS NOMBRE A UN STRING
    rt_task_create(&tarea1, str1,0,10,0); //CREAMOS LA TAREA, DIRECCIÓN, NOMBRE DEL HILO, PRIORIDADES EN XENOMAI
    rt_task_create(&tarea2, str2,0,10,0); //CREAMOS LA TAREA, DIRECCIÓN, NOMBRE DEL HILO, PRIORIDADES EN XENOMAI
    rt_task_start(&tarea1, &tareaUno,0); //INICIAMOS LA TAREA, DIRECCIÓN TAREA, FUNCIÓN A EJECUTAR, 0
    rt_task_start(&tarea2, &tareaDos,0); //INICIAMOS LA TAREA, DIRECCIÓN TAREA, FUNCIÓN A EJECUTAR, 0

    //Esperamos que las tareas finalicen 
    rt_task_join(&tarea1);
    rt_task_join(&tarea2);

    rt_sem_delete(&sem); //ELIMINAMOS EL SEMAFORO CREADO
    printf("\n El valor final es: %d \n", global);
    return 0;
}