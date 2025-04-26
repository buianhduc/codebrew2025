#ifndef _LIGHTING_H
#define _LIGHTING_H

#include "vector_math.h"

typedef struct {
    vec3_t direction;
} directional_light_t;

typedef struct {
    vec3_t position;
} point_light_t;

void initialize_lighting();
double apply_directional_light(vec3_t surface_normal);
double apply_point_light(vec3_t position, vec3_t surface_normal);
double adjust_light_levels(double brightness);

#endif