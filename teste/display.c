#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include "display.h"
#include "module_board.h"

// Inicializa a interface do display usando ncurses
void inicializa_display() {
    initscr();
    start_color();
    // Definindo pares de cores
    init_pair(1, COLOR_WHITE, COLOR_BLUE);   // Fundo azul, texto branco
    init_pair(2, COLOR_WHITE, COLOR_BLACK);  // Fundo preto, texto branco
    init_pair(3, COLOR_GREEN, COLOR_BLACK);  // Texto verde
    init_pair(4, COLOR_YELLOW, COLOR_BLACK); // Texto amarelo
    init_pair(5, COLOR_RED, COLOR_BLACK);    // Texto vermelho
    noecho();                                // Evita eco ao digitar
    cbreak();                                // Entra no modo "caracter imediato"
    curs_set(0);                             // Esconde o cursor
}

// Finaliza o display e restaura o terminal
void encerra_display() {
    endwin();
}

// Thread principal para exibição das informações
void *display_func(void *args) {
    while (1) {
        pthread_mutex_lock(&module_queue_lock);

        // Limpa a tela para atualizações
        clear();

        // Exibe título com borda decorativa
        attron(COLOR_PAIR(1));
        mvprintw(0, 0, "══════════════════════════════════════════════");
        mvprintw(1, 0, "                 STATUS DO JOGO                ");
        mvprintw(2, 0, "══════════════════════════════════════════════");
        attroff(COLOR_PAIR(1));

        // Estatísticas dos módulos
        int pending = 0, in_progress = 0, disarmed = 0;
        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING)
                pending++;
            else if (module_queue[i].status == IN_PROGRESS)
                in_progress++;
            else
                disarmed++;
        }

        // Exibe as contagens de módulos com cores
        mvprintw(4, 0, "Módulos pendentes: ");
        attron(COLOR_PAIR(4));
        printw("%d", pending);
        attroff(COLOR_PAIR(4));

        mvprintw(5, 0, "Módulos em progresso: ");
        attron(COLOR_PAIR(3));
        printw("%d", in_progress);
        attroff(COLOR_PAIR(3));

        mvprintw(6, 0, "Módulos desarmados: ");
        attron(COLOR_PAIR(5));
        printw("%d", disarmed);
        attroff(COLOR_PAIR(5));

        // Exibe detalhes de cada módulo
        mvprintw(8, 0, "Detalhes dos Módulos:");
        for (int i = 0; i < num_modules; i++) {
            mvprintw(9 + i, 0, "Módulo %d | Tipo: %c | Status: %s | Interações/Tempo: %d",
                     module_queue[i].id,
                     module_queue[i].type,
                     module_queue[i].status == PENDING ? "Pendente" :
                     module_queue[i].status == IN_PROGRESS ? "Em progresso" : "Desarmado",
                     module_queue[i].interactions);
        }

        // Atualiza a tela
        refresh();

        pthread_mutex_unlock(&module_queue_lock);

        // Aguarda antes da próxima atualização
        usleep(500000); // 500ms para uma atualização mais fluida
    }
    return NULL;
}