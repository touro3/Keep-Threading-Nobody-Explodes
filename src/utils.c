#include <stdlib.h>
#include <pthread.h>
#include "module_board.h"
#include "utils.h"

extern Module module_queue[MAX_MODULES];
extern int num_modules;
extern pthread_mutex_t module_queue_lock;

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
