// intersection.c
#include "intersection.h"

#include <math.h>

Intersection no_intersection() {
	return (Intersection){{},{},0.0,false, 0};
}

Intersection intersection_ray_sphere(Ray r, Sphere s) {

	V3 pmc = V3_sub(r.origin, s.origin);

	float A = V3_norm2(r.direction);
	float B = 2.0f*V3_dot(r.direction, pmc);
	float C = V3_norm2(pmc) - s.radius*s.radius;

	float disc = B*B - 4.0f*A*C;

	if(disc < 0.0f)
		return no_intersection();

	float sqrt_disc = sqrt(disc);

	float x1 = (-B - sqrt_disc) / (2.0f * A);
	float x2 = (-B + sqrt_disc) / (2.0f * A);

	// TODO: always false?
	if (x2 < x1) {
		// assert(false);
		float swap = x1;
		x1 = x2;
		x2 = swap;
	}

	Intersection result;
	if (x1 > 0) {
		result.distance = x1;
	} else if (x2 > 0) {
		result.distance = x2;
	} else {
		return no_intersection();
	}

	result.material_id = s.material_id;
	result.exists = true;
	result.position = V3_sum(r.origin, V3_scale(r.direction, result.distance));
	result.normal = V3_normalized(V3_sub(result.position, s.origin));

	return result;
}

Intersection intersection_ray_plane(Ray r, Plane s) {

	V3 q_minus_p = V3_sub(s.origin, r.origin);
	float qmp_dot_n = V3_dot(q_minus_p, s.normal);
	float ndotd = V3_dot(r.direction, s.normal);
	float L = qmp_dot_n/ndotd;

	if(L < 0){
		return no_intersection();
	}

	return (Intersection){
		V3_sum(r.origin, V3_scale(r.direction, L)),
		s.normal,
		L,
		true,
		s.material_id
	};
}
