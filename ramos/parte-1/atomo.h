
#ifndef __ATOMO_H__

#define __ATOMO_H__

/*
 * Tipos
 */
typedef struct s_atomo     *um_atomo;
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

    // S�mbolos
    S_VIRGULA,
    S_PONTO_E_VIRGULA,
    S_ABRE_CHAVE,    
    S_FECHA_CHAVE,  
    S_ADICAO,      
    S_SUBTRACAO,  
    S_MULTIPLICACAO,
    S_DIVISAO,     
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
    PR_OUTPUT
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
 * Uma estrutura representando um átomo reservado
 * (palavra reservada ou símbolo)
 */
struct s_reservado {
    uma_classe classe; 
    char *nome;
};
/*
 * Prot�tipos de fun��es
 */
char * nomeClasse (uma_classe c);
um_atomo novoAtomo (uma_classe c);
uma_classe busca_simbolo (char * nome);
uma_classe busca_palavra_reservada (char * nome);
char * busca_nome_classe (uma_classe c);


#endif // __ATOMO_H__
