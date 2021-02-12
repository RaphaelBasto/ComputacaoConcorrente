#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <semaphore.h>

#define L 4 //numero de threads leitoras
#define E 2 //numero de threads escritoras
#define TAM 10

//variaveis do problema
int l=0; //contador de threads lendo
int e=0; //contador de threads escrevendo
int vet[TAM];

//variaveis para sincronizacao
sem_t em_e, em_l, escr, leit; //semaforos

//thread leitora
void * leitor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        sem_wait(&leit); 
        sem_wait(&em_l);
        l++;
        printf("Thread leitora %d quer ler...\n", *id);
        if(l==1) sem_wait(&escr);
        sem_post(&em_l);
        sem_post(&leit); 
        printf("Thread leitora %d lendo...\n", *id);
        printf("Vetor atual:\n");
        for(int i = 0; i < TAM; i++){
            printf("%d ", vet[i]);
        }
        printf("\nThread leitora %d terminou de ler...\n", *id);
        sem_wait(&em_l); l--; 
        if(l==0) sem_post(&escr); 
        sem_post(&em_l); 
        sleep(1);   
    }
}

//thread escritora
void * escritor (void * arg) {
    int *id = (int *) arg;
    while(1) {
        sem_wait(&em_e); e++;
        printf("Thread escritora %d quer escrever...\n", *id);
        if(e==1) sem_wait(&leit);
        sem_post(&em_e);
        sem_wait(&escr);
        printf("Thread escritora %d escrevendo...\n", *id);
        for(int i = 0; i < TAM; i++){
            if((i == 0)){
                vet[i] = *id;
            } else{
                vet[i] = (*id) * i;
            }
        }
        printf("Thread escritora %d terminou de escrever...\n", *id);
        sem_post(&escr);
        sem_wait(&em_e); e--;
        if(e==0) sem_post(&leit);
        sem_post(&em_e);
        sleep(1);
    } 
}


//funcao principal
int main(void) {
  //identificadores das threads
  pthread_t tid[L+E];
  int id[L+E];

  sem_init(&em_e, 0, 1);
  sem_init(&em_l, 0, 1);
  sem_init(&escr, 0, E);
  sem_init(&leit, 0, L);

  for(int i = 0; i < TAM; i++){
        vetor[i] = 0;
    }

  //cria as threads leitoras
  for(int i=0; i<L; i++) {
    id[i] = i+1;
    if(pthread_create(&tid[i], NULL, leitor, (void *) &id[i])) exit(-1);
  } 
  
  //cria as threads escritoras
  for(int i=0; i<E; i++) {
    id[i+L] = i+1;
    if(pthread_create(&tid[i+L], NULL, escritor, (void *) &id[i+L])) exit(-1);
  } 

  pthread_exit(NULL);
  return 0;
}