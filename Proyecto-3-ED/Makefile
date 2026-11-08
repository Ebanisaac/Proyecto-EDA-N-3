CC = gcc
CFLAGS = -Wall -Wextra -I./inc -std=c11

# Detecta automáticamente todos los archivos .c dentro de la carpeta src/
SRC = $(wildcard src/*.c)

# Transforma las rutas de src/archivo.c a obj/archivo.o
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

EXEC = juego

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar los archivos fuente generando los .o dentro de obj/ (crea la carpeta si no existe)
obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

# Regla automática para crear el directorio obj
obj:
	mkdir -p obj

clean:
	rm -rf obj $(EXEC)

run: all
	./$(EXEC)