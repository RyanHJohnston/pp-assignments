#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <bits/time.h>

#define MATRIX_SIZE 100

void print_matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]);
void matrix_multiply(int A[MATRIX_SIZE][MATRIX_SIZE], 
        int B[MATRIX_SIZE][MATRIX_SIZE], 
        int C[MATRIX_SIZE][MATRIX_SIZE]);

int
main(int argc, char *argv[])
{
    int A_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int B_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int C_matrix[MATRIX_SIZE][MATRIX_SIZE];
    int i;
    int j;
    int k;
    
    struct timespec start;
    struct timespec end;
    double cpu_time_used;

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

    /* multiply matrixes using matrix-matrix multiply */
    clock_gettime(CLOCK_MONOTONIC, &start);
    matrix_multiply(A_matrix, B_matrix, C_matrix);

    clock_gettime(CLOCK_MONOTONIC, &end);
    cpu_time_used = end.tv_sec - start.tv_sec;
    cpu_time_used += (end.tv_nsec - start.tv_nsec) / 1000000000.0;

    FILE *fp;
    fp = fopen("times.txt", "a");
    if (fp == NULL) {
        fprintf(stderr, "File %s failed to open\n", "times.txt");
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

void 
matrix_multiply(int A[MATRIX_SIZE][MATRIX_SIZE], 
        int B[MATRIX_SIZE][MATRIX_SIZE], 
        int C[MATRIX_SIZE][MATRIX_SIZE]) 
{
    int i;
    int j;
    int k;
    #pragma omp parallel for
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            C[i][j] = 0;  // Initialize the resultant matrix element to 0
            for (k = 0; k < MATRIX_SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}


