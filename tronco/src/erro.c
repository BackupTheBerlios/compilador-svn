/***************************************************************************
 *            erro.c
 *
 *  Sun Jun 26 19:34:41 2005
 *  Copyright  2005  User
 *  Email
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
#include "analisadorLexico.h"
 
void mostra_linha_atual (char * pos)
{
    int col = coluna_atual();
    
    for (pos -= col; *pos != '\n'; pos++)
        if (*pos == '\t')
            printf ("    ");
        else
			printf ("%c", *pos);
    printf ("\n");
    
}

void mostra_posicao_erro (char * pos)
{
    int col;
    
    for (col = coluna_atual(); col > 0; col--)
        if (*(pos-col) == '\t')
            printf ("----");
        else
            printf ("-");
    printf ("^\n");
}
