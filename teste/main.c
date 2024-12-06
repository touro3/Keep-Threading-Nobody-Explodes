#define MAX_BENCHES 5
#define MAX_TEDAX 10

int main() {
    int num_benches = 3; // Número inicial de bancadas
    int num_tedax = 5;   // Número inicial de Tedax

    printf("Digite o número de Tedax (máx %d): ", MAX_TEDAX);
    scanf("%d", &num_tedax);
    if (num_tedax > MAX_TEDAX) num_tedax = MAX_TEDAX;

    printf("Digite o número de bancadas (máx %d): ", MAX_BENCHES);
    scanf("%d", &num_benches);
    if (num_benches > MAX_BENCHES) num_benches = MAX_BENCHES;

    inicializa_bancadas(num_benches);

    pthread_t tedax_threads[num_tedax];
    for (int i = 0; i < num_tedax; i++) {
        pthread_create(&tedax_threads[i], NULL, tedax_func, (void *)(intptr_t)i);
    }

    // Aguarda o término das threads Tedax
    for (int i = 0; i < num_tedax; i++) {
        pthread_join(tedax_threads[i], NULL);
    }

    encerra_bancadas();
    return 0;
}
