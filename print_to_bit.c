#include <stdio.h>
#include <math.h>
#include <stdlib.h>



int main ()
{
    char num [32];
    
    printf("Introduza um numero em binario\n");
    scanf("%s", num);
    printf("O binario introduzido corresponde em decimal ao numero %s", num);
    return 0;
}

int print_to_bit (char num [32])
{
    int dec = 0, rem = 0, place = 0;
       
       int nada = atoi (num); 
        while (num)
    {
        rem = (nada) % 10;
        if (rem)
        {
            dec = dec + ( pow(2, place) );
        }
        nada = nada / 10;
        place++;

    }
    printf("%d\n", dec);
    return dec;
}

