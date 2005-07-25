/***************************************************************************
 *            arquivo.c
 *
 *  Sat Jun 25 12:33:15 2005
 *  Copyright  2005  André
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
#include "arquivo.h"

#define MAX_BLOCO               200


char * le_arquivo (char *nome)
{
    char *dados;
    int tamanho, lido;
    FILE *arq;
    
    arq = fopen (nome, "r");

    if (arq == NULL)
        return NULL;

    dados = malloc (MAX_BLOCO * sizeof (char));
    tamanho = 0;

    while (! feof (arq))
    {
        lido = fread (dados+tamanho, sizeof (char), MAX_BLOCO, arq);
        tamanho += lido;
        if (lido == MAX_BLOCO)
            dados = realloc (dados, (tamanho + MAX_BLOCO) * sizeof (char));
    }
    
    dados [tamanho] = '\0';
    
    fclose (arq);
	
	return dados;
}
