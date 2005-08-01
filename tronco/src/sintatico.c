/***************************************************************************
 *            sintatico.c
 *
 *  Sun Jun 26 17:49:39 2005
 *  Copyright  2005  André e Igor
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
#include <stdlib.h>

#include "defs.h"
#include "sintatico.h"
#include "lexico.h"
#include "erro.h"

// Flags
/*
extern int falante;
extern int depurando;
extern int espacado;
*/

// Numeração das sub-máquinas
enum submaquinas {
	 			  SM_PROGRAMA,		/* 0 */
	 			  SM_TIPO,			/* 1 */
				  SM_COMANDO,		/* 2 */
				  SM_EXPRESSAO,		/* 3 */
				  SM_FATOR,			/* 4 */
				  SM_EXP_BOOLEANA,	/* 5 */
				  TOTAL_SUBMAQUINAS /* 6 */
				 };

char *submaquinas_nomes[TOTAL_SUBMAQUINAS] = {"SM_PROGRAMA",
	 									      "SM_TIPO",
											  "SM_COMANDO",
											  "SM_EXPRESSAO",
											  "SM_FATOR",
											  "SM_EXP_BOOLEANA"
											 };    
// Vetor com todas as máquinas
static automato maquinas[TOTAL_SUBMAQUINAS];

// Estado e máquina atuais
static estado atual; 

// Quantidade e pilha de retornos de sub-m�quina
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
    
#ifdef DEBUG_SINTATICO1
    printf ("..estado=%d finais=(%d)", estado, maquinas[maq].estados_finais);
#endif    

    for (i=0; i<maquinas[maq].estados_finais; i++)
    {
#ifdef DEBUG_SINTATICO1
        printf (" %d", maquinas[maq].estado_final[i]);
#endif    
        if (estado == maquinas[maq].estado_final[i])
        {
#ifdef DEBUG_SINTATICO1
            printf (" achou..");
#endif    
            return VERDADE;
        }
    }
#ifdef DEBUG_SINTATICO1
    printf (" xii..");
#endif    
        
    return FALSO;
}

//
// Ações Semânticas
//
void nada() {}

void converteCodigoFuncao(int codigo, void (*funcao)()) {
/* 
 * Depois de declaradas as fun��es semanticas,
 * basta passar seus endere�os para o ponteiro
 * denominado "funcao" no switch abaixo;
 * Ex: funcao = funcaoDeclarada; (o nome � o ponteiro, como em vetores)
 *
*/

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

	if ((arqSM = fopen(arqTabelaDeTransicoes, "r")) == NULL)
    {
        printf("Erro ao tentar abrir arquivo contendo tabela de transicoes!\n\n");
        return;
	}

    int entradas;
    int estados;
    int estadosFinais;
    int funcao;
	int i, j;
	int *SM_entradas;
	int *SM_finais;
	transicao **SM_transicoes;
	
	// Lê numero de entradas e estados (dimensões da tabela), 
	// além do número de estados finais.
	fscanf(arqSM, "%d", &entradas);
	fscanf(arqSM, "%d", &estados);
	fscanf(arqSM, "%d", &estadosFinais);

#ifdef DEBUG_ARQUIVO    
    printf ("entradas=%d estados=%d finais=%d\n", entradas, estados, estadosFinais);
#endif    

#ifdef DEBUG_ARQUIVO    
    printf ("entradas:");
#endif    

    // Lê tipos de entradas
    SM_entradas = (int*) calloc(entradas, sizeof(int));
    for (i = 0; i < entradas; ++i) {
        fscanf(arqSM, "%d ", &SM_entradas[i]);
#ifdef DEBUG_ARQUIVO    
        printf (" %d", SM_entradas[i]);
#endif    
    }

#ifdef DEBUG_ARQUIVO    
    printf ("\n");
#endif    

#ifdef DEBUG_ARQUIVO    
    printf ("transições:");
#endif    
    // Lê transições 
    SM_transicoes = (transicao**) calloc(estados, sizeof (transicao*));
    for (i = 0; i < estados; ++i)
    {
        SM_transicoes[i] = (transicao*) calloc(entradas, sizeof(transicao));
        for (j = 0; j < entradas; ++j)
        {
            fscanf(arqSM, "%d,%d", &SM_transicoes[i][j].estado, &funcao);
			SM_transicoes[i][j].acao = funcao;
//            converteCodigoFuncao(funcao, SM_transicoes[i][j].acao);
            
#ifdef DEBUG_ARQUIVO    
            printf (" (%d:%d)", SM_transicoes[i][j].estado, funcao);
#endif    
        }
//		  fscanf(arqSM, "\n");
#ifdef DEBUG_ARQUIVO    
        printf ("\n");
#endif    
    }

#ifdef DEBUG_ARQUIVO    
    printf ("finais:");
#endif    

    // Lê estados finais
    SM_finais = (int*) calloc(estadosFinais, sizeof (int));
    for (i = 0; i < estadosFinais; ++i)
    {
        fscanf(arqSM, "%d", &SM_finais[i]);
#ifdef DEBUG_ARQUIVO    
        printf (" %d", SM_finais[i]);
#endif
    }

#ifdef DEBUG_ARQUIVO    
       printf ("\n");
#endif    

   maquinas[SM].entradas          = entradas;
   maquinas[SM].estados           = estados;
   maquinas[SM].estados_finais    = estadosFinais;
   maquinas[SM].tipo_entradas     = SM_entradas;
   maquinas[SM].transicoes        = SM_transicoes;
   maquinas[SM].estado_final      = SM_finais;
   
   fclose(arqSM);
}

