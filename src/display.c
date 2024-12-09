#include "utils.h"
#include "module_board.h"
#include "tedax.h"
#include "timer.h"
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

void inicializa_display() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();

    for (int i = 0; i < NUM_TEDAX; i++) {
        snprintf(tedax_actions[i], sizeof(tedax_actions[i]), "Tedax %d: Disponível", i);
    }
    strcpy(temporary_message, "Bem-vindo ao jogo!");
    refresh();
}

void encerra_display() {
    endwin();
}

void *display_func(void *args) {
    (void)args;
    inicializa_display();

    while (1) {
        clear();

        // Painel de Status do Jogo
        mvprintw(0, 0, "+--------------------- Status do Jogo ----------------------+");
        mvprintw(1, 0, "| Módulos pendentes: %d", count_modules(PENDING));
        mvprintw(2, 0, "| Módulos em progresso: %d", count_modules(IN_PROGRESS));
        mvprintw(3, 0, "| Módulos desarmados: %d", count_modules(DISARMED));
        mvprintw(4, 0, "| Tempo restante: %d segundos", tempo_restante());
        mvprintw(5, 0, "+----------------------------------------------------------+");

        desenha_bomba(6, 70);

        if (count_modules(DISARMED) == 10) {
            clear();
            mvprintw(10, 10, "Parabéns! Todos os módulos foram desarmados com sucesso!");
            refresh();
            sleep(3);
            encerra_display();
            exit(0);
        }

        if (tempo_restante() <= 0) {
            clear();
            mvprintw(10, 10, "Tempo esgotado! Você perdeu!");
            refresh();
            sleep(3);
            encerra_display();
            exit(1);
        }

        // Painel de Lista de Módulos
        mvprintw(8, 0, "+---------------------- Lista de Módulos -------------------+");
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            const char *status = (module_queue[i].status == PENDING) ? "Pendente" :
                                 (module_queue[i].status == IN_PROGRESS) ? "Em progresso" :
                                 "Desarmado";

            mvprintw(9 + i, 0, "| Módulo %d | Tipo: %c | Status: %s | Interações: %d",
                     module_queue[i].id, module_queue[i].type, status,
                     module_queue[i].interactions);
        }
        pthread_mutex_unlock(&module_queue_lock);
        mvprintw(19, 0, "+----------------------------------------------------------+");

        // Painel de Mensagens
        pthread_mutex_lock(&message_lock);
        mvprintw(21, 0, "+------------------------ Mensagens ------------------------+");
        mvprintw(22, 0, "| %s", temporary_message);
        pthread_mutex_unlock(&message_lock);
        mvprintw(23, 0, "+----------------------------------------------------------+");

        // Painel de Ações dos Tedax
        pthread_mutex_lock(&tedax_action_lock);
        mvprintw(25, 0, "+--------------------- Ações dos Tedax ---------------------+");
        for (int i = 0; i < NUM_TEDAX; i++) {
            mvprintw(26 + i, 0, "| Tedax %d: %s", i, tedax_actions[i]);
        }
        pthread_mutex_unlock(&tedax_action_lock);
        mvprintw(29, 0, "+----------------------------------------------------------+");

        refresh();
        sleep(1);
    }

    encerra_display();
    return NULL;
}