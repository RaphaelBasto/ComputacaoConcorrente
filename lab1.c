// Raphael Pereira Gomes Pinto Basto - 118169160 
// Felipe Pestana Rosa - 118189916

#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
#define N 55 //tamanho do vetor

int vetor[N];

//--funcao executada pelas threads
void *incrVetor (void *arg) {

  int ident = * (int *) arg;

  if (ident == 1){
    for (int i = 0; i < N; i+=2) {
      if (i % 2  == 0){
        vetor[i]+=1;
      }
    }
  }
  if (ident == 2){
    for (int i = 1; i < N; i+=2) {
      if (i % 2  == 1){
        vetor[i]+=1;
      } 
    }
  }
  pthread_exit(NULL);
}

//--funcao principal do programa
int main(void) {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread; //variavel auxiliar
  int ident[NTHREADS] = {1,2};
  
  for (int i = 0; i < N; i++)
  {
    printf("%d ", vetor[i]);
    /* code */
  }
  printf("\n");

  for(thread=0; thread<NTHREADS; thread++) {
    printf("--Cria a thread %d\n", thread);
    if (pthread_create(&tid_sistema[thread], NULL, incrVetor, (void *) &ident[thread])) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  }

  for (int i = 0; i < NTHREADS; i++)
  {
    if (pthread_join(tid_sistema[i],NULL))
    {
      printf("ERRO -- pthread_create\n");
    }
    
  }
  
  for (int i = 0; i < N; i++)
  {
    printf("%d ", vetor[i]);
    /* code */
  }
  printf("\n");

  pthread_exit(NULL);
}

