#include "prng.h"

void prng_seed(PrngState* prng, uint32_t seed) {
	prng->state = seed;
}

uint32_t prng_next(PrngState* prng) {
	return prng->state = (prng->state * 1103515245 + 12345);
}

float prng_random01(PrngState* prng) {
	return (float)((double)prng_next(prng) / 4294967296.0);
}

