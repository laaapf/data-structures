#include "pizza.h"

TABM *inicializa(){
	return NULL;
}

TABM *cria(int t){
	TABM *novo = (TABM *)malloc(sizeof(TABM));
	novo->nchaves = 0;
	novo->chave = (int *) malloc(sizeof(int)*((2*t)-1));
	novo->pizza = (int *) malloc(sizeof(int)*((2*t)-1));
	novo->folha = 1;
	novo->filho = (int *) malloc(sizeof(int)*(2*t));
	novo->prox = -1;
	int i;
	for (i = 0; i < (t * 2); i++)
		novo->filho[i] = -1;
	for (int i = 0; i < ((2 * t) - 1); i++)
		novo->pizza[i] = -1;
	return novo;
}

int salva_no(TABM *a, FILE *out){
	int resp = ftell(out)
	fwrite(&a->nchaves, sizeof(int), 1, out);
	fwrite(a->chave, sizeof(int), sizeof(a->chave)/sizeof(int) , out);
	fwrite(&a->folha, sizeof(int), 1, out);
	fwrite(a->filho, sizeof(int), sizeof(a->filho)/sizeof(int), out);
	fwrite(a->pizza, sizeof(int), sizeof(a->pizza)/sizeof(int), out);
	fwrite(&a->prox, sizeof(int), 1, out);
	return resp
}

TABM *le_no(FILE *in){
	TABM *a = (TABM *)malloc(sizeof(TABM));
	if (0 >= fread(&a->nchaves, sizeof(int), 1, in)){
		free(a);
		return NULL;
	}
	fread(a->chave, sizeof(int), sizeof(a->chave)/sizeof(int) , in);
	fread(&a->folha, sizeof(int), 1, in);
	fread(a->filho, sizeof(int), sizeof(a->filho)/sizeof(int), in);
	fread(a->pizza, sizeof(int), sizeof(a->pizza)/sizeof(int), in);
	fread(&a->prox, sizeof(int), 1, in);
	return a;
}
int tamanho_no(int t){
	return 3*(sizeof(int)) +	   // nchaves, folha e prox
		2*(sizeof(int) * ((2*t)-1)) + // chaves e pizzas
		sizeof(int) * (2*t);// filhos
}




void libera(TABM *a){
	if (a){
		free(a->chave);
		free(a->filho);
		free(a->pizza);
		free(a);
	}
} 

 int busca_pizza(FILE *arvore, int cod){
	TABM *a = le_no(arvore);
	int i = 0;
	while ((i < a->nchaves) && (cod > a->chave[i]))
		i++; //procura em qual chave a pizza deverá estar
	if ((i < a->nchaves) && (a->folha) && (cod == a->chave[i])){ //caso o indice seja igual o codigo e seja uma folha retorna a pizza encontrada
		return a->pizza[i];
	} 
	if (a->folha)
		return -1; //se for uma folha e n atender a condição de cima, a pizza n está na arvore
	if (a->chave[i] == cod)
		i++; //caso o indice seja igual ao codigo, mas n seja uma folha, procura no filho da direita
	fseek(arvore, a->filho[i], SEEK_SET)
	libera(a);
	return busca_pizza(arvore, cod);
}
 
void altera_pizza(File *pizza, int end_pizza, char *nome, char *categoria, float preco){
	fseek(pizza, end_pizza, SEEK_SET);
	TP* p = le_pizza(pizza);
	strcpy(p->categoria, categoria);
	strcpy(p->nome, nome);
	p->preco = preco;
	fseek(pizza, end_pizza, SEEK_SET);
	salva_pizza(p, pizza);
	return;
}

void busca_categoria(TABM *a, char *categoria){
	if(!a) return;
	while(!a->folha){
		a = a->filho[0];
	}
	for (int i = 0; i < a->nchaves; i++){
		if (strcmp(a->pizza[i]->categoria, categoria) == 0) imprime_pizza(a->pizza[i]);
	}
	busca_categoria(a->prox, categoria);
	
	

}

void imprime(TABM *a, int andar){
	if (a){
		int i, j;
		for (i = 0; i <= a->nchaves - 1; i++){
			imprime(a->filho[i], andar + 1);
			for (j = 0; j <= andar; j++)
				printf("   ");
			if (a->folha)
				imprime_pizza(a->pizza[i]);
			else
				printf("%d\n", a->chave[i]);
		}
		imprime(a->filho[i], andar + 1);
	}
}

TABM *divisao(FILE *arvore, TABM *x, int i, TABM *y, int end_y, int t){
	TABM *z = cria(t);
	z->folha = y->folha;
	int j, end_z;
	if (!y->folha){
		z->nchaves = t - 1;
		for (j = 0; j < t - 1; j++)
			z->chave[j] = y->chave[j + t];
		for (j = 0; j < t; j++){
			z->filho[j] = y->filho[j + t];
			y->filho[j + t] = -1;
		}
		fseek(arvore, 0L, SEEK_END);
		end_z = salva_no(z, arvore);
	}
	else{
		z->nchaves = t; //z possuir� uma chave a mais que y se for folha
		for (j = 0; j < t; j++){
			z->chave[j] = y->chave[j + t - 1]; //Caso em que y � folha, temos q passar a info para o n� da direita
			z->pizza[j] = y->pizza[j + t - 1];
		}
		fseek(arvore, 0L, SEEK_END);
		end_z = salva_no(z);
		y->prox = end_z;
	}
	y->nchaves = t - 1;
	fseek(arvore, end_y, SEEK_SET);
	salva_no(y, arvore);
	for (j = x->nchaves; j >= i; j--)
		x->filho[j + 1] = x->filho[j]; //reorganiza filhos
	x->filho[i] = end_z;
	for (j = x->nchaves; j >= i; j--)
		x->chave[j] = x->chave[j - 1]; //sobe chaves
	x->chave[i - 1] = y->chave[t - 1];
	x->nchaves++;
	return x;
}

