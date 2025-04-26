#include "vector.h"
void initArrayVec4(Array_Vec4 *a, size_t initialSize)
{
    a->array = (vec4_t*)malloc(initialSize * sizeof(Array_Vec4));
    a->used = 0;
    a->size = initialSize;
}

void insertArrayVec4(Array_Vec4 *a, vec4_t element)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (vec4_t*)realloc(a->array, a->size * sizeof(vec4_t));
    }
    a->array[a->used++] = element;
}
void freeArrayVec4(Array_Vec4 *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}

void initArrayVec3(Array_Vec3 *a, size_t initialSize)
{
    a->array = (vec3_t*)malloc(initialSize * sizeof(Array_Vec4));
    a->used = 0;
    a->size = initialSize;
}

void insertArrayVec3(Array_Vec3 *a, vec3_t element)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (vec3_t*)realloc(a->array, a->size * sizeof(vec3_t));
    }
    a->array[a->used++] = element;
}
void freeArrayVec3(Array_Vec3 *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}
void initArrayVec2(Array_Vec2 *a, size_t initialSize) 
{
    a->array = (vec2_t*)malloc(initialSize * sizeof(Array_Vec4));
    a->used = 0;
    a->size = initialSize;
}
void insertArrayVec2(Array_Vec2 *a, vec2_t element)
{
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (vec2_t*)realloc(a->array, a->size * sizeof(vec2_t));
    }
    a->array[a->used++] = element;
}
void freeArrayVec2(Array_Vec2 *a)
{
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}


void initArrayInt(Array_int *a, size_t initialSize) {
    a->array = (int*)malloc(initialSize * sizeof(int));
    a->used = 0;
    a->size = initialSize;
}
void insertArrayInt(Array_int *a, int element) {
    // a->used is the number of used entries, because a->array[a->used++] updates a->used only *after* the array has been accessed.
    // Therefore a->used can go up to a->size
    if (a->used == a->size)
    {
        a->size *= 2;
        a->array = (int*)realloc(a->array, a->size * sizeof(int));
    }
    a->array[a->used++] = element;
}
void freeArrayInt(Array_int *a) {
    free(a->array);
    a->array = NULL;
    a->used = a->size = 0;
}