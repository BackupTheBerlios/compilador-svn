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
#include "atomo.h"
#include "defs.h"


/* SIMBOLOS
 *
 * Tabela "hardcoded" de símbolos
 */
#define TOTAL_SIMBOLOS    20
static um_reservado SIMBOLOS [TOTAL_SIMBOLOS] = {
    // Diversos
    { S_ABRE_PARENTESES,    "("     },
    { S_FECHA_PARENTESES,   ")"     },
    { S_VIRGULA,            ","     },
    { S_ABRE_COLCHETE,      "["     },
    { S_FECHA_COLCHETE,     "]"     },
    { S_ABRE_CHAVE,         "{"     },
    { S_FECHA_CHAVE,        "}"     },
    { S_PONTO_E_VIRGULA,    ";"     },
    { S_ATRIBUICAO,         ":="    },
    
    // Operadores
    { S_ADICAO,             "+"     },
    { S_SUBTRACAO,          "-"     },
    { S_MULTIPLICACAO,      "*"     },
    { S_DIVISAO,            "/"     },
    { S_POTENCIACAO,        "^"     },
    
    // Comparadores
    { S_MENOR,              "<"     },
    { S_MENOR_OU_IGUAL,     "<="    },
    { S_MAIOR,              ">"     },
    { S_MAIOR_OU_IGUAL,     ">="    },
    { S_IGUAL,              "="     },
    { S_DIFERENTE,          "!="    }
};

/* RESERVADOS
 *
 * Tabela "hardcoded" de palavras reservadas
 */
#define TOTAL_PALAVRAS_RESERVADAS    23
static um_reservado PALAVRAS_RESERVADAS [TOTAL_PALAVRAS_RESERVADAS] = {
    // (Sub)programa
    { PR_PROGRAM,       "program"   },
    { PR_PROCEDURE,     "procedure" },
    { PR_FUNCTION,      "function"  },
    { PR_RETURNS,       "returns"   },
    
    // Tipos
    { PR_REAL,          "real"      },
    { PR_INTEGER,       "integer"   },
    { PR_BOOLEAN,       "boolean"   },
    
    // Bloco
    { PR_DECLARE,       "declare"   },
    { PR_BEGIN,         "begin"     },
    { PR_END,           "end"       },
    
    // Comandos
    { PR_WHILE,         "while"     },
    { PR_LOOP,          "loop"      },
    { PR_IF,            "if"        },
    { PR_THEN,          "then"      },
    { PR_ELSE,          "else"      },
    { PR_INPUT,         "input"     },
    { PR_OUTPUT,        "output"    },
    
    // Booleanos
    { PR_TRUE,          "true"      },
    { PR_FALSE,         "false"     },
    { PR_NOT,           "not"       },
    { PR_AND,           "and"       },
    { PR_OR,            "or"        },
    { PR_XOR,           "xor"       }
};

/* busca_nome_nas_tabelas
 *
 * Busca pelo nome de uma classe nas tabelas
 */
char * busca_nome_nas_tabelas (uma_classe c)
{
    int i;

    // Tabela de símbolos
    for (i=0; i < TOTAL_SIMBOLOS; i++)
    {
        if (c == SIMBOLOS[i].classe)
            return SIMBOLOS[i].nome;
    }

    // Tabela de palavras reservadas
    for (i=0; i < TOTAL_PALAVRAS_RESERVADAS; i++)
    {
        if (c == PALAVRAS_RESERVADAS[i].classe)
            return PALAVRAS_RESERVADAS[i].nome;
    }

    return (char *) NULL;
}

/* nomeClasse
 *
 */
char * nomeClasse (uma_classe c)
{
    char *nome;

    switch (c)
    {
        case C_INVALIDA:
            nome = "INVALIDO";
            break;

        case C_FIM:
            nome = "FIM";
            break;

        case C_IDENTIFICADOR:
            nome = "IDENTIFICADOR";
            break;

        case C_INTEIRO:
            nome = "INTEIRO";
            break;

        case C_REAL:
            nome = "REAL";
            break;

        default:
            nome = busca_nome_nas_tabelas (c);
            if (nome == NULL)
                nome = "DESCONHECIDO";
    }
    return nome;
}


/* novoAtomo
 * 
 * Aloca memória para um novo atomo
 */
um_atomo novoAtomo (uma_classe c)
{
    um_atomo a;

    a = (um_atomo) malloc (sizeof (struct s_atomo));

    if (a != NULL)
        a->classe = c;

    return a;
}

/* novoAtomoInteiro
 * 
 * Aloca memória para um novo atomo com parâmetro inteiro
 */
um_atomo novoAtomoInteiro (uma_classe c, int v)
{
    um_atomo a = novoAtomo (c);
	
    if (a != NULL)
        a->valor  = v;

    return a;
}

/* novoAtomoReal
 * 
 * Aloca memória para um novo atomo com parâmetro real
 */
um_atomo novoAtomoReal (uma_classe c, double v)
{
    um_atomo a = novoAtomo (c);
	
    if (a != NULL)
        a->real  = v;

    return a;

}

/* busca_palavra_reservada
 *
 * Busca por uma palavra reservada na tabela
 */
uma_classe busca_palavra_reservada (char * nome)
{
    int i;

    for (i=0; i < TOTAL_PALAVRAS_RESERVADAS; i++)
    {
        if (strcmp (nome, PALAVRAS_RESERVADAS[i].nome) == 0)
            return PALAVRAS_RESERVADAS[i].classe;
    }

    return C_INVALIDA;
}

/* busca_simbolo
 *
 * Busca por um simbolo na tabela
 */
uma_classe busca_simbolo (char * nome)
{
    int i;

    for (i=0; i < TOTAL_SIMBOLOS; i++)
    {
        if (strcmp (nome, SIMBOLOS[i].nome) == 0)
            return SIMBOLOS[i].classe;
    }

    return C_INVALIDA;
}


//
// Funções de manipulação da fila
//

void fila_inicia  (uma_fila *fila)
{
    fila->atomo = (um_atomo *) NULL;
    fila->tamanho = 0;
}

int fila_eh_vazia (uma_fila *fila)
{
    return fila->tamanho == 0;
}

int fila_adiciona (uma_fila *fila, um_atomo atomo)
{
    int ultimo;
    
    if (fila->atomo == NULL)
    {
        fila->atomo = (um_atomo *) malloc (sizeof (um_atomo));
        fila->tamanho = 1;
    }
    else
        fila->atomo = (um_atomo *) realloc (fila->atomo, (++fila->tamanho) * sizeof (um_atomo));

    if (fila->atomo == NULL)
    {
        fila->tamanho = 0;
        ultimo = ERRO;
    }
    else
    {
        ultimo = fila->tamanho - 1;
        fila->atomo [ultimo] = atomo;
    }
    
    return ultimo;
}

um_atomo fila_retira (uma_fila *fila)
{
    um_atomo atomo;
    int i;
    
    if (fila_eh_vazia (fila))
        return (um_atomo) NULL;

    atomo = fila->atomo[0];
    
    for (i=0; i < fila->tamanho-1; i++)
        fila->atomo[i] = fila->atomo[i+1];
    
    fila->atomo = (um_atomo *) realloc (fila->atomo, (--fila->tamanho) * sizeof (um_atomo));
    
    return atomo;
}
