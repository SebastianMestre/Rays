#pragma once

typedef struct {
	float x, y, z;
} V3;

V3 V3_sum(V3 a, V3 b);
V3 V3_sub(V3 a, V3 b);

V3 V3_scale(V3 v, float s);
V3 V3_scale3(V3 a, V3 s);
V3 V3_normalized(V3 v);

float V3_dot(V3 a, V3 b);
V3 V3_cross(V3 a, V3 b);

float V3_norm(V3 v);
float V3_norm2(V3 v);
