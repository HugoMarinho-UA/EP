#include <stdio.h>
#include <stdlib.h>

void binario (long long i)
{
    if (i < 0)
    {
        printf("-");
        i = abs(i);
    }
    int nonZeroPrinted = 0;  // To verify if non-zero digit is printed

    for (long long j = 63; j >= 0; j--)
    {
        int bit = (i >> j) & 1;  
        if (bit == 1 || nonZeroPrinted)
        {
            printf("%d", bit);
            nonZeroPrinted = 1;
        }
    }
    if (!nonZeroPrinted)
    {
        printf("0");  // Print at least one zero for the value 0
    }
      
   
}
int main ()
{
    char input [50];
    long long i;

    printf("Introduza um número em base decimal para converter para binário\n");
    
    if (scanf("%s", input) == 1)
    {
        char *endptr;
        i = strtoll (input, &endptr, 10);

        if (*endptr == '\0')
        {
                binario(i);
                printf("\n");
        } 
        else
        {
            printf("Número inválido. \n");
        }
    }
    else
    {
        printf("Número inválido. \n");
    }

    return 0;
}
