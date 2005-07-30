/*

Compilar assim:

gcc -S -march=i386 -fverbose-asm -O0 teste.c 

*/


#include <stdio.h>

int currentYear;


void display (int n)
{
     printf ("[%d  %s  %d %d]", 8, "James Brown", currentYear, fatorial2 (8));
}

int fatorial (int param)
{
	if (param == 0)
	   return 1;
	else
	    return param * fatorial(param-1);
}

int fatorial2 (int param1)
{
    int result = 1;

    while (param1 > 1)
    {
        result = result * param1;    
		param1 = param1 - 1;
    }
  
    return result;
}


int main ()
{
    currentYear = 2005;
    display (555);
}
