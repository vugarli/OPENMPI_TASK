CC = mpicc
CFLAGS = -Wall -O2

SRC = main.c matrix_parser.c
OBJ = $(SRC:.c=.o)
EXEC = mpi_matrix

.PHONY: all clean run

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(EXEC)
	mpirun -np 2 ./$(EXEC)

clean:
	rm -f $(OBJ) $(EXEC)