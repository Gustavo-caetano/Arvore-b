#include <stdio.h>
#include <stdlib.h>

#include "arvoreb.h"

#define METADE MAX/2

//////funcoes de criacão e informacoes sobre a arvore
Arvb *criarArv(){//inicia a arvore
    Arvb *aux;
    aux=(Arvb*)malloc(sizeof(Arvb));
    aux->tamanho=0;
    return aux;
}

int vaziaOrdem(Arvb *arv){//informa se a arvore foi iniciada
    return arv==NULL;
}

int vazia(Arvb *arv){//informa nao possui elementos
    return arv->tamanho==0;
}

int tamanho(Arvb *arv){
    return arv->tamanho;
}

int cheia(Arvb *arv){
    return arv->tamanho==MAX-1;
}
///////funcoes de insercao

void dividir(Arvb *origem,Arvb *destino){
    /*
    pega a pagina e divide os elementos na metade com outra pagina
    */
    int i=METADE+1;
    for(;i<MAX-1;i++){
        destino->filhos[(destino->tamanho)]=origem->filhos[i];
        origem->filhos[i]=NULL;
        destino->valores[(destino->tamanho)++]=origem->valores[i];
    }
    destino->filhos[(destino->tamanho)]=origem->filhos[i];
    origem->tamanho=MAX-METADE-1;
}

void swap(int *a,int *b){
    int c=*a;
    *a=*b;
    *b=c;
}
   
void inserirOrd(Arvb *arv,int valor){//insere no vetor de forma ordenada
    int status=0;
    int guardado=0;
    if(vazia(arv)){
        arv->valores[arv->tamanho]=valor;
    }else{
    for(int i=0;i<=arv->tamanho;i++){
      if(status==0){
          if(valor<arv->valores[i]){
            printf("itens iguais\n");
            arv->tamanho--;
            break;
          }
          if(valor<arv->valores[i]){
              guardado=arv->valores[i];
              arv->valores[i]=valor;
              status=1;
          }else if(i==arv->tamanho){
              arv->valores[i]=valor;
          }
      }else{
          swap(&guardado,&(arv->valores[i]));
      }
    }
    }
    arv->tamanho++;
}

int numeroFilho(Arvb *pai,Arvb *filho){//informa qual é a posicão do filho na lista pai
    for(int i=0;i<=pai->tamanho;i++){
        if(pai->filhos[i]==filho)
            return i;
    }
    return -1;
}

void inserirFilho(Arvb *pai,Arvb *filho,int lugar){//insere um novo filho
    Arvb *aux,*aux1=filho;
    for(int i=lugar+1;i<=pai->tamanho;i++){
        aux=pai->filhos[i];
        pai->filhos[i]=aux1;
        aux1=aux;
    }
}


void split(Arvb *arv,Arvb *pai){//efetua o split
    if(arv==raiz){
        Arvb *encima=criarArv(),*lado=criarArv();
        inserirOrd(encima,arv->valores[(METADE)]);
        dividir(arv,lado);

        raiz=encima;
        raiz->filhos[0]=arv;
        raiz->filhos[1]=lado;
    }else{
        Arvb *lado=criarArv();
        inserirOrd(pai,arv->valores[(METADE)]);
        dividir(arv,lado);
        inserirFilho(pai,lado,numeroFilho(pai,arv));
    }
}

void inserir(Arvb *arv,Arvb *pai,int valor){//insercao geral
   if(vaziaOrdem(arv)){
       printf("nao inicializada\n");
   }else{
        if(vaziaOrdem(arv->filhos[0])){
            /*caso não possua filhos é sinal de q se trata da pagina mais baixa da arvore 
            */
            inserirOrd(arv,valor);
        }else{
            int i=0;
            for(;i<arv->tamanho+1;i++){
                if(valor<arv->valores[i]){
                    inserir(arv->filhos[i],arv,valor);
                    break;
                }else if(i==arv->tamanho-1 && valor>arv->valores[i]){
                    inserir(arv->filhos[i+1],arv,valor);
                    break;
                }
            }
          }
        if(cheia(arv)){
            split(arv,pai);
        }
   }
}
///funcoes de remocao



////funcoes de printar a arvore

void printLista(int *list,int tamanho){
    for(int i=0;i<tamanho;i++){
            printf("%d ",list[i]);
    }
    printf("\n");
}

void printArv(Arvb *arv,int altura){
    if(vaziaOrdem(arv)){
        printf("nao iniciada a arvore\n");
    }else{
        if(vazia(arv)){
            printf("vazia\n");
        }else{
            if(arv==raiz)
                printf("raiz%*s",15,"");
            else
                printf("filho%d%*s",altura,altura*5+10,"");
            
            printLista(arv->valores,arv->tamanho);
            if(vaziaOrdem(arv->filhos[0])==0)
                for(int i=0;i<=arv->tamanho;i++){
                    printArv(arv->filhos[i],altura+1);
                }
            altura++;
        }
    }
}
