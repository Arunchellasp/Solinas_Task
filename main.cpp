#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define BUFFER_SIZE 16
#define MAX_BLOCKS 100 // Set a limit for the number of blocks

struct Decrypted_Block {
    int32_t random_number;
    int32_t sort_key;
    char text[8];
};

// Function to calculate the SDBM hash
uint32_t sdbm_hash(const unsigned char *str, size_t len) {
    uint32_t hash = 0;
    for (size_t i = 0; i < len; ++i) {
        hash = str[i] + (hash << 6) + (hash << 16) - hash;
    }
    return hash;
}

// Function to compare blocks based on sort key
int compare_blocks(const void *a, const void *b) {
    const struct Decrypted_Block *block_a = (const struct Decrypted_Block *)a;
    const struct Decrypted_Block *block_b = (const struct Decrypted_Block *)b;
    return block_a->sort_key - block_b->sort_key;
}

int main() {
    // Open "data.bin" file in binary read mode
    FILE *file = fopen("data.bin", "rb");
    if (!file) {
        printf("Failed to open file.\n");
        return 1;
    }

    // Read the first 4 bytes as the hash
    uint32_t file_hash;
    fread(&file_hash, sizeof(uint32_t), 1, file);
    printf("Read SDBM hash: 0x%08X\n", file_hash);

    // Define the XOR key values
    uint8_t xor_keys[4] = {0xAA, 0xBB, 0xCC, 0xDD};

    // Buffer to hold 16 bytes of data
    uint8_t buffer[BUFFER_SIZE];
    uint8_t processed_blocks[MAX_BLOCKS][BUFFER_SIZE]; // Store processed blocks
    int block_count = 0;

    // Read and process each 16-byte block
    while (fread(buffer, 1, BUFFER_SIZE, file) == BUFFER_SIZE) {
        // Apply XOR to each byte in 4-byte groups
        for (int i = 0; i < BUFFER_SIZE; i += 4) {
            buffer[i] ^= xor_keys[0];
            buffer[i + 1] ^= xor_keys[1];
            buffer[i + 2] ^= xor_keys[2];
            buffer[i + 3] ^= xor_keys[3];
        }

        // Shuffle the bytes according to the specified pattern
        uint8_t temp[BUFFER_SIZE];
        temp[0] = buffer[0]; temp[1] = buffer[1]; temp[2] = buffer[2]; temp[3] = buffer[3];
        temp[4] = buffer[5]; temp[5] = buffer[6]; temp[6] = buffer[7]; temp[7] = buffer[4];
        temp[8] = buffer[10]; temp[9] = buffer[11]; temp[10] = buffer[8]; temp[11] = buffer[9];
        temp[12] = buffer[15]; temp[13] = buffer[12]; temp[14] = buffer[13]; temp[15] = buffer[14];

        // Copy the shuffled result back to buffer
        memcpy(buffer, temp, BUFFER_SIZE);

        // Invert the bits of each byte
        for (int i = 0; i < BUFFER_SIZE; i++) {
            buffer[i] = ~buffer[i];
        }

        // Store the processed block for later processing
        if (block_count < MAX_BLOCKS) {
            memcpy(processed_blocks[block_count], buffer, BUFFER_SIZE);
            block_count++;
        } else {
            printf("Maximum block limit reached. Some data may be lost.\n");
            break;
        }
    }

    // Close the file
    fclose(file);

    // Calculate the SDBM hash of the decrypted message
    uint32_t calculated_hash = sdbm_hash((unsigned char *)processed_blocks, block_count * BUFFER_SIZE);

    // Verify the calculated hash with the file hash
    if (calculated_hash != file_hash) {
        printf("Hash mismatch! Decryption might be incorrect.\n");
        return 1;
    } else {
        printf("Hash verified successfully.\n");
    }

    // Convert the processed blocks to Decrypted_Block structure array
    struct Decrypted_Block decrypted_blocks[MAX_BLOCKS];
    for (int i = 0; i < block_count; i++) {
        memcpy(&decrypted_blocks[i], processed_blocks[i], sizeof(struct Decrypted_Block));
    }

    // Sort the decrypted blocks based on sort key
    qsort(decrypted_blocks, block_count, sizeof(struct Decrypted_Block), compare_blocks);

    // Print the custom message
    printf("Custom Message:\n");
    for (int i = 0; i < block_count; i++) {
        printf("%s", decrypted_blocks[i].text);
    }
    printf("\n");

    return 0;
}
