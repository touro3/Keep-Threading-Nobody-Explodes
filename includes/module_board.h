#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>
#include <string.h>

#define MAX_MODULES 10

#define PENDING 0
#define IN_PROGRESS 1
#define DISARMED 2

typedef struct {
    int id;
    char type; // 'x' para botões, 'c' para sequência, 't' para temporizador
    int bench_id;
    int interactions; // Interações ou tempo fixo
    char sequence[10]; // Para sequência de caracteres
    int status;
} Module;

extern pthread_mutex_t module_queue_lock;
extern Module module_queue[MAX_MODULES];
extern int num_modules;

void *module_board_func(void *args);

#endif
