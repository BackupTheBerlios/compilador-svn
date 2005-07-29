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
 

#ifndef __ATOMO_H__
#define __ATOMO_H__

/*
 * Tipos
 */
typedef struct s_fila       uma_fila;
typedef struct s_atomo*		um_atomo;
typedef struct s_reservado  um_reservado;
typedef enum   e_classe     uma_classe;

/* e_classe
 *
 * Todas as classes possÃ­veis (incluindo sÃ­mbolos e palavras reservadas)
 */
enum e_classe {

/* 0  */ C_INVALIDA,
/* 1  */ C_FIM,
/* 2  */ C_IDENTIFICADOR,
/* 3  */ C_INTEIRO,
/* 4  */ C_REAL,

// Símbolos
/* 5  */ S_VIRGULA,
/* 6  */ S_PONTO_E_VIRGULA,
/* 7  */ S_ABRE_PARENTESES,
/* 8  */ S_FECHA_PARENTESES,
/* 9  */ S_ABRE_COLCHETE,
/* 10 */ S_FECHA_COLCHETE,
/* 11 */ S_ABRE_CHAVE,
/* 12 */ S_FECHA_CHAVE,
/* 13 */ S_ADICAO,
/* 14 */ S_SUBTRACAO,
/* 15 */ S_MULTIPLICACAO,
/* 16 */ S_DIVISAO,
/* 17 */ S_POTENCIACAO,
/* 18 */ S_IGUAL,
/* 19 */ S_MAIOR,
/* 20 */ S_MENOR,
/* 21 */ S_MAIOR_OU_IGUAL,
/* 22 */ S_MENOR_OU_IGUAL,
/* 23 */ S_DIFERENTE,
/* 24 */ S_ATRIBUICAO,

// Palavras Reservadas
/* 25 */ PR_PROGRAM,
/* 26 */ PR_DECLARE,
/* 27 */ PR_BEGIN,
/* 28 */ PR_END,
/* 29 */ PR_WHILE,
/* 30 */ PR_LOOP,
/* 31 */ PR_IF,
/* 32 */ PR_THEN,
/* 33 */ PR_ELSE,
/* 34 */ PR_INPUT,
/* 35 */ PR_OUTPUT,
/* 36 */ PR_PROCEDURE,
/* 37 */ PR_FUNCTION,
/* 38 */ PR_RETURNS,
/* 39 */ PR_REAL,
/* 40 */ PR_INTEGER,
/* 41 */ PR_BOOLEAN,
/* 42 */ PR_NOT,
/* 43 */ PR_AND,
/* 44 */ PR_OR,
/* 45 */ PR_XOR,
/* 46 */ PR_TRUE,
/* 47 */ PR_FALSE,

/* 48 */ TOTAL_CLASSES
};

/* s_fila
 * 
 * Estrutura que representa uma fila de Ã¡tomos
 */
struct s_fila {
    int tamanho;
    um_atomo *atomo;
};

/* s_atomo
 *
 * Estrutura que representa um Ã¡tomo
 */
struct s_atomo {
    uma_classe classe;
    union {
        int valor;
        double real;
    };
};

/* s_reservado
 *
 * Uma estrutura representando um Ãtomo reservado
 * (palavra reservada ou sÃ­mbolo)
 */
struct s_reservado {
    uma_classe classe; 
    char *nome;
};

/*
 * ProtÃ³tipos de funÃ§Ãµes
 */
char * nomeClasse (uma_classe);
um_atomo novoAtomo (uma_classe);
um_atomo novoAtomoInteiro (uma_classe, int);
um_atomo novoAtomoReal (uma_classe, double);
uma_classe busca_simbolo (char *);
uma_classe busca_palavra_reservada (char *);
// char * busca_nome_da_classe (uma_classe);

// FunÃ§Ãµes com fila (para look-ahead)
void fila_inicia        (uma_fila *);
int fila_eh_vazia       (uma_fila *);
int fila_adiciona       (uma_fila *, um_atomo);
um_atomo fila_retira    (uma_fila *);

#endif // __ATOMO_H__
