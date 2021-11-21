#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2 // Número de threads auxiliares
#define NVETOR 10000 // Tamanho do vetor

typedef struct {
   int size;
   int* segment;
} t_Args;

int vetor[NVETOR];

// Popula o vetor de 10000 elementos
int populateVetor() {
    for(int i = 0; i < NVETOR; i++) {
        vetor[i] = i;
    }
}

void *exponentialThread (void *arg) {
    t_Args *args = (t_Args *) arg;

    for(int i = 0; i < args->size; i++) {
        args->segment[i] = args->segment[i] * args->segment[i];
    }

    free(arg);

    pthread_exit(NULL);
}

int main(void) { 
    populateVetor();

    pthread_t tid_sistema[NTHREADS];
    t_Args *arg;

    // "Divide" o vetor pelo número de threads, para determinar quantas operações cada thread vai realizar.
    // Permite aumentar o número de threads, mas não funciona bem com número ímpar :/

    int size = NVETOR / NTHREADS;

    for(int thread=0; thread<NTHREADS; thread++) {
        // Cria um ponteiro para a posição inicial do segmento de cada thread. 
        int* segment = &vetor[size * thread];

        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }

        arg->size = size; 
        arg->segment = segment; 
        
        if (pthread_create(&tid_sistema[thread], NULL, exponentialThread, (void *)arg)) {
            printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }

    for (int thread=0; thread<NTHREADS; thread++) {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("--ERRO: pthread_join() \n"); exit(-1); 
        } 
    }

    printf("RESULTADO:");
    for(int i = 0; i < NVETOR; i++) {
        printf("\n%d * %d = %d", i, i, vetor[i]);
    }


}