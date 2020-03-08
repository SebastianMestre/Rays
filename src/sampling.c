// sampling.c
#include "sampling.h"

#include <math.h>

#include "vectors.h"

V3 sample_hemisphere_cosine_weighted (float r1, float r2) {
	float r = sqrt(r1);
	float phi = r2 * M_PI * 2.0f;

	return (V3){
		r * cos(phi),
		r * sin(phi),
		sqrt(1-r1)
	};
}

