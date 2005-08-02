/***************************************************************************
 *            compilador.c
 *
 *  Fri Jul 22 12:12:09 2005
 *  Copyright  2005  André Pinto e Igor Arouca
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
 
#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "defs.h"
#include "arquivo.h"
#include "sintatico.h"
#include "semantico.h"
#include "erro.h"

// Flags
extern int falante;
extern int depurando;
extern int espacado;
extern int tecla;

int le_param (int argc, char **argv)
{
    int arq = 1;
    
    if (argc == 1)
        sair (FIM_ERRO_PARAMETRO);
    
    while ((arq < argc) && argv[arq][0]=='-')
    {
        switch (argv[arq][1])
        {
            case 't':
                tecla = VERDADE;
                break;

            case 'e':
                espacado = VERDADE;
            case 'd':
                depurando = VERDADE;
            case 'v':
                falante = VERDADE;
                break;
        }
        arq++;
    }
    return arq;
}

int main (int argc, char **argv)
{
    char *dados, *pos;
    int erro, arq;

    arq = le_param (argc, argv);
    dados = le_arquivo (argv[arq]);

    if (dados == NULL)
        sair (FIM_ERRO_ARQUIVO);

    ajusta_nome_arquivo (argv[arq]);

    pos = dados;
    erro = analisadorSintatico (&pos);

    sair (erro);
    return erro;
}
