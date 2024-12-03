#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>

#define PENDING 0
#define IN_PROGRESS 1
#define DISARMED 2

// Estrutura do módulo
typedef struct {
    int id;
    char type;       // Exemplo: 'F' para fios, 'B' para botão, etc.
    int status;      // PENDING, IN_PROGRESS ou DISARMED
    int interactions; // Número de interações ou tempo restante
} Module;

// Variáveis globais 
extern Module *module_queue;
extern int num_modules;
extern pthread_mutex_t module_queue_lock;

#endif