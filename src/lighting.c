#include "lighting.h"

static directional_light_t sun;

static point_light_t lantern;

void initialize_lighting() {
    sun.direction = vec3(6, -10, -8);
    normalize(&sun.direction);

    lantern.position = vec3(2, 0, 0);
}

// get brightness of sun on this surface normal
double apply_directional_light(vec3_t surface_normal) {
    double dot = -sun.direction.x * surface_normal.x - sun.direction.y * surface_normal.y - sun.direction.z * surface_normal.z;
    double brightness = dot < 0.5 ? 0.5 : dot;
    brightness = brightness > 1 ? 1 : brightness;
    return brightness;
}

double apply_point_light(vec3_t position, vec3_t surface_normal) {
    vec3_t to_light = vec3(
        lantern.position.x - position.x,
        lantern.position.y - position.y,
        lantern.position.z - position.z
    );
    double dot = -to_light.x * surface_normal.x - to_light.y * surface_normal.y - to_light.z * surface_normal.z;
    double brightness = dot < 0.25 ? 0.25 : dot;
    brightness = brightness > 1 ? 1 : brightness;
    return brightness;
}

double adjust_light_levels(double brightness) {
    brightness *= 1.5;
    brightness = brightness > 1 ? 1 : brightness;
    return brightness;
}