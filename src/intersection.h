// intersection.h
#pragma once

#include <stdbool.h>

#include "geometry.h"
#include "ray.h"
#include "vectors.h"

typedef struct {
	V3 position;
	V3 normal;
	float distance;
	bool exists;
	int material_id;
} Intersection;

Intersection no_intersection();

Intersection intersection_ray_sphere(Ray r, Sphere s);

Intersection intersection_ray_plane(Ray r, Plane s);
