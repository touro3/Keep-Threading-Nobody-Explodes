#include <ncurses.h>
#include <unistd.h> 
#include "display.h"
#include "module_board.h"

void inicializa_display() {
    initscr();           // Inicializa a tela
    start_color();       // Habilita cores
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    noecho();            // Não mostra as teclas digitadas
    cbreak();            // Entrada sem esperar Enter
}

void *display_func(void *args) {
    (void)args; // Para evitar warnings sobre o parâmetro não utilizado
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
    endwin(); // Finaliza a interface ncurses
}