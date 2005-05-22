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
typedef struct s_pilha     uma_pilha;
typedef struct s_atomo     *um_atomo;
typedef struct s_reservado um_reservado;
typedef enum   e_classe    uma_classe;

/* e_classe
 *
 * Todas as classes possíveis (incluindo símbolos e palavras reservadas)
 */
enum e_classe {
    INVALIDO,
    FIM,
    IDENTIFICADOR,
    INTEIRO,

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
    PR_FALSE
};

struct s_pilha {
    int tamanho;
    um_atomo *atomo;
};

/* s_atomo
 *
 * Estrutura que representa um atomo
 */
struct s_atomo {
    uma_classe classe;
    int valor;
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
char * nomeClasse (uma_classe c);
um_atomo novoAtomo (uma_classe c, int v);
void removeAtomo (um_atomo a);
uma_classe busca_simbolo (char * nome);
uma_classe busca_palavra_reservada (char * nome);
char * busca_nome_da_classe (uma_classe c);

// Funções com pilha
int pilha_tamanho  (uma_pilha pilha);
int pilha_vazia    (uma_pilha pilha);
int pilha_adiciona (uma_pilha pilha, um_atomo atomo);
um_atomo pilha_remove   (uma_pilha pilha);

#endif // __ATOMO_H__
