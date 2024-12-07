#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>
#include <string.h>
#include <semaphore.h>
#include "utils.h"

#define MAX_MODULES 10

#define PENDING 0
#define IN_PROGRESS 1
#define DISARMED 2

typedef struct Module {
    int id;
    char type; // Tipo de módulo: 'x', 'c', ou 't'
    int interactions; // Interações necessárias
    char sequence[10]; // Sequência de caracteres (se necessário)
    int bench_id; // Bancada designada
    int status; // Status: PENDING, IN_PROGRESS, DISARMED
} Module;

// Variáveis globais
extern Module module_queue[MAX_MODULES]; // Fila de módulos
extern int num_modules; // Número atual de módulos
extern pthread_mutex_t module_queue_lock; // Mutex para controlar o acesso à fila de módulos

// Função do mural de módulos
void *module_board_func(void *args);

#endif