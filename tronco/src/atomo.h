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
typedef struct s_atomo      *um_atomo;
typedef struct s_reservado  um_reservado;
typedef enum   e_classe     uma_classe;

/* e_classe
 *
 * Todas as classes possíveis (incluindo símbolos e palavras reservadas)
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
/* 13 */ S_MULTIPLICACAO,
/* 14 */ S_DIVISAO,
/* 15 */ S_POTENCIACAO,
/* 16 */ S_ADICAO,
/* 17 */ S_SUBTRACAO,
/* 18 */ S_MENOR,
/* 19 */ S_MENOR_OU_IGUAL,
/* 20 */ S_MAIOR,
/* 21 */ S_MAIOR_OU_IGUAL,
/* 22 */ S_IGUAL,
/* 23 */ S_DIFERENTE,
/* 24 */ S_ATRIBUICAO,

// Palavras Reservadas
/* 25 */ PR_PROGRAM,
/* 26 */ PR_DECLARE,
/* 27 */ PR_BEGIN,
/* 28 */ PR_END,
/* 29 */ PR_WHILE,
/* 30 */ PR_LOOP,
/* 31 */ PR_END_LOOP,
/* 32 */ PR_IF,
/* 33 */ PR_END_IF,
/* 34 */ PR_THEN,
/* 35 */ PR_ELSE,
/* 36 */ PR_INPUT,
/* 37 */ PR_OUTPUT,
/* 38 */ PR_PROCEDURE,
/* 39 */ PR_FUNCTION,
/* 40 */ PR_RETURNS,
/* 41 */ PR_REAL,
/* 42 */ PR_INTEGER,
/* 43 */ PR_BOOLEAN,
/* 44 */ PR_TRUE,
/* 45 */ PR_FALSE,

/* 46 */ TOTAL_CLASSES
};

/* s_fila
 * 
 * Estrutura que representa uma fila de átomos
 */
struct s_fila {
    int tamanho;
    um_atomo *atomo;
};

/* s_atomo
 *
 * Estrutura que representa um átomo
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
 * Uma estrutura representando um Átomo reservado
 * (palavra reservada ou símbolo)
 */
struct s_reservado {
    uma_classe classe; 
    char *nome;
};

/*
 * Protótipos de funções
 */
char * nomeClasse (uma_classe);
um_atomo novoAtomo (uma_classe);
um_atomo novoAtomoInteiro (uma_classe, int);
um_atomo novoAtomoReal (uma_classe, double);
uma_classe busca_simbolo (char *);
uma_classe busca_palavra_reservada (char *);
// char * busca_nome_da_classe (uma_classe);

// Funções com fila (para look-ahead)
void fila_inicia        (uma_fila *);
int fila_eh_vazia       (uma_fila *);
int fila_adiciona       (uma_fila *, um_atomo);
um_atomo fila_retira    (uma_fila *);

#endif // __ATOMO_H__
