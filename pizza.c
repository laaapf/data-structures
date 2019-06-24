#include "pizza.h"


TABM *cria(int t){
  TABM* novo = (TABM*)malloc(sizeof(TABM));
  novo->nchaves = 0;
  novo->chave =(int*)malloc(sizeof(int)*((t*2)-1));
  novo->pizza = (TP**)malloc(sizeof(TP*)*((t*2)-1));
  novo->folha = 1;
  novo->filho = (TABM**)malloc(sizeof(TABM*)*t*2);
  novo->prox = NULL;
  int i;
  for(i=0; i<(t*2); i++) novo->filho[i] = NULL;
  for (int i = 0; i < ((2*t)-1); i++) novo->pizza[i] = NULL;
  return novo;
}


TABM *inicializa(void){
  return NULL;
}

void libera(TABM *a){
  if(a){
    if(!a->folha){
      int i;
      for(i = 0; i <= a->nchaves; i++) libera(a->filho[i]);
    }
    else{
        for (int i = 0; i < a->nchaves; i++) free(a->pizza[i]);
        free(a->pizza);
    }
    free(a->chave);
    free(a);
  }
}

TP *busca_pizza(TABM *a, int cod){
  if (!a) return NULL;
  int i = 0;
  while ((i < a->nchaves) && (cod > a->chave[i])) i++; //procura em qual chave a pizza deverá estar
  if ((i < a->nchaves) && (a->folha) && (cod == a->chave[i])) return a->pizza[i];//caso o indice seja igual o codigo e seja uma folha retorna a pizza encontrada
  if (a-> folha) return NULL;//se for uma folha e n atender a condição de cima, a pizza n está na arvore
  if (a->chave[i] == cod) i++;//caso o indice seja igual ao codigo, mas n seja uma folha, procura no filho da direita
  return busca_pizza(a->filho[i], cod);
}

void imprime(TABM *a, int andar){
  if(a){
    int i,j;
    for(i=0; i<=a->nchaves-1; i++){
      imprime(a->filho[i],andar+1);
      for(j=0; j<=andar; j++) printf("   ");
      imprime_pizza(a->pizza[i])
    }
    imprime(a->filho[i],andar+1);
  }
}

void imprime_pizza(TP *p){
	printf("%d, %s (%s), R$ %.2f\n", p->cod, p->nome, p->descricao, p->preco);
}


TP *pizza(int cod, char *nome, char *descricao, float preco){
	TP *p = (TP *) malloc(sizeof(TP));
	if (p) memset(p, 0, sizeof(TP));
	p->cod = cod;
	strcpy(p->nome, nome);
	strcpy(p->descricao, descricao);
	p->preco = preco;
	return p;
}


void salva_pizza(TP *p, FILE *out){
	fwrite(&p->cod, sizeof(int), 1, out);
	fwrite(p->nome, sizeof(char), sizeof(p->nome), out);
	fwrite(p->descricao, sizeof(char), sizeof(p->descricao), out);
	fwrite(&p->preco, sizeof(float), 1, out);
}


TP *le_pizza(FILE *in){
	TP *p = (TP *) malloc(sizeof(TP));
	if (0 >= fread(&p->cod, sizeof(int), 1, in)) {
		free(p);
		return NULL;
	}
	fread(p->nome, sizeof(char), sizeof(p->nome), in);
	fread(p->descricao, sizeof(char), sizeof(p->descricao), in);
	fread(&p->preco, sizeof(float), 1, in);
	return p;
}


int cmp_pizza(TP *p1, TP *p2){
	if (p1 == NULL) {
		return (p2 == NULL);
	}
	if (p1->cod != p2->cod) {
		return 0;
	}
	if (strcmp(p1->nome, p2->nome) != 0) {
		return 0;
	}
	if (strcmp(p1->descricao, p2->descricao) != 0) {
		return 0;
	}
	if (p1->preco != p2->preco) {
		return 0;
	}
	return 1;
}


int tamanho_pizza_bytes(){
	return sizeof(int) + // cod
		sizeof(char) * 50 + // nome
		sizeof(char) * 20 + // categoria
		sizeof(float); // preço
}

/* 
TABM *divisao(TABM *x, int i, TABM* y, int t){
  TABM *z = cria(t);
  z->folha = y->folha;
  int j;
  if(!y->folha){
    z->nchaves = t-1;
    for(j=0;j<t-1;j++) z->chave[j] = y->chave[j+t];
    for(j=0;j<t;j++){
      z->filho[j] = y->filho[j+t];
      y->filho[j+t] = NULL;
    }
  }
  else {
    z->nchaves = t; //z possuir� uma chave a mais que y se for folha
    for(j=0;j < t;j++) z->chave[j] = y->chave[j+t-1];//Caso em que y � folha, temos q passar a info para o n� da direita
    y->prox = z;
  }
  y->nchaves = t-1;
  for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
  x->filho[i] = z;
  for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
  x->chave[i-1] = y->chave[t-1];
  x->nchaves++;
  return x;
}


TABM *insere_nao_completo(TABM *x, int mat, int t){
  int i = x->nchaves-1;
  if(x->folha){
    while((i>=0) && (mat < x->chave[i])){
      x->chave[i+1] = x->chave[i];
      i--;
    }
    x->chave[i+1] = mat;
    x->nchaves++;
    return x;
  }
  while((i>=0) && (mat < x->chave[i])) i--;
  i++;
  if(x->filho[i]->nchaves == ((2*t)-1)){
    x = divisao(x, (i+1), x->filho[i], t);
    if(mat > x->chave[i]) i++;
  }
  x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
  return x;
}

TABM *insere(TABM *T, int mat, int t){
  if(busca_pizza(T, mat)) return T;
  if(!T){
    T=cria(t);
    T->chave[0] = mat;
    T->nchaves=1;
    return T;
  }
  if(T->nchaves == (2*t)-1){
    TABM *S = cria(t);
    S->nchaves=0;
    S->folha = 0;
    S->filho[0] = T;
    S = divisao(S,1,T,t);
    S = insere_nao_completo(S, mat, t);
    return S;
  }
  T = insere_nao_completo(T, mat, t);
  return T;
}
 */