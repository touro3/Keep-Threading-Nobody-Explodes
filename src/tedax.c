#include "tedax.h"
#include "module_board.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
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
    int tedax_id = (int)(long)args;

    while (1) {
        Module *assigned_module = NULL;

        pthread_mutex_lock(&module_queue_lock);
        // Procura um módulo atribuído ao Tedax atual
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == IN_PROGRESS && module_queue[i].bench_id == tedax_id) {
                assigned_module = &module_queue[i];
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);

        if (assigned_module != NULL) {
            // Processa o módulo
            printf("Tedax %d desarmando módulo %d...\n", tedax_id, assigned_module->id);

            if (assigned_module->type == 'x') {
                for (int i = 0; i < assigned_module->interactions; i++) {
                    printf("Tedax %d pressionando botão %d/%d...\n", tedax_id, i + 1, assigned_module->interactions);
                    sleep(1);
                }
            } else if (assigned_module->type == 'c') {
                for (int i = 0; i < assigned_module->interactions; i++) {
                    printf("Tedax %d inserindo '%c'...\n", tedax_id, assigned_module->sequence[i]);
                    sleep(1);
                }
            } else if (assigned_module->type == 't') {
                printf("Tedax %d aguardando %d segundos...\n", tedax_id, assigned_module->interactions);
                sleep(assigned_module->interactions);
            }

            // Marca o módulo como desarmado
            pthread_mutex_lock(&module_queue_lock);
            assigned_module->status = DISARMED;
            pthread_mutex_unlock(&module_queue_lock);

            printf("Tedax %d desarmou módulo %d com sucesso!\n", tedax_id, assigned_module->id);

            // Marca o Tedax como disponível novamente
            pthread_mutex_lock(&tedax_list[tedax_id].lock);
            tedax_list[tedax_id].is_available = 1;
            pthread_mutex_unlock(&tedax_list[tedax_id].lock);
        }

        // Tempo de espera entre as verificações
        sleep(1);
    }

    return NULL;
}
