#include <ncurses.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include "coordinator.h"
#include "module_board.h"
#include "tedax.h"

// Semáforo global para controlar o input
extern sem_t input_ready;

void *coordinator_func(void *args) {
    while (1) {
        sem_wait(&input_ready); // Espera até que um módulo esteja disponível para ser atribuído

        pthread_mutex_lock(&module_queue_lock);
        int pending_index = -1;

        // Procura um módulo pendente na fila
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING) {
                pending_index = i;
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);

        if (pending_index == -1) {
            sem_post(&input_ready); // Libera o semáforo se não houver módulos
            sleep(1);
            continue;
        }

        // Solicita o input do jogador usando ncurses
        clear();
        mvprintw(0, 0, "Módulo ID: %d disponível. Escolha Tedax (0-%d): ", module_queue[pending_index].id, NUM_TEDAX - 1);
        refresh();

        int tedax_input = getch() - '0'; // Lê o input do usuário em tempo real

        if (tedax_input >= 0 && tedax_input < NUM_TEDAX) {
            pthread_mutex_lock(&tedax_list[tedax_input].lock);

            if (tedax_list[tedax_input].is_available) {
                pthread_mutex_lock(&module_queue_lock);
                module_queue[pending_index].status = IN_PROGRESS;
                module_queue[pending_index].bench_id = tedax_input;
                pthread_mutex_unlock(&module_queue_lock);

                tedax_list[tedax_input].is_available = 0; // Marca o Tedax como ocupado
                mvprintw(2, 0, "Módulo %d atribuído ao Tedax %d!", module_queue[pending_index].id, tedax_input);
            } else {
                mvprintw(2, 0, "Tedax %d está ocupado!", tedax_input);
            }

            pthread_mutex_unlock(&tedax_list[tedax_input].lock);
        } else {
            mvprintw(2, 0, "Entrada inválida!");
        }

        refresh();
        sem_post(&input_ready); // Libera para outro input
        sleep(1);
    }
    return NULL;
}
