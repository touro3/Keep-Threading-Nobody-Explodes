#include "utils.h"
#include "module_board.h"
#include "tedax.h"
#include <ncurses.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> // Para evitar warnings sobre exit()


// Função para inicializar o display
void inicializa_display() {
    initscr();
    cbreak();
    noecho();
    curs_set(0);
    clear();

    // Inicializa mensagens dos Tedax
    for (int i = 0; i < NUM_TEDAX; i++) {
        snprintf(tedax_actions[i], sizeof(tedax_actions[i]), "Tedax %d: Disponível", i);
    }
    strcpy(temporary_message, "Bem-vindo ao jogo!");
    refresh();
}

// Função para encerrar o display
void encerra_display() {
    endwin();
}

// Função que será executada pela thread de display
void *display_func(void *args) {
    (void)args; // Evita warnings com argumentos não utilizados
    inicializa_display();

    while (1) {
        clear();

        // Painel de Status do Jogo
        mvprintw(0, 0, "+--------------------- Status do Jogo ----------------------+");
        mvprintw(1, 0, "| Módulos pendentes: %d", count_modules(PENDING));
        mvprintw(2, 0, "| Módulos em progresso: %d", count_modules(IN_PROGRESS));
        mvprintw(3, 0, "| Módulos desarmados: %d", count_modules(DISARMED));
        mvprintw(4, 0, "+----------------------------------------------------------+");

        desenha_bomba(0, 70);

        // Verifica se todos os módulos foram desarmados
        if (count_modules(DISARMED) == 10) { // Considerando 10 módulos no total
            clear();
            mvprintw(10, 10, "Parabéns! Todos os módulos foram desarmados com sucesso!");
            refresh();
            sleep(3); // Dá tempo para o jogador ver a mensagem
            encerra_display(); // Encerra o modo ncurses
            exit(0);           // Sai do programa
        }

        // Painel de Lista de Módulos
        mvprintw(6, 0, "+---------------------- Lista de Módulos -------------------+");
        pthread_mutex_lock(&module_queue_lock);
        for (int i = 0; i < num_modules; i++) {
            const char *status = (module_queue[i].status == PENDING) ? "Pendente" :
                                 (module_queue[i].status == IN_PROGRESS) ? "Em progresso" :
                                 "Desarmado";

            mvprintw(7 + i, 0, "| Módulo %d | Tipo: %c | Status: %s | Interações: %d",
                     module_queue[i].id, module_queue[i].type, status, 
                     module_queue[i].interactions);
        }
        pthread_mutex_unlock(&module_queue_lock);
        mvprintw(17, 0, "+----------------------------------------------------------+");

        // Painel de Mensagens
        pthread_mutex_lock(&message_lock);
        mvprintw(19, 0, "+------------------------ Mensagens ------------------------+");
        mvprintw(20, 0, "| %s", temporary_message);
        pthread_mutex_unlock(&message_lock);
        mvprintw(21, 0, "+----------------------------------------------------------+");

        // Painel de Ações dos Tedax
        pthread_mutex_lock(&tedax_action_lock);
        mvprintw(23, 0, "+--------------------- Ações dos Tedax ---------------------+");
        for (int i = 0; i < NUM_TEDAX; i++) {
            mvprintw(24 + i, 0, "| Tedax %d: %s", i, tedax_actions[i]);
        }
        pthread_mutex_unlock(&tedax_action_lock);
        mvprintw(27, 0, "+----------------------------------------------------------+");

        // Atualiza a interface
        refresh();
        sleep(1); // Atualização periódica
    }

    encerra_display();
    return NULL;
}