#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <string.h>
#include <unistd.h>
#include "module_board.h"

extern sem_t module_slots; // Semáforo para slots de módulos
extern sem_t input_ready;

Module module_queue[MAX_MODULES];
int num_modules = 0;
pthread_mutex_t module_queue_lock = PTHREAD_MUTEX_INITIALIZER;

void *module_board_func(void *args) {
    (void)args;
    while (1) {
        sem_wait(&module_slots); // Aguarda um slot disponível para criar um novo módulo
        pthread_mutex_lock(&module_queue_lock);

        if (num_modules < MAX_MODULES) {
            Module new_module = {
                .id = rand() % 1000,
                .bench_id = -1,
                .status = PENDING
            };

            // Determina o tipo do módulo
            int type_choice = rand() % 3;
            if (type_choice == 0) {
                new_module.type = 'x';
                new_module.interactions = rand() % 5 + 1;
            } else if (type_choice == 1) {
                new_module.type = 'c';
                new_module.interactions = strlen("abc");
                strcpy(new_module.sequence, "abc");
            } else {
                new_module.type = 't';
                new_module.interactions = 5;
            }

            module_queue[num_modules++] = new_module;
            printf("Novo módulo criado: ID %d, Tipo %c\n", new_module.id, new_module.type);
        }

        pthread_mutex_unlock(&module_queue_lock);
        sem_post(&input_ready); // Notifica o coordenador que há um novo módulo
        sleep(2); // Pequena pausa para evitar excesso de criação
    }
    return NULL;
}
