#ifndef UTILS_H
#define UTILS_H

#include "module_board.h" // Necessário para ModuleStatus
#include "tedax.h"
#include <pthread.h>

// Mutex para mensagens temporárias
extern pthread_mutex_t message_lock;
extern char temporary_message[256]; // Mensagem temporária para exibição

// Mutex e array para ações dos Tedax
extern pthread_mutex_t tedax_action_lock;
extern char tedax_actions[NUM_TEDAX][256]; // Ações realizadas por cada Tedax

int generate_random(int min, int max);
int count_modules(ModuleStatus status);

#endif
