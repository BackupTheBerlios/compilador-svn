/***************************************************************************
 *            sintatico.c
 *
 *  Sun Jun 26 17:49:39 2005
 *  Copyright  2005  André e Igor
 *  andredsp@gmail.com
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

#include "defs.h"
#include "sintatico.h"
#include "lexico.h"
#include "erro.h"

// Numeração das sub-máquinas
enum submaquinas {SM_PROGRAMA, SM_TIPO, SM_COMANDO, SM_EXPRESSAO,
    SM_FATOR, SM_EXP_BOOLEANA, TOTAL_SUBMAQUINAS};

char *submaquinas_nomes[TOTAL_SUBMAQUINAS] = {"SM_PROGRAMA", "SM_TIPO", "SM_COMANDO", "SM_EXPRESSAO",
    "SM_FATOR", "SM_EXP_BOOLEANA"};
    
    
// Vetor com todas as máquinas
static automato maquinas[TOTAL_SUBMAQUINAS];

// Estado e máquina atuais
static estado atual; 

// Quantidade e pilha de retornos de sub-máquina
static int retornos;
static estado *pilha_retornos;
    
void adiciona_retorno ()
{
    retornos++;

    if (retornos > 1)
        pilha_retornos = realloc (pilha_retornos, retornos * sizeof (estado));
    else
        pilha_retornos = malloc (retornos * sizeof (estado));

    pilha_retornos[retornos-1].estado = atual.estado;
    pilha_retornos[retornos-1].maquina = atual.maquina;
}    

int le_retorno ()
{
    if (retornos)
    {
        retornos--;
        atual.estado = pilha_retornos[retornos].estado;
        atual.maquina = pilha_retornos[retornos].maquina;
        pilha_retornos = realloc (pilha_retornos, retornos * sizeof (estado));
        return VERDADE;
    }
    
    return FALSO;
}    

int estado_final (enum submaquinas maq, int estado)
{
    int i;
    
    for (i=0; i<maquinas[maq].estados_finais ; i++)
        if (estado == maquinas[maq].estado_final[i])
            return VERDADE;
        
    return FALSO;
}

//
// Ações Semânticas
//
void nada() {}

//
// Definição das Máquinas
//

/* Máquina programa (P): */
#define P_ENTRADAS          12
#define P_ESTADOS           1
#define P_ESTADOS_FINAIS    1
int P_entradas[P_ENTRADAS] = 
    { C_INVALIDA, PR_PROGRAM, PR_PROCEDURE, PR_FUNCTION, PR_RETURNS, S_ABRE_PARENTESES, S_FECHA_PARENTESES, S_VIRGULA, S_PONTO_E_VIRGULA, -SM_TIPO, C_IDENTIFICADOR, -SM_COMANDO};
transicao P_transicoes[P_ESTADOS][P_ENTRADAS] = {
//    {{ND, NULL},  { 1, NULL},   {ND, NULL},  {ND, NULL}, {ND, NULL},        {ND, NULL},         {ND, NULL}, {ND, NULL},     {ND, NULL}, {ND, NULL},      {ND, NULL},  {ND, NULL}},
//    {{ND, NULL},  { 1, NULL},   {ND, NULL},  {ND, NULL}, {ND, NULL},        {ND, NULL},         {ND, NULL}, {ND, NULL},     {ND, NULL}, {ND, NULL},      {ND, NULL},  {ND, NULL}},
//    {{ND, NULL},  { 1, NULL},   {ND, NULL},  {ND, NULL}, {ND, NULL},        {ND, NULL},         {ND, NULL}, {ND, NULL},     {ND, NULL}, {ND, NULL},      {ND, NULL},  {ND, NULL}},
    {{ND, NULL},  { 1, NULL},   {ND, NULL},  {ND, NULL}, {ND, NULL},        {ND, NULL},         {ND, NULL}, {ND, NULL},     {ND, NULL}, {ND, NULL},      {ND, NULL},  {ND, NULL}}
};
int P_finais[P_ESTADOS_FINAIS] = { 3 };
    
/* Sub-máquina tipo (T): */
#define T_ENTRADAS          8
#define T_ESTADOS           5
#define T_ESTADOS_FINAIS    1
static int T_entradas[T_ENTRADAS] = 
    { C_INVALIDA, PR_REAL, PR_INTEGER, PR_BOOLEAN, S_ABRE_CHAVE, S_FECHA_CHAVE, S_VIRGULA, C_INTEIRO };
static transicao T_transicoes[T_ESTADOS][T_ENTRADAS] = {
    {{ND, NULL}, { 1, NULL}, { 1, NULL}, { 1, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}},
    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, { 3, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}},
    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}},
    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, { 4, NULL}},
    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, { 2, NULL}, { 3, NULL}, {ND, NULL}}
};
static int T_finais[T_ESTADOS_FINAIS] = { 2 };


transicao ** aloca_transicoes (transicao *t, int estados, int entradas)
{
    transicao ** tabela;
    
    tabela = (transicao **) malloc (estados * sizeof (transicao *));
    
    if (tabela)
    {
        int i;
        
        for (i=0; i<estados; i++)
            tabela[i] = &t[i*entradas];
    }
    
    return tabela;
}

