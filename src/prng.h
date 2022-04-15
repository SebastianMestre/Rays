#pragma once

#include <stdint.h>

typedef struct {
	uint32_t state;
} PrngState;

void prng_seed(PrngState* prng, uint32_t seed);

uint32_t prng_next(PrngState* prng);

float prng_random01(PrngState* prng);
