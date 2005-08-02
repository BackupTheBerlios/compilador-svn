/***************************************************************************
 *            erro.h
 *
 *  Sun Jun 26 19:48:18 2005
 *  Copyright  2005  André Pinto    
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
 
#ifndef _ERRO_H
#define _ERRO_H

#include <stdio.h>

// Flags
extern int falante;
extern int depurando;
extern int espacado;

// Modos de encerramento
enum fins {
    FIM_OK,
    
    FIM_ERRO_PARAMETRO,
    FIM_ERRO_ARQUIVO,
    
    FIM_ERRO_LEXICO,
    FIM_ERRO_SINTATICO,
    FIM_ERRO_MAQUINAS
};


// Macro que imprime apenas quando o flag falante está setado
#define IMPRIME(x...)                       \
    if (falante)                            \
    {                                       \
        printf(x);                          \
    };

// Macros para depuração
#define DEPURA(fmt,...)                     \
    if (depurando)                          \
    {                                       \
        if (!espacado && fmt[0]==' ')       \
            IMPRIME(",");                   \
        IMPRIME (fmt, ##__VA_ARGS__);       \
        if (espacado)                       \
            IMPRIME ("\n");                 \
    };
#define DEPURA_FIM()                        \
    if (depurando && !espacado)             \
    {                                       \
        IMPRIME ("\n");                     \
    };



//
// Funções públicas
//
int mostra_linha_atual  (char *);
void mostra_posicao_erro (char *);
void sair (int);
    
#endif /* _ERRO_H */
