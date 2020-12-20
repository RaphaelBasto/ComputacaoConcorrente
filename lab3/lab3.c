#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"
#include<math.h>

long int dim;
double *vetor;
int nthreads;

//fluxo das threads
void * tarefa(void * arg){
	long int id = (long int) arg;
	double *somaLocal;
	long int tamBloco = dim/nthreads;
	long int ini = id * tamBloco;
	long int fim;
	
	somaLocal = (double*) malloc(sizeof(double));
	if(somaLocal == NULL) {
		fprintf(stderr, "Erro-malloc\n"); exit(1);
		
	}
	*somaLocal = 0;
	if(id ==nthreads-1) fim = dim;
	else fim = ini + tamBloco;
	
	for(long int i=ini; i<fim; i++)
	*somaLocal +=vetor[i];
	pthread_exit((void *) somaLocal);
}
int main(int argc, char *argv[]) {
	double somaSeq = 0;
	double somaConc = 0;
	double ini, fim;
	pthread_t *tid;
	double *retorno;
	
	//parâmetros de entrada
	if (argc < 3) {
		fprintf(stderr, "Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
		return 1;
	}
	dim = atoi(argv[1]);
	nthreads = atoi(argv[2]);
	//aloca vetor de entrada
	vetor = (double*) malloc(sizeof(double)*dim);
	if(vetor == NULL){
		fprintf(stderr, "ERRO-malloc\n");
		return 2;
	}
	//valor de entrada
	for(long int i=0; i<dim; i++){
		double denominador = 1+(2*i);
		vetor[i] = 1/denominador;
		if(i%2 == 1){
			vetor[i] *= -1;
		}	
	}
	GET_TIME(ini);
	for(long int i=0; i<dim; i++)
		somaSeq += vetor[i];
	GET_TIME(fim);
	printf("Tempo Sequencial: %lf\n", fim-ini);
	
	//soma concorrente
	GET_TIME(ini);
	tid = (pthread_t *) malloc(sizeof(pthread_t) * nthreads);
	if(tid==NULL) {
	fprintf(stderr, "ERRO-malloc\n");
	return 2;
	}
	//criar as threads
	for(long int i=0; i<nthreads; i++) {
	if(pthread_create(tid+i, NULL, tarefa, (void*) i)){
	fprintf(stderr, "ERRO-pthread_create\n");
	return 3;
	}
	}
	//aguarda o termino das threads
	for(long int i=0; i<nthreads; i++) {
		if(pthread_join(*(tid+i),(void**) &retorno)){
			fprintf(stderr, "ERRO-pthread_create\n");
			return 3;
		}
		//soma global
		somaConc += *retorno;
		free(retorno);
	}
	GET_TIME(fim);
	printf("Tempo Concorrente:   %lf\n", fim-ini); 
	
	
	printf("Soma sequencial:  %.15f\n", 4 * somaSeq);
	printf("Soma concorrente: %.15f\n", 4 * somaConc);
	printf("Math_PI:          %.15f\n", M_PI);
	
	free(vetor);
	free(tid);
	
	return 0;
}
