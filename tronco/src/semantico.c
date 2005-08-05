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
#include "semantico.h"
#include "atomo.h"
#include "tabid.h"
#include "erro.h"

// Possívei operações, em ordem de prioridade
enum tipo_operacao {ATRIBUI, EMPILHA, CHAMA, SOMA, SUBTRAI, MULTIPLICA, DIVIDE, NEGA};
char *nome_operacao[] = {"movl", "pushl", "call", "addl", "subl", "imull", "idivl", "negl"};

//
// Globais
//
static FILE * saida;

static uma_pilha globais;
static uma_pilha locais;
static uma_pilha params;

static uma_pilha operadores;
static uma_pilha operandos;

static int tamanho_var;
static int posicao_param;

static char *nome_entrada;
static char *nome_saida;
static char *nome_funcao;
static um_operando id;

static enum tipo_var escopo = VAR_GLOBAL;
static enum tipo_operacao operacao;

static int eh_funcao;
static int eh_chamada;

static int parametros;


//////////////////////
// Funções de apoio //
//////////////////////
void ajusta_nomes_arquivos (char *e, char *s)
{
    nome_entrada = e;
    nome_saida = s;
}

/* referencia_operando
 *
 * Converte a referência a um operando para o assembly
 */
char *referencia_operando (um_item op)
{
    int pos;
    char *ref;
    
    switch (op->tipo)
    {
        case NOME:
            pos = pilha_busca (&params, op->nome);
            if (pos != ERRO)
            {
                if ((ref = calloc (15, sizeof (char))))
                    sprintf (ref, "%d(%%ebp)", params.item[pos]->valor);
                break;
            }
            
            pos = pilha_busca (&locais, op->nome);
            if (pos != ERRO)
            {
                if ((ref = calloc (15, sizeof (char))))
                    sprintf (ref, "%d(%%ebp)", locais.item[pos]->valor);
                break;
            }
            
            pos = pilha_busca (&globais, op->nome);
            if (pos != ERRO)
            {
                if ((ref = calloc (1+strlen(op->nome), sizeof (char))))
                    strcpy (ref, op->nome);
                break;
            }
            
            break;
            
        case VALOR:
            if ((ref = calloc (10, sizeof (char))))
                sprintf (ref, "$%d", op->valor);
            break;
            
        default:
            IMPRIME ("Falta de memória!!!!\n");
            exit (100);
    }
    
    return ref;
}

