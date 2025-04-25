#ifndef RAY_H
#define RAY_H

#include "triangles.h"
typedef struct ray {
    vec3_t origin, direction;
} ray_t;
vec3_t at(ray_t ray, double t);

#endif