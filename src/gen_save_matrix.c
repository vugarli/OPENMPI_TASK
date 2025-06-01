#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void generate_and_write_matrix(int rows, int cols, const char* filename) {
    int* matrix = (int*)malloc(rows * cols * sizeof(int));
    if (matrix == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    
    srand(time(NULL));

    
    for (int i = 0; i < rows * cols; i++) {
        matrix[i] = rand() % 100;
    }

    
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to open file");
        free(matrix);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows * cols; i++) {
        fprintf(file, "%d ", matrix[i]);
    }

    fclose(file);
    free(matrix);
}

int main() {
int rows, cols;
    char filename[256];  

    printf("Enter number of rows: ");
    scanf("%d", &rows);

    printf("Enter number of columns: ");
    scanf("%d", &cols);

    printf("Enter output filename: ");
    scanf("%s", filename);  

    generate_and_write_matrix(rows, cols, filename);

    printf("Matrix written to %s in 1D format.\n", filename);
    return 0;
}