void gera_codigo_expressao (void)
{
    char *str_saida;
    char linha[20];
    um_item operador;
    int tamanho;
    
    operador = pilha_retira (&operadores);
    
    switch (operador->tipo)
    {
        case OPERADOR:
            {
                um_item operando_A, operando_B;
                int codigo;
                char * ref;

                codigo = FALSO;
                operando_B = pilha_retira (&operandos);
                operando_A = pilha_retira (&operandos);
            
                // Calcula e aloca espaço pra string que será gerada
                tamanho = (operando_B->tipo == CODIGO? operando_B->valor + 20: 0)
                        + (operando_A->tipo == CODIGO? operando_A->valor: 20)
                        + 20;
    
                str_saida = calloc (tamanho, sizeof (char));
            
                // Se operando B já era código (com valor em %eax), move para %ecx
                if (operando_B->tipo == CODIGO)
                {
                    codigo = VERDADE;
                    
                    strncat (str_saida, operando_B->nome, operando_B->valor);
                    if (operador->valor != ATRIBUI)
                    {
                        sprintf (linha, "\t%s\t%s,%s\n", nome_operacao[ATRIBUI], "%eax", "%ecx");                
                        strncat (str_saida, linha, 20);
                    }
                }
                
                // Carrega A em %eax
                if (operando_A->tipo == CODIGO)
                    strncat (str_saida, operando_A->nome, operando_A->valor);
                else if (operador->valor != ATRIBUI)
                {
                    ref = referencia_operando (operando_A);
                    sprintf (linha, "\t%s\t%s,%s\n", nome_operacao[ATRIBUI], ref, "%eax");
                    strncat (str_saida, linha, 20);
                    free (ref);
                }
                
                // Faz operação com B em A (%eax):  A <- A op B
                {
                    char *dest;
                    
                    if (operador->valor == ATRIBUI)
                        dest = referencia_operando (operando_A);
                    else
                        dest = "%eax";
                    
                    if (codigo)
                    {
                        char *orig;
                        
                        if (operador->valor == ATRIBUI)
                            orig = "%eax";
                        else
                            orig = "%ecx";
                        sprintf (linha, "\t%s\t%s,%s\n", nome_operacao[operador->valor], orig, dest);
                    }
                    else
                    {
                        ref = referencia_operando (operando_B);
                        sprintf (linha, "\t%s\t%s,%s\n", nome_operacao[operador->valor], ref, dest);
                        free (ref);
                    }
                    
                    if (operador->valor == ATRIBUI)
                        free (dest);
                }
    
                codigo = FALSO;
                strncat (str_saida, linha, 20);
                
                // Libera memória utilizada
                free (operando_A->nome);
                free (operando_B->nome);
                free (operando_A);
                free (operando_B);
                
                // Coloca novo código na pilha de operandos
                pilha_adiciona (&operandos, CODIGO, strlen (str_saida), str_saida);            
                break;
            }

        case CHAMADA:
        case PARAM:
            {
                um_item operando;
                char *ref;
                
                parametros++;
                operando = pilha_retira (&operandos);
                
                // Calcula e aloca espaço pra string que será gerada
                tamanho = (operando->tipo == CODIGO? operando->valor: 0) + 20;
                if (operador->tipo == CHAMADA)
                    tamanho += operador->valor + 10;
                str_saida = calloc (tamanho, sizeof (char));

                if (parametros)
                {
                    // Copia operando se for código (valor em %eax)
                    if (operando->tipo == CODIGO)
                    {
                        strncat (str_saida, operando->nome, operando->valor);
                    
                        // Faz operação: push %eax
                        sprintf (linha, "\t%s\t%s\n", nome_operacao[EMPILHA], "%eax");
                    }
                    else // Faz operação: push <operando>
                    {
                        ref = referencia_operando (operando);
                        sprintf (linha, "\t%s\t%s\n", nome_operacao[EMPILHA], ref);
                        free (ref);
                    }
                    
                    strncat (str_saida, linha, 20);
                }
                
                if (operador->tipo == CHAMADA)
                {
                    sprintf (linha, "\t%s\t%s\n", nome_operacao[CHAMA], operador->nome);
                    if (parametros)
                        sprintf (linha, "\t%s\t$%d,%s\n", nome_operacao[SOMA], 4*parametros, "%esp"); 
                    
                    free (pilha_retira (&params));
                    free (operador->nome);
                }
                
                // Libera memória utilizada
                free (operando->nome);
                free (operando);
                
                // Coloca novo código na pilha de operandos
                pilha_adiciona (&operandos, CODIGO, strlen (str_saida), str_saida);            
            }
            break;

        case ABRE_PAREN:
        default:
            break;
    }
    free (operador);
}

//////////////////////
// Ações semânticas //
//////////////////////

//
// Máquina Programa
//
void acao_programa_inicio (um_atomo a)
{
    pilha_inicia (&globais);
    pilha_inicia (&locais);
    pilha_inicia (&params);
    
    pilha_inicia (&operadores);
    pilha_inicia (&operandos);

    operacao = ATRIBUI;
    tamanho_var = 0;
    eh_funcao = FALSO;
    eh_chamada = FALSO;
    nome_funcao = NULL;
    id.tipo = NOME;
    id.nome = NULL;

	saida = fopen (nome_saida, "w");
	fprintf (saida, "\t.file\t\"%s\"\n\n", nome_entrada);
    
	fprintf (saida, "\t.section\t.rodata\n");
	fprintf (saida, ".INT_SCAN:\n");
	fprintf (saida, "\t.string\t\"%%d\"\n\n");
	fprintf (saida, ".INT_PRINT:\n");
	fprintf (saida, "\t.string\t\"%%d\\n\"\n\n");

	fprintf (saida, "\t.text\n\n");
}

