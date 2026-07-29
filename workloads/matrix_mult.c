#include <stdio.h>
#include <stdlib.h>

#define SIZE 256 // Scaled up to blow out the 256KB L2 Cache!

// Global arrays placed in the .bss section.
// This avoids complex glibc malloc() syscalls that can crash gem5 SE mode.
static int A[SIZE][SIZE];
static int B[SIZE][SIZE];
static int C[SIZE][SIZE];

void multiply() {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            C[i][j] = 0;
            for (int k = 0; k < SIZE; k++) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main() {
    // Initialize with dummy data
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            A[i][j] = i + j;
            B[i][j] = i - j;
        }
    }

    printf("Starting CPU Matrix Multiplication...\n");
    multiply();
    printf("Finished Matrix Multiplication!\n");

    return 0;
}
