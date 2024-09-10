#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 256  // Tamaño máximo para el comando
#define MAX_FAVS 50  // Máximo de comandos favoritos

typedef struct {
    int id;  // Identificador del comando favorito
    char command[MAX_INPUT_SIZE];  // Almacena el comando favorito
} FavCommand;

FavCommand favoritos[MAX_FAVS];  // Array de comandos favoritos
int favCount = 0;  // Número de comandos favoritos
char favFilePath[MAX_INPUT_SIZE] = "";  // Ruta del archivo de favoritos

// Borra el contenido del archivo de favoritos y resetea el contador
void borrar_favoritos() {
    if (strlen(favFilePath) > 0) {
        FILE *file = fopen(favFilePath, "w");
        if (file == NULL) {
            perror("Error al abrir el archivo de favoritos para borrar");
            return;
        }
        fclose(file);
        printf("Archivo de favoritos %s ha sido vaciado.\n", favFilePath);
    } else {
        printf("No se ha definido un archivo de favoritos.\n");
    }

    favCount = 0;
    printf("Todos los comandos favoritos han sido eliminados.\n");
}

// Guarda los favoritos en el archivo
void guardar_favoritos() {
    if (strlen(favFilePath) == 0) {
        printf("No se ha creado un archivo para favoritos.\n");
        return;
    }

    FILE *file = fopen(favFilePath, "w");
    if (file == NULL) {
        perror("Error al abrir el archivo de favoritos");
        return;
    }

    for (int i = 0; i < favCount; i++) {
        fprintf(file, "%d %s\n", favoritos[i].id, favoritos[i].command);
    }

    fclose(file);
    printf("Favoritos guardados en %s\n", favFilePath);
}

// Muestra los favoritos guardados
void mostrar_favoritos() {
    if (favCount == 0) {
        printf("No hay comandos favoritos.\n");
        return;
    }

    for (int i = 0; i < favCount; i++) {
        printf("%d: %s\n", favoritos[i].id, favoritos[i].command);
    }
}

// Carga los favoritos desde un archivo
void cargar_favoritos(const char *filePath) {
    if (strlen(filePath) == 0) {
        printf("No se ha especificado un archivo para favoritos.\n");
        return;
    }

    FILE *file = fopen(filePath, "r");
    if (file == NULL) {
        perror("Error al abrir el archivo de favoritos");
        return;
    }

    favCount = 0;
    while (fscanf(file, "%d %[^\n]", &favoritos[favCount].id, favoritos[favCount].command) != EOF) {
        favCount++;
    }

    fclose(file);
    printf("Favoritos cargados de %s\n", filePath);
    mostrar_favoritos();
}

// Añade un nuevo favorito si no está ya en la lista
void agregar_favorito(char *command) {
    for (int i = 0; i < favCount; i++) {
        if (strcmp(favoritos[i].command, command) == 0) {
            return;
        }
    }

    if (favCount < MAX_FAVS) {
        favoritos[favCount].id = favCount + 1;
        strcpy(favoritos[favCount].command, command);
        favCount++;
    } else {
        printf("Lista de favoritos llena.\n");
    }
}

// Elimina uno o más favoritos según los IDs proporcionados
void eliminar_favorito(char *nums) {
    int numArray[MAX_FAVS];
    int numCount = 0;

    // Divide la cadena en IDs
    char *token = strtok(nums, ",");
    while (token != NULL && numCount < MAX_FAVS) {
        numArray[numCount++] = atoi(token);
        token = strtok(NULL, ",");
    }

    // Ordena los IDs en orden descendente
    for (int i = 0; i < numCount - 1; i++) {
        for (int j = i + 1; j < numCount; j++) {
            if (numArray[i] < numArray[j]) {
                int temp = numArray[i];
                numArray[i] = numArray[j];
                numArray[j] = temp;
            }
        }
    }

    // Elimina los favoritos por ID
    for (int i = 0; i < numCount; i++) {
        int num = numArray[i];
        for (int j = 0; j < favCount; j++) {
            if (favoritos[j].id == num) {

                // Reorganiza la lista de favoritos
                for (int k = j; k < favCount - 1; k++) {
                    favoritos[k] = favoritos[k + 1];
                }
                favCount--;

                // Actualiza los IDs
                for (int k = 0; k < favCount; k++) {
                    favoritos[k].id = k + 1;
                }
                break;
            }
        }
    }
}

// Ejecuta el comando favorito por su ID
void ejecutar_favorito(int id) {
    for (int i = 0; i < favCount; i++) {
        if (favoritos[i].id == id) {
            printf("Ejecutando: %s\n", favoritos[i].command);
            system(favoritos[i].command);
            return;
        }
    }
    printf("Comando con ID %d no encontrado.\n", id);
}

// Crea el archivo de favoritos, si no existe
void favs_crear(const char *ruta) {
    if (strlen(ruta) == 0) {
        getcwd(favFilePath, sizeof(favFilePath));
        strcat(favFilePath, "/favoritos.txt");
    } else {
        strcpy(favFilePath, ruta);
    }

    FILE *file = fopen(favFilePath, "a");
    if (file == NULL) {
        perror("Error al crear el archivo de favoritos");
        exit(EXIT_FAILURE);
    }
    fclose(file);

    printf("Archivo de favoritos creado: %s\n", favFilePath);
}

// Busca comandos favoritos que contengan una cadena
void favs_buscar(const char *cmd) {
    int found = 0;
    for (int i = 0; i < favCount; i++) {
        if (strstr(favoritos[i].command, cmd)) {
            printf("%d: %s\n", favoritos[i].id, favoritos[i].command);
            found = 1;
        }
    }
    if (!found) {
        printf("No se encontraron comandos que coincidan con '%s'.\n", cmd);
    }
}
