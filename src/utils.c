#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include "module_board.h"
#include "utils.h"

extern Module module_queue[MAX_MODULES];
extern int num_modules;
extern pthread_mutex_t module_queue_lock;
pthread_mutex_t message_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tedax_action_lock = PTHREAD_MUTEX_INITIALIZER;
char tedax_actions[NUM_TEDAX][256];
char temporary_message[100];


int count_modules(ModuleStatus status) {
    int count = 0;
    pthread_mutex_lock(&module_queue_lock);

    for (int i = 0; i < num_modules; i++) {
        if (module_queue[i].status == status) {
            count++;
        }
    }

    pthread_mutex_unlock(&module_queue_lock);
    return count;
}

int generate_random(int min, int max) {
    return min + rand() % (max - min + 1);
}

void desenha_bomba(int linha, int coluna) {
    mvprintw(linha, coluna, "                        $#     #     #¢");
    mvprintw(linha + 1, coluna, "           #######       #¢   #   ø#");
    mvprintw(linha + 2, coluna, "          ##    ø###      oø  ø  øo");
    mvprintw(linha + 3, coluna, "          #7       ###      1   1    1o");
    mvprintw(linha + 4, coluna, "       #######       ###7        1o###ø");
    mvprintw(linha + 5, coluna, "       #######         ########  1");
    mvprintw(linha + 6, coluna, "     o#########ø                  o$¢");
    mvprintw(linha + 7, coluna, "   ###############          ¢  1ø   1##");
    mvprintw(linha + 8, coluna, "  #################o       1$   #");
    mvprintw(linha + 9, coluna, " ###################       #    o#");
    mvprintw(linha + 10, coluna, " ####################     ##");
    mvprintw(linha + 11, coluna, "  ##################");
    mvprintw(linha + 12, coluna, "   ################");
    mvprintw(linha + 13, coluna, "     ############");
    mvprintw(linha + 14, coluna, "       ########");
}