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
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "arquivo.h"
#include "sintatico.h"
#include "erro.h"

//#define ESPERA_TECLA

void sair (int ret)
{
    char *msg[] = {
        "Execução bem sucedida!", 

        "Uso: teste <arquivo>",
        "Arquivo nao encontrado!", 

        "Token desconhecido!", 
        "Erro de sintaxe.", 
        "Erro interno na construção das máquinas!"
    };
        
    printf ("%s\n", msg[ret]);

#ifdef ESPERA_TECLA
    printf ("\nPressione algo para continuar...\n");
    fflush(stdout);
    getchar();
#endif

    exit (ret);
}

int main (int argc, char **argv)
{
    char *dados, *pos;
    int erro;

    if (argc == 1)
        sair (FIM_ERRO_PARAMETRO);

    dados = le_arquivo (argv[1]);
    
    if (dados == NULL)
        sair (FIM_ERRO_ARQUIVO);

    pos = dados;
    erro = analisadorSintatico (&pos);

    if (erro >= FIM_ERRO_LEXICO)
    {
#ifdef DEBUG        
        printf ("\n");
#endif        
        printf ("Linha Atual:\n");
        mostra_posicao_erro (pos);
        printf ("\n");
    }
        
    sair (erro);
    return erro;
}
