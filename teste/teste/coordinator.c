#include <ncurses.h>
#include <stdio.h>
#include "coordinator.h"
#include "module_board.h"
#include "bench.h"

void *coordinator_func(void *args) {
    (void)args;

    while (1) {
        int module_id, tedax_id, bench_id;

        mvprintw(5, 0, "Escolha módulo, Tedax e bancada (M T B): ");
        refresh();
        scanw("%d %d %d", &module_id, &tedax_id, &bench_id);

        // Simula a designação
        mvprintw(6, 0, "Designando módulo %d ao Tedax %d na bancada %d.\n",
                 module_id, tedax_id, bench_id);
        refresh();

        // Integração lógica: marcando o módulo como em progresso e ocupando a bancada
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].id == module_id && module_queue[i].status == PENDING) {
                module_queue[i].status = IN_PROGRESS;
                ocupa_bancada(bench_id);
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);
    }

    return NULL;
}
