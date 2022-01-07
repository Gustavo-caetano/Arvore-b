#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    return arv->tamanho>=MAX-1;
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
    int status=0;//informa se ja foi feita a troca 
    int guardado=0;//guarda o numero
    if(vazia(arv)){
        arv->valores[arv->tamanho]=valor;
    }else{
    for(int i=0;i<=arv->tamanho;i++){
      if(status==0){
          if(valor==arv->valores[i]){
            printf("itens iguais:%d\n",valor);
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
    // printf("inserindo%d\n",valor);
   if(vaziaOrdem(arv)){
       printf("nao inicializada\n");
   }else{
        if(vaziaOrdem(arv->filhos[0])){
            /*caso não possua filhos é sinal de q se trata da pagina mais baixa da arvore 
            onde de se efetuar a remoção
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
///funcoes de pesquisa

int possui(Arvb *arv,int valor){
    for(int i=0;i<arv->tamanho;i++){
        if(valor==arv->valores[i]){
            return 1;
        }
    }
    return 0;
}

Arvb *pesquisa(Arvb *arv,int valor){
    if(vaziaOrdem(arv)){
        printf("nao iniciada\n");
        exit(1);
    }else{
        if(vazia(arv)){
            printf("sem elementos\n");
            exit(1);
        }else{
            if(possui(arv,valor)){
                printf("possui na lista\n");
                return arv;
            }else{
                if(vaziaOrdem(arv->filhos[0])){
                    printf("nao possui na lista\n");
                    return NULL;
                }
                else{
                    int i=0;
                    for(;i<arv->tamanho+1;i++){
                        if(valor<arv->valores[i]){
                            pesquisa(arv->filhos[i],valor);
                            break;
                        }else if(i==arv->tamanho-1 && valor>arv->valores[i]){
                            pesquisa(arv->filhos[i+1],valor);
                            break;
                        }
                    }
                }
            }
        }
    }
}


///funcoes de remocao
void adicionarPai(Arvb *pai,Arvb *filho,int local){

    /*
    essa funcao remove da pagina pai e adiciona na pagina filha 
    */
    filho->valores[filho->tamanho++]=pai->valores[local];

    for(int i=local;i<pai->tamanho-1;i++){//
        pai->valores[i]=pai->valores[i+1];
        pai->filhos[i+1]=pai->filhos[i+2];
    }
    pai->tamanho--;
}

int suporta(Arvb *a,Arvb *b){//testa se suporta adicionar uma pagina b na pagina a e um item da pagina pai
    return tamanho(a)+tamanho(b)<MAX-2;
}

int metade(Arvb *arv){//testa se está abaixo da capacidade minima
    return arv->tamanho<METADE;
}

void unir(Arvb *a,Arvb *b){
    /*
        essa funcão une a pagina A com a B e apaga a B 
    */
    printf("iniciada a uniao\n");
    if(vaziaOrdem(a) || vaziaOrdem(b)){
        printf("nao pode unir\n");
        exit(0);
    }
    int i=a->tamanho,j=0;
    for(;i<(a->tamanho+b->tamanho+1);i++){
        a->valores[i]=b->valores[j];
        a->filhos[i]=b->filhos[j++];
    }
    a->tamanho+=b->tamanho;

    b->tamanho=0;
}

void underSplit(Arvb *arv,Arvb *pai){//testa e executa o movimento inverso do split

    int local=numeroFilho(pai,arv);//descobre qual a localizaçao do filho na pagina pai

    if(local<tamanho(pai)-1 && local!=0){
        /*verifica se de acordo com a posicão a pagina vai ter 
        vizinhos tanto na esquerda quanto na direita,caso seja maior que 0
        e menor do que o tamanho ela possui dos dois vizinhos*/
        if(suporta(arv,pai->filhos[local-1])){
            Arvb *aux=pai->filhos[local-1];
            adicionarPai(pai,pai->filhos[local-1],local-1);
            unir(aux,arv);
            if(vazia(raiz)){//caso a raiz fique vazia muda a raiz para o filho
                free(raiz);
                raiz=aux;
            }
        }
        if(suporta(arv,pai->filhos[local+1])){
            Arvb *aux=pai->filhos[local+1];
            adicionarPai(pai,arv,local);
            unir(arv,aux);
            
            if(vazia(raiz)){
                free(raiz);
                raiz=arv;
            }
        }
    }
    else if(local==0){//caso seja o 0 so possui o vizinho da direita
        if(suporta(arv,pai->filhos[local+1])){
            Arvb *aux=pai->filhos[local+1];
            adicionarPai(pai,arv,local);
            unir(arv,aux);
            if(cheia(arv))
                split(arv,pai);
            }
            if(vazia(raiz)){
                free(raiz);
                raiz=arv;
            }
    }else{//caso seja o ultimo so possui o vizinho da esquerda
        if(suporta(arv,pai->filhos[local-1])){
            Arvb *aux=pai->filhos[local-1];
            adicionarPai(pai,pai->filhos[local-1],local-1);
            unir(aux,arv);
           
            if(vazia(raiz)){
                free(raiz);
                raiz=aux;
            }
        }
    }
    
}

void substituir(Arvb *arv,int *valor,Arvb *pai){
    /*caso precise apagar um item de uma pagina que possua filho ela vai
    substituir o item com o numero mais proximo*/

    if(vaziaOrdem(arv->filhos[0])){
        swap(&(arv->valores[arv->tamanho-1]),valor);
        arv->tamanho--;
    }else{
        substituir(arv->filhos[arv->tamanho],valor,arv);
    }
    if(metade(arv))
        underSplit(arv,pai);

}

int removerPagina(Arvb *arv,int valor){//remove o item da pagina

    int status=0;
    if(vaziaOrdem(arv->filhos[0])){
        for(int i=0;i<arv->tamanho;i++){
            if(arv->valores[i]==valor){//faz o item ir para o final da pagina e diminui o tamanho dela
                swap(&(arv->valores[i]),&(arv->valores[i+1]));
                status=1;
            }
        }
    }
    if(status==1)
        arv->tamanho--;
    return status;
}

void removerArv(Arvb *arv,Arvb *pai,int valor){//remocão geral
    if(vaziaOrdem(arv)){
        printf("arvore nao inicida ainda\n");
    }else{
        if(vazia(arv)){
            printf("nao possui elementos para remover\n");
        }else{
           if(vaziaOrdem(arv->filhos[0])){//caso ela nao possua filhos
               removerPagina(arv,valor)? printf("removeu\n"):printf("nao existe\n");
           }else{
               for(int i=0;i<arv->tamanho;i++){
                   if(valor==arv->valores[i]){//executa a funcao de troca
                        substituir(arv->filhos[i],&(arv->valores[i]),arv);
                        break;
                   }else if(valor<arv->valores[i]){
                        removerArv(arv->filhos[i],arv,valor);
                        break;
                   }else if(i==arv->tamanho-1){
                        removerArv(arv->filhos[i+1],arv,valor);
                        break;
                   }
               }
           }

           if(metade(arv) && arv!=raiz && vazia(arv)!=1){//testa se esta da capacidade minima
                underSplit(arv,pai);
           }
        }
    }
}

////Apagar tudo
void apagar(Arvb *arv){
    if(vaziaOrdem(arv)){
        printf("nao iniciada\n");
    }else{
        if(vazia(arv)){
            printf("nao possue elementos\n");
        }else{
            if(vaziaOrdem(arv->filhos[0])){
                free(arv);
            }else{
                for(int i=0;i<arv->tamanho;i++){
                    apagar(arv->filhos[i]);
                }
                if(arv==raiz)
                    arv->tamanho=0;
                else
                    free(arv);
            }
        }
    }
}

////funcoes de printar a arvore

void printLista(int *list,int tamanho){
    for(int i=0;i<tamanho;i++){
            printf("%d ",list[i]);
    }
}

void printArv(Arvb *arv,int altura){
    if(vaziaOrdem(arv)){
        printf("nao iniciada a arvore\n");
    }else{
        if(vazia(arv)){
            printf("vazia\n");
        }else{
            if(arv==raiz)
                printf("raiz%*s",10,"");
            else
                printf("filho[%d]%*s",altura,altura*3 +10,"|");
            
            printLista(arv->valores,arv->tamanho);
            printf("\n");

            if(vaziaOrdem(arv->filhos[0])==0)
                for(int i=0;i<=arv->tamanho;i++){
                    printArv(arv->filhos[i],altura+1);
                }
            altura++;
        }
    }
}

void printVetor(Arvb *arv){//printa e forma de vetor
    if(vaziaOrdem(arv)){
        printf("nao inicializada\n");
    }else{
        if(vazia(arv)){
            printf("sem elementos\n");
        }else{
            printLista(arv->valores,arv->tamanho);
            // if(arv==raiz)
            //     printf("\n");
            if(vaziaOrdem(arv->filhos[0])==0){
                for(int i=0;i<=arv->tamanho;i++){
                    printVetor(arv->filhos[i]);
                }
            }
        }
    }
}

////////////////
//Menu

void menu(int *b){
    char escolha='0';
    while(escolha!='N'){
        printf("Escolha uma opcão\n");
        printf("1-inserir\n");
        printf("2-inserir aleatorio\n");
        printf("3-imprimir\n");
        printf("4-pesquizar\n");
        printf("5-remover\n");
        printf("6-limpar a tela\n");
        printf("7-apagar arvore\n");
        printf("8-sair\n");
        scanf("%c",&escolha);
        switch(escolha){
            case '1':
                printf("digite um numero\n");
                int numero;
                scanf("%d",&numero);
                inserir(raiz,NULL,numero);
                break;
            case '2':
                printf("digite a quantidade de numeros\n");
                int quantidade;
                scanf("%d",&quantidade);
                srand(time(NULL));
                for(int i=1;i<quantidade;i++){
                    inserir(raiz,NULL,rand()%(i*10));
                }
                break;
            case '3':printArv(raiz,0);
                break;
            case '4':
                printf("Digite o numero que quer pesquisar\n");
                int pesquisar;
                scanf("%d",&pesquisar);
                pesquisa(raiz,pesquisar);
                break;
            case '5': 
                printf("Digite o item para ser removido\n");
                int valor;
                scanf("%d",&valor);
                removerArv(raiz,NULL,valor);
                break;
            case '6': system("clear"); break;
            case '7': apagar(raiz); break;
            case '8':
            printf("saindo\n");
                *b=1;
                exit(0);
            break;
            default:
                printf("Opcao invalida\n");
                scanf("%c",&escolha);
        }
        scanf("%c",&escolha);
    }
}