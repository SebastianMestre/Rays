// main.c
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#include "bmp.h"
#include "geometry.h"
#include "vectors.h"
#include "intersection.h"

#define W 512
#define H 512

uint8_t r[W][H];
uint8_t g[W][H];
uint8_t b[W][H];

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

	for (int x = 0; x < W; ++x){
		for (int y = 0; y < H; ++y){
			V3 screen = {
				(float)x/(W-1)*2.0f-1.0f,
				(float)y/(H-1)*2.0f-1.0f,
				1.0f
			};

			V3 view = V3_normalized(screen);

			Intersection result = trace((Ray){camera_pos, view});

			if (!result.exists) {
				r[x][y] = 0;
				g[x][y] = 0;
				b[x][y] = 0;
				continue;
			}

			int attempts = 500;
			int hits = 0;
			// Ambient Occlusion
			V3 up = {0.0f, 0.0f, 1.0f};
			V3 side = V3_normalized(V3_cross(up, result.normal));
			up = V3_normalized(V3_cross(side, result.normal));
			for(int i = 0; i < attempts; ++i){
				float r1 = (float)rand()/RAND_MAX;
				float r2 = (float)rand()/RAND_MAX;

				float R = sqrt(r1);
				float phi = r2 * M_PI * 2.0f;

				V3 bounce_direction_tangent = {
					R * cos(phi),
					R * sin(phi),
					sqrt(1-r1)
				};

				V3 bounce_direction = V3_sum(V3_sum(
					V3_scale(result.normal, bounce_direction_tangent.z),
					V3_scale(side, bounce_direction_tangent.x)),
					V3_scale(up, bounce_direction_tangent.y));

				Intersection bounce_result = trace((Ray){V3_sum(result.position, V3_scale(bounce_direction, 0.001)), bounce_direction});
				if(bounce_result.exists){
					hits += 1;
				}
			}

			float ratio = (float)hits / (float)attempts;
			// printf("%d/%d -> %f\n", hits, attempts, ratio);

			float ix = sqrt(ratio);
			float iy = sqrt(ratio);
			float iz = sqrt(ratio);

			r[x][y] = ix*255;
			g[x][y] = iy*255;
			b[x][y] = iz*255;
		}
	}

	bmp_write((uint8_t*)r,(uint8_t*)g,(uint8_t*)b, W,H);
}
