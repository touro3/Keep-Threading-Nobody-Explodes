#include <stdlib.h>
#include <time.h>
#include "utils.h"

// Gera um número aleatório entre min e max (inclusive)
int generate_random(int min, int max) {
    if (min > max) {
        int temp = min;
        min = max;
        max = temp;
    }
    return min + rand() % (max - min + 1);
}

// Inicializa o gerador de números aleatórios
void initialize_random_generator() {
    srand((unsigned int)time(NULL)); // Usa o tempo atual como semente
}
