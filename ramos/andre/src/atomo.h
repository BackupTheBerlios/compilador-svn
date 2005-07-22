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
typedef struct s_atomo     *um_atomo;
typedef struct s_reservado  um_reservado;
typedef enum   e_classe     uma_classe;

/* e_classe
 *
 * Todas as classes possíveis (incluindo símbolos e palavras reservadas)
 */
enum e_classe {
    C_INVALIDA,
    C_FIM,
    C_IDENTIFICADOR,
    C_INTEIRO,
    C_REAL,
/*    C_BOOLEANO, */

    // Símbolos
    S_VIRGULA,
    S_PONTO_E_VIRGULA,
    S_ABRE_CHAVE,    
    S_FECHA_CHAVE,  
    S_ABRE_PARENTESES,
    S_FECHA_PARENTESES,
    S_ADICAO,      
    S_SUBTRACAO,  
    S_MULTIPLICACAO,
    S_DIVISAO,     
    S_POTENCIACAO,
    S_IGUAL,      
    S_MAIOR,     
    S_MENOR,    
    S_MAIOR_OU_IGUAL,  
    S_MENOR_OU_IGUAL, 
    S_DIFERENTE,     
    S_ATRIBUICAO,   

    // Palavras Reservadas
    PR_PROGRAM,
    PR_BEGIN, 
    PR_END,  
    PR_DECLARE,
    PR_WHILE, 
    PR_LOOP, 
    PR_IF,  
    PR_THEN,
    PR_ELSE,
    PR_INPUT,
    PR_OUTPUT,
    PR_PROCEDURE,
    PR_FUNCTION,
    PR_RETURNS,
    PR_REAL,   
    PR_INTEGER,
    PR_BOOLEAN,
    PR_NOT,
    PR_AND,
    PR_OR, 
    PR_XOR,
    PR_TRUE,
    PR_FALSE,
    
    TOTAL_CLASSES
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
