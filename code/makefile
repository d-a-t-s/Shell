#Ejecutable
EXEC = main

# Definicion de variables
CC = gcc
CFLAGS = -Wall -g

#Regla para compilar el programa
$(EXEC): main.o comandos.o favs.o
	$(CC) $(CFLAGS) -o $(EXEC) main.o comandos.o favs.o

# Regla para compilar main.o
main.o: main.c comandos.h
	$(CC) $(CFLAGS) -c main.c

#Regla para compilar comandos.o
comandos.o: comandos.c comandos.h
	$(CC) $(CFLAGS) -c comandos.c

#Regla para compilar favs.o
favs.o: favs.c favs.h
	$(CC) $(CFLAGS) -c favs.c

# Regla para limpiar archivos
clean:
	rm -f *.o