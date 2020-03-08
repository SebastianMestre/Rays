// material.h
#pragma once

#include "vectors.h"

typedef struct {
	int is_emissive;
	V3 base_color;
	float base_specular;
} Material;
