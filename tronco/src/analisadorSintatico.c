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

uma_classe transicoes_cabecalho[] = {
    C_INVALIDA,         // A primeira classe será ignorada
    C_FIM,
	C_IDENTIFICADOR,
    C_INTEIRO,
    C_REAL
};

struct s_transicoes {
	int proximo_estado;
	void func*();	
};

struct s_transicoes transicoes[][] = {
	{ {0, programa},        {1, funcao} },
	{ {1, procedimento},    {2, algo}   }
};
