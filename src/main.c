// main.c
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "bmp.h"
#include "geometry.h"
#include "vectors.h"
#include "intersection.h"

#define W 128
#define H 128
#define SPP 10
#define SC (W*H*SPP)

typedef struct {
	float r,g,b;
	float x,y;
} Sample;

uint8_t r[W][H];
uint8_t g[W][H];
uint8_t b[W][H];
Sample samples[SC];

#define PLANE_COUNT 3
Plane planes[PLANE_COUNT] = {
	{{-1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}},
	{{0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}},
	{{-0.71f, -0.71f, 0.0f}, {0.71f, 0.71f, 0.0f}},
};

#define SPHERE_COUNT 2
Sphere spheres[SPHERE_COUNT] = {
	{{1.0f, 0.0f, 5.0f}, 1.0f},
	{{-0.5f, -0.5f, 5.0f}, 1.0f},
};

V3 sample_hemisphere_cosine_weighted (float r1, float r2) {
	float r = sqrt(r1);
	float phi = r2 * M_PI * 2.0f;

	return (V3){
		r * cos(phi),
		r * sin(phi),
		sqrt(1-r1)
	};
}

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

int main () {

	V3 camera_pos = {0.0f, 0.0f, 0.0f};
	V3 camera_up = {0.0f, 0.0f, 1.0f};
	V3 camera_right = {};
	V3 camera_front = {};

	int s = 0;
	for (int s = 0; s < SC; ++s){
		float px = (float)rand() / (RAND_MAX);
		float py = (float)rand() / (RAND_MAX);

		printf("(%.2f %.2f)\n", px, py);

		V3 screen = {
			px*2.0f-1.0f,
			py*2.0f-1.0f,
			1.0f
		};

		V3 view = V3_normalized(screen);

		Intersection result = trace((Ray){camera_pos, view});

		if (!result.exists) {
			samples[s] = (Sample){
				1.0f, 1.0f, 1.0f,
				px, py
			};
			continue;
		}

		// Ambient Occlusion
		V3 up = {0.0f, 0.0f, 1.0f};
		V3 side = V3_normalized(V3_cross(up, result.normal));
		up = V3_normalized(V3_cross(side, result.normal));

		float r1 = (float)rand()/RAND_MAX;
		float r2 = (float)rand()/RAND_MAX;

		V3 bounce_direction_tangent = sample_hemisphere_cosine_weighted(r1, r2);

		V3 bounce_direction = V3_sum(V3_sum(
			V3_scale(result.normal, bounce_direction_tangent.z),
			V3_scale(side, bounce_direction_tangent.x)),
			V3_scale(up, bounce_direction_tangent.y));

		Ray bounce_ray = {
			V3_sum(result.position, V3_scale(bounce_direction, 0.001)),
			bounce_direction,
		};

		Intersection bounce_result = trace(bounce_ray);

		if (bounce_result.exists){
			// hit something, no light
			samples[s] = (Sample){
				0.0f, 0.0f, 0.0f,
				px, py
			};
		} else {
			// hit nothing, skylight
			samples[s] = (Sample){
				1.0f, 1.0f, 1.0f,
				px, py
			};
		}
	}


	for(int x = 0; x < W; ++x){
		for(int y = 0; y < H; ++y){
			float px = (float)x/(W-1);
			float py = (float)y/(H-1);

			double pr = 0.0;
			double pg = 0.0;
			double pb = 0.0;
			double coeffs = 0.0;

			for(int s = 0; s < SC; ++s){
				float sx = samples[s].x;
				float sy = samples[s].y;

				float dx = (sx - px)*W*2;
				float dy = (sy - py)*H*2;

				float d2 = dx*dx + dy*dy;

				float coeff = exp(-d2);

				pr += samples[s].r * coeff;
				pg += samples[s].g * coeff;
				pb += samples[s].b * coeff;
				coeffs += coeff;
			}

			pr /= coeffs;
			pg /= coeffs;
			pb /= coeffs;

			r[x][y] = pr * 255;
			g[x][y] = pg * 255;
			b[x][y] = pb * 255;
		}
	}

	for(int x = 0; x < W; ++x){
		for(int y = 0; y < H; ++y){
			r[x][y] = 255 - r[x][y];
			g[x][y] = 255 - g[x][y];
			b[x][y] = 255 - b[x][y];
		}
	}

	bmp_write("helper.bmp", (uint8_t*)r,(uint8_t*)g,(uint8_t*)b, W,H);
}
