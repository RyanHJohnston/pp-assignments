#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4  // Adjust as needed
#define VAL 2  // Constant value for initializing matrices

void print_matrix(int m[N][N]);

int main(int argc, char* argv[]) {
    double start_time;
    double end_time;
    int rank, size, i, j, k;
    int A[N][N], B[N][N], C[N][N], buffer[N];

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Initialization of matrices by master
    if (rank == 0) {
        start_time = MPI_Wtime();
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = VAL;
                B[i][j] = VAL;
            }
        }
    }

    // Broadcast matrix B to all processes
    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);

    // Master sends rows of A and gathers results into C
    if (rank == 0) {
        for (i = 1; i < size; i++) {
            int rows = N / size;
            int start_row = i * rows;
            MPI_Send(&A[start_row][0], rows*N, MPI_INT, i, 0, MPI_COMM_WORLD);
        }

        // Master computes its portion
        for (i = 0; i < N / size; i++) {
            for (j = 0; j < N; j++) {
                C[i][j] = 0;
                for (k = 0; k < N; k++) {
                    C[i][j] += A[i][k] * B[k][j];
                }
            }
        }

        // Gather results from other processes
        for (i = 1; i < size; i++) {
            MPI_Recv(buffer, N*N/size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            for (j = 0; j < N / size; j++) {
                for (k = 0; k < N; k++) {
                    C[i * N/size + j][k] = buffer[j * N + k];
                }
            }
        }

    } else {
        // Slaves receive their portion of A, compute result and send back to master
        MPI_Recv(buffer, N*N/size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (i = 0; i < N / size; i++) {
            for (j = 0; j < N; j++) {
                int sum = 0;
                for (k = 0; k < N; k++) {
                    sum += buffer[i * N + k] * B[k][j];
                }
                buffer[i * N + j] = sum;
            }
        }
        MPI_Send(buffer, N*N/size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    // Master prints the result
    if (rank == 0) {
        end_time = MPI_Wtime();
        printf("Result matrix C:\n");
        print_matrix(C);
        printf("Execution time with %d processors: %f seconds\n", 
                size, end_time - start_time);
    }

    MPI_Finalize();

    return 0;
}

void print_matrix(int m[N][N]) {
    int i;
    int j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            printf("%d\t", m[i][j]);
        }
        printf("\n");
    }
}
