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
#include "defs.h"
#include "atomo.h"
#include "tabid.h"
#include "analisadorLexico.h"


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
//        case ':':
        case ';':
        case '<':
        case '=':
        case '>':
//        case '?':
//        case '[':
//        case '\\':
//        case ']':
//        case '^':
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

/* ehBranco
 *
 * Retorna verdadeiro se o caracter de entrada for um espaco em branco
 */
int ehBranco (char e)
{
    switch (e)
    {
        case ' ':   // espaço
        case '\n':  // avanço de linha
        case '\r':  // retorno de carro
        case '\t':  // tabulação
            return VERDADE;
    }
    return FALSO;
}


um_atomo leNome (char **entrada) 
{
    um_atomo a;
    uma_classe c;
    char * nome;
    int tamanho;
    int cod;

    tamanho = 0;
    nome = malloc ((++tamanho) * sizeof (char));

    while (ehDigito (**entrada) || ehLetra (**entrada))
    {
        nome [tamanho-1] = **entrada;
        (*entrada)++;
        nome = realloc (nome, (++tamanho) * sizeof (char));
    }

    nome [tamanho-1] = '\0';

    c = busca_palavra_reservada (nome);
    if (c != INVALIDO)
    {
        return novoAtomo(c);
    }

    cod = busca_cod_ID (nome);

    if (cod == BUSCA_NAO_ENCONTRADA)
    {
        cod = adicionaID (nome);
    }

    a = novoAtomo(IDENTIFICADOR);
    a->valor = cod;
    return a;
}


um_atomo leSimbolo (char **entrada) 
{
    uma_classe c;
    char * nome;
    int tamanho;

    tamanho = 0;
    nome = malloc ((++tamanho) * sizeof (char));

    while (ehSimbolo (**entrada))
    {
        nome [tamanho-1] = **entrada;
        (*entrada)++;
        nome = realloc (nome, (++tamanho) * sizeof (char));
    }

    nome [tamanho-1] = '\0';

    c = busca_simbolo (nome);
    if (c != INVALIDO)
    {
        return novoAtomo(c);
    }

    return novoAtomo(INVALIDO);
}


um_atomo leInteiro(char **entrada) 
{
    um_atomo a;

    a = novoAtomo(INTEIRO);

    while (ehDigito (**entrada))
    {
        a->valor = (10 * a->valor) + (**entrada - '0');
        (*entrada)++;
    }

    return a;
}

/*
  As transições em vazio do automato são representadas por retornos da função
  analisadorLexico. As demais transições podem ser representadas por devios
  para partes do programa ou por chamdas de funções internas (estados), e o
  consumo de caracteres corresponde ao avanço do cursor no programa fonte.
  Uma rotina auxiliar chamada "leFonte" encarrega-se de ler novas linhas de dados
  do programa fonte sempre que necessário, informando ao analisador léxico qual
  é o próximo caractere ainda não analisado ("entrada"). A retornar ao programa
  chamador, "entrada" esta sempre se referindo ao proximo caractere ainda não
  analisado.
*/

um_atomo analisadorLexico(char **entrada)
{
    um_atomo saida = NULL;

    /*
     * Estado 1
     */
    while (ehBranco(**entrada))
    {
        (*entrada)++;
    }

    // Próximo estado
    if (ehLetra (**entrada))
    {
        /*
         * Estado 2
         */
        saida = leNome (entrada);
    }
    else if (ehDigito (**entrada))
    {
        /*
         * Estado 3
         */
        saida = leInteiro (entrada);
    }
    else if (ehSimbolo (**entrada))
    {
        /*
         * Estados 4,5
         */
        saida = leSimbolo (entrada);
    }
    else if (**entrada == '%')
    {
        /*
         * Estado 6
         */
//        saida = leComentario (entrada);
    }
    else if (**entrada == '\0')
        saida = novoAtomo (FIM);
    else
        saida = novoAtomo (INVALIDO);

    return saida;
}
