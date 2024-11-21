#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>

// Constantes
#define MAX_MODULES 10

// Tipos
typedef enum {
    PENDING,
    IN_PROGRESS,
    DISARMED
} ModuleStatus;

typedef struct {
    int id;
    int type;               // Tipo do módulo (ex.: tarefa simples)
    int disarm_time;        // Tempo necessário para desarmar
    ModuleStatus status;    // Status do módulo
} Module;

// Variáveis globais compartilhadas
extern pthread_mutex_t module_queue_lock;
extern Module module_queue[MAX_MODULES];
extern int num_modules;

// Funções
void *module_board_func(void *args); // Thread que gera módulos
Module get_next_module();           // Recupera o próximo módulo pendente

#endif
