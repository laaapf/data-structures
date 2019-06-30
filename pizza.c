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
	for (i = 0; i < (t * 2); i++){
		novo->filho[i] = -1;
	}
	for (int i = 0; i < ((2 * t) - 1); i++){
		novo->pizza[i] = -1;
	}
	return novo;
}

int salva_no(TABM *a, FILE *out){
	int resp = ftell(out);
	fwrite(&a->nchaves, sizeof(int), 1, out);
	fwrite(a->chave, sizeof(int), sizeof(a->chave)/sizeof(int) , out);
	fwrite(&a->folha, sizeof(int), 1, out);
	fwrite(a->filho, sizeof(int), sizeof(a->filho)/sizeof(int), out);
	fwrite(a->pizza, sizeof(int), sizeof(a->pizza)/sizeof(int), out);
	fwrite(&a->prox, sizeof(int), 1, out);
	return resp;
}

TABM *le_no(FILE *in){
	TABM *a = cria(2); 
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
	if(!a){
		return -1;
	} 
	int i = 0;
	while ((i < a->nchaves) && (cod >= a->chave[i]))
		i++; //procura em qual chave a pizza deverá estar
	if ((i < a->nchaves) && (a->folha) && (cod == a->chave[i])){ //caso o indice seja igual o codigo e seja uma folha retorna a pizza encontrada
		return a->pizza[i];
	} 
	if (a->folha)
		return -1; //se for uma folha e n atender a condição de cima, a pizza n está na arvore
	if (a->chave[i] == cod)
		i++; //caso o indice seja igual ao codigo, mas n seja uma folha, procura no filho da direita
	fseek(arvore, a->filho[i], SEEK_SET);
	libera(a);
	return busca_pizza(arvore, cod);
}
 
void altera_pizza(FILE *pizza, int end_pizza, char *nome, char *categoria, float preco){
	fseek(pizza, end_pizza, SEEK_SET);
	TP* p = le_pizza(pizza);
	strcpy(p->categoria, categoria);
	strcpy(p->nome, nome);
	p->preco = preco;
	fseek(pizza, end_pizza, SEEK_SET);
	salva_pizza(p, pizza);
	return;
}

void busca_categoria(FILE *pizza, FILE *arvore, char *categoria){
	long pos_cat = 0;
	long pos_ini = 0;
	char* cat_atual;
	cat_atual = (char *) malloc(sizeof(char) * 20);
	while(1){
		pos_cat = pos_ini + sizeof(int) + sizeof(char) *50;
		fseek(pizza,pos_cat,SEEK_SET);
		int i = fread(cat_atual,sizeof(char)*20,1,pizza);
		if(i<=0) break;
		if(strcmp(cat_atual,categoria) == 0) {
			fseek(pizza,pos_ini,SEEK_SET);
			imprime_pizza(le_pizza(pizza));
		}		
		pos_ini +=(long) tamanho_pizza_bytes();
	}
	free(cat_atual);
}

TABM *buscaPai(FILE* arvore,int cod){
	TABM *a = le_no(arvore);
	if(!a){
		return NULL;
	} 
	int i = 0;
	while ((i < a->nchaves) && (cod >= a->chave[i]))
		i++; //procura em qual chave a pizza deverá estar
	fseek(arvore,a->filho[i],SEEK_SET);
	TABM* filhoI = le_no(arvore);
	if ((filhoI->folha)){
        int j = 0;
        while ((j < filhoI->nchaves) && (cod > filhoI->chave[j]))
            j++;
        if ((j < filhoI->nchaves) && (cod == filhoI->chave[j])){
            int posfim = ftell(arvore);
			posfim -= sizeof(TABM);
			fseek(arvore,posfim,SEEK_SET);
			return a;
		}
    }
	if (a->folha)
		return NULL; //se for uma folha e n atender a condição de cima, a pizza n está na arvore
	if (a->chave[i] == cod)
		i++; //caso o indice seja igual ao codigo, mas n seja uma folha, procura no filho da direita
	fseek(arvore, a->filho[i], SEEK_SET);
	libera(a);
	libera(filhoI);
	return buscaPai(arvore, cod);
}

