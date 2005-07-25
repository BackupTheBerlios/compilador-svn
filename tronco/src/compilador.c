/***************************************************************************
 *            compilador.c
 *
 *  Fri Jul 22 12:12:09 2005
 *  Copyright  2005  Andr√© Pinto e Igor Arouca
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
<<<<<<< .trabalho
    printf("%s\n", msg);
=======
    char *msg[] = {
        "Execu√ß√£o bem sucedida!", 

        "Uso: teste <arquivo>",
        "Arquivo nao encontrado!", 

        "Token desconhecido!", 
        "Erro de sintaxe.", 
        "Erro interno na constru√ß√£o das m√°quinas!"
    };
        
    printf ("%s\n", msg[ret]);

>>>>>>> .mesclagem-direita.r67
#ifdef ESPERA_TECLA
<<<<<<< .trabalho
    printf("Pressione algo para continuar...\n");
=======
    printf ("\nPressione algo para continuar...\n");
>>>>>>> .mesclagem-direita.r67
    fflush(stdout);
    getchar();
#endif
<<<<<<< .trabalho
    exit(ret);
=======

    exit (ret);
>>>>>>> .mesclagem-direita.r67
}

int main (int argc, char **argv)
{
    char *dados, *pos;
    int erro;

    if (argc == 1)
<<<<<<< .trabalho
        sair(1, "Uso: teste <arquivo>\n");
=======
        sair (FIM_ERRO_PARAMETRO);
>>>>>>> .mesclagem-direita.r67

    dados = le_arquivo (argv[1]);
    
    if (dados == NULL)
<<<<<<< .trabalho
	{
		char msg[100]="";
		strcat(msg, argv[1]);
		strcat(msg, " nao encontrado!\n");
        sair(2, msg);
	}
    
    printf("Atomos:\n");
    printf("%20s %5s\n", "Classe", "Valor");
    printf("%20s %5s\n", "------", "-----");
    
    pos = dados;                   // PosiÁ„o inicial de leitura
    fila_inicia(&fila);
    fim = 0;
    i = 0;

    do {
        if (i >= 3 && i <= 5)
        {
            // LÍ novo ·tomo com look-ahead
            at = analisadorLexico(&pos, VERDADE, &fila);
        
            fila_adiciona(&fila, at);
            printf("O atomo '%s' (%d) foi pra fila!\n", nomeClasse (at->classe),  at->valor);
        } else {
            // LÍ novo ·tomo sem look-ahead
            at = analisadorLexico(&pos, FALSO, &fila);
=======
        sair (FIM_ERRO_ARQUIVO);
>>>>>>> .mesclagem-direita.r67

<<<<<<< .trabalho
            printf ("%20s ", nomeClasse (at->classe));
            if (at->classe == C_REAL)
                printf("%5.10g", at->real);
            else
                printf("%5d", at->valor);
    
            if (at->classe == C_IDENTIFICADOR)
                printf(" (%s)", busca_nome_ID (at->valor));
    
            printf("\n");
            
            if (at->classe == C_FIM)
                fim = 1;
            if (at->classe == C_INVALIDA)
                fim = 2;
=======
    pos = dados;
    erro = analisadorSintatico (&pos);
>>>>>>> .mesclagem-direita.r67

<<<<<<< .trabalho
            // Limpa ·tomo
            free(at);
        }
        i++;
    } while (!fim);
    
    // Mostra linha do erro, quando ocorrer
    if (fim == 2)
=======
    if (erro >= FIM_ERRO_LEXICO)
>>>>>>> .mesclagem-direita.r67
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
