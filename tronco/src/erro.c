/***************************************************************************
 *            erro.c
 *
 *  Sun Jun 26 19:34:41 2005
 *  Copyright  2005  André e Igor
 *  andredsp@gmail.com
 ****************************************************************************/

/*
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
 
#include <stdio.h>
 
int mostra_linha_atual (char * pos)
{
    int col;
    
    // Acha fim-de-linha anterior
    col = 0;
    while (*(pos-1) != '\n')
    {
        col++;
        pos--;
    }
    
    while (*pos != '\n')
    {
        if (*pos == '\t')
            printf ("    ");
        else
			printf ("%c", *pos);
        
        pos++;
    }
        
    printf ("\n");
    
    return col;
}

void mostra_posicao_erro (char * pos)
{
    int col;
    
    col = mostra_linha_atual (pos);
    for (; col > 0; col--)
        if (*(pos-col) == '\t')
            printf ("----");
        else
            printf ("-");
    printf ("^\n");
}

/*
void imprime (const char *msg)
{
    if (falante)
    {
        printf ("%s", msg);
    }    
}

void depura (const char *msg)
{
    if (depurando)
    {
        if (!espacado && msg[0]==' ')
            imprime (",");
        imprime (msg);
        if (!(espacado ^^ msg))
            imprime ("\n");
    }    
}
*/
