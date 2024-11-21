#include <stdlib.h>
#include "utils.h"

int generate_random(int min, int max) {
    return min + rand() % (max - min + 1);
}
