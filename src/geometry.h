// geometry.h
#pragma once

#include "vectors.h"

typedef struct {
	V3 origin;
	float radius;
} Sphere;

typedef struct {
	V3 origin;
	V3 normal;
} Plane;

typedef struct {
	V3 begin;
	V3 end;
} AABB;

/* maybe I'll want polymorphism later?

typedef enum {
	SphereTypeTag,
	AABBTypeTag,
	ObjectTypeCount
} ObjectType;

typedef struct {
	ObjectType tag;
	union {
		Sphere as_sphere;
		AABB as_aabb;
	};
} Object;

Object make_sphere (V3 center, float radius);
Object make_aabb (V3 begin, V3 end);

*/
