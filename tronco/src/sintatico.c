/***************************************************************************
 *            sintatico.c
 *
 *  Sun Jun 26 17:49:39 2005
 *  Copyright  2005  Andr√© e Igor
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

// NumeraÁ„o das sub-m·quinas
enum submaquinas {
	 			  SM_PROGRAMA,		/* 0 */
	 			  SM_TIPO,			/* 1 */
				  SM_COMANDO,		/* 2 */
				  SM_EXPRESSAO,		/* 3 */
				  SM_FATOR,			/* 4 */
				  SM_EXP_BOOLEANA,	/* 5 */
				  TOTAL_SUBMAQUINAS /* 6 */
				 };

char *submaquinas_nomes[TOTAL_SUBMAQUINAS] = {"SM_PROGRAMA", "SM_TIPO", "SM_COMANDO", "SM_EXPRESSAO",
    "SM_FATOR", "SM_EXP_BOOLEANA"};
    
    
// Vetor com todas as m√°quinas
static automato maquinas[TOTAL_SUBMAQUINAS];

// Estado e m·quina atuais
static estado atual; 

// Quantidade e pilha de retornos de sub-m·quina
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
// A√ß√µes Sem√¢nticas
//
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
  	   // LÍ numero de entradas e estados (dimensıes da tabela), 
	   // alÈm do n˙mero de estados finais.
	   fscanf(arqSM, "%d\n", entradas);
	   fscanf(arqSM, "%d\n", estados);
	   fscanf(arqSM, "%d\n", estadosFinais);

	   // LÍ tipos de entradas
	   int* SM_entradas = (int*) calloc(entradas, sizeof(int));
	   for (i = 0; i < entradas; ++i) {
	      fscanf(arqSM, "%d ", SM_entradas[i]);
	   }
	   fscanf(arqSM, "\n");

	   // LÍ transiÁıes 
	   transicao** SM_transicoes = (transicao**) calloc(estados, sizeof (transicao*));
	   for (i = 0; i < estados; ++i) {
	      SM_transicoes[i] = (transicao*) calloc(entradas, sizeof(transicao));
	      for (j = 0; j < entradas; ++j) {
		     fscanf(arqSM, "%d, %d ", (SM_transicoes[i][j]).estado, funcao);
		     converteCodigoFuncao(funcao, SM_transicoes[i][j].acao);
		  }
		  fscanf(arqSM, "\n");
	   }

	   // LÍ estados finais
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

//
// Defini√ß√£o das M√°quinas
//

/* M√°quina programa (P): */
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
    
/* Sub-m√°quina tipo (T): */
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
 * FunÁ„o que define o vetor de m·quinas, bem como a m·quina e estado iniciais
 */
void inicia_submaquinas_hardcoded()
{
	//
    // Cria√ß√£o das m√°quinas no vetor
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

    // M·quina e estado iniciais
    atual.estado = 0;
    atual.maquina = SM_PROGRAMA;
    
    // No inÌcio, n„o h· retornos
    retornos = 0;
    pilha_retornos = (estado *) NULL;
}

void inicia_submaquinas()
{
 	char* arqSM;

	// DefiniÁ„o das M·quinas:

	// Sub-m·quina programa (P):
	arqSM = "subMaquina_Programa.dat";
	defineSubMaquina(arqSM, SM_PROGRAMA);
	
	// Sub-m·quina tipo (T):
	arqSM = "subMaquina_Tipo.dat";
	defineSubMaquina(arqSM, SM_TIPO);

	// Sub-m·quina comando (C):
	arqSM = "subMaquina_Comando.dat";
	defineSubMaquina(arqSM, SM_COMANDO);

	// Sub-m·quina express„o (E):
	arqSM = "subMaquina_Expressao.dat";
	defineSubMaquina(arqSM, SM_EXPRESSAO);
	
	// Sub-m·quina fator (F):
	arqSM = "subMaquina_Fator.dat";
	defineSubMaquina(arqSM, SM_FATOR);
	
	// Sub-m·quina express„o booleana (O):
	arqSM = "subMaquina_ExpressaoBooleana.dat";
	defineSubMaquina(arqSM, SM_EXP_BOOLEANA);

    // M·quina e estado iniciais
    atual.estado = 0;
    atual.maquina = SM_PROGRAMA;
    
    // No inÌcio, n„o h· retornos
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
        
    // Se n√£o encontrar, v√™ se alguma entrada √© sub-m√°quina
    for (i=0; i < maquinas[maq].entradas; i++)
    {
        // √â sub-m√°quina? (entrada negativa)
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

    // L√™ novo √°tomo sem look-ahead
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
    
    // Procura a coluna referente ao √°tomo lido
    col = busca_coluna (atual.maquina, at->classe);

#ifdef DEBUG    
    printf (" coluna %d\n", col);
#endif    

    if (atual.estado > (maquinas[atual.maquina].estados-1))
        return FIM_ERRO_MAQUINAS;
    
    // Executa a√ß√£o sem√¢ntica
    acao = maquinas[atual.maquina].transicoes[atual.estado][col].acao;
    if (acao)
    {
#ifdef DEBUG    
        printf (" a√ß√£o\n");
#endif    
        (*acao) ();
    }
    
    // Verifica se o pr√≥ximo estado √© inv√°lido
    prox_estado = maquinas[atual.maquina].transicoes[atual.estado][col].estado;
    if (prox_estado == ND)
    {
        // O estado atual √© final?
        if (estado_final (atual.maquina, atual.estado))
        {
            // Retorna FIM caso a subm√°quina anterior caso exista
            return !le_retorno ();
        }
        else // Erro de sintaxe
        {
            return FIM_ERRO_SINTATICO;
        }
    }
    else // Pr√≥ximo estado √© v√°lido
    {
        atual.estado = prox_estado;
        
        // Se for entrar numa sub-m√°quina, armazena m√°quina e estado atuais
        // para retornar depois
        tipo_entrada = maquinas[atual.maquina].tipo_entradas[col];        
        if (tipo_entrada < 0)
        {
            adiciona_retorno ();
            
            // Vai para subm√°quina
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

    // Fica consumindo √°tomos at√© ocorrer um erro ou acabar o buffer
    while (!erro)
        if (**entrada == '\0')
            break;
        else
            erro = maquina_sintatico(entrada, fila);
    
    return erro;
}
