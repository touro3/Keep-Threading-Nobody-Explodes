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
    int tedax_id = (intptr_t)args;
    while (1) {
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING) {
                module_queue[i].status = IN_PROGRESS;
                pthread_mutex_unlock(&module_queue_lock);

                printf("Tedax %d desarmando módulo %d (tempo: %d segundos)...\n",
                       tedax_id, module_queue[i].id, module_queue[i].disarm_time);
                sleep(module_queue[i].disarm_time);

                pthread_mutex_lock(&module_queue_lock);
                module_queue[i].status = DISARMED;
                pthread_mutex_unlock(&module_queue_lock);

                printf("Tedax %d desarmou o módulo %d com sucesso!\n", tedax_id, module_queue[i].id);
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);
        sleep(1); // Aguardar um pouco antes de buscar outro módulo
    }
    return NULL;
}