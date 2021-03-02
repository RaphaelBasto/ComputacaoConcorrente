// Raphael Pereira Gomes Pinto Basto, 118169160 
// Felipe Pestana Rosa, 118189916

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include "timer.h"

long long int posicao_A;
long long int tamanho_A;
int valor_A;
long long int triplas=0, sequencias=0;

int *blocos, tam_buffer, tam_bloco, ultimo=-1, resto=-1;
FILE *arq;

int *zero_permite_escrita;

pthread_mutex_t *mutex;
pthread_cond_t escr, leit, cond;

int bloqueadas=0;

//Função para esperar as threads leitoras terminarem a execução e liberar a escrita
void barreira(int pos) {

    pthread_mutex_lock(mutex+pos); //inicio secao critica
    if (bloqueadas == 2) {
        //ultima thread a chegar na barreira
        pthread_cond_broadcast(&cond);
        bloqueadas=0;
    } else {
        bloqueadas++;
        pthread_cond_wait(&cond, mutex+pos);
    }
    pthread_mutex_unlock(mutex+pos); //fim secao critica
}

// Thread que preenche o buffer

void *preenche(void *args){
    int pos_atual=0;
    long long int lidos, restam;
    fread(&restam, sizeof(long long int), 1, arq);

    while(restam > 0){

        pthread_mutex_lock(mutex+pos_atual);
        while(zero_permite_escrita[pos_atual] > 0){
            // Se houver thread lendo, espera
            pthread_cond_wait(&escr, mutex+pos_atual);
        }
        pthread_mutex_unlock(mutex+pos_atual);
        if (restam >= tam_bloco){
            lidos = fread(blocos+(tam_bloco*pos_atual), sizeof(int), tam_bloco, arq);
            if (restam == tam_bloco){
                //Testa se falta apenas o bloco atual
                ultimo = pos_atual;
                resto = lidos;
            }
        } else {
            lidos = fread(blocos+(tam_bloco*pos_atual), sizeof(int), restam, arq);
            ultimo = pos_atual;
            resto = lidos;
        }
        restam = restam-lidos;
        pthread_mutex_lock(mutex+pos_atual);
        zero_permite_escrita[pos_atual] = 3; // Libera execução das outras threads
        pthread_mutex_unlock(mutex+pos_atual);
        pos_atual = (pos_atual + 1) % tam_buffer; // Anda em circulo no buffer
        pthread_cond_broadcast(&leit);
    }

    pthread_exit(NULL);
}

// Thread para encontrar maior sequência de valores idênticos

void *A(void *args){

    int leit_index=0, tam=tam_bloco;
    int valor=0, temp;
    long long int pos_inicial=0, tam_seq=0, pos_atual=1;
    long long int pos_temp, tam_temp=0;

    while(1){

        pthread_mutex_lock(mutex+leit_index);
        while(zero_permite_escrita[leit_index] == 0){
            // Se a thread de preencher estiver ativa, espera
            pthread_cond_wait(&leit, mutex+leit_index);
        }
        pthread_mutex_unlock(mutex+leit_index);
        //Parte onde identifica a maior sequência
        if (leit_index == ultimo){
            tam = resto;
        }
        for (int i=0; i<tam; i++){
            int sequencia = blocos[leit_index*tam_bloco+i];
            if (tam_temp == 0){
                temp = sequencia;
                pos_temp = pos_atual;
            }
            if (sequencia == temp){
                tam_temp++;
            } else {
                if (tam_temp > tam_seq){
                    pos_inicial = pos_temp;
                    tam_seq = tam_temp;
                    valor = temp;
                }
                tam_temp = 1;
                temp = sequencia;
                pos_temp = pos_atual;
            }
            pos_atual++;
        }
        pthread_mutex_lock(mutex+leit_index);
        zero_permite_escrita[leit_index]--; //Diminui para liberar a escrita
        if (leit_index==ultimo){
            pthread_mutex_unlock(mutex+leit_index);
            break;
        }
        if (zero_permite_escrita[leit_index] == 0) pthread_cond_broadcast(&escr);
        pthread_mutex_unlock(mutex+leit_index);

        leit_index = (leit_index + 1) % tam_buffer;
        if (leit_index == 0){
            barreira(leit_index);
        }
    }
    if (tam_temp > tam_seq){
        //Guarda a maior sequência até o momento
        pos_inicial = pos_temp;
        tam_seq = tam_temp;
        valor = temp;
    }
    //Guarda a maior sequência no escopo global
    posicao_A = pos_inicial-1;
    tamanho_A = tam_seq;
    valor_A = valor;

    pthread_exit(NULL);
}

// Thread para encontrar quantidade de triplas

