#ifndef UTILS_H
#define UTILS_H

#include "module_board.h" // Necessário para ModuleStatus
#include "tedax.h"
#include <pthread.h>


#define NUM_TEDAX 3

// Mutex para mensagens temporárias
extern pthread_mutex_t message_lock;
extern pthread_mutex_t tedax_action_lock;
extern char tedax_actions[NUM_TEDAX][256];
extern char temporary_message[100];

// Mutex e array para ações dos Tedax
extern pthread_mutex_t tedax_action_lock;
extern char tedax_actions[NUM_TEDAX][256]; // Ações realizadas por cada Tedax

int generate_random(int min, int max);
int count_modules(ModuleStatus status);

void desenha_bomba(int linha, int coluna);

#endif
