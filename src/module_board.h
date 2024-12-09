#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>

#define MAX_MODULES 10

typedef enum {
    PENDING,
    IN_PROGRESS,
    DISARMED
} ModuleStatus;

typedef struct {
    int id;
    int bench_id;
    char type;
    int interactions;
    char sequence[10];
    ModuleStatus status;
} Module;

extern Module module_queue[MAX_MODULES];
extern int num_modules;
extern pthread_mutex_t module_queue_lock;

void *module_board_func(void *args);

#endif // MODULE_BOARD_H
