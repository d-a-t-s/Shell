# Shell
<h1 align="center">Proyecto CustomShell</h1>
<h3 align="center">Sistemas Operativos</h3>

### Instrucciones de uso:

### Comandos:

La shell soporta el uso de diferentes comandos, incluyendo los tipicos de una shell y comandos personalizados que fueron añadidos, se pueden interconectar los comandos mediante pipes(|)

Comandos añadidos usando chdir()
- `cd [directorio]`: Cambia el directorio actual al especificado.
- `cd ..`: Cambia el directorio actual al directorio padre.

Se pueden ejecutar comandos con argumentos adicionales y combinar la salida de estos usando pipes (|). Un ejemplo es:
```sh
ls | wc -l
```

### Recordatorio 
- `set recordatorio [segundos] [mensaje]:` Configura una alarma que desplegará un mensaje luego de los segundos especificados. Todo lo que venga después del 
tiempo en segundos será considerado como mensaje.

### Comandos `favs`(Gestión de Comandos Favoritos) :

Si no se ingresan los comandos con los argumentos mostrados a continuación la shell no ejecutara el comando por falta de argumentos.

`favs crear [ruta]` : Crea un archivo que contendrá los comandos que se irán guardando como favoritos
```sh
favs crear /ruta/archivo_favoritos.txt
```

`favs mostrar` : Muestra todos los comandos favoritos guardados de forma enumerada. 

`favs eliminar [num1,num2]` : Elimina el o los comandos favoritos guardados proporcionando el número
mostrado en favs mostrar
```sh
favs eliminar 1,2
```
Esto borraria los comandos 1 y 2 de la lista de comandos favoritos visto del favs mostrar

`favs buscar [substring]` : Busca comandos en la lista de favoritos que contengan la subcadena proporcionada en el argumento
```sh
favs buscar ls  
```
esto buscará y mostrará los comandos favortios que contengan ls

`favs borrar` : Limpia los comandos favoritos tanto del archivo asociado como de la estructura de datos.

`favs guardar` : Guarda los comandos favoritos actuales en el archivo de texto, para que no se pierdan al cerrar la shell.

`favs cargar [ruta]` : Lee ese archivo guardado y vuelve a cargar los comandos favoritos en la memoria, para que puedan ser utilizados en la sesión actual del shell.

`favs borrar` :  Borra todos los comandos favoritos almacenados en el archivo de comandos y vacía la lista de comandos en memoria.
