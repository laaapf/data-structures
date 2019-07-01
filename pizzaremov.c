#include <stdio.h>
#include <stdlib.h>

const int t = 2;

typedef struct arvbm
{
    int nchaves, folha, *chave;
    struct arvbm **filho, *prox;
} TA;

TA *cria(int t)
{
    TA *novo = (TA *)malloc(sizeof(TA));
    novo->nchaves = 0;
    novo->chave = (int *)malloc(sizeof(int) * ((t * 2) - 1));
    novo->folha = 1;
    novo->filho = (TA **)malloc(sizeof(TA *) * t * 2);
    novo->prox = NULL;
    int i;
    for (i = 0; i < (t * 2); i++)
        novo->filho[i] = NULL;
    return novo;
}

TA *inicializa(void)
{
    return NULL;
}

void libera(TA *a)
{
    if (a)
    {
        if (!a->folha)
        {
            int i;
            for (i = 0; i <= a->nchaves; i++)
                libera(a->filho[i]);
        }
        free(a->chave);
        free(a);
    }
}

TA *busca(TA *a, int mat)
{
    if (!a)
        return NULL;
    int i = 0;
    while ((i < a->nchaves) && (mat > a->chave[i]))
        i++;
    if ((i < a->nchaves) && (a->folha) && (mat == a->chave[i]))
        return a;
    if (a->folha)
        return NULL;
    if (a->chave[i] == mat)
        i++;
    return busca(a->filho[i], mat);
}

void imprime(TA *a, int andar)
{
    if (a)
    {
        int i, j;
        for (i = 0; i <= a->nchaves - 1; i++)
        {
            imprime(a->filho[i], andar + 1);
            for (j = 0; j <= andar; j++)
                printf("   ");
            printf("%d\n", a->chave[i]);
        }
        imprime(a->filho[i], andar + 1);
    }
}
TA *divisao(TA *x, int i, TA *y, int t)
{
    TA *z = cria(t);
    z->folha = y->folha;
    int j;
    if (!y->folha)
    {
        z->nchaves = t - 1;
        for (j = 0; j < t - 1; j++)
            z->chave[j] = y->chave[j + t];
        for (j = 0; j < t; j++)
        {
            z->filho[j] = y->filho[j + t];
            y->filho[j + t] = NULL;
        }
    }
    else
    {
        z->nchaves = t; //z possuir� uma chave a mais que y se for folha
        for (j = 0; j < t; j++)
            z->chave[j] = y->chave[j + t - 1]; //Caso em que y � folha, temos q passar a info para o n� da direita
        y->prox = z;
    }
    y->nchaves = t - 1;
    for (j = x->nchaves; j >= i; j--)
        x->filho[j + 1] = x->filho[j];
    x->filho[i] = z;
    for (j = x->nchaves; j >= i; j--)
        x->chave[j] = x->chave[j - 1];
    x->chave[i - 1] = y->chave[t - 1];
    x->nchaves++;
    return x;
}

TA *insere_nao_completo(TA *x, int mat, int t)
{
    int i = x->nchaves - 1;
    if (x->folha)
    {
        while ((i >= 0) && (mat < x->chave[i]))
        {
            x->chave[i + 1] = x->chave[i];
            i--;
        }
        x->chave[i + 1] = mat;
        x->nchaves++;
        return x;
    }
    while ((i >= 0) && (mat < x->chave[i]))
        i--;
    i++;
    if (x->filho[i]->nchaves == ((2 * t) - 1))
    {
        x = divisao(x, (i + 1), x->filho[i], t);
        if (mat > x->chave[i])
            i++;
    }
    x->filho[i] = insere_nao_completo(x->filho[i], mat, t);
    return x;
}

TA *insere(TA *T, int mat, int t)
{
    if (busca(T, mat))
        return T;
    if (!T)
    {
        T = cria(t);
        T->chave[0] = mat;
        T->nchaves = 1;
        return T;
    }
    if (T->nchaves == (2 * t) - 1)
    {
        TA *S = cria(t);
        S->nchaves = 0;
        S->folha = 0;
        S->filho[0] = T;
        S = divisao(S, 1, T, t);
        S = insere_nao_completo(S, mat, t);
        return S;
    }
    T = insere_nao_completo(T, mat, t);
    return T;
}

TA *buscaPai(TA *a, int mat)
{
    if (!a)
        return NULL;
    int i = 0;
    while ((i < a->nchaves) && (mat >= a->chave[i]))
        i++;
    if ((a->filho[i]->folha))
    {
        TA *filho = a->filho[i];
        int j = 0;
        while ((j < filho->nchaves) && (mat > filho->chave[j]))
            j++;
        if ((j < filho->nchaves) && (mat == filho->chave[j]))
            return a;
    }
    if (a->folha)
        return NULL;
    if (a->chave[i] == mat)
        i++;
    return busca(a->filho[i], mat);
}

