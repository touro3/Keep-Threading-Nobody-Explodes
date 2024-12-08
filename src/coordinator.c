#include "tedax.h"
#include "coordinator.h"
#include "module_board.h"
#include "utils.h"
#include <ncurses.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

// Semáforo para controle de input
extern sem_t input_ready;

// Função de coordenação
void *coordinator_func(void *args) {
    (void)args;

    while (1) {
        sem_wait(&input_ready); // Aguarda sinal para processar input

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
            pthread_mutex_lock(&message_lock);
            snprintf(temporary_message, sizeof(temporary_message), "Nenhum módulo pendente no momento.");
            pthread_mutex_unlock(&message_lock);
            sleep(1);
            continue;
        }

        // Solicita o input do jogador
        int tedax_input = -1;
        pthread_mutex_lock(&message_lock);
        snprintf(temporary_message, sizeof(temporary_message), "Módulo %d disponível. Escolha Tedax (0-%d): ",
                 module_queue[pending_index].id, NUM_TEDAX - 1);
        pthread_mutex_unlock(&message_lock);

        refresh(); // Atualiza o display
        tedax_input = getch() - '0'; // Recebe input do jogador

        if (tedax_input >= 0 && tedax_input < NUM_TEDAX) {
            pthread_mutex_lock(&tedax_list[tedax_input].lock);

            if (tedax_list[tedax_input].is_available) {
                pthread_mutex_lock(&module_queue_lock);
                module_queue[pending_index].status = IN_PROGRESS;
                module_queue[pending_index].bench_id = tedax_input;
                pthread_mutex_unlock(&module_queue_lock);

                tedax_list[tedax_input].is_available = 0;

                // Atualiza mensagem de atribuição
                pthread_mutex_lock(&message_lock);
                snprintf(temporary_message, sizeof(temporary_message),
                         "Módulo %d atribuído ao Tedax %d!",
                         module_queue[pending_index].id, tedax_input);
                pthread_mutex_unlock(&message_lock);

                // Atualiza ações do Tedax
                pthread_mutex_lock(&tedax_action_lock);
                snprintf(tedax_actions[tedax_input], sizeof(tedax_actions[tedax_input]),
                         "Tedax %d desarmando módulo %d...", tedax_input, module_queue[pending_index].id);
                pthread_mutex_unlock(&tedax_action_lock);

            } else {
                // Tedax está ocupado
                pthread_mutex_lock(&message_lock);
                snprintf(temporary_message, sizeof(temporary_message),
                         "Tedax %d está ocupado!", tedax_input);
                pthread_mutex_unlock(&message_lock);
            }

            pthread_mutex_unlock(&tedax_list[tedax_input].lock);
        } else {
            // Entrada inválida
            pthread_mutex_lock(&message_lock);
            snprintf(temporary_message, sizeof(temporary_message), "Entrada inválida!");
            pthread_mutex_unlock(&message_lock);
        }

        refresh();
        sem_post(&input_ready); // Libera para novo input
        sleep(1);
    }

    return NULL;
}
