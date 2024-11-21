#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h> 
#include <stdlib.h>
#include "bench.h"
#include "coordinator.h"
#include "display.h"
#include "module_board.h"
#include "tedax.h"

#define NUM_TEDAX 3

int main() {
    pthread_t module_board_thread, display_thread, tedax_threads[NUM_TEDAX];

    inicializa_display(); // Inicializa a interface ncurses

    pthread_create(&module_board_thread, NULL, module_board_func, NULL);
    pthread_create(&display_thread, NULL, display_func, NULL);

    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_create(&tedax_threads[i], NULL, tedax_func, (void *)(intptr_t)i);
    }

    pthread_join(module_board_thread, NULL);
    pthread_join(display_thread, NULL);

    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_join(tedax_threads[i], NULL);
    }

    encerra_display(); // Finaliza a interface ncurses
    return 0;
}