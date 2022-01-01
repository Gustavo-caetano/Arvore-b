#include <stdio.h>
#include <stdlib.h>
#include "arvoreb.h"
#include <time.h>

int main(int argc, char *argv[]){

    
    raiz=criarArv();
    
    int b=atoi(argv[1]);
    srand(time(NULL));
    for(int i=1;i<b;i++){
        int a=rand()%(i*10);
        inserir(raiz,NULL,a);
    }
    printArv(raiz,0);
}
