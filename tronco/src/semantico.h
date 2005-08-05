/***************************************************************************
 *            semantico.h
 *
 *  Tue Aug  2 16:39:06 2005
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
 
#ifndef _SEMANTICO_H
#define _SEMANTICO_H

#include "atomo.h"

//
// Tipos 
//
enum tipo_var {VAR_GLOBAL, VAR_LOCAL, VAR_PARAM};
enum tipo_item {NOME, VALOR, CODIGO,                    // Operandos
                OPERADOR, CHAMADA, PARAM, ABRE_PAREN};  // Operadores
typedef struct s_item   *um_item;
typedef struct s_pilha  uma_pilha;
typedef struct s_item   um_operando;
    
struct s_pilha
{
//    enum tipo_var tipo;
	int tamanho;
	um_item *item;
};
struct s_item
{
    enum tipo_item tipo;
    int valor;
    char *nome;
};


/*
 * Protótipos de funções de manipulação da pilha
 */
void pilha_inicia           (uma_pilha *);
int pilha_eh_vazia          (uma_pilha *);
int pilha_adiciona          (uma_pilha *, enum tipo_item, int, char*);
um_item pilha_retira        (uma_pilha *);
um_item pilha_consulta      (uma_pilha *);
void pilha_esvazia          (uma_pilha *);
int pilha_busca             (uma_pilha *, char *);


/*
 * Protótipos de funções de ajustes
 */
void ajusta_nomes_arquivos (char *, char *);

/*
 * Protótipos de funções de ação semântica
 */
void acao_programa_inicio (um_atomo);                   // 01
void acao_programa_funcao_inicio (um_atomo);            // 02
void acao_programa_funcao_nome (um_atomo);              // 03
void acao_programa_funcao_adiciona_param (um_atomo);    // 04
void acao_programa_comando (um_atomo);                  // 05
void acao_programa_funcao_fim (um_atomo);               // 06

void acao_tipo_tamanho_var (um_atomo);                  // 07

void acao_comando_declara_adiciona (um_atomo);          // 08
void acao_comando_declara_fim (um_atomo);               // 09
void acao_comando_id (um_atomo);                        // 10
void acao_comando_atrib (um_atomo);                     // 11
void acao_comando_chamada (um_atomo);                   // 12
void acao_comando_param (um_atomo);                     // 16
void acao_comando_fecha_paren (um_atomo);               // 25
void acao_comando_fim (um_atomo);                       // 13
void acao_comando_output (um_atomo);                    // 23
void acao_comando_input (um_atomo);                     // 24

void acao_fator_id (um_atomo);                          // 17
void acao_fator_operando (um_atomo);                    // 14
void acao_fator_chamada (um_atomo);                     // 18
void acao_fator_param (um_atomo);                       // 19
void acao_fator_abre_paren (um_atomo);                  // 20
void acao_fator_fecha_paren (um_atomo);                 // 21
void acao_fator_nega (um_atomo);                        // 22

void acao_expressao_operador (um_atomo);                // 15


#endif /* _SEMANTICO_H */