void *B(void *args){
    int leit_index=0, tam=tam_bloco;
    int valor, cont = 0;
    while(1){

        pthread_mutex_lock(mutex+leit_index);
        while(zero_permite_escrita[leit_index] == 0){ 
            // Se a thread de preencher estiver ativa, espera
            pthread_cond_wait(&leit, mutex+leit_index);
        }
        pthread_mutex_unlock(mutex+leit_index);
        //Parte onde identifica e conta as triplas
        if (leit_index == ultimo){
            tam = resto;
        }
        for (int n=0; n<tam; n++){
            int sequencia = blocos[tam_bloco*leit_index+n];
            if (sequencia != valor){
                cont = 0;
                valor = sequencia;
            }
            if (sequencia == valor){
                cont++;
            }
            if (cont % 3 == 0){
                //Se a contagem atual for multiplo de 3 aumenta o contador de triplas
                triplas++;
            }
        }
        pthread_mutex_lock(mutex+leit_index);
        zero_permite_escrita[leit_index]--;//Diminui para liberar a escrita
        if (leit_index==ultimo){
            pthread_mutex_unlock(mutex+leit_index);
            break;
        }
        if (zero_permite_escrita[leit_index] == 0) pthread_cond_broadcast(&escr);
        pthread_mutex_unlock(mutex+leit_index);
        leit_index = (leit_index + 1) % tam_buffer;
        if (leit_index == 0){
            barreira(leit_index);
        }
    }
    
    pthread_exit(NULL);
}

// Thread para encontrar quantidade de ocorrências da sequência <012345>

void *C(void *args){
    int leit_index=0, tam=tam_bloco;
    int temp = 0;
    while(1){

        pthread_mutex_lock(mutex+leit_index);
        while(zero_permite_escrita[leit_index] == 0){
            // Se a thread de preencher estiver ativa, espera
            pthread_cond_wait(&leit, mutex+leit_index);
        }
        pthread_mutex_unlock(mutex+leit_index);
        //Parte onde identifica e conta as sequencias <012345>
        if (leit_index == ultimo){
            tam = resto;
        }
        for (int i=0; i<tam; i++){
            int sequencia = blocos[leit_index*tam_bloco+i];
            if (sequencia == temp-1 && sequencia == 0){
            }else{
                if (sequencia == temp){
                    temp++ ;
                } else {
                    temp = 0;
                }
                if (temp == 5){
                    sequencias++;
                    temp = 0;
                }
            }
        }
        pthread_mutex_lock(mutex+leit_index);
        zero_permite_escrita[leit_index]--;//Diminui para liberar a escrita
        if (leit_index==ultimo){
            pthread_mutex_unlock(mutex+leit_index);
            break;
        }
        if (zero_permite_escrita[leit_index] == 0) pthread_cond_broadcast(&escr);
        pthread_mutex_unlock(mutex+leit_index);
        leit_index = (leit_index + 1) % tam_buffer;
        if (leit_index == 0){
            barreira(leit_index);
        }
    }
    pthread_exit(NULL);
}


int main(int argc, char *argv[]){
    double tempo_inicio, tempo_fim, tempo_delta;
    pthread_t threads[4];
    if (argc < 3){
        printf("Erro: Digite <tamanho do buffer> <tamanho do bloco> para o programa %s\n", argv[0]);
        return 1;
    }
    tam_buffer = atoi(argv[1]);
    tam_bloco = atoi(argv[2]);
    GET_TIME(tempo_inicio)
    blocos = malloc(sizeof(int) * tam_buffer * tam_bloco);
    if (blocos == NULL){
        printf("Não foi possível alocar memória para o buffer de blocos\n");
        return 2;
    }
    zero_permite_escrita = malloc(sizeof(char) * tam_buffer);
    if (zero_permite_escrita == NULL){
        printf("Não foi possível alocar memória para o zero_permite_escrita\n");
        return 2;
    }
    for (int i=0; i<tam_buffer; i++){
        zero_permite_escrita[i] = 0;
    }
    mutex = malloc(sizeof(pthread_mutex_t) * tam_buffer);
    if (mutex == NULL){
        printf("Não foi possível alocar memória para o vetor mutex\n");
        return 2;
    }
    for (int i=0; i<tam_buffer; i++){
        pthread_mutex_init(mutex+i, NULL);
    }
    pthread_cond_init(&escr, NULL);
    pthread_cond_init(&leit, NULL);
    arq = fopen("arq", "rb");
    if (!arq){
        printf("Não foi possível abrir o arquivo\n");
        return 3;
    }
    //Inicia as threads
    pthread_create(&threads[0], NULL, preenche, NULL);
    pthread_create(&threads[1], NULL, A, NULL);
    pthread_create(&threads[2], NULL, B, NULL);
    pthread_create(&threads[3], NULL, C, NULL);
    //Espera as threads terminarem a execução
    pthread_join(*threads, NULL);
    pthread_join(*(threads+1), NULL);
    pthread_join(*(threads+2), NULL);
    pthread_join(*(threads+3), NULL);

    fclose(arq);
    printf("\nMaior sequência de valores idênticos: %lld %lld %d\n", posicao_A, tamanho_A, valor_A);
    printf("Quantidade de triplas: %lld \n", triplas);
    printf("Quantidade de ocorrências da sequência <012345>: %lld \n", sequencias);
    GET_TIME(tempo_fim)
    tempo_delta = tempo_fim - tempo_inicio;
    printf("Tempo total: %lf\n", tempo_delta);

    return 0;
}