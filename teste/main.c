#include <pthread.h>
#include <ncurses.h>
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
#define NUM_BENCHES 3

int main() {
    pthread_t module_board_thread, display_thread, tedax_threads[NUM_TEDAX], coordinator_thread;

    inicializa_display();
    inicializa_bancadas(NUM_BENCHES);

    // Cria as threads
    pthread_create(&module_board_thread, NULL, module_board_func, NULL);
    pthread_create(&display_thread, NULL, display_func, NULL);
    pthread_create(&coordinator_thread, NULL, coordinator_func, NULL);

    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_create(&tedax_threads[i], NULL, tedax_func, (void *)(intptr_t)i);
    }

    // Aguarda o término das threads
    pthread_join(module_board_thread, NULL);
    pthread_join(display_thread, NULL);
    pthread_join(coordinator_thread, NULL);
    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_join(tedax_threads[i], NULL);
    }

    encerra_display();
    encerra_bancadas();
    return 0;
}