TABM *removeCaso1(FILE* arvore, FILE*pizzas, TABM* a,int cod){
	//o cursor do arquivo arvore deve ta no inicio da pizza a
	int i = 0;
    int achou = 0;
    while (i < a->nchaves - 1){
        if (cod == a->chave[i])
            achou = 1;
        if (achou){
            a->chave[i] = a->chave[i + 1];
        }
        i++;
    }
    a->nchaves -= 1;
	int pos_ini = salva_no(a,arvore);
	//Falta tirar do arquivo de pizzas
}
void removea(FILE* arvore, FILE* pizzas, int cod, int t){
	TABM* a = buscaPai(arvore,cod);
	if(!a) printf("pizza não encontrada");
	int i = 0;
	int pospai = ftell(arvore);
	while ((i < a->nchaves) && (cod >= a->chave[i]))
        i++;
	fseek(arvore,a->filho[i],SEEK_SET);
	int posFilhoI = ftell(arvore);
	TABM* filhoI = le_no(arvore); //filho[i]
	if (filhoI->nchaves >= t){ //caso 1  i=2
        printf("caso1");
        filhoI = removeCaso1(arvore,pizzas,filhoI, cod);
		fseek(arvore,posFilhoI,SEEK_SET);
		salva_no(filhoI,arvore);
		libera(a);
		libera(filhoI);
        return;
    }
	if (filhoI->nchaves == t - 1){
		fseek(arvore,a->filho[i-1],SEEK_SET);
		int posFilhoAntesI = ftell(arvore);
		TABM* filhoAntesI = le_no(arvore);
        if((i!=0)&&(filhoAntesI->nchaves >= t)){ //caso 2
            printf("caso2");
            int maior = filhoAntesI->chave[filhoAntesI->nchaves - 1];
            filhoAntesI->nchaves -= 1;
            filhoI = removeCaso1(arvore, pizzas, filhoI, cod);
            int k = filhoI->nchaves;
            while (k > 0){
                filhoI->chave[k] = filhoI->chave[k - 1];
                k--;
            }
            filhoI->chave[k] = maior;
            filhoI->nchaves += 1;
            a->chave[i - 1] = maior;
			fseek(arvore,pospai,SEEK_SET);
			pospai = salva_no(a,arvore);
			fseek(arvore,posFilhoI,SEEK_SET);
			posFilhoI = salva_no(filhoI,arvore);
			fseek(arvore,posFilhoAntesI,SEEK_SET);
			posFilhoAntesI = salva_no(filhoAntesI,arvore);
			libera(a);
			libera(filhoI);
			libera(filhoAntesI);
            return;
        }
		fseek(arvore,a->filho[i+1],SEEK_SET);
		int posFilhoDepoisI = ftell(arvore);
		TABM* filhoDepoisI = le_no(arvore);
		if ((i!=(2*t))&&(filhoDepoisI->nchaves >= t)){ //caso 3
            printf("caso3");
            int menor = filhoDepoisI->chave[0];
            filhoDepoisI = removeCaso1(arvore, pizzas, filhoDepoisI, menor);
            int subir = filhoDepoisI->chave[0];
            a->chave[i] = subir;
            filhoI = removeCaso1(arvore, pizzas, filhoI, cod);
            int k = filhoI->nchaves;
            filhoI->chave[k] = menor;
            filhoAntesI->nchaves += 1;
			fseek(arvore,pospai,SEEK_SET);
			pospai = salva_no(a,arvore);
			fseek(arvore,posFilhoI,SEEK_SET);
			posFilhoI = salva_no(filhoI,arvore);
			fseek(arvore,posFilhoDepoisI,SEEK_SET);
			posFilhoDepoisI = salva_no(filhoDepoisI,arvore);
			libera(a);
			libera(filhoI);
			libera(filhoDepoisI);
			libera(filhoAntesI);
            return;
        }else if (i != 0){ // caso 4
            printf("caso4");
            filhoI = removeCaso1(arvore, pizzas, filhoI, cod);
            int x = filhoAntesI->nchaves; //posição no vetor esq
            int b = 0;                        //posição no vetor que retirou o elemento
            int y = filhoI->nchaves;     //tamanho do vetor que retirou o elemento
            while (b < y){
                filhoAntesI->chave[x] = filhoI->chave[b];
                b++;
                x++;
            }
            filhoI->nchaves = 0;
            filhoAntesI->nchaves += y;
            int k = a->nchaves;
            int j = i - 1;
            while (j < k - 1){
                a->chave[j] = a->chave[j + 1];
                a->filho[j + 1] = a->filho[j + 2];
                j++;
            }
            a->nchaves--;
            filhoAntesI->prox = a->filho[i];
			fseek(arvore,pospai,SEEK_SET);
			pospai = salva_no(a,arvore);
			fseek(arvore,posFilhoI,SEEK_SET);
			posFilhoI = salva_no(filhoI,arvore);
			fseek(arvore,posFilhoAntesI,SEEK_SET);
			posFilhoAntesI = salva_no(filhoAntesI,arvore);
			libera(a);
			libera(filhoI);
			libera(filhoDepoisI);
			libera(filhoAntesI);
        }
		else if (i != 2 * t) { //caso 5
            printf("caso5");
            filhoI = removeCaso1(arvore, pizzas, filhoI, cod);
            int x = filhoDepoisI->nchaves; //posição no vetor dir
            int b = 0;                        //posição no vetor que retirou o elemento
            int y = filhoI->nchaves;     //tamanho do vetor que retirou o elemento
            int z = filhoDepoisI->nchaves;
            filhoDepoisI->nchaves += y;
            while (b < z){
                filhoDepoisI->chave[x] = filhoDepoisI->chave[x - 1];
                x--;
                b++;
            }
            b = 0;
            while (b < y){
                filhoDepoisI->chave[b] = filhoI->chave[b];
                b++;
            }
            filhoI->nchaves = 0;
            b = 0;
            while (b < a->nchaves)
            {
                a->chave[b] = a->chave[b + 1];
                a->filho[b] = a->filho[b + 1];
                b++;
            }
            a->nchaves -= 1;
			filhoAntesI->prox = a->filho[i];
			fseek(arvore,pospai,SEEK_SET);
			pospai = salva_no(a,arvore);
			fseek(arvore,posFilhoI,SEEK_SET);
			posFilhoI = salva_no(filhoI,arvore);
			fseek(arvore,posFilhoAntesI,SEEK_SET);
			posFilhoAntesI = salva_no(filhoAntesI,arvore);
			fseek(arvore,posFilhoDepoisI,SEEK_SET);
			posFilhoDepoisI = salva_no(filhoDepoisI,arvore);
			libera(a);
			libera(filhoI);
			libera(filhoDepoisI);
			libera(filhoAntesI);
        }


	}
}

