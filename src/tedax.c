#include "tedax.h"
#include "module_board.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <ncurses.h>
#include <semaphore.h>

// Lista global de Tedax e número de Tedax
Tedax tedax_list[NUM_TEDAX];


// Inicializa a lista de Tedax
void initialize_tedax_list(void) {
    for (int i = 0; i < NUM_TEDAX; i++) {
        tedax_list[i].id = i;
        tedax_list[i].is_available = 1;
        pthread_mutex_init(&tedax_list[i].lock, NULL);
    }
    printf("Lista de Tedax inicializada com %d Tedax.\n", NUM_TEDAX);
}

// Limpa a lista de Tedax
void cleanup_tedax_list(void) {
    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_mutex_destroy(&tedax_list[i].lock);
    }
    printf("Lista de Tedax limpa.\n");
}

// Função para o Tedax processar os módulos
void *tedax_func(void *args) {
    int tedax_id = (int)(intptr_t)args;

    while (1) {
        Module *assigned_module = NULL;

        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == IN_PROGRESS && module_queue[i].bench_id == tedax_id) {
                assigned_module = &module_queue[i];
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);

        if (assigned_module != NULL) {
            mvprintw(15 + tedax_id, 0, "Tedax %d desarmando módulo %d...\n", tedax_id, assigned_module->id);
            refresh();

            if (assigned_module->type == 'x') {
                for (int i = 0; i < assigned_module->interactions; i++) {
                    mvprintw(15 + tedax_id, 0, "Tedax %d pressionando botão %d/%d...",
                             tedax_id, i + 1, assigned_module->interactions);
                    refresh();
                    sleep(1);
                }
            } else if (assigned_module->type == 'c') {
                for (int i = 0; i < assigned_module->interactions; i++) {
                    mvprintw(15 + tedax_id, 0, "Tedax %d inserindo '%c'...",
                             tedax_id, assigned_module->sequence[i]);
                    refresh();
                    sleep(1);
                }
            } else if (assigned_module->type == 't') {
                mvprintw(15 + tedax_id, 0, "Tedax %d esperando %d segundos...",
                         tedax_id, assigned_module->interactions);
                refresh();
                sleep(assigned_module->interactions);
            }

            pthread_mutex_lock(&module_queue_lock);
            assigned_module->status = DISARMED;
            pthread_mutex_unlock(&module_queue_lock);

            mvprintw(15 + tedax_id, 0, "Tedax %d desarmou módulo %d com sucesso!",
                     tedax_id, assigned_module->id);
            refresh();
            sleep(2);

            pthread_mutex_lock(&tedax_list[tedax_id].lock);
            tedax_list[tedax_id].is_available = 1;
            pthread_mutex_unlock(&tedax_list[tedax_id].lock);
        }
        sleep(1);
    }
    return NULL;
}