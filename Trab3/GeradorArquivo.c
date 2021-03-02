// Raphael Pereira Gomes Pinto Basto, 118169160 
// Felipe Pestana Rosa, 118189916

#include<stdio.h>
#include <stdlib.h>
#include<time.h>//necessário p/ função time()

int main () {
    FILE *arq;
    int n;
    printf("Digite 1 para preencher o arquivo com numeros gerados aleatoriamente (Recomendado para testes grandes)\nOU\nDigite 2 para preencher o arquivo com numeros da sua escolha (Recomendado para testes menores):\n");
    scanf("%i", &n);
    long long int tam;
    printf("Digite o tamanho: \n");
    scanf("%lli", &tam);
    int numeros[tam];
    switch (n){
    case 1:
        //Aleatorio
        srand(time(NULL));
        for(long long int i = 0; i<tam; i++){
            numeros[i] = rand() % 6;
        }
        break;
    
    case 2:
        //À escolha
        for(long long int i = 0; i<tam; i++){
            printf("Digite o valor na posição %lli: \n",i);
            scanf("%i", &numeros[i]);
        }
        break;
    }
    
    int numero_atual;
    arq = fopen( "arq" , "wb" );
    fwrite(&tam, sizeof(long long int) , 1,  arq );
    for (int i=0; i<tam; i++){
        fwrite(&numeros[i], sizeof(int) , 1,  arq );
    }
    fclose(arq);
    arq = fopen( "arq" , "rb" );
    fread(&numero_atual, sizeof(long long int) , 1,  arq );
    printf("tamanho: %d\n",numero_atual);
    for (long long int i=0; i<tam; i++){
        fread(&numero_atual, sizeof(int) , 1,  arq );
        if (tam < 50){
            printf("Valor na posição %lli: %d \n",i, numero_atual);
        }
    }
    return(0);
}