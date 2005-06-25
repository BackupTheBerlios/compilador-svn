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
#include "atomo.h"
#include "tabid.h"
#include "analisadorLexico.h"

// #define ESPERA_TECLA

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
    um_atomo at;
    uma_pilha pilha;
    char *pos, *dados;
    int i, ids, fim;

    if (argc == 1)
        sair (1, "Uso: teste <arquivo>\n");

    dados = le_arquivo (argv[1]);
    
    if (dados == NULL)
	{
		char msg[100]="";
		strcat (msg, argv[1]);
		strcat (msg, " nao encontrado!\n");
        sair (2, msg);
	}
    
    printf ("Atomos:\n");
    printf ("%20s %5s\n", "Classe", "Valor");
    printf ("%20s %5s\n", "------", "-----");
    
    pos = dados;                   // Posição inicial de leitura
    pilha_inicia (&pilha);
    fim = 0;
    i = 0;
    do
    {
        // Lê novo átomo
        at = analisadorLexico (&pos, &pilha);
        
        if (i==3)
        {
            pilha_adiciona (&pilha, at);
            printf ("O atomo %s (%d) foi pra pilha!\n", nomeClasse (at->classe),  at->valor);
        }
        else
        {
            printf ("%20s %5d", nomeClasse (at->classe),  at->valor);
    
            if (at->classe == C_IDENTIFICADOR)
                printf (" (%s)", busca_nome_ID (at->valor));
    
            printf ("\n");
            
            if (at->classe == C_FIM)
                fim = 1;
            if (at->classe == C_INVALIDA)
                fim = 2;

            // Limpa átomo
            free (at);
        }
        i++;
    }
    while (! fim);
    
    // Mostra linha do erro, quando ocorrer
    if (fim == 2)
    {
        int col = coluna_atual();
        
        printf ("\nErro na seguinte linha:\n");
        
        for (pos -= col, i=0; pos[i] != '\n'; i++)
            printf ("%c", pos[i]);
        printf ("\n");
        
        for (i=0; i < col; i++)
            if (pos[i] == '\t')
                printf ("--------");
            else
                printf ("-");
        printf ("^\n");
    }

    // Mostra tabela de identificadores
    ids = tam_tabelaID();
    printf ("\nIdentificadores (%d):\n", ids);
    for (i = 0; i < ids; i++)
    {
        printf ("%3d: %s\n", i,  busca_nome_ID (i));
    }
    
    
    
    sair (0, "\nFim.");
    return 0;
}
