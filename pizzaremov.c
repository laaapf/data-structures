#include "pizza.c"

void removeNo1(FILE *arvore, TABM* a, long end, int posi){
    int i = posi;
    while (i < a->nchaves - 1){
        a->chave[i] = a->chave[i+1];//Anda com o vetor de chaves para a esquerda removendo a chave[i]
    }
    a->nchaves-= 1;
    salva_no(a,arvore);
}

int verificaCaso(FILE* arvore, long end){
    TABM* a = le_no(arvore);
    if(a->folha) 
}