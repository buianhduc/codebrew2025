#ifndef VECTOR_MATH_H
#define VECTOR_MATH_H

#include <math.h>

// 2-dimensional vector
typedef struct {
	double x, y;
} vec2_t;

// 3-dimensional vector
typedef struct {
	double x, y, z;
} vec3_t;

typedef struct{
    double x, y, z, w;
} vec4_t;

//column major matrix type 
//all transformations are done in a 4d space 
typedef struct {
    vec4_t v1, v2, v3, v4;
} mat4x4_t;





// Helper functions to populate vector variables

static inline vec2_t vec2(double x, double y) {
	vec2_t p;
	p.x = x;
	p.y = y;
	return p;
}

static inline vec3_t vec3(double x, double y, double z) {
	vec3_t p;
	p.x = x;
	p.y = y;
	p.z = z;
	return p;
}

static inline vec4_t vec4(double x, double y, double z, double w) {
	vec4_t p;
	p.x = x;
	p.y = y;
	p.z = z;
    p.w = w;
	return p;
}

static void normalize(vec3_t *p) {
	double length = sqrt(p->x * p->x + p->y * p->y + p->z * p->z);
	p->x /= length;
	p->y /= length;
	p->z /= length;
}

static inline vec4_t vec4_position(vec3_t *pos) {
	vec4_t p4;
	p4.x = pos->x;
	p4.y = pos->y;
	p4.z = pos->z;
	p4.w = 1;
	return p4;
}

static inline vec4_t vec4_rotation(vec3_t *pos) {
	vec4_t p4;
	p4.x = pos->x;
	p4.y = pos->y;
	p4.z = pos->z;
	p4.w = 0;
	return p4;
}

//initialze matrix as an identity matrix 
static inline void load_identity(mat4x4_t* m) {
	m->v1 = vec4(1, 0, 0, 0);
	m->v2 = vec4(0, 1, 0, 0);
	m->v3 = vec4(0, 0, 1, 0);
	m->v4 = vec4(0, 0, 0, 1);
}

//initialize projection matrix 
static inline mat4x4_t make_projection_matrix(double aspect, double fov, double z_near, double z_far){
    mat4x4_t m;
    m.v1 = vec4(1.0 / (aspect * tan(fov/2)), 0, 0, 0);
    m.v2 = vec4(0, 1.0/tan(fov/2), 0, 0);
    m.v3 = vec4(0, 0, -(z_far + z_near) / (z_far - z_near), -1);
    m.v4 = vec4(0, 0, (2 * z_far * z_near) / (z_far - z_near), 0);
    return m;
}

static inline mat4x4_t make_translation_matrix(vec3_t t ){
    mat4x4_t m;
    m.v1 = vec4(1, 0, 0 , 0);
    m.v2 = vec4(0, 1, 0 , 0);
    m.v3 = vec4(0, 0, 1, 0);
    m.v4 = vec4(t.x, t.y, t.z, 1);
    return m;
}

static inline mat4x4_t make_rotation_matrix_x(double theta){
	mat4x4_t m;
    m.v1 = vec4(1, 0, 0 , 0);
    m.v2 = vec4(0, cos(theta), sin(theta) , 0);
    m.v3 = vec4(0, -sin(theta), cos(theta), 0);
    m.v4 = vec4(0, 0, 0, 1);
    return m;
}

static inline mat4x4_t make_rotation_matrix_y(double theta){
	mat4x4_t m;
    m.v1 = vec4(cos(theta), 0, -sin(theta) , 0);
    m.v2 = vec4(0, 1, 0 , 0);
    m.v3 = vec4(sin(theta), 0, cos(theta), 0);
    m.v4 = vec4(0, 0, 0, 1);
    return m;
}

static inline mat4x4_t make_rotation_matrix_z(double theta){
	mat4x4_t m;
    m.v1 = vec4(cos(theta), sin(theta), 0 , 0);
    m.v2 = vec4(-sin(theta), cos(theta), 0 , 0);
    m.v3 = vec4(0, 0, 1, 0);
    m.v4 = vec4(0, 0, 0, 1);
    return m;
}

static inline vec4_t mat4x4_multiply(mat4x4_t* m, vec4_t p){
	vec4_t result;
	result.x = (m->v1.x * p.x) + (m->v2.x * p.y) + (m->v3.x * p.z) + (m->v4.x * p.w);
	result.y = (m->v1.y * p.x) + (m->v2.y * p.y) + (m->v3.y * p.z) + (m->v4.y * p.w);
	result.z = (m->v1.z * p.x) + (m->v2.z * p.y) + (m->v3.z * p.z) + (m->v4.z * p.w);
	result.w = (m->v1.w * p.x) + (m->v2.w * p.y) + (m->v3.w * p.z) + (m->v4.w *  p.w);

	return result;
}

static inline mat4x4_t mat4x4_multiply_mat4x4(mat4x4_t* m, mat4x4_t* p){
	mat4x4_t results;
	results.v1 = mat4x4_multiply(m, p->v1);
	results.v2 = mat4x4_multiply(m, p->v2);
	results.v3 = mat4x4_multiply(m, p->v3);
	results.v4 = mat4x4_multiply(m, p->v4);

	return results;
}



#endif