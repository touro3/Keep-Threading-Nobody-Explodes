#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "tedax.h"
#include "bench.h"
#include "module_board.h"

void *tedax_func(void *args) {
    int tedax_id = (intptr_t)args; // ID único para o Tedax
    while (1) {
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING) {
                module_queue[i].status = IN_PROGRESS;
                pthread_mutex_unlock(&module_queue_lock);

                printf("[Tedax %d] Esperando uma bancada para desarmar módulo %d...\n", tedax_id, module_queue[i].id);
                ocupa_bancada(module_queue[i].bench_id); // Espera por uma bancada

                printf("[Tedax %d] Começando a desarmar módulo %d na bancada %d...\n",
                       tedax_id, module_queue[i].id, module_queue[i].bench_id);

                // Realiza a ação específica baseada no tipo do módulo
                if (module_queue[i].type == 'x') {
                    for (int j = 0; j < module_queue[i].interactions; j++) {
                        printf("[Tedax %d] Pressionando botão %d/%d\n", tedax_id, j + 1, module_queue[i].interactions);
                        sleep(1);
                    }
                } else if (module_queue[i].type == 'c') {
                    for (int j = 0; j < module_queue[i].interactions; j++) {
                        printf("[Tedax %d] Inserindo sequência '%c'\n", tedax_id, module_queue[i].sequence[j]);
                        sleep(1);
                    }
                } else if (module_queue[i].type == 't') {
                    printf("[Tedax %d] Aguardando %d segundos para desarmar...\n",
                           tedax_id, module_queue[i].interactions);
                    sleep(module_queue[i].interactions);
                }

                // Marca o módulo como desarmado
                pthread_mutex_lock(&module_queue_lock);
                module_queue[i].status = DISARMED;
                pthread_mutex_unlock(&module_queue_lock);

                libera_bancada(module_queue[i].bench_id); // Libera a bancada
                printf("[Tedax %d] Desarmou módulo %d com sucesso e liberou bancada %d.\n",
                       tedax_id, module_queue[i].id, module_queue[i].bench_id);
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);
        sleep(1); // Intervalo para evitar sobrecarga
    }
    return NULL;
}
