#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 16

int main() {
    FILE *file = fopen("data.bin", "rb");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    // Read the SDBM hash
    uint32_t file_hash;
    fread(&file_hash, sizeof(uint32_t), 1, file);
    printf("Read SDBM hash: 0x%08X\n", file_hash);

    // Define the number of blocks (for this example, assuming a known value)
    int num_blocks = 10; // Placeholder, should be set based on file size in final implementation
    uint8_t block[BLOCK_SIZE];

    // Process each encrypted block
    for (int i = 0; i < num_blocks; i++) {
        fread(block, sizeof(uint8_t), BLOCK_SIZE, file);
        printf("Encrypted block %d: ", i);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            printf("%02X ", block[j]);
        }
        printf("\n");
    }

    fclose(file);
    return 0;
}
