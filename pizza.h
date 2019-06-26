#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct pz{
    int cod;
    char nome[50];
    char categoria[20];
    float preco;
}TP;

typedef struct arvbm{
  int nchaves, folha, *chave;
  TP **pizza;
  struct arvbm **filho, *prox;
}TABM;


TABM *cria(int t);
TABM *inicializa(void);
void libera(TABM *a);
TABM *busca(TABM *a, int mat);
void imprime(TABM *a, int andar);
TABM *divisao(TABM *x, int i, TABM* y, int t);
TABM *insere_nao_completo(TABM *a, int cod, char* nome, char* categoria, float preco, int t);
TABM *insere(TABM *a, int cod, char* nome, char* categoria,float preco, int t);
// Imprime pizza
void imprime_pizza(TP *p);
// Cria pizza
TP *pizza(int cod, char *nome, char *categoria, float preco);
// Salva pizza no arquivo out, na posicao atual do cursor
void salva_pizza(TP *p, FILE *out);
// Le uma pizza do arquivo in na posicao atual do cursor
// Retorna um ponteiro para pizza lida do arquivo
TP *le_pizza(FILE *in);
// Compara duas pizzas
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_pizza(TP *p1, TP *p2);
// Retorna tamanho do registro que representa a pizza em bytes
int tamanho_pizza_bytes();
