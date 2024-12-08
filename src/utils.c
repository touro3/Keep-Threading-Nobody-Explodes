#include <stdlib.h>
#include <pthread.h>
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
