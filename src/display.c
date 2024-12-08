#include "display.h"
#include "module_board.h"
#include "tedax.h"
#include "utils.h"
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>

// Inicializa o display do ncurses
void inicializa_display() {
    initscr();       // Inicializa o terminal ncurses
    cbreak();        // Desativa o buffer de linha
    noecho();        // Não exibe caracteres digitados
    keypad(stdscr, TRUE); // Permite o uso de teclas especiais
    curs_set(0);     // Oculta o cursor
}

// Encerra o display do ncurses
void encerra_display() {
    endwin(); // Finaliza o terminal ncurses
}

// Função de exibição do jogo
void *display_func(void *args) {
    (void)args;
    initscr();
    cbreak();
    noecho();
    curs_set(0);

    while (1) {
        clear();

        // Atualizar o status do jogo
        mvprintw(0, 0, "Status do Jogo:");
        mvprintw(1, 0, "Módulos pendentes: %d", count_modules(PENDING));
        mvprintw(2, 0, "Módulos em progresso: %d", count_modules(IN_PROGRESS));
        mvprintw(3, 0, "Módulos desarmados: %d", count_modules(DISARMED));

        // Atualizar a lista de módulos
        mvprintw(5, 0, "Lista de Módulos:");
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            const char *status;
            switch (module_queue[i].status) {
                case PENDING: status = "Pendente"; break;
                case IN_PROGRESS: status = "Em progresso"; break;
                case DISARMED: status = "Desarmado"; break;
                default: status = "Desconhecido"; break;
            }
            mvprintw(6 + i, 0, "Módulo %d | Tipo: %c | Status: %s | Interações/Tempo: %d",
                     module_queue[i].id, module_queue[i].type, status, module_queue[i].interactions);
        }
        pthread_mutex_unlock(&module_queue_lock);

        // Atualizar mensagens temporárias
        pthread_mutex_lock(&message_lock);
        mvprintw(LINES - 5, 0, "%s", temporary_message);
        pthread_mutex_unlock(&message_lock);

        // Atualizar ações dos Tedax
        mvprintw(LINES - 3, 0, "Ações dos Tedax:");
        pthread_mutex_lock(&tedax_action_lock);
        for (int i = 0; i < NUM_TEDAX; i++) {
            mvprintw(LINES - 2 + i, 0, "Tedax %d: %s", i, tedax_actions[i]);
        }
        pthread_mutex_unlock(&tedax_action_lock);

        refresh();
        sleep(1);
    }

    endwin();
    return NULL;
}
