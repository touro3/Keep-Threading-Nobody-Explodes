#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "module_board.h"

pthread_mutex_t module_queue_lock = PTHREAD_MUTEX_INITIALIZER;
Module module_queue[MAX_MODULES];
int num_modules = 0;

void *module_board_func(void *args) {
    while (1) {
        sleep(2); // Gera um módulo a cada 2 segundos
        pthread_mutex_lock(&module_queue_lock);
        if (num_modules < MAX_MODULES) {
            Module new_module = {
                .id = rand() % 1000,
                .type = rand() % 3,
                .disarm_time = rand() % 5 + 1,
                .status = PENDING
            };
            module_queue[num_modules++] = new_module;
            printf("Novo módulo gerado! ID: %d, Tipo: %d, Tempo: %d\n",
                   new_module.id, new_module.type, new_module.disarm_time);
        }
        pthread_mutex_unlock(&module_queue_lock);
    }
    return NULL;
}

Module get_next_module() {
    pthread_mutex_lock(&module_queue_lock);
    for (int i = 0; i < num_modules; i++) {
        if (module_queue[i].status == PENDING) {
            module_queue[i].status = IN_PROGRESS;
            pthread_mutex_unlock(&module_queue_lock);
            return module_queue[i];
        }
    }
    pthread_mutex_unlock(&module_queue_lock);
    Module empty = { .id = -1 }; // Retorna um módulo inválido se nenhum estiver disponível
    return empty;
}