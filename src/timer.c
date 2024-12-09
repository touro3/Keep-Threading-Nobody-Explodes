#include "timer.h"
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

static int remaining_time = 120;  // Inicializa o tempo em 120 segundos
pthread_mutex_t timer_lock = PTHREAD_MUTEX_INITIALIZER;

void inicializa_timer(int tempo) {
    pthread_mutex_lock(&timer_lock);
    remaining_time = tempo;  // Configura o tempo inicial
    pthread_mutex_unlock(&timer_lock);
}

int tempo_restante() {
    int tempo;
    pthread_mutex_lock(&timer_lock);
    tempo = remaining_time;  // Retorna o tempo restante
    pthread_mutex_unlock(&timer_lock);
    return tempo;
}

void* timer_func(void* arg) {
    (void)arg;

    while (remaining_time > 0) {
        sleep(1);  // Aguarda 1 segundo
        pthread_mutex_lock(&timer_lock);
        remaining_time--;  // Decrementa o tempo
        pthread_mutex_unlock(&timer_lock);
    }

    return NULL;
}
