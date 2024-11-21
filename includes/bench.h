#ifndef BENCH_H
#define BENCH_H

void inicializa_bancadas(int num_benches);
void ocupa_bancada(int bench_id);
void libera_bancada(int bench_id);
void encerra_bancadas();

#endif
