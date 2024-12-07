#ifndef TEDAX_H
#define TEDAX_H

#include <pthread.h>
#include <semaphore.h>

#define NUM_TEDAX 3

typedef struct {
    int id;
    int is_available;
    pthread_mutex_t lock;
} Tedax;

// Declare tedax_list as a static array
extern Tedax tedax_list[NUM_TEDAX];

void initialize_tedax_list(void);
void cleanup_tedax_list(void);
void *tedax_func(void *args);

#endif
