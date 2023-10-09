#include <stdio.h>
#include <stdlib.h>

void binario (int i)
{
   if (i == 0)
   printf("%d", i);
   else{
    binario(i / 2);
    printf("%d", i % 2);
   } 
}
int main ()
{
    int i;
    printf("Introduza um numero em base decimal para converter para binario\n");
    scanf("%d", &i);  

    binario(i);

}