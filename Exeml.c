#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Function to print an integer in binary format
void printBits(int num) 
{

    char str[32]; // Assuming a 32-bit integer
    snprintf(str, sizeof(str), "%d", num);

    char* endptr;
    long int parsed_value = strtol(str, &endptr, 10);

    if (*endptr != '\0' || parsed_value < 0) 
    {
        fprintf(stderr, "Invalid input: Input must be a positive integer.\n");
        return;
    }

    // The rest of the code for printing the binary representation
    printf("Binary representation of %d: ", num);
    for (int i = 31; i >= 0; i--) 
    {
        int bit = (num >> i) & 1;
        printf("%d", bit);
    }
    printf("\n");

}

// Function to convert binary digits to an integer
int bitsToInt(const char* binary_str) 
{
    int decimal = 0;
    int length = strlen(binary_str);

    for (int i = 0; i < length; i++) 
    {
        if (binary_str[i] != '0' && binary_str[i] != '1') 
        {
            printf("Invalid binary input: %s\n", binary_str);
            exit(1);
        }    
        decimal = (decimal << 1 | (binary_str[i] - '0'));      
    }
    return decimal;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <operation> <input>\n", argv[0]);
        fprintf(stderr, "Operations: binario | decimal\n");
        return 1;
    }
    if (strcmp(argv[1], "binario") == 0) 
    {
        int num = atoi(argv[2]);
        if (num < 0)
        {
            fprintf(stderr, "Input value must be positive\n");
            return 1;
        }
        printBits(num);
    } 
    else if (strcmp(argv[1], "decimal") == 0) 
    {
        int decimal = bitsToInt(argv[2]);
        printf("Decimal equivalent: %d\n", decimal);
    } else {
        fprintf(stderr, "Invalid operation. Use 'print' or 'convert'.\n");
        return 1;
    }

    return 0;
}
