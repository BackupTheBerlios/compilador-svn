
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "atomo.h"

/* SIMBOLOS
 *
 * Tabela "hardcoded" de s�mbolos
 */
#define TOTAL_SIMBOLOS    15
static struct s_reservado SIMBOLOS [TOTAL_SIMBOLOS] = {
    // S�mmbolos
    { S_VIRGULA,            ","     },
    { S_PONTO_E_VIRGULA,    ";"     },
    { S_ABRE_CHAVE,         "["     },
    { S_FECHA_CHAVE,        "]"     },
    { S_ADICAO,             "+"     },
    { S_SUBTRACAO,          "-"     },
    { S_MULTIPLICACAO,      "*"     },
    { S_DIVISAO,            "/"     },
    { S_IGUAL,              "="     },
    { S_MAIOR,              ">"     },
    { S_MENOR,              "<"     },
    { S_MAIOR_OU_IGUAL,     ">="    },
    { S_MENOR_OU_IGUAL,     "<="    },
    { S_DIFERENTE,          "!="    },
    { S_ATRIBUICAO,         ":="    }
};

/* RESERVADOS
 *
 * Tabela "hardcoded" de palavras reservadas
 */
#define TOTAL_PALAVRAS_RESERVADAS    11
static struct s_reservado PALAVRAS_RESERVADAS [TOTAL_PALAVRAS_RESERVADAS] = {
    // Palavras Reservadas
    { PR_PROGRAM,       "program"   },
    { PR_BEGIN,         "begin"     },
    { PR_END,           "end"       },
    { PR_DECLARE,       "declare"   },
    { PR_WHILE,         "while"     },
    { PR_LOOP,          "loop"      },
    { PR_IF,            "if"        },
    { PR_THEN,          "then"      },
    { PR_ELSE,          "else"      },
    { PR_INPUT,         "input"     },
    { PR_OUTPUT,        "output"    }
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
            strcpy (nome, busca_nome_classe (c));
            if (nome == NULL)
                strcpy (nome, "DESCONHECIDO");
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
    {
        a->classe = c;
        a->valor  = 0;
    }

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
char * busca_nome_classe (uma_classe c)
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

    return NULL;
}
