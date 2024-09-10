#ifndef FAVS
#define FAVS

void guardar_favoritos();
void cargar_favoritos(const char *filePath);
void mostrar_favoritos();
void agregar_favorito(char *command);
void eliminar_favorito(char *nums);
void ejecutar_favorito(int id);
void borrar_favoritos();
void favs_crear(const char *ruta);
void favs_buscar(const char *cmd);

#endif
