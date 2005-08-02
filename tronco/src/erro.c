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
#include <stdlib.h>

#include "defs.h"
#include "erro.h"

// Flags
int falante   = FALSO;
int depurando = FALSO;
int espacado  = FALSO;
int tecla     = FALSO;

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
        {
            IMPRIME ("    ");
        }
        else
        {
			IMPRIME ("%c", *pos);
        }
        
        pos++;
    }
        
    IMPRIME ("\n");
    return col;
}

void mostra_posicao_erro (char * pos)
{
    int col;
    
    col = mostra_linha_atual (pos);
    for (; col > 0; col--)
        IMPRIME ("%*s", *(pos-col) == '\t'? 4: 1, "-");

    IMPRIME ("^\n");
}


void sair (int ret)
{
    char *msg[] = {
        "Execução bem sucedida!", 

        "Uso: teste -[v|d|e] [-t] <arquivo>",
        "Arquivo nao encontrado!",

        "Compilação interrompida!"
    };
        
    printf ("%s\n", msg[ret>FIM_ERRO_LEXICO? FIM_ERRO_LEXICO: ret]);

    if (tecla)
    {
        printf ("\nPressione algo para continuar...\n");
        fflush(stdout);
        getchar();
    }

    exit (ret);
}