void acao_programa_funcao_inicio (um_atomo a)
{
    if (a->classe == PR_FUNCTION)
        eh_funcao = VERDADE;
    else
        eh_funcao = FALSO;
        
    pilha_esvazia (&locais);
    pilha_esvazia (&params);
    escopo = VAR_LOCAL;
    posicao_param = 8; // Início dos parâmetros na 'stack'
}

void acao_programa_funcao_nome (um_atomo a)
{
    nome_funcao = busca_nome_ID (a->valor);
}

void acao_programa_funcao_adiciona_param (um_atomo a)
{
    pilha_adiciona (&params, NOME, posicao_param, busca_nome_ID (a->valor));
    posicao_param += tamanho_var;
}

void acao_programa_comando (um_atomo a)
{
    if (nome_funcao == NULL)
        nome_funcao = "main";
    
    // O retorno de uma função é uma variável local de mesmo nome
    if (eh_funcao)
        pilha_adiciona (&locais, NOME, -tamanho_var, nome_funcao);
    
    // Define rótulo da função
    fprintf(saida, ".globl\t%s\n", nome_funcao);
    fprintf(saida, "%s:\n", nome_funcao);
    
    // Salva %ebp e %esp atuais
    fprintf(saida, "\tpushl\t%%ebp\n");
    fprintf(saida, "\tmovl\t%%esp,%%ebp\n");
    fprintf(saida, "\n");

    // O gcc gera esse "alinhamento" do %esp no main()
    if (escopo == VAR_GLOBAL)
    {
        fprintf(saida, "\tsubl\t$8,%%esp\n");
        fprintf(saida, "\tandl\t$-16,%%esp\n");
        fprintf(saida, "\tmovl\t$0,%%eax\n");
        fprintf(saida, "\tsubl\t%%eax,%%esp\n");
    }
    
    fprintf(saida, "\n");
}



void acao_programa_funcao_fim (um_atomo a)
{
    if (eh_funcao)
    {
        eh_funcao = 0;
        // Transfere valor da primeira variável local ao %eax (retorno)
        fprintf (saida, "\t%s\t%s,%s\n",
            nome_operacao[ATRIBUI], referencia_operando(locais.item[0]), "%eax");
        exit (200);
    }
    
	fprintf (saida, "\tleave\n");
	fprintf (saida, "\tret\n");
	fprintf (saida, "\n");

    if (escopo == VAR_GLOBAL)
    {
        um_item v;
    
        // Variáveis globais
        while (!pilha_eh_vazia (&globais))
        {
            v = pilha_retira (&globais);
            fprintf (saida, "\t.comm\t%s,%d\n", v->nome, v->valor);
            free (v);
        }
        
        fprintf (saida, "\n");
        fprintf (saida, "\t.ident\t\"%s\"\n", "CompilaDOR 1.0alfa");
        fprintf (saida, "\n");
        
        fclose (saida);
    }
    else
    {
        // Quando a função termina, volta ao escopo global
        escopo = VAR_GLOBAL;
        nome_funcao = NULL;
    }
}

//
// Máquina Tipo
//
void acao_tipo_tamanho_var (um_atomo a)
{
    if (a->classe == PR_INTEGER)
        tamanho_var = 4;
}

//
// Máquina Comando
//
void acao_comando_declara_adiciona (um_atomo a)
{
    if (escopo == VAR_GLOBAL)
        pilha_adiciona (&globais, NOME, tamanho_var, busca_nome_ID (a->valor));
    else
    {
        int ultima, pos;
        
        ultima = locais.tamanho - 1;

        if (ultima)
            pos = locais.item[ultima-1]->valor;
        else
            pos = 0;
        
        pos -= tamanho_var;
                
        pilha_adiciona (&locais, NOME, pos, busca_nome_ID (a->valor));
    }
}

