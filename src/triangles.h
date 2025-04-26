#ifndef TRIANGLES_H
#define TRIANGLES_H
#include "vector_math.h"
#include "texture.h"

// Vertex in 3D space
typedef struct {
	vec4_t position;
	vec2_t textureCoords;
 	vec3_t normal;
 } vertex_t;

// 3-dimensional triangle
typedef struct {
	vertex_t *vertices[3]; // 3 vertices, ideally to be listed in anti-clockwise order
} triangle_t;

// 3-dimensional mesh
typedef struct {
	vertex_t *vertices;
	triangle_t *triangles;
	unsigned int vertex_count;
	unsigned int triangle_count;
} mesh_t;

void initialize_rendering();
void clear_z_buffer();

//use pointers for optimisation 
static inline triangle_t make_triangle(vertex_t* v1, vertex_t* v2, vertex_t* v3){
	triangle_t tri;
	tri.vertices[0] = v1;
	tri.vertices[1] = v2;
	tri.vertices[2] = v3;
	return tri;
}

static inline vertex_t make_basic_vertex(vec3_t *p){
	vertex_t v;
	v.position = vec4(p->x, p->y, p->z, 1);
	v.textureCoords = vec2(0, 0);
	v.normal =  vec3(0, 0, 0);
	return v;
}

static inline vertex_t make_vertex(vec3_t position, vec2_t textureCoords, vec3_t normal) {
	vertex_t v;
	v.position = vec4(position.x, position.y, position.z, 1);
	v.textureCoords = textureCoords;
	v.normal = normal;
	return v;
}

void free_mesh(mesh_t *mesh);
void render_mesh(mesh_t *mesh, texture_t *texture);
void render_triangle(triangle_t *tri, texture_t *texture);
void set_view_matrix(vec3_t *p, double x_rotation, double y_rotation);


#endif