// sampling.h
#pragma once

#include "vectors.h"

V3 sample_hemisphere_cosine_weighted (float r1, float r2);

float specular_sample_probablity(V3 normal, V3 view);

V3 sample_dirac_reflection(V3 normal, V3 view);
