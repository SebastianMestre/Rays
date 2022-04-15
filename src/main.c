// main.c
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "bmp.h"
#include "geometry.h"
#include "intersection.h"
#include "material.h"
#include "prng.h"
#include "sampling.h"
#include "vectors.h"

#define W 512
#define H 512

#define BW 1
#define BH 1

#define BCOLS (W/BW)
#define BROWS (H/BH)

#define SPP 64
#define SPB (BW*BH*SPP)
#define SC (W*H*SPP)

typedef struct {
	V3 color;
	float x,y;
} Sample;

uint8_t r[W][H];
uint8_t g[W][H];
uint8_t b[W][H];
Sample samples[BCOLS][BROWS][BW*BH*SPP];

#define PLANE_COUNT 1
Plane planes[PLANE_COUNT] = {
	{0, {+0.00f, -1.00f, +0.00f}, {+0.00f, +1.00f, +0.00f}},
};

#define SPHERE_COUNT 3
Sphere spheres[SPHERE_COUNT] = {
	{2, {+2.5f, +0.0f, +5.0f}, 1.0f},
	{0, {-2.5f, +0.0f, +5.0f}, 1.0f},
	{1, {+0.0f, +0.0f, +5.5f}, 1.0f},
};

#define MATERIAL_COUNT 3
Material materials[MATERIAL_COUNT] = {
	{false, {0.80f, 0.50f, 0.70f}, 0.04f, 0.2f},
	{false, {0.00f, 0.00f, 0.00f}, 0.70f, 0.05f},
	{true,  {12.00f, 4.00f, 2.00f}, 0.00f, 0.01f}
};

Intersection trace (Ray r) {
	Intersection result = no_intersection();

	for (int i = 0; i < PLANE_COUNT; ++i) {
		Intersection it = intersection_ray_plane(r, planes[i]);
		if(it.exists) {
			if(!result.exists || it.distance < result.distance){
				result = it;
			}
		}
	}

	for (int i = 0; i < SPHERE_COUNT; ++i) {
		Intersection it = intersection_ray_sphere(r, spheres[i]);
		if(it.exists) {
			if(!result.exists || it.distance < result.distance){
				result = it;
			}
		}
	}

	return result;
}

V3 full_trace (PrngState* prng, Ray r) {

	const V3 sky_color = V3_scale((V3){0.3f, 0.6f, 0.9f}, 1);
	const float i_pi = 0.31830988618;
	V3 factor = {1.0f, 1.0f, 1.0f};


	// for up to three bounces, we try to find a light source
	int steps = 0;
	while(1){

		Intersection result = trace(r);

		if (!result.exists) {
			return V3_scale3(sky_color, factor);
		}

		Material material = materials[result.material_id];

		if(material.is_emissive)
			return V3_scale3(material.base_color, factor);

		if(steps++ == 3)
			break;

		float fresnel = specular_sample_probablity(
			result.normal,
			V3_scale(r.direction, -1.0f),
			material.base_specular);

		float r0 = prng_random01(prng);

		Mat3 tangent_to_world = make_look_at_matrix((V3){0.0f, 0.0f, 1.0f}, result.normal);

		float r1 = prng_random01(prng);
		float r2 = prng_random01(prng);

		V3 bounce_direction;
		if(fresnel < r0){
			factor = V3_scale3(V3_scale(factor, i_pi), material.base_color);

			V3 bounce_direction_tangent = sample_hemisphere_cosine_weighted(r1, r2);
			V3 bounce_direction = V3_Mat3_mul(bounce_direction_tangent, tangent_to_world);
		} else {
			// implicit factor = V3_scale(factor, 1.0f);

			V3 micronormal_tangent = sample_hemisphere_ggx(r1, r2, material.roughness);
			V3 micronormal = V3_Mat3_mul(micronormal_tangent, tangent_to_world);

			V3 view = V3_scale(r.direction, -1.0f);
			V3 bounce_direction = sample_dirac_reflection(micronormal, view);
		}

		r = (Ray){
			V3_sum(result.position, V3_scale(bounce_direction, 0.001)),
			bounce_direction,
		};
	}

	// didn't find a light source
	return (V3){0.0f, 0.0f, 0.0f};
}

int main () {

	PrngState prng = { 3141592 };

	V3 camera_pos = {0.0f, 0.0f, 0.0f};
	V3 camera_up = {0.0f, 0.0f, 1.0f};
	V3 camera_right = {};
	V3 camera_front = {};

	int const bucket_count = BCOLS * BROWS;

	for (int bucket = 0; bucket < bucket_count; ++bucket) {

		int bx = bucket % BCOLS;
		int by = bucket / BCOLS;

		for (int sb = 0; sb < SPB; ++sb) {
			float px_in_bucket = prng_random01(&prng);
			float py_in_bucket = prng_random01(&prng);

			float px = ((float)bx + px_in_bucket) / (float)BCOLS;
			float py = ((float)by + py_in_bucket) /  (float)BROWS;

			V3 screen = {
				px*2.0f-1.0f,
				py*2.0f-1.0f,
				1.0f
			};

			V3 view_direction = V3_normalized(screen);

			Ray view = {camera_pos, view_direction};

			V3 color = full_trace(&prng, view);
			samples[bx][by][sb] = (Sample){color, px, py};
		}
	}

	for (int x = 0; x < W; ++x) {
		float px = (float)x/(W-1);

		// printf("[%d] => %d\n", x, bx);

		for (int y = 0; y < H; ++y) {
			float py = (float)y/(H-1);

			V3 pcolor = {0.0f, 0.0f, 0.0f};
			double coeffs = 0.0;

			int bcx = x / BW;
			int bcy = y / BH;

			int bx_start = bcx-1 < 0 ? 0 : bcx-1;
			int by_start = bcy-1 < 0 ? 0 : bcy-1;

			for (int bx = bx_start; bx<=bcx+1 && bx<BCOLS; ++bx) {
				for (int by = by_start; by<=bcy+1 && by<BROWS; ++by) {
					for (int sb = 0; sb < SPB; ++sb) {
						float sx = samples[bx][by][sb].x;
						float sy = samples[bx][by][sb].y;

						float dx = (sx - px)*W*2;
						float dy = (sy - py)*H*2;

						float d2 = dx*dx + dy*dy;

						float coeff = exp(-d2);

						pcolor = V3_sum(pcolor, V3_scale(samples[bx][by][sb].color, coeff));
						coeffs += coeff;
					}
				}
			}

			pcolor = V3_scale(pcolor, 1/coeffs);

			pcolor.x = 2 * pcolor.x / (2 * pcolor.x + 1);
			pcolor.y = 2 * pcolor.y / (2 * pcolor.y + 1);
			pcolor.z = 2 * pcolor.z / (2 * pcolor.z + 1);

			r[x][y] = pcolor.x * 255;
			g[x][y] = pcolor.y * 255;
			b[x][y] = pcolor.z * 255;
		}
	}

	for (int x = 0; x < W; ++x) {
		for (int y = 0; y < H; ++y) {
			r[x][y] = 255 - r[x][y];
			g[x][y] = 255 - g[x][y];
			b[x][y] = 255 - b[x][y];
		}
	}

	bmp_write("helper.bmp", (uint8_t*)r, (uint8_t*)g, (uint8_t*)b, W, H);
}