TA *removeCaso1(TA *a, int mat)
{
    int i = 0;
    int achou = 0;
    while (i < a->nchaves - 1)
    {
        if (mat == a->chave[i])
            achou = 1;
        if (achou)
        {
            a->chave[i] = a->chave[i + 1];
        }
        i++;
    }
    a->nchaves -= 1;
}

void removea(TA *arvore, int mat, int t)
{   
    printf("\nRemove %d\n",mat);
    TA *a = buscaPai(arvore, mat);
    int i = 0;
    while ((i < a->nchaves) && (mat >= a->chave[i]))
        i++;
    if (a->filho[i]->nchaves >= t)
    { //caso 1  i=2
        printf("caso1");
        a->filho[i] = removeCaso1(a->filho[i], mat);
        return;
    }
    if (a->filho[i]->nchaves == t - 1)
    {
        if((i!=0)&&(a->filho[i - 1]->nchaves >= t))
        { //caso 2
            printf("caso2");
            int maior = a->filho[i - 1]->chave[a->filho[i - 1]->nchaves - 1];
            a->filho[i - 1]->nchaves -= 1;
            a->filho[i] = removeCaso1(a->filho[i], mat);
            int k = a->filho[i]->nchaves;
            while (k > 0)
            {
                a->filho[i]->chave[k] = a->filho[i]->chave[k - 1];
                k--;
            }
            a->filho[i]->chave[k] = maior;
            a->filho[i]->nchaves += 1;
            a->chave[i - 1] = maior;
            return;
        }
        else if ((i!=(2*t))&&(a->filho[i + 1]->nchaves >= t))
        { //caso 3
            printf("caso3");
            int menor = a->filho[i + 1]->chave[0];
            a->filho[i + 1] = removeCaso1(a->filho[i + 1], menor);
            int subir = a->filho[i + 1]->chave[0];
            a->chave[i] = subir;
            a->filho[i] = removeCaso1(a->filho[i], mat);
            int k = a->filho[i]->nchaves;
            a->filho[i]->chave[k] = menor;
            a->filho[i]->nchaves += 1;
        }
        else if (i != 0)
        { // caso 4
            printf("caso4");
            a->filho[i] = removeCaso1(a->filho[i], mat);
            int x = a->filho[i - 1]->nchaves; //posição no vetor esq
            int b = 0;                        //posição no vetor que retirou o elemento
            int y = a->filho[i]->nchaves;     //tamanho do vetor que retirou o elemento
            while (b < y)
            {
                a->filho[i - 1]->chave[x] = a->filho[i]->chave[b];
                b++;
                x++;
            }
            a->filho[i]->nchaves = 0;
            a->filho[i - 1]->nchaves += y;
            int k = a->nchaves;
            int j = i - 1;
            while (j < k - 1)
            {
                a->chave[j] = a->chave[j + 1];
                a->filho[j + 1] = a->filho[j + 2];
                j++;
            }
            a->nchaves--;
            a->filho[i-1]->prox = a->filho[i];
        }
        else if (i != 2 * t) 
        { //caso 5
            printf("caso5");
            a->filho[i] = removeCaso1(a->filho[i], mat);
            int x = a->filho[i + 1]->nchaves; //posição no vetor dir
            int b = 0;                        //posição no vetor que retirou o elemento
            int y = a->filho[i]->nchaves;     //tamanho do vetor que retirou o elemento
            int z = a->filho[i + 1]->nchaves;
            a->filho[i + 1]->nchaves += y;
            while (b < z)
            {
                a->filho[i + 1]->chave[x] = a->filho[i + 1]->chave[x - 1];
                x--;
                b++;
            }
            b = 0;
            while (b < y)
            {
                a->filho[i + 1]->chave[b] = a->filho[i]->chave[b];
                b++;
            }
            a->filho[i]->nchaves = 0;
            b = 0;
            while (b < a->nchaves)
            {
                a->chave[b] = a->chave[b + 1];
                a->filho[b] = a->filho[b + 1];
                b++;
            }
            a->nchaves -= 1;
            if(i>0) a->filho[i-1]->prox = a->filho[i]; // se for o filho 0 n precisa ajeitar o encadeamento
        }
    }
    return;
}

int main(void)
{
    TA *a = inicializa();
    for (int i = 0; i < 30; i += 3)
    {
        a = insere(a, i, 3);
    }
    a = insere(a, 1, 3);
    imprime(a, 0);
    removea(a, 3, 3);
    printf("\n");
    imprime(a, 0);
    removea(a, 6, 3);
    printf("\n");
    imprime(a, 0);
    // removea(a, 12, 3);
    // printf("\n"); 
    // imprime(a, 0);
    // a=insere(a, 13, 3);
    // printf("\n"); 
    // imprime(a, 0);
    // removea(a, 21, 3);
    // printf("\n");
    // imprime(a, 0);
    // removea(a, 18, 3);
    // printf("\n");
    // imprime(a, 0);
    // removea(a, 15, 3);
    // printf("\n"); 
    // imprime(a, 0);
    // a=insere(a, 20, 3);
    // printf("\n");
    // imprime(a, 0);
    // removea(a, 13, 3);
    // printf("\n");    
    // imprime(a, 0);
}