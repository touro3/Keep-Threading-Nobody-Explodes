#include <ncurses.h>
#include "display.h"
#include "module_board.h"

void inicializa_display() {
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    noecho();
    cbreak();
}

void *display_func(void *args) {
    (void)args; // Evita aviso de parâmetro não usado
    while (1) {
        clear();
        mvprintw(0, 0, "Status do Jogo:");
        mvprintw(1, 0, "Módulos pendentes: ...");
        mvprintw(2, 0, "Tedax disponíveis: ...");
        refresh();
        sleep(1); // Atualiza a cada segundo
    }
    return NULL;
}

void encerra_display() {
    endwin();
}