#include "vectors.h"

#include <math.h>

V3 V3_scale(V3 a, float s){
	return (V3){a.x * s, a.y * s, a.z * s};
}

V3 V3_scale3(V3 a, V3 s){
	return (V3){a.x * s.x, a.y * s.y, a.z * s.z};
}

V3 V3_sum(V3 a, V3 b){
	return (V3){a.x + b.x, a.y + b.y, a.z + b.z};
}

V3 V3_sub(V3 a, V3 b){
	return (V3){a.x - b.x, a.y - b.y, a.z - b.z};
}

float V3_dot(V3 a, V3 b){
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

V3 V3_cross(V3 a, V3 b){
	return (V3){
		a.y*b.z - a.z*b.y,
		a.z*b.x - a.x*b.z,
		a.x*b.y - a.y*b.x
	};
}

float V3_norm(V3 v){
	return (float)sqrt(V3_norm2(v));
}

float V3_norm2(V3 v){
	return V3_dot(v, v);
}

V3 V3_normalized(V3 v){
	return V3_scale(v, 1.0f/V3_norm(v));
}


Mat3 make_look_at_matrix(V3 up, V3 look_at){
	look_at = V3_normalized(look_at);
	V3 side = V3_normalized(V3_cross(up, look_at));
	up = V3_normalized(V3_cross(side, look_at));
	return (Mat3){ up, side, look_at };
}

V3 V3_Mat3_mul(V3 vec, Mat3 mat){
	return V3_sum(V3_sum(
		V3_scale(mat.x, vec.x),
		V3_scale(mat.y, vec.y)),
		V3_scale(mat.z, vec.z));
}
