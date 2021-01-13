/* Disciplina: Computacao Concorrente */
/* Prof.: Silvana Rossetto */
/* Laboratório: 4 */
/* Codigo: Uso de variáveis de condição e suas operações básicas para sincronização por condição */

/***** Condicao logica da aplicacao: as threads A so podem imprimir "Bye" depois que a thread B imprimir  "Hello"  ****/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

/* Thread 1 */
void *A (void *t) {

  //printf("1: Comecei\n");
  
  
  printf("tudo bem?\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  //printf("1: x = %d, vai sinalizar a condicao \n", x);
  if (x==2)
  {
    pthread_cond_broadcast(&x_cond);
  }
  
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread 2 */
void *B (void *t) {

  //printf("2: Comecei\n");
  
  
  printf("bom dia!\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  //printf("2: x = %d, vai sinalizar a condicao \n", x);
  if (x==2)
  {
    pthread_cond_broadcast(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread 3 */
void *C (void *t) {
  //printf("3: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  if (x != 2) { 
     //printf("3: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     //printf("3: sinal recebido e mutex realocado, x = %d\n", x);
  }
  pthread_mutex_unlock(&x_mutex); 
  printf("até mais!\n");
  pthread_exit(NULL);
}

/* Thread 4 */
void *D (void *t) {
  //printf("4: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  if (x != 2) { 
     //printf("4: vai se bloquear...\n");
     pthread_cond_wait(&x_cond, &x_mutex);
     //printf("4: sinal recebido e mutex realocado, x = %d\n", x);
  }
  pthread_mutex_unlock(&x_mutex); 
  printf("boa tarde!\n");
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[0], NULL, A, NULL);
  pthread_create(&threads[1], NULL, B, NULL);
  pthread_create(&threads[2], NULL, C, NULL);
  pthread_create(&threads[3], NULL, D, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
  return 0;
}
