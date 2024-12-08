#include <pthread.h>
#include <ncurses.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "bench.h"
#include "coordinator.h"
#include "display.h"
#include "module_board.h"
#include "tedax.h"

sem_t input_ready;
sem_t module_slots;

int main() {
    pthread_t module_board_thread, display_thread, coordinator_thread;
    pthread_t tedax_threads[NUM_TEDAX];

    inicializa_display();

    // Initialize semaphores
    sem_init(&input_ready, 0, 1);
    sem_init(&module_slots, 0, MAX_MODULES);

    // Initialize Tedax list
    initialize_tedax_list();

    // Create threads
    pthread_create(&module_board_thread, NULL, module_board_func, NULL);
    pthread_create(&display_thread, NULL, display_func, NULL);
    pthread_create(&coordinator_thread, NULL, coordinator_func, NULL);

    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_create(&tedax_threads[i], NULL, tedax_func, (void *)(long)i);
    }


    // Join threads
    pthread_join(module_board_thread, NULL);
    pthread_join(display_thread, NULL);
    pthread_join(coordinator_thread, NULL);
    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_join(tedax_threads[i], NULL);
    }

    // Cleanup
    sem_destroy(&input_ready);
    sem_destroy(&module_slots);
    cleanup_tedax_list();
    encerra_display();

    return 0;
}
