#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include "comandos.h"

int main(int argc, char const *argv[])
{
    setbuf(stdin, NULL);
    char entrada[4096];
    char ruta[256];
    char ***comandos_argumentos;
    char **comandos;
    char *token;

    int numero_comandos = 0;
    int numero_argumento = 0;

    //Matriz tridimensional donde se guardan las tokenizaciones de cada uno de los comando de la matriz siguiente (cuyo nombre es "comandos") con delimitador de espacio " "
    comandos_argumentos = (char***) malloc(512 * sizeof(char***));
    if (comandos_argumentos == NULL) {
        perror("Error de memoria");
        exit(1);
    }
    //Matriz donde se guardan las tokenizacion de la entrada con delimitador de pipe "|"
    comandos = (char**) malloc(512 * sizeof(char**));
    if (comandos == NULL) {
        perror("Error de memoria");
        exit(1);
    }

    //Ciclo que se estara ejecutandose infinitiamente mientras la shell este en funcionamiento
    while (1){
        if (getcwd(ruta, sizeof(ruta)) == NULL)
        {
            printf("Error en obtener la ruta\n");
            exit(0);
        }
        printf("\e[38;5;198mCustom\e[38;5;45mShell:\e[38;5;190m%s\e[0m$ ",ruta);
        fflush(NULL);
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = 0;

        //Si el comando ingresado es "exit" se sale de la shell
        if (!strcmp(entrada, "exit"))
        {
            exit(0);
        }
        
        //Condicional para que la shell siga en funcionamiento si se le entrega un string vacio
        if(strlen(entrada) == 0){
            continue;
        }

        char copia_entrada[sizeof(entrada)];
        strcpy(copia_entrada, entrada);

        //Tokenizacion de la entrada estandar con delimitador de pipes que se guardaran en la matriz "comandos"
        token = strtok(entrada, "|");

        while (token != NULL) {
            comandos[numero_comandos] = token;
            numero_comandos++;

            token = strtok(NULL, "|");
        }
        //Tokenizacion de cada comando junto con argumentos con delimitador de espacio que se guardaran en la matriz "comandos_argumentos"
        for (size_t i = 0; i < numero_comandos; i++)
        {

            token = strtok(comandos[i], " ");
            comandos_argumentos[i] = (char**) malloc(8*sizeof(char**));
            while (token != NULL) {
                comandos_argumentos[i][numero_argumento] = token;
                numero_argumento++;

                token = strtok(NULL, " ");

            }
            numero_argumento = 0;
        }
        //Bloque de codigo en el que se verifica que el comando ingresado sea el correspondiente al "set recordatorio" para llevar a cabo dicho comando
        if(strcmp(comandos_argumentos[0][0], "set") == 0){
            if(!(comandos_argumentos[0][1] == NULL || comandos_argumentos[0][2] == NULL || comandos_argumentos[0][3] == NULL)){
                int x = atoi(comandos_argumentos[0][2]);
                if(x == 0){
                    printf("Error al ingresar el tiempo\n");
                    continue;
                }else{
                    signal(SIGCHLD, SIG_IGN);
                    pid_t pid = fork();
                    if(pid < 0){
                        perror("fork failed\n");
                        exit(1);
                    }else if(pid == 0){
                        int cont = 3;
                        char recordatorio[4096] = "Recordatorio:";
                        while (comandos_argumentos[0][cont]){
                            strcat(recordatorio,(comandos_argumentos[0][cont]));
                            strcat(recordatorio," ");
                            cont++;
                        }
                        sleep(x);
                        printf("\n%s\n",recordatorio);
                        return(0);
                    }
                }
            }else{
                printf("Error al ingresar el comando\n");
            }
        }
        //Ciclo donde se ejecuta cada comando excluyendo el "set" pues "set" se utiliza para  el comando "set recordatorio"
        if (strcmp(comandos_argumentos[0][0], "set"))
        {
            ejecutar_comandos(numero_comandos, comandos_argumentos);
        }
        
        fflush(stdout);
        numero_comandos = 0;
    }
    //Liberacion de la memoria asignada para las matrices inicialmente declaradas
    free(comandos);
    free(comandos_argumentos);
    return 0;
}