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
 
/*
 * defs.h
 *
 * Defini��es de tipos, constantes etc.
 */

#ifndef __DEFS_H__

#define __DEFS_H__

// Depurações
#define DEBUG_SINTATICO
//#define DEBUG_ARQUIVO

// Booleanos
#define VERDADE                 1
#define FALSO                   0

// Códigos de erro (busca, adições etc.)
#define ERRO                    -1

// Modos de encerramento
enum fins {
    FIM_OK,
    
    FIM_ERRO_PARAMETRO,
    FIM_ERRO_ARQUIVO,
    
    FIM_ERRO_LEXICO,
    FIM_ERRO_SINTATICO,
    FIM_ERRO_MAQUINAS
};

#endif // __DEFS_H__
