#ifndef GEOMETRY_H
#define GEOMETRY_H

#define EMPTY_VEC3D vec3(0.0f,0.0f,0.0f)
#define EMPTY_VEC2D vec2(0.0f,0.0f)

// 2-dimensional vector
typedef struct {
	double x, y;
} vec2_t;

// 3-dimensional vector
typedef struct {
	double x, y, z;
} vec3_t;

// Vertex in 3D space
typedef struct {
	vec3_t position;
	vec2_t textureCoords;
	vec3_t normal;
} vertex_t;

// 3-dimensional triangle
typedef struct {
	vertex_t vertices[3]; // 3 vertices, ideally to be listed in anti-clockwise order
} triangle_t;
typedef struct mesh
{
	/* data */
	triangle_t* tris;
	int length;
} mesh_t;
typedef struct mat4x4 {
  float m[4][4];
} mat4x4_t;


// Helper functions to define vector variables

vec2_t vec2(double x, double y);

vec3_t vec3(double x, double y, double z);

triangle_t triangle(vertex_t a, vertex_t b, vertex_t c);
vertex_t vertex(vec3_t position, vec2_t textureCoords, vec3_t normal);
vec3_t add(vec3_t a, vec3_t b);
vec3_t sub(vec3_t a, vec3_t b);
vec3_t mul(vec3_t from, double t);
vec3_t vec3_div(vec3_t from, double t);
double distance(vec3_t vec);
vec3_t unit_vector(vec3_t vec);

vec3_t multiply_matrix_vector(vec3_t i, mat4x4_t m);
mesh_t create_mesh(triangle_t triangles[], int length);
void free_mesh(mesh_t* mesh);
#endif