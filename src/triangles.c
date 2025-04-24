#include "triangles.h"

#include <math.h>
#include <stdlib.h>

vec3_t vec3(double x, double y, double z) {
	vec3_t p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}


vec2_t vec2(double x, double y) {
	vec2_t p;
	p.x = x;
	p.y = y;
	return p;
}
triangle_t triangle(vertex_t a, vertex_t b, vertex_t c){
    triangle_t p = {
        .vertices = {a,b,c},
    };
    return p;
}
vertex_t vertex(vec3_t position, vec2_t textureCoords, vec3_t normal) {
    return (vertex_t) {
        .position = position,
        .textureCoords = textureCoords,
        .normal = normal
    };
}


vec3_t add(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = a.x + b.x,
        .y = a.y + b.y,
        .z = a.z + b.z,
    };
    return result;
}
vec3_t sub(vec3_t a, vec3_t b) {
    vec3_t result = {
        .x = a.x - b.x,
        .y = a.y - b.y,
        .z = a.z - b.z,
    };
    return result;
}

vec3_t mul(vec3_t from, double t){
    vec3_t result = {
        .x = from.x*t,
        .y = from.y*t,
        .z = from.z*t,
    };
    return result;
}
vec3_t vec3_div(vec3_t from, double t) {
    return mul(from, 1/t);
}

double distance(vec3_t vec){
    return sqrt(pow(vec.x, 2.0) + pow(vec.y, 2.0) + pow(vec.z, 2.0));
}
vec3_t unit_vector(vec3_t vec){
    return vec3_div(vec, distance(vec));
}

vec3_t multiply_matrix_vector(vec3_t i, mat4x4_t m) {
    vec3_t output = (vec3_t) {
        .x = i.x * m.m[0][0] + i.y*m.m[1][0] + i.z*m.m[2][0] + m.m[3][0],
        .y = i.x * m.m[0][1] + i.y*m.m[1][1] + i.z*m.m[2][1] + m.m[3][1],
        .z = i.x * m.m[0][2] + i.y*m.m[1][2] + i.z*m.m[2][2] + m.m[3][2],
    };
    float w = i.x * m.m[0][3] + i.y * m.m[1][3] + i.z * m.m[2][3] + m.m[3][3];
    if (w != 0.0f)
    {
        output.x /= w; output.y /= w; output.z /= w;
    }
    return output;
}
mesh_t create_mesh(triangle_t triangles[], int length) {
    mesh_t new_mesh = (mesh_t) { 
        .tris = malloc(sizeof(triangle_t)*length),
        .length = length,
    };

    for(int i = 0; i < length; i++) {
        new_mesh.tris[i] = triangles[i];
    }
    return new_mesh;
}
void free_mesh(mesh_t* mesh) {
    free(mesh->tris);
    mesh = NULL;
}