#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>
#include <string.h>
#include <semaphore.h>

#define MAX_MODULES 10

// Enum para representar o status do módulo
typedef enum {
    PENDING = 0,
    IN_PROGRESS,
    DISARMED
} ModuleStatus;

// Estrutura do módulo
typedef struct Module {
    int id;
    char type; // Tipo de módulo: 'x', 'c', ou 't'
    int interactions; // Interações necessárias
    char sequence[10]; // Sequência de caracteres (se necessário)
    int bench_id; // Bancada designada
    ModuleStatus status; // Status do módulo
} Module;

// Variáveis globais
extern Module module_queue[MAX_MODULES];
extern int num_modules;
extern pthread_mutex_t module_queue_lock;

void *module_board_func(void *args);

#endif