void *insere_nao_completo(FILE *arvore, FILE * fpizza, TP *pizza, int t){
	int end_a = ftell(arvore);
	TABM *a = le_no(arvore), *filho;
	int i = a->nchaves - 1;
	if (a->folha){
		while ((i >= 0) && (pizza->cod < a->chave[i])){
			a->chave[i + 1] = a->chave[i];
			i--;
		}
		a->chave[i + 1] = pizza->cod;
		int end_p = salva_pizza(pizza, fpizza);
		a->pizza[i + 1] = end_p;
		a->nchaves++;
		fseek(arvore, end_a, SEEK_SET);
		salva_no(a, arvore);
		return;
	}
	while ((i >= 0) && (pizza->cod < a->chave[i]))
		i--;
	i++;
	fseek(arvore, a->filho[i], SEEK_SET);
	filho = le_no(arvore);
	if (filho->nchaves == ((2 * t) - 1)){
		a = divisao(arvore, a, (i + 1), filho, a->filho[i], t);
		if (pizza->cod > a->chave[i])
			i++;
	}
	fseek(arvore, a->filho[i], SEEK_SET);
	insere_nao_completo(arvore, fpizza, pizza, t);
	return a;
}

void insere(FILE *arvore, FILE *fpizza , TP *pizza, int t){
	int end_pizza = busca_pizza(arvore , pizza->cod);
	if (end_pizza =! -1){
		altera_pizza(fpizza, end_piza, pizza->nome, pizza->categoria, pizza->preco);
		return;	
	}
	TABM *a = le_no(arvore);
	if (!a){
		a = cria(t);
		a->chave[0] = pizza->cod;
		a->pizza[0] = salva_pizza(pizza, fpizza);
		a->nchaves = 1;
		salva_no(a, arvore);
		libera(a);
		return;
	}
	rewind(arvore);
	if (a->nchaves == (2 * t) - 1){
		TABM *S = cria(t);
		S->nchaves = 0;
		S->folha = 0;
		fseek(arvore, 0l, SEEK_END);
		int end_a = salva_no(a, arvore)
		S->filho[0] = end_a;
		S = divisao(arvore, S, 1, a, end_a, t);
		rewind(arvore);
		salva_no(S, arvore);
		rewind(arvore);
		insere_nao_completo(arvore, fpizza, pizza, t);
		return;
	}
	insere_nao_completo(arvore,fpizza, pizza, t);
}

void imprime_pizza(TP *p){
	printf("%d, %s (%s), R$ %.2f\n", p->cod, p->nome, p->categoria, p->preco);
}

TP *pizza(int cod, char *nome, char *categoria, float preco){
	TP *p = (TP *)malloc(sizeof(TP));
	if (p)
		memset(p, 0, sizeof(TP));
	p->cod = cod;
	strcpy(p->nome, nome);
	strcpy(p->categoria, categoria);
	p->preco = preco;
	return p;
}

int salva_pizza(TP *p, FILE *out){
	int resp = ftell(out)
	fwrite(&p->cod, sizeof(int), 1, out);
	fwrite(p->nome, sizeof(char), sizeof(p->nome), out);
	fwrite(p->categoria, sizeof(char), sizeof(p->categoria), out);
	fwrite(&p->preco, sizeof(float), 1, out);
	return resp;
}

TABM *le_dados(char* dados, int t){
	FILE *fpd = fopen(dados, "rb+");
	if(!fpd) exit(1);
	TABM *a = inicializa();
	TP *p;
	while (p){
		p = le_pizza(fpd);
		a = insere(a,p,t);
	}
	free(p);
	return a;
}

TP *le_pizza(FILE *in){
	TP *p = (TP *)malloc(sizeof(TP));
	if (0 >= fread(&p->cod, sizeof(int), 1, in)){
		free(p);
		return NULL;
	}
	fread(p->nome, sizeof(char), sizeof(p->nome), in);
	fread(p->categoria, sizeof(char), sizeof(p->categoria), in);
	fread(&p->preco, sizeof(float), 1, in);
	return p;
}

int cmp_pizza(TP *p1, TP *p2){
	if (p1 == NULL){
		return (p2 == NULL);
	}
	if (p1->cod != p2->cod){
		return 0;
	}
	if (strcmp(p1->nome, p2->nome) != 0){
		return 0;
	}
	if (strcmp(p1->categoria, p2->categoria) != 0){
		return 0;
	}
	if (p1->preco != p2->preco){
		return 0;
	}
	return 1;
} 

int tamanho_pizza_bytes(){
	return sizeof(int) +	   // cod
		   sizeof(char) * 50 + // nome
		   sizeof(char) * 20 + // categoria
		   sizeof(float);	  // preço
}

int main(void){
	TABM *a = inicializa();
	a = cria(2);

}