void remove_categoria(FILE *pizza, FILE *arvore, char *categoria){
	long pos_cat = 0;
	long pos_ini = 0;
	char* cat_atual;
	cat_atual = (char *) malloc(sizeof(char) * 20);
	while(1){
		pos_cat = pos_ini + sizeof(int) + sizeof(char) *50;
		fseek(pizza,pos_cat,SEEK_SET);
		int i = fread(cat_atual,sizeof(char)*20,1,pizza);
		if(i<=0) break;
		if(strcmp(cat_atual,categoria) == 0) {
			int cod;
			int i = fread(&cod,sizeof(int),1,arvore);
			//remove no cod
		}		
		pos_ini +=(long) tamanho_pizza_bytes();
	}
	free(cat_atual);
}

void imprime(FILE *arq, int andar){
	rewind(arq);
	TABM* a = le_no(arq);
	int i, j;
	if(a){
		for(i = 0; i<=a->nchaves - 1; i++){
			fseek(arq,a->filho[i],SEEK_SET);
			imprime(arq,andar + 1);
			for(int j = 0; j<= andar; j++){
				printf("    ");
			}
			if(a->folha) imprime_pizza_end(arq,a->pizza[i]);
			else printf("%d\n",a->chave[i]);
		}
		fseek(arq,a->filho[i],SEEK_SET);
		imprime(arq,andar+1);
	}
	libera(a);
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
		end_z = salva_no(z, arvore);
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

void insere_nao_completo(FILE *arvore, FILE * fpizza, TP *pizza, int t){
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
}

void insere(FILE *arvore, FILE *fpizza , TP *pizza, int t){
	int end_pizza = busca_pizza(arvore , pizza->cod);
	if (end_pizza =! -1){
		altera_pizza(fpizza, end_pizza, pizza->nome, pizza->categoria, pizza->preco);
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
		int end_a = salva_no(a, arvore);
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

void imprime_pizza_end(FILE *pizza, long end){
	fseek(pizza,end,SEEK_SET);
	TP *p = (TP *)malloc(sizeof(TP));
	int i = fread(p, tamanho_pizza_bytes(), 1,pizza);
	if(i>0) imprime_pizza(p);
	free(p);
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
	int resp = ftell(out);
	fwrite(&p->cod, sizeof(int), 1, out);
	fwrite(p->nome, sizeof(char), sizeof(p->nome), out);
	fwrite(p->categoria, sizeof(char), sizeof(p->categoria), out);
	fwrite(&p->preco, sizeof(float), 1, out);
	return resp;
}
/* 
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
} */

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
	int op = 0;
	int t;
	printf("Digite o fator de ramificação");
	scanf("%i", &t);
	FILE* pizzas = fopen("dados_iniciais.dat","rb+");
	FILE* arvore = fopen("arvore.dat","wb");
	if(!arvore) exit(1);
	if(!pizzas) exit(1);
	while(op != -1){
		imprime(arvore, 0);
		printf("Digite:\n\t0 para inserir uma pizza\n\t1 para remover uma pizza\n\t2 para buscar uma pizza com base no codigo\n\t3 para buscar todas as pizzas de uma categoria\n\t4 remover todas as pizzas de uma categoria\n\t-1 para sair\n");
		scanf("%i", &op);
		if(op == 0){
			int cod;
			char *nome = (char *) malloc(sizeof(char) * 50);
			char* categoria = (char *) malloc(sizeof(char) * 20);;
			float preco;
			printf("Digite o codigo:");
			scanf("%d",&cod);
			printf("Digite o nome:");
			scanf("%s",nome);
			printf("Digite a categoria:");
			scanf("%s",categoria);
			printf("Digite o preço:");
			scanf("%f",&preco);
			TP *p = pizza(cod,nome,categoria,preco);
			insere(arvore, pizzas, p, t);
		}else if(op == 1){
			int cod;
			printf("Digite o codigo da pizza a ser removida:");
			scanf("%d",&cod);
			//chamar remove
		}else if(op == 2){
			int cod;
			printf("Digite o codigo da pizza a ser buscada:");
			scanf("%d",&cod);
			int end = busca_pizza(arvore,cod);
			imprime_pizza_end(pizzas,end);
		}else if(op == 3){
			char* categoria = (char *) malloc(sizeof(char) * 20);;
			printf("Digite a categoria a ser buscada:");
			scanf("%s",categoria);
			busca_categoria(pizzas,arvore,categoria);
		}else if(op == 4){
			char* categoria = (char *) malloc(sizeof(char) * 20);
			printf("Digite a categoria a ser removida:");
			scanf("%s",categoria);
			remove_categoria(pizzas,arvore,categoria);
		}else{
			fclose(pizzas);
			fclose(arvore);
		}
	}
	fclose(pizzas);
}
