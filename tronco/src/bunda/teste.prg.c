#include <stdio.h>

int num;
    
int quadrado (int val) 
{
	int temp;

	temp = val * val;
	return temp;
}

int main ()
{
	num = 11;
	printf ("%d\n", quadrado(num));
    
    return 0;
}
