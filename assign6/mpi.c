#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <string.h>

#define MATRIX_SIZE 4 

void generateMatrix(float *matrix, int n) {
    int i;
    for (i = 0; i < n * n; i++) {
        matrix[i] = (float)rand() / (float)(RAND_MAX); // Generates a float between 0 and 1
    }
}

void printMatrix(float *matrix, int n) {
    int i;
    int j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            printf("%.2f ", matrix[i * n + j]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    int i;
    int rank, size, n, bandSize;
    int step;
    int k;
    int j;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    n = MATRIX_SIZE;
    bandSize = n / size;

    float *A, *B, *C, *tempB;
    A = (float *)malloc(bandSize * n * sizeof(float));
    B = (float *)malloc(bandSize * n * sizeof(float));
    C = (float *)malloc(bandSize * n * sizeof(float));
    tempB = (float *)malloc(bandSize * n * sizeof(float));

    if (rank == 0) {
        // Generate full matrices and distribute
        float *fullA = (float *)malloc(n * n * sizeof(float));
        float *fullB = (float *)malloc(n * n * sizeof(float));
        generateMatrix(fullA, n);
        generateMatrix(fullB, n);
        

        // Distribute A and B^T
        for (i = 0; i < size; ++i) {
            if (i == 0) {
                // Copy directly for P0
                memcpy(A, fullA, bandSize * n * sizeof(float));
                // Transpose and copy B
                for (j = 0; j < bandSize; ++j)
                    for (k = 0; k < n; ++k)
                        B[j * n + k] = fullB[k * n + j];
            } else { // Send to other processes
                MPI_Send(fullA + i * bandSize * n, bandSize * n, MPI_FLOAT, i, 0, MPI_COMM_WORLD);
                // Send transposed B
                for (j = 0; j < bandSize; ++j)
                    for (k = 0; k < n; ++k)
                        tempB[j * n + k] = fullB[k * n + (i * bandSize + j)];
                MPI_Send(tempB, bandSize * n, MPI_FLOAT, i, 1, MPI_COMM_WORLD);
            }
        }
        free(fullA);
        free(fullB);
    } else {
        // Receive A and B^T
        MPI_Recv(A, bandSize * n, MPI_FLOAT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(B, bandSize * n, MPI_FLOAT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // Perform computation and communication
    for (step = 0; step < size; ++step) {
        // Compute a part of C
        for (i = 0; i < bandSize; ++i) {
            for (j = 0; j < bandSize; ++j) {
                C[i * n + (bandSize * ((rank + step) % size) + j)] = 0;
                for (k = 0; k < n; ++k) {
                    C[i * n + (bandSize * ((rank + step) % size) + j)] += A[i * n + k] * B[j * n + k];
                }
            }
        }

        // Ring communication of B
        if (size > 1) {
            int sendTo = (rank + 1) % size;
            int recvFrom = (rank - 1 + size) % size;
            MPI_Sendrecv_replace(B, bandSize * n, MPI_FLOAT, sendTo, 2, recvFrom, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if (rank == 0) {
        // Collect results from all processes
        float *finalC = (float *)malloc(n * n * sizeof(float));
        memcpy(finalC, C, bandSize * n * sizeof(float));
        for (i = 1; i < size; ++i) {
            MPI_Recv(tempB, bandSize * n, MPI_FLOAT, i, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            memcpy(finalC + i * bandSize * n, tempB, bandSize * n * sizeof(float));
        }

        // Print final result
        printMatrix(finalC, n);
        free(finalC);
    } else {
        // Send results back to P0
        MPI_Send(C, bandSize * n, MPI_FLOAT, 0, 3, MPI_COMM_WORLD);
    }

    free(A);
    free(B);
    free(C);
    free(tempB);

    MPI_Finalize();
    return 0;
}
