CC = gcc
CFLAGS = -Wall -Wextra -I./inc -std=c11
SRC = $(wildcard src/*.c)
OBJ = $(SRC:.c=.o)
EXEC = juego

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: all
	./$(EXEC)