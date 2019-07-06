#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>

typedef struct pz{
    int cod;
    char nome[50];
    char categoria[20];
    float preco;
}TP;

typedef struct arvbm{
  int nchaves, folha, *chave, *pizza, *filho, prox;
}TABM;

//Função de insere n trata se o código já é existente
//Função remove deixando o pai com nchaves = 0 no caso 4

void removeArq(FILE* pizzas, FILE* arvore, int cod, int t);
void alteraEnderecoPizza(FILE *arvore, int cod, int t, int novoEnd);
void diminuiPizzas(FILE* pizzas, int posFim);
TABM *inicializa(void);
TABM *cria(int t);
int salva_no(TABM *a, FILE *out, int t);
TABM *le_no(FILE *in, int t);
int tamanho_no(int t);
void libera(TABM *a);
int busca_pizza(FILE *arvore, int cod, int t);
void busca_categoria(FILE *pizza, FILE* arvore, char *categoria);
void removea(FILE* arvore, FILE* pizzas, int cod, int t);
TABM *buscaPai(FILE* arvore,int cod, int t);
TABM *removeCaso1(FILE* arvore, FILE*pizzas, TABM* a, int cod, int t);
void remove_categoria(FILE *pizza, FILE* arvore, char *categoria, int t);
void imprime(FILE *arq, int andar, int t);
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
