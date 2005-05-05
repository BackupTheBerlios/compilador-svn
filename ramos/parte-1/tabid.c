
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "defs.h"
#include "tabid.h"


// Tabela dinâmica de identificadores
static int total_IDs = 0;
static struct s_id *tabelaID;


int adicionaID (char * nome)
{
    if (tabelaID == NULL)
    {
        tabelaID = malloc (sizeof (struct s_id));
        total_IDs = 1;
    }
    else
        tabelaID = (struct s_id *) realloc (tabelaID, (++total_IDs) * sizeof (struct s_id));

    if (tabelaID != NULL)
    {
        tabelaID [total_IDs - 1].cod = total_IDs - 1;
        tabelaID [total_IDs - 1].nome = malloc ((strlen(nome)+1) * sizeof (char));
        strcpy (tabelaID [total_IDs - 1].nome, nome);
    }
    return total_IDs - 1;
}


int busca_cod_ID (char * nome)
{
    int i;
    for (i = 0; i < total_IDs; i++)
    {
        if (strcmp (tabelaID[i].nome, nome) == 0)
            return tabelaID[i].cod;
    }

    return BUSCA_NAO_ENCONTRADA;
}

int tam_tabelaID ()
{
    return total_IDs;
}

char * busca_nome_ID (int cod)
{
    int i;
    for (i = 0; i < total_IDs; i++)
    {
        if (tabelaID[i].cod == cod)
            return tabelaID[i].nome;
    }

    return NULL;
}
