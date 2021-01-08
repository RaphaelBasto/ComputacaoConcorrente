// Raphael Pereira Gomes Pinto Basto, 118169160 
// Felipe Pestana Rosa, 118189916
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

//Inicialização de variáveis globais
long int* vet;
int nthreads;
long int tam;
int trava = 0;
pthread_mutex_t mutex;

//Função que fará o quicksort
void quick_sort(long int *a, long int left, long int right) {
    long int i, j, x, y;
     
    i = left;
    j = right;
    x = a[(left + right) / 2];
     
    while(i <= j) {
        while(a[i] < x && i < right) {
            i++;
        }
        while(a[j] > x && j > left) {
            j--;
        }
        if(i <= j) {
            y = a[i];
            a[i] = a[j];
            a[j] = y;
            i++;
            j--;
        }
    }
     
    if(j > left) {
        quick_sort(a, left, j);
    }
    if(i < right) {
        quick_sort(a, i, right);
    }
}

//Função que as threads executarão
void * tarefa(void *arg){
    int id = (int) arg;
    long int bloco = tam/nthreads;
    long int ini = id * bloco;
    long int fim;
    if(id ==nthreads-1) fim = tam;
	else fim = ini + bloco;

    printf("Thread %d começou\n", id);

    quick_sort(vet,ini,fim);

    pthread_mutex_lock(&mutex);
    trava++;
    pthread_mutex_unlock(&mutex);
    while (trava!=nthreads ){
    }
    if (id == 0){
        quick_sort(vet, 0, tam-1);
    }
    

    printf("Thread %d terminou\n", id );
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int quer;
    printf("Digite 0 para NÃO PRINTAR O VETOR ou digite 1 para PRINTAR O VETOR: ");
    scanf("%d", quer);
    srand(time(NULL));
    pthread_t *tid;
    double inicio, fim, delta, deltaConc;
    GET_TIME(inicio);
    if (argc<3){
        printf("Digite: %s <tamanho do vetor> <numero de threads>\n", argv[0]);
        return 1;
    }
    tam = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    int args[nthreads];
    //Inicialização do vetor a ser ordenado
    vet = (long int *)malloc(sizeof(long int)* tam);
    if (vet == NULL){printf("ERRO--malloc\n"); return 2;}
    for (int i = 0; i < tam; i++)
    {
        vet[i]= rand()%tam; //Uso da função rand para gerar valores aleatorios até o valor maximo = tamanho do vetor -1
    }
    
    GET_TIME(fim);
    delta = fim-inicio;
    printf("Tempo de inicializacao:%lf\n", delta);

    GET_TIME(inicio);
    tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL){puts("ERRO--malloc"); return 2;}

    //Executa as threads
    for (int i = 0; i < nthreads; i++){
        args[i] = i;
        if (pthread_create(tid+i,NULL, tarefa, (void *) (args[i]))){
            puts("ERRO--pthread_create");
            return 3;
        }
    }
    
    //Espera as threads terminarem
    for(int i = 0; i<nthreads; i++){
    pthread_join(*(tid+i),NULL);
    }

    GET_TIME(fim);
    deltaConc = fim-inicio;
    printf("Tempo de concorrente:%lf\n", deltaConc);

    GET_TIME(inicio);
    
    puts("\n");
    //Caso o usuário queira, printa o vetor já ordenado
    if (quer==1)
    {
        for (long int i = 0; i < tam; i++){
            printf("%ld ", vet[i]);
        }
        puts("\n");
    }
    free(vet);
    free(tid); 
    pthread_mutex_destroy(&mutex);
    GET_TIME(fim);
    delta += fim-inicio;
    printf("Tempo Final menos a parte concorrente: %lf\n", delta);
    
    
    return 0;
}
