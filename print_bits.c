#include <stdio.h>
#include <stdlib.h>

void binario (int i)
{
    if (i < 0)
    {
        printf("-");
        i = abs(i);
    }
    int nonZeroPrinted = 0;  // A flag to track if non-zero digit is printed

    for (int j = 31; j >= 0; j--)
    {
        int bit = (i >> j) & 1;  // Extract the j-th bit
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
    /*
    if (i == 0)
    {
        printf("0");
    }
        else
        {
            binario(i / 2);
            printf("%d", i % 2);
        } 
    */    
   
}
int main ()
{
    int i;
    printf("Introduza um numero em base decimal para converter para binario\n");
    scanf("%d", &i);  

    binario(i);
    printf("\n");

    return 0;
}