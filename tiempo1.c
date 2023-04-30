#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <alchemy/task.h> //Libreria xenomai para generar tareas RT


RT_TASK tarea_hola; //CREAMOS LA ESTRUCTURA DE TAREA

void mensaje (void *arg){  // FUNCIÓN DE LA TAREA "MENSAJE"
    RT_TASK_INFO infoTarea; // CREAMOS LA VARIABLE DE LA INFORMACIÓN DE LA TAREA
    printf("Hola mundo\n"); // ACCIÓN DE LA TAREA

    rt_task_inquire(NULL, &infoTarea); //SOLICITAMOS INFORMACIÓN DE LA TAREA QUE EJECUTA LA FUNCIÓN
    printf("Tarea: %s \n", infoTarea.name); //IMPRIMIMOS LA INFORMACIÓN DE LA TAREA
     // ASI IDENTIFICAMOS QUE SE EJECUTA CON LE HILO DE LA TAREA MENSAJE
}  


int main(int argc, char* argv[]){ //ARGUMENTOS DE LA FUNCIÓN PRINCIPAL
    char str[10];  //CREAMOS EL STRING (ARREGLO DE CARACTERES)
    sprintf(str,"hola"); //COLOCAMOS NOMBRE A UN STRING
    rt_task_create(&tarea_hola, str,0,50,0); //CREAMOS LA TAREA, DIRECCIÓN, NOMBRE DEL HILO, PRIORIDADES EN XENOMAI
    rt_task_start(&tarea_hola, &mensaje,0); //INICIAMOS LA TAREA, DIRECCIÓN TAREA, FUNCIÓN A EJECUTAR, 0
}