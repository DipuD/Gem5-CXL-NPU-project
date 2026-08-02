#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define SIZE 256 

// Global arrays placed in the .bss section.
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
    // Set up our MMIO register pointers based on offsets
    volatile uint32_t *npu_cmd    = (uint32_t *)0x80000000;
    volatile uint32_t *npu_addr_a = (uint32_t *)0x80000004;
    volatile uint32_t *npu_addr_b = (uint32_t *)0x80000008;
    volatile uint32_t *npu_addr_c = (uint32_t *)0x8000000C;
    volatile uint32_t *npu_size_n = (uint32_t *)0x80000010;

    printf("CPU: Programming NPU Registers...\n");
    
    // Write pointers to the NPU's MMIO registers. We cast the addresses of our arrays to uint32_t.
    // Using your actual array names: A, B, and C
    *npu_addr_a = (uint32_t)(uintptr_t)A;
    *npu_addr_b = (uint32_t)(uintptr_t)B;
    *npu_addr_c = (uint32_t)(uintptr_t)C;
    *npu_size_n = SIZE; // Using your SIZE macro

    printf("CPU: Starting NPU Hardware...\n");
    *npu_cmd = 0x01; // Send START command

    // CPU Busy-Wait Loop: Wait for the NPU to clear the command register to 0
    printf("CPU: Waiting for NPU to finish...\n");
    while (*npu_cmd != 0x00) {
        // Optionally, you can add a small delay or yield here to avoid busy-waiting too aggressively
    }

    printf("CPU: NPU finished successfully!\n");
    return 0;
}
