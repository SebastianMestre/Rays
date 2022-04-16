#include "vectors.h"

V3 V3_scale(V3 a, float s);
V3 V3_scale3(V3 a, V3 s);
V3 V3_sum(V3 a, V3 b);
V3 V3_sub(V3 a, V3 b);
float V3_dot(V3 a, V3 b);
V3 V3_cross(V3 a, V3 b);

float V3_norm(V3 v);

float V3_norm2(V3 v);

V3 V3_normalized(V3 v);

Mat3 make_look_at_matrix(V3 up, V3 look_at);

V3 V3_Mat3_mul(V3 vec, Mat3 mat);
