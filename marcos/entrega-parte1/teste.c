#include <stdio.h>
#include <stdlib.h>
#include "defs.h"
#include "teste.h"
#include "atomo.h"
#include "tabid.h"
#include "analisadorLexico.h"


int main (int argc, char **argv)
{
    um_atomo at;
    char *dados, *pos, *nome;
    int i, ids, tamanho, lido;
    FILE *arq;

    if (argc == 1)
    {
        printf ("Uso: teste <arquivo>\n");
        return 1;
    }

    arq = fopen (argv[1], "r");

    if (arq == NULL)
    {
        printf ("Arquivo nao encontrado!\n");
        return 2;
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
    
    pos = dados;                // Posição inicial de leitura
    at = novoAtomo (INVALIDO);  // Átomo inicial apenas para entrar no while
    while (at->classe != FIM)
    {
        // Limpa átomo anterior
        free (at);


        // Lê novo átomo
        at = analisadorLexico (&pos);
        if (at != NULL)
        {
            nome = nomeClasse (at->classe);
            printf ("%20s %5d", nome,  at->valor);
            free (nome);
            if (at->classe == IDENTIFICADOR)
                printf (" (%s)", busca_nome_ID (at->valor));

            printf ("\n");
        }
    }
    free (at);

    ids = tam_tabelaID();
        
    printf ("\nIdentificadores (%d):\n", ids);
    for (i = 0; i < ids; i++)
    {
        printf ("%3d: %s\n", i,  busca_nome_ID (i));
    }
    
    printf ("\nPressione algo para continuar...");
    getch();
    
    return 1;
}
