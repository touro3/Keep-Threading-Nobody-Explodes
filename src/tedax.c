#include "tedax.h"
#include "module_board.h"
#include "utils.h"
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

        // Verifica se há um módulo atribuído ao Tedax
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == IN_PROGRESS && module_queue[i].bench_id == tedax_id) {
                assigned_module = &module_queue[i];
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);

        if (assigned_module != NULL) {
            // Atualiza a ação do Tedax para desarmando o módulo
            pthread_mutex_lock(&tedax_action_lock);
            snprintf(tedax_actions[tedax_id], sizeof(tedax_actions[tedax_id]),
                     "Tedax %d desarmando módulo %d...", tedax_id, assigned_module->id);
            pthread_mutex_unlock(&tedax_action_lock);
            refresh();

            // Processa o módulo
            if (assigned_module->type == 'x') {
                for (int i = 0; i < assigned_module->interactions; i++) {
                    sleep(1); // Simula o tempo de processamento
                }
            } else if (assigned_module->type == 'c') {
                for (int i = 0; i < assigned_module->interactions; i++) {
                    sleep(1); // Simula o tempo de processamento
                }
            } else if (assigned_module->type == 't') {
                sleep(assigned_module->interactions); // Simula o tempo de espera
            }

            // Marca o módulo como desarmado
            pthread_mutex_lock(&module_queue_lock);
            assigned_module->status = DISARMED;
            pthread_mutex_unlock(&module_queue_lock);

            // Atualiza a mensagem de sucesso e redefine como disponível
            pthread_mutex_lock(&tedax_action_lock);
            snprintf(tedax_actions[tedax_id], sizeof(tedax_actions[tedax_id]),
                     "Tedax %d: Disponível", tedax_id);
            pthread_mutex_unlock(&tedax_action_lock);
            refresh();

            // Marca o Tedax como disponível novamente
            pthread_mutex_lock(&tedax_list[tedax_id].lock);
            tedax_list[tedax_id].is_available = 1;
            pthread_mutex_unlock(&tedax_list[tedax_id].lock);
        } else {
            // Se não há módulos atribuídos, o Tedax permanece disponível
            pthread_mutex_lock(&tedax_action_lock);
            snprintf(tedax_actions[tedax_id], sizeof(tedax_actions[tedax_id]),
                     "Tedax %d: Disponível", tedax_id);
            pthread_mutex_unlock(&tedax_action_lock);
            refresh();
            sleep(1);
        }
    }

    return NULL;
}