void acao_comando_declara_fim (um_atomo a)
{
    // Aloca espaço das variáveis locais
    if (escopo == VAR_LOCAL && locais.tamanho)
        fprintf(saida, "\t%s\t$%d,%s\n", nome_operacao[SUBTRAI], locais.item[locais.tamanho-1]->valor, "%esp");
}


void acao_comando_id (um_atomo a)
{
    acao_fator_id (a);
}

void acao_comando_atrib (um_atomo a)
{
    acao_fator_operando (a);
}

void acao_comando_chamada (um_atomo a)
{
    acao_fator_chamada (a);
}

void acao_comando_param (um_atomo a)
{
    acao_fator_param (a);
}

void acao_comando_fecha_paren (um_atomo a)
{
    acao_fator_fecha_paren (a);
}

void acao_comando_fim (um_atomo a)
{
    um_item item;
    
    while (!pilha_eh_vazia (&operadores))
        gera_codigo_expressao();
    
    item = pilha_retira (&operandos);
    if (item)
    {
        fprintf (saida, "%s", item->nome);        
        free (item->nome);
        free (item);
    }
}

void acao_comando_output (um_atomo a)
{
    acao_comando_fim (a);

    fprintf (saida, "\t%s\t%s\n", nome_operacao[EMPILHA], "%eax");
    fprintf (saida, "\t%s\t%s\n", nome_operacao[EMPILHA], "$.INT_PRINT");
    fprintf (saida, "\t%s\t%s\n", nome_operacao[CHAMA], "printf");
    fprintf (saida, "\t%s\t$%d,%s\n", nome_operacao[SOMA], 4*2, "%esp"); 
}

void acao_comando_input (um_atomo a)
{
    char *ref;
    
    acao_comando_id (a);
    ref = referencia_operando (&id);
    fprintf (saida, "\t%s\t%s,%s\n", "leal", ref, "%eax");
    fprintf (saida, "\t%s\t%s\n", nome_operacao[EMPILHA], "%eax");
    fprintf (saida, "\t%s\t%s\n", nome_operacao[EMPILHA], "$.INT_SCAN");
    fprintf (saida, "\t%s\t%s\n", nome_operacao[CHAMA], "scanf");
    fprintf (saida, "\t%s\t$%d,%s\n", nome_operacao[SOMA], 4*2, "%esp"); 
    free (ref);
}



//
// Máquina Fator
//
void acao_fator_id (um_atomo a)
{
    id.nome = busca_nome_ID (a->valor);
    id.valor = strlen (id.nome);
}

void acao_fator_operando (um_atomo a)
{
    um_operando operando;

    DEPURA (" operando");
    switch (a->classe)
    {
        case C_INTEIRO:
            operando.tipo = VALOR;
            operando.valor = a->valor;
            operando.nome = NULL;
            DEPURA (" inteiro (%d)", operando.valor);
            break;

        default:
            DEPURA (" id (%s)", id.nome);
            operando.tipo = NOME;
            operando.valor = id.valor;
            operando.nome = calloc (id.valor+1, sizeof (char));
            strncat (operando.nome, id.nome, id.valor);

            if (a->classe == S_ATRIBUICAO)
                pilha_adiciona (&operadores, OPERADOR, ATRIBUI, NULL);
    }
    pilha_adiciona (&operandos, operando.tipo, operando.valor, operando.nome);

}

void acao_fator_chamada (um_atomo a)
{
    um_operando operador;

    operador.tipo = CHAMADA;
    operador.valor = id.valor;
    operador.nome = calloc (id.valor+1, sizeof (char));
    strncat (operador.nome, id.nome, id.valor);

    pilha_adiciona (&operadores, operador.tipo, operador.valor, operador.nome);
    
    acao_fator_abre_paren (a);
}

void acao_fator_param (um_atomo a)
{
    pilha_adiciona (&operadores, PARAM, 0, NULL);
}


