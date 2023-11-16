CC=mpicc
FLAGS=-Wall -g

all:
	$(CC) $(FLAGS) mpi.c -o mpi.o

run:
	mpiexec -np 4 ./mpi.o

clean:
	rm -f *.o
