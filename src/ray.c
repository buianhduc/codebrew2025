#include "ray.h"
#include "triangles.h"

vec3_t at(ray_t ray, double t) {
    return add(ray.origin, mul(ray.direction, t));
}