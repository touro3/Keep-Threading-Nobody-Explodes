#include <ncurses.h>
#include <unistd.h> 
#include <stdlib.h>
#include "display.h"
#include "module_board.h"

void inicializa_display() {
    initscr();
    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    noecho();
    cbreak();
}

void encerra_display() {
    endwin();
}

void *display_func(void *args) {
    while (1) {
        pthread_mutex_lock(&module_queue_lock);
        clear();
        mvprintw(0, 0, "Status do Jogo:");
        int pending = 0, in_progress = 0, disarmed = 0;

        for (int i = 0; i < num_modules; i++) {
            if (module_queue[i].status == PENDING) pending++;
            else if (module_queue[i].status == IN_PROGRESS) in_progress++;
            else disarmed++;
        }

        mvprintw(1, 0, "Módulos pendentes: %d", pending);
        mvprintw(2, 0, "Módulos em progresso: %d", in_progress);
        mvprintw(3, 0, "Módulos desarmados: %d", disarmed);

        int line = 5;
        for (int i = 0; i < num_modules; i++) {
            mvprintw(line++, 0, "Módulo %d | Tipo: %c | Status: %s | Interações/Tempo: %d",
                     module_queue[i].id,
                     module_queue[i].type,
                     module_queue[i].status == PENDING ? "Pendente" :
                     module_queue[i].status == IN_PROGRESS ? "Em progresso" : "Desarmado",
                     module_queue[i].interactions);
        }

        refresh();
        pthread_mutex_unlock(&module_queue_lock);
        sleep(1);
    }
    return NULL;
}