void acao_fator_abre_paren (um_atomo a)
{
    pilha_adiciona (&operadores, ABRE_PAREN, 0, NULL);
}

void acao_fator_fecha_paren (um_atomo a)
{
    um_item item;
    int tem_param;
    
    tem_param = FALSO;
    parametros = 0;

    do
    {
        item = pilha_consulta (&operadores);
        
        if (item && (item->tipo != ABRE_PAREN))
        {
            tem_param = VERDADE;
            gera_codigo_expressao();
        }
        else
            break;
    }
    while (VERDADE);
        
    if (! tem_param)
        parametros = -1;

    free (pilha_retira (&operadores));
}

void acao_fator_nega (um_atomo a)
{
    pilha_adiciona (&operadores, OPERADOR, NEGA, NULL);
}


//
// Máquina Expressão
//
void acao_expressao_operador (um_atomo a)
{
    um_item item;
    int tipo;
    
    switch (a->classe)
    {
        case S_ADICAO: tipo = SOMA; break;
        case S_SUBTRACAO: tipo = SUBTRAI; break;
        case S_MULTIPLICACAO: tipo = MULTIPLICA; break;
        case S_DIVISAO: tipo = DIVIDE; break;
        default: tipo = ATRIBUI; break;
    }
    
    do
    {
        item = pilha_consulta (&operadores);
        
        // Verifica precedência
        if (item && (item->tipo == OPERADOR) && (item->valor >= tipo))
            gera_codigo_expressao();
        else
            break;
    }
    while (VERDADE);

    pilha_adiciona (&operadores, OPERADOR, tipo, NULL);
}


/////////////////////////////////////
// Funções de manipulação da pilha //
/////////////////////////////////////
void pilha_inicia (uma_pilha * pilha)
{
    pilha->item = (um_item *) NULL;
    pilha->tamanho = 0;
}

int pilha_eh_vazia (uma_pilha *pilha)
{
    return pilha->tamanho == 0;
}

int pilha_adiciona (uma_pilha *pilha, enum tipo_item t, int n, char *nome)
{
    um_item v;     
    int ultima;    
    
    if (pilha->item == NULL)
    {
        pilha->item = (um_item *) malloc(sizeof(um_item));
        pilha->tamanho = 1;
    }
    else
        pilha->item = (um_item *) realloc(pilha->item, 
            (++pilha->tamanho) * sizeof (um_item));

    v = (um_item) malloc (sizeof (struct s_item));
    
    if (pilha->item == NULL || v == NULL)
    {
        if (v)
            free (v);
        if (pilha->item)
            free (pilha->item);
            
        pilha->tamanho = 0;
        ultima = ERRO;
    }
    else
    {       
        ultima = pilha->tamanho - 1;
        v->tipo = t;
        v->valor = n;
        v->nome = nome;
        pilha->item[ultima] = v;
    }
    
    return ultima;
}

int pilha_busca (uma_pilha *pilha, char *nome)
{
    int i;
    
    for (i=0; i < pilha->tamanho; i++)
        if (strcmp (pilha->item[i]->nome, nome) == 0)
            return i;
    
    return ERRO;
}

um_item pilha_retira (uma_pilha *pilha)
{
    um_item item;
    int ultima;
    
    if (pilha_eh_vazia (pilha))
        return (um_item) NULL;

    ultima = pilha->tamanho - 1;
    item = pilha->item[ultima];
    
    pilha->item = (um_item *) realloc (pilha->item, (--pilha->tamanho) * sizeof (um_item));
    
    return item;
}

um_item pilha_consulta (uma_pilha *pilha)
{
    um_item item;
    int ultima;
    
    if (pilha_eh_vazia (pilha))
        return (um_item) NULL;

    ultima = pilha->tamanho - 1;
    item = pilha->item[ultima];
    
    return item;
}

void pilha_esvazia (uma_pilha *pilha)
{
    while (! pilha_eh_vazia (pilha))
        free (pilha_retira (pilha));
}
