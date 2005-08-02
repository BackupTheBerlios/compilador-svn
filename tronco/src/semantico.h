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

//
// Tipos privados
//
enum tipo_var {VAR_GLOBAL, VAR_LOCAL};

typedef struct s_variavel    *uma_variavel;
typedef struct s_pilha_var   uma_pilha;
    
struct s_variavel
{
    union {
        int tamanho;
        int posicao;
    };
	char *nome;
};
struct s_pilha_var
{
    enum tipo_var tipo;
	int tamanho;
	uma_variavel *variavel;
};


/*
 * Protótipos de funções de manipulação da pilha
 */
void pilha_inicia           (uma_pilha *, enum tipo_var);
int pilha_eh_vazia          (uma_pilha *);
int pilha_adiciona          (uma_pilha *, int, char*);
uma_variavel pilha_retira   (uma_pilha *);
void pilha_esvazia          (uma_pilha *);


/*
 * Protótipos de funções de ajustes
 */
void ajusta_nome_arquivo (char *);

/*
 * Protótipos de funções de ação semântica
 */
void acao_inicio (um_atomo);
void acao_fim (um_atomo);

void acao_ajusta_tamanho_var (um_atomo);
void acao_adiciona_global (um_atomo);


#endif /* _SEMANTICO_H */
