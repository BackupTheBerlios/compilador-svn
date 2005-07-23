/***************************************************************************
 *            sintatico.c
 *
 *  Sun Jun 26 17:49:39 2005
 *  Copyright  2005  Andr� e Igor
 *  andredsp@gmail.com
 *  igor_arouca@yahoo.com.br
 *
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

// Numera��o das sub-m�quinas
enum submaquinas {
	 			  SM_PROGRAMA,		/* 0 */
	 			  SM_TIPO,			/* 1 */
				  SM_COMANDO,		/* 2 */
				  SM_EXPRESSAO,		/* 3 */
				  SM_FATOR,			/* 4 */
				  SM_EXP_BOOLEANA,	/* 5 */
				  TOTAL_SUBMAQUINAS /* 6 */
				 };

// Vetor com todas as m�quinas
static automato maquinas[TOTAL_SUBMAQUINAS];

// Estado e m�quina atuais
static estado atual; 

// Quantidade e pilha de retornos de sub-m�quina
static int retornos;
static estado *pilha_retornos;

// A��es Sem�nticas
void nada() {}

void converteCodigoFuncao(int codigo, void (*funcao)()) {

   switch(codigo) {
      case 0:
	  	 funcao = NULL;  
   	     break;
      
	  case 1:
	  	 funcao = NULL;
   	     break;
   
      case 2:
	  	 funcao = NULL;
   	     break;
      
      case 3:
   	     funcao = NULL;
		 break;
   
      default:
	  	 funcao = NULL;
	     break;
   }
}

void defineSubMaquina(char* arqTabelaDeTransicoes, const int SM) {
	FILE* arqSM;

	if ((arqSM = fopen(arqTabelaDeTransicoes, "r")) == NULL) {
 	   printf("Erro ao tentar abrir arquivo contendo tabela de transicoes!\n\n");
 	   return;
 	
	 } else {
  	   int entradas;
  	   int estados;
  	   int estadosFinais;
  	   int funcao;
  	   int i, j;
  	   // L� numero de entradas e estados (dimens�es da tabela), 
	   // al�m do n�mero de estados finais.
	   fscanf(arqSM, "%d\n", entradas);
	   fscanf(arqSM, "%d\n", estados);
	   fscanf(arqSM, "%d\n", estadosFinais);

	   // L� tipos de entradas
	   int* SM_entradas = (int*) calloc(entradas, sizeof(int));
	   for (i = 0; i < entradas; ++i) {
	      fscanf(arqSM, "%d ", SM_entradas[i]);
	   }
	   fscanf(arqSM, "\n");

	   // L� transi��es 
	   transicao** SM_transicoes = (transicao**) calloc(estados, sizeof (transicao*));
	   for (i = 0; i < estados; ++i) {
	      SM_transicoes[i] = (transicao*) calloc(entradas, sizeof(transicao));
	      for (j = 0; j < entradas; ++j) {
		     fscanf(arqSM, "%d, %d ", (SM_transicoes[i][j]).estado, funcao);
		     converteCodigoFuncao(funcao, SM_transicoes[i][j].acao);
		  }
		  fscanf(arqSM, "\n");
	   }

	   // L� estados finais
	   int* SM_finais = (int*) malloc(estadosFinais);
	   for (i = 0; i < estadosFinais; ++i) {
	      fscanf(arqSM, "%d ", SM_finais[i]);
	   }
  	   maquinas[SM].entradas          = entradas;
       maquinas[SM].estados           = estados;
       maquinas[SM].tipo_entradas     = (int *) &SM_entradas;
       maquinas[SM].transicoes        = (transicao **) &SM_transicoes;
       maquinas[SM].estados_finais    = (int *) &SM_finais;
       
       fclose(arqSM);
   }
}

/* inicia_submaquinas
 * 
 * Fun��o que define o vetor de m�quinas, bem como a m�quina e estado iniciais
 */
void inicia_submaquinas()
{
 	char* arqSM;

	// Defini��o das M�quinas:

	// Sub-m�quina programa (P):
	arqSM = "subMaquina_Programa.dat";
	defineSubMaquina(arqSM, SM_PROGRAMA);
	
	// Sub-m�quina tipo (T):
	arqSM = "subMaquina_Tipo.dat";
	defineSubMaquina(arqSM, SM_TIPO);

	// Sub-m�quina comando (C):
	arqSM = "subMaquina_Comando.dat";
	defineSubMaquina(arqSM, SM_COMANDO);

	// Sub-m�quina express�o (E):
	arqSM = "subMaquina_Expressao.dat";
	defineSubMaquina(arqSM, SM_EXPRESSAO);
	
	// Sub-m�quina fator (F):
	arqSM = "subMaquina_Fator.dat";
	defineSubMaquina(arqSM, SM_FATOR);
	
	// Sub-m�quina express�o booleana (O):
	arqSM = "subMaquina_ExpressaoBooleana.dat";
	defineSubMaquina(arqSM, SM_EXP_BOOLEANA);

    // M�quina e estado iniciais
	atual.estado = 0;
    atual.maquina = SM_PROGRAMA;
    
    // No in�cio, n�o h� retornos
    retornos = 0;
    pilha_retornos = (estado *) NULL;
}
