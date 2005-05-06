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
