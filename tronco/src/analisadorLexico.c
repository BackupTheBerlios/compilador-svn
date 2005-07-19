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
#include "atomo.h"
#include "tabid.h"
#include "analisadorLexico.h"

enum estados_lexico
{
	BRANCO,
	IDENTIFICADOR,
	NUMERO,
	SIMBOLO,
	COMENTARIO
};

static enum estados_lexico estado_lexico = BRANCO; // Estado Inicial

static int linha = 1;
static int coluna = 0;

// Protótipo das funções utilizadas nesse arquivo
um_atomo maquina_lexico (char **);
um_atomo estado_branco (char **);
um_atomo estado_identificador (char **);
um_atomo estado_numero (char **);
um_atomo estado_simbolo (char **);
um_atomo estado_comentario (char **);
void consome_entrada(char **, int);
int ehLetra (char);
int ehDigito (char);
int ehSimbolo (char);
int ehFimDeLinha (char);
int ehBranco (char);

// As funções

um_atomo analisadorLexico(char **entrada, uma_pilha *pilha)
{
    um_atomo a;

    // Se houver um átomo na pilha, retorna esse átomo
    if (! pilha_vazia (pilha))
        return pilha_retira (pilha);

    // Roda a máquina de estados até pegar um átomo
    do
        a = maquina_lexico(entrada);
    while (!a);

    return a;
}


um_atomo maquina_lexico (char **entrada)
{
    um_atomo a;
    
	switch (estado_lexico)
	{
		case BRANCO:
            a = estado_branco (entrada);
			break;
		
		case IDENTIFICADOR:
            a = estado_identificador (entrada);
			break;
		
		case NUMERO:
            a = estado_numero (entrada);
			break;
		
		case SIMBOLO:
            a = estado_simbolo (entrada);
			break;
		
		case COMENTARIO:
            a = estado_comentario (entrada);
			break;
		
		default:
			a = novoAtomoInteiro (C_INVALIDA, linha);
			break;
	}
	return a;
}


/* As funções estado_xxx retornam um átomo */

um_atomo estado_branco (char **entrada)
{
    um_atomo a = NULL;
    
    // Excetuando o caso de brancos, as entradas só serão consumidas dentro
    // de seus respectivos estados
    if (ehBranco(**entrada))
        consome_entrada(entrada, 1);
    
    else if (ehLetra (**entrada) || **entrada == '_')
        estado_lexico = IDENTIFICADOR;
    
    else if (ehDigito (**entrada) || **entrada == '.')
        estado_lexico = NUMERO;
    
    else if (ehSimbolo (**entrada))
        estado_lexico = SIMBOLO;
    
    else if (**entrada == '%')
        estado_lexico = COMENTARIO;
    
    else if (**entrada == '\0')
        a = novoAtomoInteiro (C_FIM, linha);
    
    else
        a = novoAtomoInteiro (C_INVALIDA, linha);
    
    return a;
}

um_atomo estado_identificador (char **entrada) 
{
    um_atomo a = NULL;
    static char * nome;
    static int tamanho=0, max=0;

    // Verifica se acabou de entrar nesse estado
    if (tamanho == 0)
        nome = malloc (sizeof (char));

    if (ehDigito (**entrada) || ehLetra (**entrada) || **entrada == '_')
    {
        // Realoca mais 10 bytes ao nome, caso este estoure
        if (++tamanho > max)
        {
            max += 10;
            nome = realloc (nome, (max+1) * sizeof (char));
        }
        
        nome [tamanho-1] = **entrada;
        consome_entrada (entrada, 1);
    }
    else // Transição vazia
    {
        uma_classe c;
        
        estado_lexico = BRANCO;    
        nome [tamanho] = '\0';
        tamanho = 0;
        
        // Procura nome na tabela de palavras reservadas
        c = busca_palavra_reservada (nome);
        if (c != C_INVALIDA)
        {
            free (nome);
            a = novoAtomoInteiro (c, 0);
        }
        else // Não é palavra reservada
        {
            int cod;
            
            cod = busca_cod_ID (nome);
            if (cod == ERRO)
                cod = adicionaID (nome);
            a = novoAtomoInteiro (C_IDENTIFICADOR, cod);
        }
    }
    return a;
}


