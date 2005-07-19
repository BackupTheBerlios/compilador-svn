/***************************************************************************
 *            analisadorSintatico.c
 *
 *  Sun Jun 26 17:49:39 2005
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

#include "defs.h"
#include "atomo.h"

typedef struct s_transicao transicao;
struct s_transicao {
	int estado;
	void (*maq)();	
};

static int estado_atual;
static void *maq_atual();	

static int retornos;
transicao * pilha_retornos;


/* Tipo (T):

Estado	Entrada	
        real	integer	boolean	[	]	,	N
0	1	1	1				
1				3			
2							
3							4
4					2	3	
*/
uma_classe cab_tipo [] = {
    C_INVALIDA,         // A primeira classe será ignorada
    PR_REAL,
    PR_INTEGER,
    PR_BOOLEAN,
    S_ABRE_CHAVE,
    S_FECHA_CHAVE,
    S_VIRGULA,
    C_INTEIRO
};

void funcao ()
{
}

void algo ()
{
}

transicao transicoes[5][2] = {
	{ {0, funcao},        {1, funcao} },
	{ {0, funcao},        {1, funcao} },
	{ {0, funcao},        {1, funcao} },
	{ {0, funcao},        {1, funcao} },
	{ {1, funcao},    {2, algo}   }
};
