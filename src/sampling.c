// sampling.c
#include "sampling.h"

#include <math.h>

#include "vectors.h"

V3 sample_hemisphere_cosine_weighted (float r1, float r2) {
	float r = sqrt(r1);
	float phi = r2 * (M_PI * 2.0f);

	return (V3){
		r * cos(phi),
		r * sin(phi),
		sqrt(1-r1)
	};
}

float specular_sample_probablity(V3 normal, V3 view, float r0) {
	float cos_t = V3_dot(normal, view);

	if (cos_t < 0.0f)
		return 1.0f;

	float x = 1.0f - cos_t;
	float x2 = x * x;
	float x5 = x2 * x2 * x;

	float result = r0 + (1.0f - r0) * x5;

	return result;
}

V3 sample_dirac_reflection(V3 normal, V3 view) {
	float ndotv = V3_dot(normal, view);

	V3 result = V3_sub(V3_scale(normal, 2.0f * ndotv), view);
}

V3 sample_hemisphere_ggx(float r1, float r2, float roughness) {
	float alpha = roughness * roughness;
	float theta = acos(sqrt((1.0 - r1) / ((alpha - 1.0) * r1 + 1.0)));
	float phi = r2 * (M_PI * 2.0f);

	return (V3){
		sin(theta) * cos(phi),
		sin(theta) * sin(phi),
		cos(theta)};
}
