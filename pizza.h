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
  int nchaves, folha, *chave, *pizza, *filho, prox;
}TABM;
/* 
  A estrutura de arvore possui as seguintes informações:
    nchaves: armazena o numero de chaves preenchidas no vetor;
    folha: se a arvore é uma folha ou não;
    vetor de chaves: as chaves da arvore q vão ser usadas pra fazer o controle dos limites de cada nó;
    vetor de pizzas: caso seja uma folha o endereço da respectiva pizza no arquivo de pizzas;
    vetor de filhos: caso não seja folha o endereço, para o mesmo arquivo da arvore, do filho daquela posição;
    prox: caso seja uma folha o endereço da proxima pizza dentro do arquivo de pizzas


  OBS: as funçoes abaixo comentadas foram feitas pela vanessa logo, estão funcionando.
*/

TABM *inicializa(void);
TABM *cria(int t);
int salva_no(TABM *a, FILE *out);
TABM *le_no(FILE *in);
int tamanho_no(int t);
void libera(TABM *a);
int busca_pizza(FILE *arvore, int cod);
void busca_categoria(FILE *pizza, char *categoria);
void imprime(FILE *arq, int andar);
void imprime_pizza_end(FILE *pizza, long end);
void altera_pizza(FILE *pizza, int end_pizza, char *nome, char *categoria, float preco);
TABM *divisao(FILE *arvore, TABM *x, int i, TABM *y, int end_y, int t);
void insere_nao_completo(FILE *arvore, FILE * fpizza, TP *pizza, int t);
void insere(FILE *arvore, FILE *fpizza , TP *pizza, int t);
// Imprime pizza
void imprime_pizza(TP *p);
// Cria pizza
TP *pizza(int cod, char *nome, char *categoria, float preco);
// Salva pizza no arquivo out, na posicao atual do cursor
int salva_pizza(TP *p, FILE *out);
// Le uma pizza do arquivo in na posicao atual do cursor
// Retorna um ponteiro para pizza lida do arquivo
TP *le_pizza(FILE *in);
// Compara duas pizzas
// Retorna 1 se os valores dos atributos de ambos forem iguais
// e 0 caso contrario
int cmp_pizza(TP *p1, TP *p2);
// Retorna tamanho do registro que representa a pizza em bytes
int tamanho_pizza_bytes();
