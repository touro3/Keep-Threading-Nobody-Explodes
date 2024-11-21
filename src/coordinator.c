#include <ncurses.h>
#include <stdio.h>
#include "coordinator.h"

void *coordinator_func(void *args) {
    while (1) {
        int module_id, tedax_id, bench_id;
        mvprintw(5, 0, "Escolha módulo, Tedax e bancada (M T B): ");
        refresh();

        scanw("%d %d %d", &module_id, &tedax_id, &bench_id);

        // Simula a designação
        mvprintw(6, 0, "Designando módulo %d ao Tedax %d na bancada %d.\n",
                 module_id, tedax_id, bench_id);
        refresh();
    }
    return NULL;
}
