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
        Module module = get_next_module(); // Obtém o próximo módulo pendente
        if (module.id != -1) {
            ocupa_bancada(tedax_id); // Ocupa uma bancada para desarmar
            printf("Tedax %d desarmando módulo %d (tempo: %d segundos)...\n",
                   tedax_id, module.id, module.disarm_time);
            sleep(module.disarm_time); // Simula o tempo necessário para desarmar
            libera_bancada(tedax_id); // Libera a bancada após o desarme

            printf("Tedax %d desarmou o módulo %d com sucesso!\n", tedax_id, module.id);
            pthread_mutex_lock(&module_queue_lock);
            for (int i = 0; i < num_modules; i++) {
                if (module_queue[i].id == module.id) {
                    module_queue[i].status = DISARMED;
                    break;
                }
            }
            pthread_mutex_unlock(&module_queue_lock);
        } else {
            sleep(1); // Espera um momento antes de tentar novamente
        }
    }
    return NULL;
}