/* inicia_submaquinas
 * 
 * Função que define o vetor de máquinas, bem como a máquina e estado iniciais
 */
void inicia_submaquinas (void)
{
	//
    // Criação das máquinas no vetor
    //
    
    maquinas[SM_PROGRAMA].entradas          = P_ENTRADAS;
    maquinas[SM_PROGRAMA].estados           = P_ESTADOS;
    maquinas[SM_PROGRAMA].estados_finais    = P_ESTADOS_FINAIS;
    maquinas[SM_PROGRAMA].tipo_entradas     = (int *) &P_entradas;
    maquinas[SM_PROGRAMA].transicoes        = aloca_transicoes ((transicao *) P_transicoes, P_ESTADOS, P_ENTRADAS);
    maquinas[SM_PROGRAMA].estado_final      = (int *) &P_finais;
    
    maquinas[SM_TIPO].entradas          = T_ENTRADAS;
    maquinas[SM_TIPO].estados           = T_ESTADOS;
    maquinas[SM_TIPO].estados_finais    = T_ESTADOS_FINAIS;
    maquinas[SM_TIPO].tipo_entradas     = (int *) &T_entradas;
    maquinas[SM_TIPO].transicoes        = aloca_transicoes ((transicao *) T_transicoes, T_ESTADOS, T_ENTRADAS);
    maquinas[SM_TIPO].estado_final      = (int *) &T_finais;
	
    
    
    
    // MÁquina e estado iniciais
    atual.estado = 0;
    atual.maquina = SM_PROGRAMA;
    
    // No início, não há retornos
    retornos = 0;
    pilha_retornos = (estado *) NULL;
}

int busca_coluna (enum submaquinas maq, int tipo_entrada)
{
    int i, col;
//    enum submaquinas opcoes[TOTAL_SUBMAQUINAS];
    
    // Procura por uma classe
    for (i=0; i < maquinas[maq].entradas; i++)
        if (tipo_entrada == maquinas[maq].tipo_entradas[i])
            return i;
        
    // Se não encontrar, vê se alguma entrada é sub-máquina
    for (i=0; i < maquinas[maq].entradas; i++)
    {
        // É sub-máquina? (entrada negativa)
        if (maquinas[maq].tipo_entradas[i] < 0)
        {
            enum submaquinas s = -maquinas[maq].tipo_entradas[i];
            col = busca_coluna (s, tipo_entrada); 
            if (col > 0)
                return col;
        }
    }
        
    return 0;
}

int maquina_sintatico (char **entrada, uma_fila fila)
{
    um_atomo at;
    int col, tipo_entrada, prox_estado;
    void (*acao)();

    // Lê novo átomo sem look-ahead
    at = analisadorLexico (entrada, FALSO, &fila);

#ifdef DEBUG    
    printf ("%s,%d:\n", submaquinas_nomes[atual.maquina], atual.estado);    
    printf (" leu %s\n", nomeClasse (at->classe));
#endif    
    
    // Interrompe em caso de erro ou fim do arquivo
    if (at->classe == C_INVALIDA)
        return FIM_ERRO_LEXICO;
    else if (at->classe == C_FIM)
        return FIM_ERRO_LEXICO;
    
    // Procura a coluna referente ao átomo lido
    col = busca_coluna (atual.maquina, at->classe);

#ifdef DEBUG    
    printf (" coluna %d\n", col);
#endif    

    if (atual.estado > (maquinas[atual.maquina].estados-1))
        return FIM_ERRO_MAQUINAS;
    
    // Executa ação semântica
    acao = maquinas[atual.maquina].transicoes[atual.estado][col].acao;
    if (acao)
    {
#ifdef DEBUG    
        printf (" ação\n");
#endif    
        (*acao) ();
    }
    
    // Verifica se o próximo estado é inválido
    prox_estado = maquinas[atual.maquina].transicoes[atual.estado][col].estado;
    if (prox_estado == ND)
    {
        // O estado atual é final?
        if (estado_final (atual.maquina, atual.estado))
        {
            // Retorna FIM caso a submáquina anterior caso exista
            return !le_retorno ();
        }
        else // Erro de sintaxe
        {
            return FIM_ERRO_SINTATICO;
        }
    }
    else // Próximo estado é válido
    {
        atual.estado = prox_estado;
        
        // Se for entrar numa sub-máquina, armazena máquina e estado atuais
        // para retornar depois
        tipo_entrada = maquinas[atual.maquina].tipo_entradas[col];        
        if (tipo_entrada < 0)
        {
            adiciona_retorno ();
            
            // Vai para submáquina
            atual.estado = 0;
            atual.maquina = -tipo_entrada;
        }
    }            

    free (at);
    
    return 0;
}

int analisadorSintatico (char **entrada)
{
    uma_fila fila;
    int erro;
    
    erro = FIM_OK;
    fila_inicia (&fila);
    inicia_submaquinas ();

    // Fica consumindo átomos até ocorrer um erro ou acabar o buffer
    while (!erro)
        if (**entrada == '\0')
            break;
        else
            erro = maquina_sintatico(entrada, fila);
    
    return erro;
}
