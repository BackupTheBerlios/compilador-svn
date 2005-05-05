#ifndef __TABID_H__

#define __TABID_H__

// Estrutura que representa um identificador
struct s_id {
   int cod;
   char * nome;
//   int linha; // primeira ocorrência (opcional)
};

int adicionaID (char * nome);
int tam_tabelaID ();
int busca_cod_ID (char * nome);
char * busca_nome_ID (int cod);

#endif // __TABID_H__
