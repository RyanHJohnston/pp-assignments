#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <bits/time.h>

#define MATRIX_SIZE 100
#define NUM_THREADS 2

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

int A_matrix[MATRIX_SIZE][MATRIX_SIZE];
int B_matrix[MATRIX_SIZE][MATRIX_SIZE];
int C_matrix[MATRIX_SIZE][MATRIX_SIZE];

typedef struct ThreadData
{
    int start_row;
    int end_row;
} ThreadData;

void *matrix_multiply_threads(void *arg);

int
main(int argc, char *argv[])
{
    int i;
    int j;
    int k;
    struct timespec start;
    struct timespec end;
    double cpu_time_used;
    pthread_t threads[NUM_THREADS];
    ThreadData data[NUM_THREADS];



    for (i = 0; i < MATRIX_SIZE; ++i) {
        for (j = 0; j < MATRIX_SIZE; ++j) {
            A_matrix[i][j] = 2;
        }
    }
    
    for (i = 0; i < MATRIX_SIZE; ++i) {
        for (j = 0; j < MATRIX_SIZE; ++j) {
            B_matrix[i][j] = 4;
        }
    }

    for (i = 0; i < MATRIX_SIZE; ++i) {
        for (j = 0; j < MATRIX_SIZE; ++j) {
            C_matrix[i][j] = 0;
        }
    }
    
    data[0].start_row = 0;
    data[0].end_row = MATRIX_SIZE / 2;
    data[1].start_row = MATRIX_SIZE / 2;
    data[1].end_row = MATRIX_SIZE;

    clock_gettime(CLOCK_MONOTONIC, &start);

    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_create(&threads[i], NULL, matrix_multiply_threads, &data[i]);
    }

    for (i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    
    clock_gettime(CLOCK_MONOTONIC, &end);
    cpu_time_used = end.tv_sec - start.tv_sec;
    cpu_time_used += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    FILE *fp;
    fp = fopen("thread_times.txt", "a");
    if (fp == NULL) {
        fprintf(stderr, "File %s failed to open\n", "thread_times.txt");
        exit(EXIT_FAILURE);
    }
    
    fprintf(fp, "%f\n", cpu_time_used);

    fclose(fp);

    exit(EXIT_SUCCESS);
}

void
print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE])
{
    int i;
    int j;
    
    for (i = 0; i < MATRIX_SIZE; ++i) {
        for (j = 0; j < MATRIX_SIZE; ++j) {
            fprintf(stdout, "%i ", matrix[i][j]);
        }
        fprintf(stdout, "\n");
    }

}

void *
matrix_multiply_threads(void *arg)
{
    ThreadData *data = (ThreadData *)arg;
    int i;
    int j;
    int k;

    #pragma omp parallel for
    for (i = data->start_row; i < data->end_row; ++i) {
        for (j = 0; j < MATRIX_SIZE; ++j) {
            for (k = 0; k < MATRIX_SIZE; ++k) {
                C_matrix[i][j] += A_matrix[i][k] * B_matrix[k][j];
            }
        }
    }

    pthread_exit(NULL);
}

