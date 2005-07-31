/***************************************************************************
 *            sintatico.h
 *
 *  Sun Jun 26 17:50:03 2005
 *  Copyright  2005  User
 *  Email
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
 
#ifndef __SINTATICO_H__
#define __SINTATICO_H__

#include "atomo.h"

// Estado indefinido
#define ND  -1

/*
 * Tipos
 */
typedef struct s_transicao  transicao;
typedef struct s_estado     estado;
typedef struct s_automato   automato;

/* s_transicao
 * 
 * Estrutura que representa uma transição de estados
 */
struct s_transicao {
    int estado;         // Próximo estado
	int acao;
//    void (*acao)();     // Ação semântica
};

/* s_estado
 *
 * Estrutura que representa o estado e a mÃ¡quina atuais (ou retornos)
 */
struct s_estado {
    int estado;         // Estado atual
    int maquina;        // Máquina atual
};

/* s_automato
 * 
 * Estrutura que representa um autômato
 */
struct s_automato {
    int entradas;           // Qtde de entradas
    int estados;            // Qtd de estados
    int estados_finais;     // Qtd de estados finais
    int *tipo_entradas;     // Tipos de entradas (classes ou sub-mÃ¡quinas)
    transicao **transicoes; // Tabela de transiÃ§Ãµes
    int *estado_final;      // Estados Finais
};

//
// Protótipos
//
int analisadorSintatico (char **entrada);

#endif /* __SINTATICO_H__ */
