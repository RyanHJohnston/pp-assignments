all:
	gcc -fopenmp -Wall -g main.c -o main.o
	gcc -fopenmp -lpthread -g main_threads.c -o main_threads.o

run:
	./main.o
	./main_threads.o

clean:
	rm -f *.o
