#ifndef MODULE_BOARD_H
#define MODULE_BOARD_H

#include <pthread.h>

// Definições dos estados dos módulos
#define PENDING 0
#define IN_PROGRESS 1
#define DISARMED 2

// Constantes relacionadas à fila de módulos
#define MAX_MODULES 10    // Número máximo de módulos na fila
#define MAX_SEQUENCE 10   // Tamanho máximo de sequência para módulos do tipo 'c'

// Estrutura de um módulo
typedef struct {
    int id;                 // Identificador único do módulo
    char type;              // Tipo do módulo ('x' para botões, 'c' para sequência, 't' para temporizador)
    int status;             // Estado do módulo: PENDING, IN_PROGRESS ou DISARMED
    int interactions;       // Número de interações ou tempo restante (em segundos)
    char sequence[MAX_SEQUENCE]; // Sequência necessária (somente para módulos do tipo 'c')
    int bench_id;           // ID da bancada associada (-1 se não estiver em uso)
} Module;

// Variáveis globais compartilhadas
extern Module module_queue[MAX_MODULES];  // Fila de módulos
extern int num_modules;                   // Número atual de módulos na fila
extern pthread_mutex_t module_queue_lock; // Mutex para acesso à fila de módulos

// Declaração da função associada à thread do módulo
void *module_board_func(void *args);

#endif // MODULE_BOARD_H
