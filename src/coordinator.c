#include "module_board.h" // Deve vir primeiro, pois define ModuleStatus
#include "utils.h"        // Depende de module_board.h
#include "tedax.h"
#include "coordinator.h"
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

extern sem_t input_ready;

void *coordinator_func(void *args) {
    (void)args;
    char status_message[256] = "";
    int message_timer = 0;

    while (1) {
        sem_wait(&input_ready); // Aguarda sinal de input

        pthread_mutex_lock(&module_queue_lock);
        int pending_index = -1;

        // Localiza um módulo pendente
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING) {
                pending_index = i;
                break;
            }
        }
        pthread_mutex_unlock(&module_queue_lock);

        if (pending_index == -1) {
            snprintf(status_message, sizeof(status_message),
                     "Nenhum módulo pendente.");
            message_timer = 3;
            sleep(1);
            continue;
        }

        // Solicita o input do jogador
        mvprintw(LINES - 3, 0, "Módulo %d disponível. Escolha Tedax (0-%d): ",
                 module_queue[pending_index].id, NUM_TEDAX - 1);
        refresh();

        int tedax_input = getch() - '0';

        if (tedax_input >= 0 && tedax_input < NUM_TEDAX) {
            pthread_mutex_lock(&tedax_list[tedax_input].lock);

            if (tedax_list[tedax_input].is_available) {
                pthread_mutex_lock(&module_queue_lock);
                module_queue[pending_index].status = IN_PROGRESS;
                module_queue[pending_index].bench_id = tedax_input;
                pthread_mutex_unlock(&module_queue_lock);

                tedax_list[tedax_input].is_available = 0;
                snprintf(status_message, sizeof(status_message),
                         "Módulo %d atribuído ao Tedax %d!",
                         module_queue[pending_index].id, tedax_input);
                message_timer = 3;
            } else {
                snprintf(status_message, sizeof(status_message),
                         "Tedax %d está ocupado!", tedax_input);
                message_timer = 3;
            }

            pthread_mutex_unlock(&tedax_list[tedax_input].lock);
        } else {
            snprintf(status_message, sizeof(status_message),
                     "Entrada inválida!");
            message_timer = 3;
        }

        refresh();

        // Exibe a mensagem temporária
        if (message_timer > 0) {
            mvprintw(LINES - 2, 0, "%s", status_message);
            refresh();
            sleep(1);
            message_timer--;
        }
        sem_post(&input_ready);
    }
    return NULL;
}
