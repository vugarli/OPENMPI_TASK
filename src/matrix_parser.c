#include <stdio.h>
#include <stdlib.h>


int* read_matrix_1d(const char* filename, int rows, int cols) {
    printf("ss");
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return NULL;
    }

    int* matrix = (int*)malloc(rows * cols * sizeof(int));
    if (matrix == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        return NULL;
    }

    for (int i = 0; i < rows * cols; i++) {
        if (fscanf(file, "%d", &matrix[i]) != 1) {
            fprintf(stderr, "Error reading value at index %d\n", i);
            free(matrix);
            fclose(file);
            return NULL;
        }
    }

    fclose(file);
    return matrix;
}
