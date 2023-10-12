#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function to print an integer in binary format
void printBits(int num) {
    printf("Binary representation of %d: ", num);

    // Assuming 32-bit integer
    for (int i = 31; i >= 0; i--) {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }

    printf("\n");
}

// Function to convert binary digits to an integer
int bitsToInt(const char* binary_str) {
    int decimal = 0;
    int length = strlen(binary_str);

    for (int i = 0; i < length; i++) {
        if (binary_str[i] == '1') {
            decimal = (decimal << 1) | 1;
        } else if (binary_str[i] == '0') {
            decimal = decimal << 1;
        } else {
            printf("Invalid binary input: %s\n", binary_str);
            exit(1);
        }
    }

    return decimal;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <operation> <input>\n", argv[0]);
        printf("Operations: print | convert\n");
        return 1;
    }

    if (strcmp(argv[1], "print") == 0) {
        int num = atoi(argv[2]);
        printBits(num);
    } else if (strcmp(argv[1], "convert") == 0) {
        int decimal = bitsToInt(argv[2]);
        printf("Decimal equivalent: %d\n", decimal);
    } else {
        printf("Invalid operation. Use 'print' or 'convert'.\n");
        return 1;
    }

    return 0;
}
