/***************************************************************************
 *            semantico.c
 *
 *  Mon Aug  1 04:12:56 2005
 *  Copyright  2005  André Pinto
 *  andre.pinto@poli.usp.br
 ****************************************************************************/

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

//
// Globais
//
static FILE * saida;
static uma_pilha globais;
static uma_pilha locais;
static int tamanho_var;
static char *nome_entrada;
static char *nome_saida;
static enum tipo_var escopo = VAR_GLOBAL;


/*
 * Acerta o nome dos arquivos de entrada e saída
 */
void ajusta_nome_arquivo (char * n)
{
    char *pos;
    int tam;
    
    nome_entrada = n;

    // Ajusta o nome do arquivo de saída
    pos = strrchr (nome_entrada, '.');
    if (pos)
        tam = pos - nome_entrada;
    else
        tam = strlen (nome_entrada);
    
    nome_saida = calloc (tam+4, sizeof (char));
    strncpy (nome_saida, nome_entrada, tam);
    strcat (nome_saida, ".s");
}


//
// Ações semânticas
//

void acao_inicio (um_atomo a)
{
    pilha_inicia (&globais, VAR_GLOBAL);

//	m_operator = 0;

	saida = fopen (nome_saida, "w");
	fprintf (saida, "\t.file\t\"%s\"\n\n", nome_entrada);
    
	fprintf (saida, "\t.section\t.rodata\n");
	fprintf (saida, ".INTEIRO:\n");
	fprintf (saida, "\t.string\t\"%d\\n\"\n\n");

	fprintf (saida, "\t.text\n\n");
}

void acao_fim (um_atomo a)
{
    uma_variavel v;
    
	fprintf (saida, "\n");

    while (!pilha_eh_vazia (&globais))
    {
        v = pilha_retira (&global);
        fprintf (saida, "\t.comm\t%s,%d\n", v->nome, v->tamanho);
        free (v);
    }
    
	fprintf (saida, "\n");
	fprintf (saida, "\t.ident\t%s\n", "CompilaDOR 1.0alfa");
    
    fclose (saida);
}

void acao_ajusta_tamanho_var (um_atomo a)
{
    if (a->classe == PR_INTEGER)
        tamanho_var = 4;
}

void acao_adiciona_global (um_atomo a)
{
    if (a->classe == C_IDENTIFICADOR)
        pilha_adiciona (&globais, tamanho_var, busca_nome_ID (a->valor));
}

void acao_funcao_inicio (um_atomo a)
{
    if (a->classe == C_IDENTIFICADOR)
    {
        char * nome;
        nome = busca_nome_ID (a->valor);
        
        strcpy(m_currentFunction, funcName);
        fprintf(saida, "\t.globl\t%s\n", nome);
        fprintf(saida, "%s:\n", nome);
        fprintf(saida, "\tpushl\t%%ebp\n");
        fprintf(saida, "\tmovl\t%%esp,%%ebp\n");
        
//        fprintf(saida, "\tpushl	%%edi\n");
        fprintf(saida, "\n");
    }
}


/*
 * Funções de manipulação da pilha
 */
void pilha_inicia (uma_pilha * pilha, enum tipo_var t)
{
    pilha->variavel = (uma_variavel *) NULL;
    pilha->tamanho = 0;
    pilha->tipo = t;
}

int pilha_eh_vazia (uma_pilha *pilha)
{
    return pilha->tamanho == 0;
}

int pilha_adiciona (uma_pilha *pilha, int n, char *nome)
{
    uma_variavel v;        
    int ultima;
    
    if (pilha->variavel == NULL)
    {
        pilha->variavel = (uma_variavel *) malloc(sizeof(uma_variavel));
        pilha->tamanho = 1;
    }
    else
        pilha->variavel = (uma_variavel *) realloc(pilha->variavel, 
            (++pilha->tamanho) * sizeof (uma_variavel));

    v = (uma_variavel) malloc (sizeof (struct s_variavel));
    
    if (pilha->variavel == NULL || v == NULL)
    {
        if (v)
            free (v);
        if (pilha->variavel)
            free (pilha->variavel);
            
        pilha->tamanho = 0;
        ultima = ERRO;
    }
    else
    {
        ultima = pilha->tamanho - 1;
        v->tamanho = n;
        v->nome = nome;
        pilha->variavel[ultima] = v;
    }
    
    return ultima;
}

uma_variavel pilha_retira (uma_pilha *pilha)
{
    uma_variavel variavel;
    int ultima;
    
    if (pilha_eh_vazia (pilha))
        return (uma_variavel) NULL;

    ultima = pilha->tamanho - 1;
    variavel = pilha->variavel[ultima];
    
    pilha->variavel = (uma_variavel *) realloc (pilha->variavel, (--pilha->tamanho) * sizeof (uma_variavel));
    
    return variavel;
}

void pilha_esvazia (uma_pilha *pilha)
{
    while (! pilha_eh_vazia (pilha))
        free (pilha_retira (pilha));
}
