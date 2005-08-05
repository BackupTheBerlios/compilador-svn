/*

   Compilar assim:

   gcc -S -march=i386 -fverbose-asm -O0 teste.c 

*/


#include <stdio.h>

int currentYear;

int ret_dois_local_dois_param (int a, int b)
{
    int c;
    int d;
    c = -a;
    d = c*(2*c+b*(5*c+4-1*a));
    return d;
}





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
	int ano;
	int num;
    int i, j, k;

	ano = 2005;
	num = 555;

	currentYear = ano;
	display (num);
    i = j = k = 50;
}
