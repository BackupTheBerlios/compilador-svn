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
#include <strings.h>
#include "defs.h"
#include "compilador.h"
#include "atomo.h"
#include "tabid.h"
#include "analisadorLexico.h"

#define ESPERA_TECLA

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
    char *dados, *pos;
    int i, ids, tamanho, lido;
    FILE *arq;

    if (argc == 1)
        sair (1, "Uso: teste <arquivo>\n");

    arq = fopen (argv[1], "r");

    if (arq == NULL)
	{
		char msg[50]="";
		strcat (msg, argv[1]);
		strcat (msg, " nao encontrado!\n");
        sair (2, msg);
	}

    dados = malloc (MAX_BLOCO * sizeof (char));
    tamanho = 0;

    while (! feof (arq))
    {
        lido = fread (dados, sizeof (char), MAX_BLOCO, arq);
        tamanho += lido;
        if (lido == MAX_BLOCO)
            dados = realloc (dados, (tamanho + MAX_BLOCO) * sizeof (char));
    }
    
    dados [tamanho] = '\0';
    
    fclose (arq);

    printf ("Atomos:\n");
    printf ("%20s %5s\n", "Classe", "Valor");
    printf ("%20s %5s\n", "------", "-----");
    
    pos = dados;                   // Posição inicial de leitura
    pilha_inicia (&pilha);
    at = NULL;
    do
    {
        // Limpa átomo anterior
        if (at)
            free (at);

        // Lê novo átomo
        at = analisadorLexico (&pos, &pilha);
        
        printf ("%20s %5d", nomeClasse (at->classe),  at->valor);

        if (at->classe == C_IDENTIFICADOR)
            printf (" (%s)", busca_nome_ID (at->valor));

        printf ("\n");
    }
    while ((at->classe != C_FIM) && (at->classe != C_INVALIDA));
    
    if (at->classe == C_INVALIDA)
    {
        printf ("\nToken invalido na linha %d, coluna %d\n", linha_atual(), coluna_atual());
    }

    free (at);

    ids = tam_tabelaID();
        
    printf ("\nIdentificadores (%d):\n", ids);
    for (i = 0; i < ids; i++)
    {
        printf ("%3d: %s\n", i,  busca_nome_ID (i));
    }
    
    sair (0, "\nFim.");
    
    return 0;
}
