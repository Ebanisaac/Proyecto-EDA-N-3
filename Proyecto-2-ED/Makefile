CC = gcc
CFLAGS = -Wall -Wextra -I./inc -std=c11

SRC = $(wildcard src/*.c)
# Transforma la ruta src/archivo.c en obj/archivo.o
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
EXEC = juego

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

# Compila los .c de src/ y genera los .o en obj/ (creando la carpeta si falta con | obj)
obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

# Regla para crear la carpeta obj automáticamente
obj:
	mkdir -p obj

clean:
	rm -rf obj $(EXEC)

run: all
	./$(EXEC)