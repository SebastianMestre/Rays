// random.c
#include "random.h"

#include <stdlib.h>

float random01() {
	return (float)rand() / RAND_MAX;
}
