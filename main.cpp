#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK_SIZE 16
#define KEY {0xAA, 0xBB, 0xCC, 0xDD}

void decrypt_block(uint8_t *block) {
    uint8_t key[] = KEY;
    uint8_t temp[BLOCK_SIZE];

    // Step 1: XOR with key
    for (int i = 0; i < BLOCK_SIZE; i++) {
        temp[i] = block[i] ^ key[i % 4];
    }

    // Step 2: Reverse byte shuffle
    uint8_t reshuffled[BLOCK_SIZE] = {
        temp[0], temp[1], temp[2], temp[3],
        temp[7], temp[4], temp[5], temp[6],
        temp[10], temp[11], temp[8], temp[9],
        temp[15], temp[12], temp[13], temp[14]
    };

    // Step 3: Bit inversion
    for (int i = 0; i < BLOCK_SIZE; i++) {
        block[i] = ~reshuffled[i];
    }
}

int main() {
    FILE *file = fopen("data.bin", "rb");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    uint32_t file_hash;
    fread(&file_hash, sizeof(uint32_t), 1, file);

    int num_blocks = 10; // Placeholder for the example
    uint8_t block[BLOCK_SIZE];

    for (int i = 0; i < num_blocks; i++) {
        fread(block, sizeof(uint8_t), BLOCK_SIZE, file);
        decrypt_block(block);

        // Display decrypted block for verification
        printf("Decrypted block %d: ", i);
        for (int j = 0; j < BLOCK_SIZE; j++) {
            printf("%02X ", block[j]);
        }
        printf("\n");
    }

    fclose(file);
    return 0;
}