/* inicia_submaquinas
 * 
 * Função que define o vetor de m�quinas, bem como a m�quina e estado iniciais
 */

void inicia_submaquinas()
{
 	char* arqSM;

	// Definição das Máquinas:

	// Sub-máquina programa (P):
	arqSM = "SubMaquina_Programa.dat";
	defineSubMaquina(arqSM, SM_PROGRAMA);
	
	// Sub-máquina tipo (T):
	arqSM = "SubMaquina_Tipo.dat";
	defineSubMaquina(arqSM, SM_TIPO);

	// Sub-máquina comando (C):
	arqSM = "SubMaquina_Comando.dat";
	defineSubMaquina(arqSM, SM_COMANDO);

	// Sub-máquina express�o (E):
	arqSM = "SubMaquina_Expressao.dat";
	defineSubMaquina(arqSM, SM_EXPRESSAO);
	
	// Sub-máquina fator (F):
	arqSM = "SubMaquina_Fator.dat";
	defineSubMaquina(arqSM, SM_FATOR);
	
	// Sub-máquina express�o booleana (O):
	arqSM = "SubMaquina_ExpressaoBooleana.dat";
	defineSubMaquina(arqSM, SM_EXP_BOOLEANA);

    // Máquina e estado iniciais
    atual.estado = 0;
    atual.maquina = SM_PROGRAMA;
    
    // No início, não há retornos
    retornos = 0;
    pilha_retornos = (estado *) NULL;
}

int busca_coluna (enum submaquinas maq, int tipo_entrada, int profundidade)
{
    int i, estado_atual;

    DEPURA (" %*sbuscando em %s", espacado?profundidade:0, "", submaquinas_nomes[maq]);
	    
    if (profundidade == 1) {
        estado_atual = atual.estado;
    }
    else {
        estado_atual = 0;
    }

    // Procura por uma classe
    for (i=1; i < maquinas[maq].entradas; i++)
        if (maquinas[maq].tipo_entradas[i] == tipo_entrada)
        {
            // Transição válida?
            int prox_estado = maquinas[maq].transicoes[estado_atual][i].estado;
            if (prox_estado >= 0 || estado_final (maq, estado_atual))
            {
                DEPURA (" %*scol=%d", espacado?profundidade:0, "", i);
                return i;
            }
        }
        
    for (i=1; i < maquinas[maq].entradas; i++)
        // É sub-máquina? (entrada negativa)
        if (maquinas[maq].tipo_entradas[i] < 0)
        {
            // Transição válida?
            int prox_estado = maquinas[maq].transicoes[estado_atual][i].estado;
            if (prox_estado >= 0)
            {
                // Procura dentro da submáquina
                enum submaquinas s = -maquinas[maq].tipo_entradas[i];
                if (busca_coluna (s, tipo_entrada, profundidade+1) > 0)
                    return i;
            }
        }
        
    return 0;
}

int maquina_sintatico (char **entrada, uma_fila *fila)
{
    um_atomo at;
    int col, tipo_entrada, prox_estado;
    void (*acao)();

    // Lê novo átomo sem look-ahead
    at = analisadorLexico (entrada, FALSO, fila);

    DEPURA ("%s(%d)", submaquinas_nomes[atual.maquina], atual.estado);    
    DEPURA (" leu %s", nomeClasse (at->classe));
    
    // Interrompe em caso de erro ou fim do arquivo
    if (at->classe == C_INVALIDA)
    {
        DEPURA(" token"); DEPURA_FIM();
        return FIM_ERRO_LEXICO;
    }
    else if (at->classe == C_FIM)
    {
        DEPURA(" fim"); DEPURA_FIM();
        return FIM_ERRO_LEXICO;
    }

    // Procura a coluna referente ao átomo lido
    col = busca_coluna (atual.maquina, at->classe, 1);

    DEPURA (" coluna %d", col);

    if (atual.estado > (maquinas[atual.maquina].estados-1))
    {
        DEPURA (" estado %d/%d\n", atual.estado, maquinas[atual.maquina].estados);
        DEPURA_FIM();
        return FIM_ERRO_MAQUINAS;
    }

    // Executa ação semântica
    acao = maquinas[atual.maquina].transicoes[atual.estado][col].acao;
    if (acao)
    {
        DEPURA (" acao");
		if (acao)
			(*acao) ();
    }

    // Verifica se o próximo estado é inválido
    prox_estado = maquinas[atual.maquina].transicoes[atual.estado][col].estado;
    if (prox_estado == ND)
    {
        fila_adiciona (fila, at);
        
        // O estado atual é final?
        if (estado_final (atual.maquina, atual.estado))
        {
            // Retorna FIM caso a submáquina anterior caso exista
            int fim = !le_retorno ();
            DEPURA (" volta p/ máquina %s, estado %d", submaquinas_nomes[atual.maquina], atual.estado);
            DEPURA_FIM();
            return fim;
        }
        else // Erro de sintaxe
        {
            DEPURA_FIM ();
            IMPRIME ("Erro de sintaxe: '%s' não era esperado.\n", nomeClasse (at->classe));
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
        
            fila_adiciona (fila, at);
            
            // Vai para submáquina
            atual.estado = 0;
            atual.maquina = -tipo_entrada;
            DEPURA (" vai p/ máquina %s", submaquinas_nomes[atual.maquina]);
        }
        else
        {
            free (at);
            DEPURA (" prox. estado %d", atual.estado);
        }

    }            

    DEPURA_FIM();
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
    while (!erro) {
        if (**entrada == '\0') {
            break;
        }
        else
        {
		    erro = maquina_sintatico(entrada, &fila);
        }
    }
    
    return erro;
}
