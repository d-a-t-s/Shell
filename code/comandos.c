#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "favs.h"


//Funcion para ejecutar comandos ingresados en la Shell
void ejecutar_comandos(int cant_comandos, char*** comandos){
    
    //Variable que representa el indice del comando ejecutado
    int index = 0;
    //Cantidad de pipes a crear
    int cant_pipes = cant_comandos - 1;
    
    //Caso en que el comando ingresado sea cd
    if (strcmp(comandos[0][0], "cd") == 0) {
            
            //El directorio de trabajo cambia al ambiente HOME
            if (comandos[0][1] == NULL || strcmp(comandos[0][1], "~") == 0) {
                chdir(getenv("HOME"));
            } 
            //El directorio de trabajo cambia al directorio padre
            else if(strcmp(comandos[0][1], "..") == 0){
                chdir("..");
            }
            //El directorio de trabajo cambia al entregado como argumento
            else {
                if (chdir(comandos[0][1]) != 0) {
                    perror("Error cambiando el directorio");
                }
            }
    }

    //Caso en el que se ejecute un comando favs
    else if((strcmp(comandos[0][0], "favs") == 0)){
        if(!(comandos[0][1] == NULL)){
            //Muestra los favoritos contenidos en la estructura de datos
            if ((strcmp(comandos[0][1], "mostrar") == 0))
            {
                mostrar_favoritos();
            }
            //Borra los favoritos de la estructura de datos y del archivo asociado
            else if((strcmp(comandos[0][1], "borrar") == 0)){
                borrar_favoritos();
            }
            //Guarda los favoritos de la estructura de datos en el archivo asociado
            else if ((strcmp(comandos[0][1], "guardar") == 0))
            {
                guardar_favoritos();
            }else{
                printf("Error al ingresar el comando\n");
            }
        }else if(!(comandos[0][2] == NULL)){
            //Crea un archivo para guardar los favoritos de la estructura en un archivo entregado por el usuario
            if ((strcmp(comandos[0][1], "crear") == 0))
            {
                favs_crear(comandos[0][2]);
            }
            //Elimina el favorito que esta asociado al id entregado como argumento
            else if ((strcmp(comandos[0][1], "eliminar") == 0))
            {
                eliminar_favorito(comandos[0][2]);
            }
            //Busca y imprime en pantalla todos los comandos con el substring a buscar
            else if ((strcmp(comandos[0][1], "buscar") == 0))
            {
                favs_buscar(comandos[0][2]);
            }
            //Ejecuta el comando favorito asociado al id entregado
            else if ((strcmp(comandos[0][1], "ejecutar") == 0))
            {
                ejecutar_favorito(atoi(comandos[0][2]));
            }
            //Carga los favoritos de un archivo entregado en la estructura de datos
            else if ((strcmp(comandos[0][1], "cargar") == 0))
            {
                cargar_favoritos(comandos[0][2]);
            }else{
                printf("Error al ingresar el comando\n");
            }
        }else{
            printf("Error al ingresar el comando\n");
        }
        
    }

    else{
        //Arreglo 2D que representa las pipes del comando
        int pipes[cant_pipes][2];
        
        //Se crean las pipes con la funcion correspondiente a partir del arreglo
        for (int i = 0; i < cant_pipes; i++)
        {
            if (pipe(pipes[i]) < 0){
            printf("Error");
            exit(1);
            }
        }
        
        //Ciclo que itera por cada comando ingresado
        for (index = 0; index < cant_comandos; index++){
            
            pid_t pid;
            
            //Caso en el que la funcion fork falle
            if ((pid = fork()) < 0) {
                printf("ERROR: forking process failed\n");
                exit(1);
            }
            
            //Programa a ejecutar por el proceso hijo
            if (pid == 0)
            {   
                //Caso en el que el comando lee de la pipe 
                if (index > 0) {
                    //Se duplica el descriptor de archivo de la entrada estandar
                    dup2(pipes[index-1][0], STDIN_FILENO);
                }
                //Caso en el que el comando escribe en la pipe 
                if (index + 1 < cant_comandos) {
                    //Se duplica el descriptor de archivo de la salida estandar
                    dup2(pipes[index][1], STDOUT_FILENO); 
                }

                //Se cierran todas las pipes al ser compartidas por todos lo procesos
                for (int i = 0; i < cant_pipes; i++) {
                    close(pipes[i][0]);
                    close(pipes[i][1]);
                }

                //Se hace un cambio de imagen del proceso para que ejecute el comando asociado a ese indice
                if (execvp(comandos[index][0], comandos[index]) == -1) { 
                    perror("Comando no reconocido"); 
                    exit(EXIT_FAILURE);
                } 
            } 
        }   
        
        
        int i = 0;
        //Se cierran las pipes en el proceso padre
        for (i = 0; i < cant_pipes; i++) {
            close(pipes[i][0]);
            close(pipes[i][1]);
        }

        //El padre espera a que todos los hijos terminen de ejecutarse
        for (int i = 0; i < cant_comandos; i++) {
            wait(NULL);
        }
    }
}