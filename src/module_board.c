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
        sleep(2); // Geração fixa de módulos
        pthread_mutex_lock(&module_queue_lock);

        if (num_modules < MAX_MODULES) {
            Module new_module = {
                .id = rand() % 1000,
                .bench_id = rand() % 3,
                .status = PENDING
            };

            int type_choice = rand() % 3;
            if (type_choice == 0) {
                new_module.type = 'x'; // Pressionar botões
                new_module.interactions = rand() % 5 + 1; // 1-5 interações
            } else if (type_choice == 1) {
                new_module.type = 'c'; // Inserir sequência
                new_module.interactions = strlen("abc");
                strcpy(new_module.sequence, "abc"); // Sequência fixa
            } else {
                new_module.type = 't'; // Temporizador fixo
                new_module.interactions = 5; // 5 segundos fixos
            }

            module_queue[num_modules++] = new_module;
        }

        pthread_mutex_unlock(&module_queue_lock);
    }
    return NULL;
}