um_atomo estado_simbolo (char **entrada) 
{
    um_atomo a = NULL;
    uma_classe c;
    static uma_classe classe_valida;
    static char *nome, *simbolo_valido;
    static int pos=0, max=0;

    // Verifica se acabou de entrar nesse estado
    if (pos == 0)
    {
        nome = malloc (sizeof (char));
        simbolo_valido = NULL;
    }
    
    if (ehSimbolo ((*entrada)[pos]))
    {
        // Realoca mais 5 bytes ao símbolo, caso este estoure
        if (++pos > max)
        {
            max += 5;
            nome = realloc (nome, (max+1) * sizeof (char));
        }
        
        nome [pos-1] = (*entrada)[pos-1];
        nome [pos] = '\0';
        
        // Se o nome atual já for um símbolo, considera como válido
        c = busca_simbolo(nome);             
        if (c != C_INVALIDA)
        {
            // Se já existia um símbolo válido, detona ele
            if (simbolo_valido)
                free (simbolo_valido);
            
            simbolo_valido = malloc ((pos+1) * sizeof (char));
            strncpy (simbolo_valido, nome, pos+1);
            classe_valida = c;
        }
    }
    else
    {
        // Transição vazia
        estado_lexico = BRANCO;    
        
        free (nome);
        pos = 0;
    
        // Retorna o maior símbolo encontrado, ou C_INVALIDA
        if (simbolo_valido)
        {
            // Antes, consome entrada e libera memória
            consome_entrada (entrada, strlen (simbolo_valido));
            free (simbolo_valido);
            
            a = novoAtomoInteiro (classe_valida, 0);
        }
        else
            a = novoAtomoInteiro (C_INVALIDA, linha);
    }
    return a;
}

um_atomo estado_numero (char **entrada) 
{
    um_atomo a = NULL;
    static int inteiro = 0;
    static double real = 0.0;
    static double decimal = 1.0;
	static enum { INTEIRO, REAL } tipo = INTEIRO;
    
    if (ehDigito (**entrada))
    {
		if (tipo == INTEIRO)
	        inteiro = (10 * inteiro) + (**entrada - '0');
		else
			real += (decimal /= 10) * (**entrada - '0');
		
        consome_entrada(entrada, 1);
    }
    else if ((**entrada == '.') && (tipo == INTEIRO))
	{
		tipo = REAL;
		real = inteiro;
        consome_entrada(entrada, 1);
	}
    else // Transição vazia
    {
        estado_lexico = BRANCO;    
        
        if (tipo == INTEIRO)		
            a = novoAtomoInteiro (C_INTEIRO, inteiro);    
        else
            a = novoAtomoReal (C_REAL, real);    
        
        // Zera variáveis
        inteiro = 0;
        decimal = 1.0;
        tipo = INTEIRO;
    }
    return a;
}

um_atomo estado_comentario (char **entrada)
{
    if (ehFimDeLinha (**entrada))
        estado_lexico = BRANCO;
    else
        consome_entrada(entrada, 1);
    
    return NULL;
}


void consome_entrada(char **entrada, int total)
{
    static int mudanca=0;
    
    while (total)
    {        
        switch (**entrada)
        {
            case '\n':  // avanço de linha
            case '\r':  // retorno de carro
                if (!mudanca)
                {
                    linha++;
                    coluna = 0;
                    mudanca = VERDADE;
                }
                break;
    
            case '\t':  // tabulação
//                coluna += 3;
            default:
                coluna ++;        
                mudanca = FALSO;
                break;
        }
        
        (*entrada)++;
        total--;
    }
}

int linha_atual ()
{
    return linha;
}

int coluna_atual ()
{
    return coluna;
}


/* ehLetra
 *
 * Retorna verdadeiro se o caracter de entrada for a..z A..Z _
 */
int ehLetra (char e)
{
    return (e >= 'a' && e <= 'z') || (e >= 'A' && e <= 'Z') || (e == '_');
}

/* ehDigito
 *
 * Retorna verdadeiro se o caracter de entrada for 0..9
 */
int ehDigito (char e)
{
    return (e >= '0') && (e <= '9');
}

/* ehSimbolo
 *
 * Retorna verdadeiro se o caracter de entrada for um símbolo
 */
int ehSimbolo (char e)
{
    switch (e)
    {
//        case '!':
//        case '"':
//        case '#':
//        case '$':
//        case '%':
//        case '&':
//        case '\'':
        case '(':
        case ')':
        case '*':
        case '+':
        case ',':
        case '-':
        case '.':
        case '/':
        case ':':
        case ';':
        case '<':
        case '=':
        case '>':
//        case '?':
        case '[':
//        case '\\':
        case ']':
        case '^':
//        case '`':
//        case '{':
//        case '|':
//        case '}':
//        case '~':
            return VERDADE;
    }
    return FALSO;
}


/* ehFimDeLinha
 *
 * Retorna verdadeiro se o caracter de entrada for um fim-de-linha
 */
int ehFimDeLinha (char e)
{
    switch (e)
    {
        case '\n':  // avanço de linha
        case '\r':  // retorno de carro
            return VERDADE;
    }
    return FALSO;
}

/* ehBranco
 *
 * Retorna verdadeiro se o caracter de entrada for um espaco em branco
 */
int ehBranco (char e)
{
    switch (e)
    {
        case '\n':  // avanço de linha
        case '\r':  // retorno de carro
        case '\t':  // tabulação
        case ' ':   // espaço
            return VERDADE;
    }
    return FALSO;
}
