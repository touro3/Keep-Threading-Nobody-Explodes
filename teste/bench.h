#ifndef BENCH_H
#define BENCH_H

#include <semaphore.h>

// Inicializa as bancadas com um número específico
void inicializa_bancadas(int num_benches);

// Ocupa uma bancada específica
void ocupa_bancada(int bench_id);

// Libera uma bancada específica
void libera_bancada(int bench_id);

// Encerra e libera os recursos relacionados às bancadas
void encerra_bancadas();

#endif // BENCH_H
