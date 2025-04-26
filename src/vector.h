#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>
#include <stdio.h>
#include "vector_math.h"

typedef struct
{
    vec4_t *array;
    size_t used;
    size_t size;
} Array_Vec4;


typedef struct
{
    vec2_t *array;
    size_t used;
    size_t size;
} Array_Vec2;

typedef struct
{
    vec3_t *array;
    size_t used;
    size_t size;
} Array_Vec3;

typedef struct
{
    int *array;
    size_t used;
    size_t size;
} Array_int;
void initArrayVec4(Array_Vec4 *a, size_t initialSize);
void insertArrayVec4(Array_Vec4 *a, vec4_t element);
void freeArrayVec4(Array_Vec4 *a);

void initArrayVec3(Array_Vec3 *a, size_t initialSize);
void insertArrayVec3(Array_Vec3 *a, vec3_t element);
void freeArrayVec3(Array_Vec3 *a);

void initArrayVec2(Array_Vec2 *a, size_t initialSize);
void insertArrayVec2(Array_Vec2 *a, vec2_t element);
void freeArrayVec2(Array_Vec2 *a);


void initArrayInt(Array_int *a, size_t initialSize);
void insertArrayInt(Array_int *a, int element);
void freeArrayInt(Array_int *a);
#endif