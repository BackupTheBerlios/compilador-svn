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
	INTEIRO,
	SIMBOLO,
	COMENTARIO
};

static int linha = 1;
static int coluna = 0;
static enum estados_lexico estado_lexico = BRANCO;
static char **entrada;

// Protótipo das funções utilizadas nesse arquivo
um_atomo maquina_lexico ();
um_atomo estado_branco ();
um_atomo estado_identificador ();
um_atomo estado_inteiro ();
um_atomo estado_simbolo ();
um_atomo estado_comentario ();
void consome_entrada(int total);
int ehLetra (char e);
int ehDigito (char e);
int ehSimbolo (char e);
int ehFimDeLinha (char e);
int ehBranco (char e);

// As funções

um_atomo analisadorLexico(char **ent, uma_pilha *pilha)
{
    um_atomo a;
    
    if (! pilha_vazia (pilha))
        return pilha_retira (pilha);

    entrada = ent;
    estado_lexico = BRANCO;
    
    do
        a = maquina_lexico();
    while (!a);

    return a;
}


um_atomo maquina_lexico ()
{
    um_atomo a;
    
	switch (estado_lexico)
	{
		case BRANCO:
            a = estado_branco ();
			break;
		
		case IDENTIFICADOR:
            a = estado_identificador ();
			break;
		
		case INTEIRO:
            a = estado_inteiro ();
			break;
		
		case SIMBOLO:
            a = estado_simbolo ();
			break;
		
		case COMENTARIO:
            a = estado_comentario ();
			break;
		
		default:
			a = novoAtomo (C_INVALIDA, linha);
			break;
	}
    
	return a;
}


/* As funções estado_xxx retornam um átomo */

um_atomo estado_branco ()
{
    um_atomo a = NULL;
    
    if (ehBranco(**entrada))
        consome_entrada(1);
    else if (ehLetra (**entrada))
        estado_lexico = IDENTIFICADOR;
    else if (ehDigito (**entrada))
        estado_lexico = INTEIRO;
    else if (ehSimbolo (**entrada))
        estado_lexico = SIMBOLO;
    else if (**entrada == '%')
        estado_lexico = COMENTARIO;
    else if (**entrada == '\0')
        a = novoAtomo (C_FIM, linha);
    else
        a = novoAtomo (C_INVALIDA, linha);
    
    return a;
}

um_atomo estado_identificador () 
{
    uma_classe c;
    um_atomo a;
    static char * nome;
    static int tamanho=0, max=0;
    int cod;

    if (tamanho == 0)
        nome = malloc (sizeof (char));

    if (ehDigito (**entrada) || ehLetra (**entrada))
    {
        if (++tamanho > max)
        {
            max += 10;
            nome = realloc (nome, (max+1) * sizeof (char));
        }
        
        nome [tamanho-1] = **entrada;
        consome_entrada(1);
        return NULL;
    }

    nome [tamanho] = '\0';

    tamanho = 0;
    
    c = busca_palavra_reservada (nome);
    if (c != C_INVALIDA)
    {
        free (nome);
        a = novoAtomo(c, 0);
    }
    else
    {
        cod = busca_cod_ID (nome);
        if (cod == ERRO)
            cod = adicionaID (nome);
        a = novoAtomo (C_IDENTIFICADOR, cod);
    }

    return a;
}


um_atomo estado_simbolo () 
{
    uma_classe c;
    char e;
    static uma_classe classe_valida;
    static char *nome, *simbolo_valido;
    static int i=0, max=0;

    if (i == 0)
    {
        nome = malloc (sizeof (char));
        simbolo_valido = NULL;
    }

    if (ehSimbolo ((*entrada)[i]))
    {
        if (++i > max)
        {
            max += 5;
            nome = realloc (nome, (max+1) * sizeof (char));
        }
        
        e = (*entrada)[i-1];
//        printf ("e [%c]", e);

        nome [i-1] = e;
        nome [i] = '\0';
//        printf (" nome [%s]", nome);
        
        c = busca_simbolo(nome);     
        
        if (c != C_INVALIDA)
        {
            if (simbolo_valido)
                free (simbolo_valido);
            simbolo_valido = malloc ((i+1) * sizeof (char));
            strncpy (simbolo_valido, nome, i+1);
            classe_valida = c;
//            printf (" simbolo_valido [%s] ", simbolo_valido);
        }
        
//        printf ("\n");
            
        return NULL;
    }

    free (nome);
	i = 0;

    if (simbolo_valido)
    {
        consome_entrada (strlen (simbolo_valido));
        free (simbolo_valido);
        return novoAtomo(classe_valida, 0);
    }
    else
        return novoAtomo(C_INVALIDA, linha);
}

um_atomo estado_inteiro () 
{
    um_atomo a;
    static int valor = 0;
    
    if (ehDigito (**entrada))
    {
        valor = (10 * valor) + (**entrada - '0');
        consome_entrada(1);
        return NULL;
    }

    a = novoAtomo(C_INTEIRO, 0);    
    a->valor = valor;
    valor = 0;    
    
    return a;
}

um_atomo estado_comentario ()
{
    if (ehFimDeLinha (**entrada))
        estado_lexico = BRANCO;
    else
        consome_entrada(1);
    
    return NULL;
}


void consome_entrada(int total)
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
                coluna += 3;
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
//        case '_':
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
