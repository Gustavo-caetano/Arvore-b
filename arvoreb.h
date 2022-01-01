#ifndef ARVOREB_H_INCLUDED
#define ARVOREB_H_INCLUDED

#define MAX 9//quantidade maxima de ponteiros por paginas

////////////////////////////////////////////
//////defincao da arvore b//////////////////
///////////////////////////////////////////

typedef struct pagina{
    int tamanho;
    int valores[MAX-1];
    struct pagina *filhos[MAX];
}Arvb;

Arvb *raiz;

////////////////////////////////////////////////
///////funcoes///////////////
///////////////////////////////////////////////

Arvb *criarArv();
void inserir(Arvb *arv,Arvb *pai,int valor);
void printArv(Arvb *arv,int altura);

#endif