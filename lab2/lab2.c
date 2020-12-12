// Raphael Pereira Gomes Pinto Basto - 118169160 
// Felipe Pestana Rosa - 118189916
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

float *mat;
float *mat2;
float *saida;
int nthreads;

typedef struct
{
    int id;
    int dim;
}tArgs;

void * tarefa(void *arg){
    tArgs *args = (tArgs *) arg;
    printf("Thread %d\n", args->id);
    for (int i = args->id; i < args->dim; i+=nthreads){
        for (int j = 0; j < args->dim; j++){
            for (int k = 0; k < args->dim; k++){
                saida[i*args->dim + j] += mat[j*args->dim + k] * mat2[k*args->dim + j];
            }
        }
    } 
    pthread_exit(NULL);
}

int main(int argc, char* argv[]){
    int dim;
    pthread_t *tid;
    tArgs *args;
    double inicio, fim, delta;
    double inicioTotal, fimTotal, deltaTotal;
    GET_TIME(inicio);
    if (argc<3){
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1;
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);

    mat = (float *) malloc(sizeof(float) * dim * dim);
    if (mat == NULL){printf("ERRO--malloc\n"); return 2;}
    mat2 = (float *) malloc(sizeof(float) * dim * dim);
    if (mat2 == NULL){printf("ERRO--malloc\n"); return 2;}
    saida = (float *) malloc(sizeof(float) * dim * dim);
    if (saida == NULL){printf("ERRO--malloc\n"); return 2;}

    for (int i = 0; i < dim; i++){
        for (int j = 0; j < dim; j++){
            mat[i*dim + j] = 1;
            mat2[i*dim + j] = 1;
            saida[i*dim + j]=0;
        }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("Tempo de inicializacao:%lf\n", delta);

    //multiplicacao das matrizes
    GET_TIME(inicioTotal);
    tid = (pthread_t *) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL){puts("ERRO--malloc"); return 2;}
    args = (tArgs *) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL){puts("ERRO--malloc"); return 2;}
    
    if (nthreads == 1){
        args->dim = dim;
        for (int i = 0; i < args->dim; i++){
            for (int j = 0; j < args->dim; j++){
                for (int k = 0; k < args->dim; k++){
                    saida[i*args->dim + j] += mat[j*args->dim + k] * mat2[k*args->dim + j];
                }
            }
        }
    }
    else{
        for (int i = 0; i < nthreads; i++){
            (args+i)->id = i;
            (args+i)->dim = dim;
            if (pthread_create(tid+1,NULL, tarefa, (void *) (args+i))){
                puts("ERRO--pthread_create");
                return 3;
            }
        }
        for(int i = 0; i<nthreads; i++){
        pthread_join(*(tid+i),NULL);
        }
    }

    GET_TIME(fimTotal);
    deltaTotal = fimTotal - inicioTotal;
    printf("Tempo Concorrente: %lf\n", deltaTotal);

    GET_TIME(inicio);
    /*puts("Vetor de saida: ");
    for (int i = 0; i < dim; i++){
        for (int j=0; j<dim; j++){
            printf("%.1f ", saida[i*dim + j]);
        }
        puts("");
    }*/
    
    puts("\n");

    free(mat);
    free(mat2);
    free(saida);
    free(tid);
    free(args);
    GET_TIME(fim);
    delta += fim-inicio;
    printf("Tempo Sequencial: %lf\n", delta);
    return 0;
}
