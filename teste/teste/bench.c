#include <semaphore.h>
#include <stdio.h>
#include "bench.h"

sem_t benches;

void inicializa_bancadas(int num_benches) {
    sem_init(&benches, 0, num_benches); // Inicializa o semáforo com o número de bancadas disponíveis
    printf("[Bancadas] Inicializadas com %d bancadas disponíveis.\n", num_benches);
}

void ocupa_bancada(int bench_id) {
    sem_wait(&benches); // Ocupa uma bancada
    printf("[Bancadas] Bancada %d ocupada.\n", bench_id);
}

void libera_bancada(int bench_id) {
    sem_post(&benches); // Libera uma bancada
    printf("[Bancadas] Bancada %d liberada.\n", bench_id);
}

void encerra_bancadas() {
    sem_destroy(&benches); // Destroi o semáforo
    printf("[Bancadas] Recursos de bancadas liberados.\n");
}
