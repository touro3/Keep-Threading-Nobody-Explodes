#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include "display.h"
#include "module_board.h"
#include "tedax.h"
#include "timer.h"
#include "coordinator.h"

sem_t input_ready;
sem_t module_slots;

int main() {
    pthread_t module_board_thread, display_thread, coordinator_thread;
    pthread_t tedax_threads[NUM_TEDAX];
    pthread_t timer_thread;  // Nova thread para o temporizador

    inicializa_display();

    // Inicializa semáforos
    sem_init(&input_ready, 0, 1);
    sem_init(&module_slots, 0, MAX_MODULES);

    // Inicializa a lista de Tedax
    initialize_tedax_list();

    // Criação das threads
    pthread_create(&module_board_thread, NULL, module_board_func, NULL);
    pthread_create(&display_thread, NULL, display_func, NULL);
    pthread_create(&coordinator_thread, NULL, coordinator_func, NULL);
    pthread_create(&timer_thread, NULL, timer_func, NULL);  // Cria a thread do temporizador

    for (int i = 0; i < NUM_TEDAX; i++) {
        pthread_create(&tedax_threads[i], NULL, tedax_func, (void *)(long)i);
    }

    // Inicializa o temporizador
    inicializa_timer(120);  // Configura o temporizador para 120 segundos

    // Aguarda finalização das threads
    pthread_join(module_board_thread, NULL);
    pthread_join(display_thread, NULL);
    pthread_join(coordinator_thread, NULL);
    pthread_join(timer_thread, NULL);  // Aguarda a thread do temporizador
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
