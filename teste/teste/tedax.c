#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include "tedax.h"
#include "bench.h"
#include "coordinator.h"
#include "module_board.h"

void *tedax_func(void *args) {
    int tedax_id = (intptr_t)args; // ID único para o Tedax
    while (1) {
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING) {
                module_queue[i].status = IN_PROGRESS;
                module_queue[i].bench_id = tedax_id; // Atribuir Tedax ao módulo
                pthread_mutex_unlock(&module_queue_lock);

                printf("Tedax %d desarmando módulo %d...\n", tedax_id, module_queue[i].id);

                // Realiza a ação específica baseada no tipo do módulo
                if (module_queue[i].type == 'x') {
                    for (int j = 0; j < module_queue[i].interactions; j++) {
                        printf("Tedax %d pressionando botão %d/%d\n", tedax_id, j + 1, module_queue[i].interactions);
                        sleep(1);
                    }
                } else if (module_queue[i].type == 'c') {
                    for (int j = 0; j < module_queue[i].interactions; j++) {
                        printf("Tedax %d inseriu '%c'\n", tedax_id, module_queue[i].sequence[j]);
                        sleep(1);
                    }
                } else if (module_queue[i].type == 't') {
                    printf("Tedax %d esperando %d segundos para desarmar...\n", tedax_id, module_queue[i].interactions);
                    sleep(module_queue[i].interactions);
                }

                // Marca o módulo como desarmado
                pthread_mutex_lock(&module_queue_lock);
                module_queue[i].status = DISARMED;
                module_queue[i].bench_id = -1; // Libera a bancada
                pthread_mutex_unlock(&module_queue_lock);

                printf("Tedax %d desarmou módulo %d com sucesso!\n", tedax_id, module_queue[i].id);
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);
        sleep(1); // Intervalo para evitar sobrecarga
    }
    return NULL;
}
