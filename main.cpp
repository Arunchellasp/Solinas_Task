#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

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

    // Close the file (we will reopen it in the next step when processing data)
    fclose(file);
    return 0;
}
