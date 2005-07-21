/***************************************************************************
 *            sintatico.c
 *
 *  Sun Jun 26 17:49:39 2005
 *  Copyright  2005  Andr� e Igor
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

#include "defs.h"
#include "sintatico.h"

// Numeração das sub-máquinas
enum submaquinas {SM_PROGRAMA, SM_TIPO, SM_COMANDO, SM_EXPRESSAO,
    SM_FATOR, SM_EXP_BOOLEANA, TOTAL_SUBMAQUINAS};

// Vetor com todas as máquinas
static automato maquinas[TOTAL_SUBMAQUINAS];

// Estado e máquina atuais
static estado atual; 

// Quantidade e pilha de retornos de sub-máquina
static int retornos;
static estado *pilha_retornos;

//
// Ações Semânticas
//
void nada() {}

/* inicia_submaquinas
 * 
 * Função que define o vetor de máquinas, bem como a máquina e estado iniciais
 */
void inicia_submaquinas (void)
{
    //
    // Definição das Máquinas
    //

	/* Máquina programa (P): */
	#define P_ENTRADAS   12
	#define P_ESTADOS    1
    static const int P_entradas[P_ENTRADAS] = 
        { C_INVALIDA, PR_PROGRAM, PR_PROCEDURE, PR_FUNCTION, PR_RETURNS, S_ABRE_PARENTESES, S_FECHA_PARENTESES, S_VIRGULA, S_PONTO_E_VIRGULA, -SM_TIPO, C_IDENTIFICADOR, -SM_COMANDO};
    static const transicao P_transicoes[P_ESTADOS][P_ENTRADAS] = {
        {{ND, NULL},  { 1, NULL},   {ND, NULL},  {ND, NULL}, {ND, NULL},        {ND, NULL},         {ND, NULL}, {ND, NULL},     {ND, NULL}, {ND, NULL},      {ND, NULL},  {ND, NULL}}
    };
    static const int P_finais[] = { 3 };
        
	/* Sub-máquina tipo (T): */
	#define T_ENTRADAS   8
	#define T_ESTADOS    5
	static const int T_entradas[T_ENTRADAS] = 
        { C_INVALIDA, PR_REAL, PR_INTEGER, PR_BOOLEAN, S_ABRE_CHAVE, S_FECHA_CHAVE, S_VIRGULA, C_INTEIRO };
	static const transicao T_transicoes[T_ESTADOS][T_ENTRADAS] = {
	    {{ND, NULL}, { 1, NULL}, { 1, NULL}, { 1, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}},
	    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, { 3, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}},
	    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}},
	    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, { 4, NULL}},
	    {{ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, {ND, NULL}, { 2, NULL}, { 3, NULL}, {ND, NULL}}
	};
    static const int T_finais[] = { 2 };
    
    
    
    
	//
    // Criação das máquinas no vetor
    //
    
    maquinas[SM_PROGRAMA].entradas          = P_ENTRADAS;
    maquinas[SM_PROGRAMA].estados           = P_ESTADOS;
    maquinas[SM_PROGRAMA].tipo_entradas     = (int *) &P_entradas;
    maquinas[SM_PROGRAMA].transicoes        = (transicao **) &P_transicoes;
    maquinas[SM_PROGRAMA].estados_finais    = (int *) &P_finais;
    
    maquinas[SM_TIPO].entradas          = T_ENTRADAS;
    maquinas[SM_TIPO].estados           = T_ESTADOS;
    maquinas[SM_TIPO].tipo_entradas     = (int *) &T_entradas;
    maquinas[SM_TIPO].transicoes        = (transicao **) &T_transicoes;
    maquinas[SM_TIPO].estados_finais    = (int *) &T_finais;
	
    
    
    
    // MÁquina e estado iniciais
	atual.estado = 0;
    atual.maquina = SM_PROGRAMA;
    
    // No início, não há retornos
    retornos = 0;
    pilha_retornos = (estado *) NULL;
}

