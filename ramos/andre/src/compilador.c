/***************************************************************************
 *            compilador.c
 *
 *  Fri Jul 22 12:12:09 2005
 *  Copyright  2005  André Pinto e Igor Arouca
 *  andre.pinto@poli.usp.br
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
//#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "arquivo.h"
#include "sintatico.h"

//#define ESPERA_TECLA

void sair (int ret, const char * msg)
{
    printf ("%s\n", msg);
#ifdef ESPERA_TECLA
    printf ("Pressione algo para continuar...\n");
    fflush(stdout);
    getchar();
#endif
    exit (ret);
}

int main (int argc, char **argv)
{
    char *dados;
    int erro;

    if (argc == 1)
        sair (FIM_ERRO_PARAMETRO, "Uso: teste <arquivo>\n");

    dados = le_arquivo (argv[1]);
    
    if (dados == NULL)
    {
        char msg[101]="";
        strncat (msg, argv[1], 100);
        strncat (msg, " nao encontrado!\n", 100 - strlen (msg));
        sair (FIM_ERRO_ARQUIVO, msg);
    }

    erro = analisadorSintatico (dados);
        
    sair (erro, "");
    return erro;
}
