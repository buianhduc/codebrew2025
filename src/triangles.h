#ifndef TRIANGLES_H
#define TRIANGLES_H
#include "vector_math.h"

// Vertex in 3D space
typedef struct {
	vec4_t position;
	vec2_t textureCoords;
 	vec3_t normal;
 } vertex_t;

// 3-dimensional triangle
typedef struct {
vertex_t vertices[3]; // 3 vertices, ideally to be listed in anti-clockwise order
} triangle_t;

void initialize_matrices();

//use pointers for optimisation 
static inline triangle_t make_triangle(vertex_t* v1, vertex_t* v2, vertex_t* v3){
triangle_t tri;
tri.vertices[0] = *v1;
tri.vertices[1] = *v2;
tri.vertices[2] = *v3;
return tri;
}

static inline vertex_t make_basic_vertex(vec3_t *p){
vertex_t v;
v.position = vec4(p->x, p->y, p->z, 1);
v.textureCoords = vec2(0, 0);
v.normal =  vec3(0, 0, 0);
return v;
}

void draw_point(vec4_t *p);
void render_triangle(triangle_t *tri);
void set_view_matrix(vec3_t *p, double x_rotation, double y_rotation);


#endif