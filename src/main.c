#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#include <time.h>
#include <sys/time.h>
#include "matrix_parser.h"

int N = 1000;
MPI_Status status;


int main(int argc, char **argv)
{
    int processCount, processId, slaveTaskCount, source, dest, rows, offset;
    double *matrix_a = NULL, *matrix_b = NULL, *matrix_c = NULL;
    struct timeval start, stop;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &processId);
    MPI_Comm_size(MPI_COMM_WORLD, &processCount);
    slaveTaskCount = processCount - 1;

    printf("ProcessId: %d Process Count: %d\n",processId,processCount);

    if (processId == 0) {
        gettimeofday(&start, NULL);

        matrix_a = malloc(N * N * sizeof(double));
        matrix_b = malloc(N * N * sizeof(double));
        matrix_c = malloc(N * N * sizeof(double));

        int* flat_a = read_matrix_1d("ma.txt", N, N);
        int* flat_b = read_matrix_1d("mb.txt", N, N);

        if (flat_a == NULL || flat_b == NULL) {
            fprintf(stderr, "Failed to read matrix files.\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }

        for (int i = 0; i < N * N; i++) {
            matrix_a[i] = flat_a[i];
            matrix_b[i] = flat_b[i];
        }

        free(flat_a);
        free(flat_b);

        rows = N / slaveTaskCount;
        offset = 0;

        for (dest = 1; dest <= slaveTaskCount; dest++) {
            MPI_Send(&offset, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&rows, 1, MPI_INT, dest, 1, MPI_COMM_WORLD);
            MPI_Send(&matrix_a[offset * N], rows * N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
            MPI_Send(matrix_b, N * N, MPI_DOUBLE, dest, 1, MPI_COMM_WORLD);
            offset += rows;
        }

        for (int i = 1; i <= slaveTaskCount; i++) {
            MPI_Recv(&offset, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&rows, 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
            MPI_Recv(&matrix_c[offset * N], rows * N, MPI_DOUBLE, i, 2, MPI_COMM_WORLD, &status);
        }

        gettimeofday(&stop, NULL);
        double elapsed = (stop.tv_sec - start.tv_sec) +
                         (stop.tv_usec - start.tv_usec) / 1e6;
        printf("Elapsed time: %.6f seconds\n", elapsed);

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);
    }

    if (processId > 0) {
        MPI_Recv(&offset, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(&rows, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, &status);

        matrix_a = malloc(rows * N * sizeof(double));
        matrix_b = malloc(N * N * sizeof(double));
        matrix_c = malloc(rows * N * sizeof(double));

        MPI_Recv(matrix_a, rows * N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);
        MPI_Recv(matrix_b, N * N, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD, &status);

        for (int k = 0; k < N; k++) {
            for (int i = 0; i < rows; i++) {
                matrix_c[i * N + k] = 0.0;
                for (int j = 0; j < N; j++) {
                    matrix_c[i * N + k] += matrix_a[i * N + j] * matrix_b[j * N + k];
                }
            }
        }

        MPI_Send(&offset, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(&rows, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
        MPI_Send(matrix_c, rows * N, MPI_DOUBLE, 0, 2, MPI_COMM_WORLD);

        free(matrix_a);
        free(matrix_b);
        free(matrix_c);
    }

    MPI_Finalize();
    return 0;
}
