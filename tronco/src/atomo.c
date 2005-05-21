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
 * Tabela "hardcoded" de s�mbolos
 */
#define TOTAL_SIMBOLOS    18
static um_reservado SIMBOLOS [TOTAL_SIMBOLOS] = {
    // Diversos
    { S_ABRE_PARENTESES,    "("     },
    { S_FECHA_PARENTESES,   ")"     },
    { S_VIRGULA,            ","     },
    { S_ABRE_CHAVE,         "["     },
    { S_FECHA_CHAVE,        "]"     },
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
    { PR_TRUE,          "true"       },
    { PR_FALSE,         "false"       },
    { PR_NOT,           "not"       },
    { PR_AND,           "and"       },
    { PR_OR,            "or"        },
    { PR_XOR,           "xor"       }
};


/* nomeClasse
 *
 */
char * nomeClasse (uma_classe c)
{
    char *nome;

    nome = malloc (50 * sizeof (char));
    
    switch (c)
    {
        case INVALIDO:
            strcpy (nome, "INVALIDO");
            break;

        case FIM:
            strcpy (nome, "FIM");
            break;

        case IDENTIFICADOR:
            strcpy (nome, "IDENTIFICADOR");
            break;

        case INTEIRO:
            strcpy (nome, "INTEIRO");
            break;

        default:
            strcpy (nome, busca_nome_da_classe (c));
            if (nome == NULL)
                strcpy (nome, "DESCONHECIDO");
    }
    return nome;
}


/* novoAtomo
 * 
 * Aloca mem�ria para um novo atomo
 */
um_atomo novoAtomo (uma_classe c, int v)
{
    um_atomo a;

    a = (um_atomo) malloc (sizeof (struct s_atomo));

    if (a != NULL)
    {
        a->classe = c;
        a->valor  = v;
    }

    return a;
}

/* removeAtomo
 * 
 * Desaloca mem�ria de um atomo
 */
void removeAtomo (um_atomo a)
{
    free (a);
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

    return INVALIDO;
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

    return INVALIDO;
}

/* busca_nome_classe
 *
 * Busca pelo nome de uma classe nas tabelas
 */
char * busca_nome_da_classe (uma_classe c)
{
    int i;

    // Tabela de sÃ­mbolos
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

    return NULL;
}

// Fun��es com pilha

int pilha_vazia (uma_pilha pilha)
{
    return pilha.tamanho == 0;
}

int pilha_adiciona (uma_pilha pilha, um_atomo atomo)
{
    int ultimo;
    
    if (pilha.atomo == NULL)
    {
        pilha.atomo = (um_atomo *) malloc (sizeof (um_atomo));
        pilha.tamanho = 1;
    }
    else
        pilha.atomo = (um_atomo *) realloc (pilha.atomo, (++pilha.tamanho) * sizeof (um_atomo));

    if (pilha.atomo == NULL)
    {
        pilha.tamanho = 0;
        ultimo = ERRO;
    }
    else
    {
        ultimo = pilha.tamanho - 1;
        pilha.atomo [ultimo] = atomo;
    }
    
    return ultimo;
}

um_atomo pilha_remove (uma_pilha pilha)
{
    um_atomo atomo;
    int ultimo;
    
    if (pilha_vazia (pilha))
        return (um_atomo) NULL;

    ultimo = pilha.tamanho - 1;    
    atomo = pilha.atomo[ultimo];
    
    pilha.atomo = (um_atomo *) realloc (pilha.atomo, (--pilha.tamanho) * sizeof (um_atomo));
        
    return atomo;
}
