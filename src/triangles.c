#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "triangles.h"
#include "../include/tigr.h"
#include "pixel_grid.h"
#include "vector.h"
#include "../include/fast_obj.h"


mat4x4_t projection_matrix;
mat4x4_t view_matrix;

vec3_t sun;

void initialize_matrices()
{
    double a = 320 / 240.0;
    double fov = 70.0 * M_PI / 180;
    double z_near = 0.01;
    double z_far = 100;
    projection_matrix = make_projection_matrix(a, fov, z_near, z_far);

    load_identity(&view_matrix);

    sun = vec3(-6, -10, 8);
    normalize(&sun);
}

void free_mesh(mesh_t *mesh)
{
    free(mesh->vertices);
    free(mesh->triangles);
}

void render_mesh(mesh_t *mesh)
{
    for (unsigned int i = 0; i < mesh->triangle_count; i++)
    {
        render_triangle(&mesh->triangles[i]);
    }
}

double edge_function(vec2_t a, vec2_t b, vec2_t c)
{
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

static inline vec2_t to_pixel_space(vec4_t *p)
{
    vec2_t pix = vec2(p->x / p->z, p->y / p->z);
    pix.x = pix.x * PIXEL_GRID_WIDTH / 2 + PIXEL_GRID_WIDTH / 2;
    pix.y = pix.y * PIXEL_GRID_HEIGHT / 2 + PIXEL_GRID_HEIGHT / 2;
    return pix;
}

void render_triangle(triangle_t *tri)
{

    mat4x4_t transformation = mat4x4_multiply_mat4x4(&projection_matrix, &view_matrix);

    // get position of the vertices on screen
    vec4_t p1 = mat4x4_multiply(&transformation,
                                tri->vertices[0]->position);
    vec4_t p2 = mat4x4_multiply(&transformation,
                                tri->vertices[1]->position);
    vec4_t p3 = mat4x4_multiply(&transformation,
                                tri->vertices[2]->position);

    // clip the triangle
    if (p1.z <= 0 || p2.z <= 0 || p3.z <= 0)
    {
        return;
    }

    // points on the screen in pixels
    vec2_t A = to_pixel_space(&p1);
    vec2_t B = to_pixel_space(&p2);
    vec2_t C = to_pixel_space(&p3);
    const double ABC = edge_function(A, B, C);

    vec2_t p = vec2(0, 0);

    for (p.y = 0; p.y < PIXEL_GRID_HEIGHT; p.y++)
    {
        for (p.x = 0; p.x < PIXEL_GRID_WIDTH; p.x++)
        {
            double BCP = edge_function(B, C, p);
            double CAP = edge_function(C, A, p);
            double ABP = edge_function(A, B, p);

            if (BCP >= 0 && CAP >= 0 && ABP >= 0)
            {
                const double weight_a = BCP / ABC;
                const double weight_b = CAP / ABC;
                const double weight_c = ABP / ABC;
                vec3_t *normal_a = &tri->vertices[0]->normal;
                vec3_t *normal_b = &tri->vertices[1]->normal;
                vec3_t *normal_c = &tri->vertices[2]->normal;
                vec3_t normal = vec3(
                    weight_a * normal_a->x + weight_b * normal_b->x + weight_c * normal_c->x,
                    weight_a * normal_a->y + weight_b * normal_b->y + weight_c * normal_c->y,
                    weight_a * normal_a->z + weight_b * normal_b->z + weight_c * normal_c->z);

                double dot = sun.x * normal.x + sun.y * normal.y + sun.z * normal.z;
                double brightness = 0.25;
                if (dot > 0)
                {
                    brightness = dot;
                }

                // Draw the pixel
                tigrPlot(gScreen, (int)p.x, (int)p.y, tigrRGB((int)(255 * brightness), 0, (int)(255 * brightness)));
            }
        }
    }
}

void set_view_matrix(vec3_t *p, double x_rotation, double y_rotation)
{
    mat4x4_t translate = make_translation_matrix(vec3(
        -p->x,
        -p->y,
        -p->z));
    mat4x4_t x_rotate = make_rotation_matrix_x(-x_rotation);
    mat4x4_t y_rotate = make_rotation_matrix_y(-y_rotation);

    view_matrix = mat4x4_multiply_mat4x4(&y_rotate, &translate);
    view_matrix = mat4x4_multiply_mat4x4(&x_rotate, &view_matrix);
}

// mesh_t* load_object_from_file(char *filePath, mesh_t *mesh, bool fullVersion)
// {
//     FILE *file = fopen(filePath, "r");
//     if (file == NULL)
//     {
//         printf("Impossible to open the file !\n");
//         return NULL;
//     }
//     int cnt = 0;

//     Array_Vec3 position;
//     Array_Vec2 textureCoords;
//     Array_Vec3 normals;
//     Array_int vertexIndices, uvIndices, normalIndices;
//     // Initialize the arrays
//     initArrayVec3(&position, 1);
//     initArrayVec2(&textureCoords, 1);
//     initArrayVec3(&normals, 1);
//     initArrayInt(&vertexIndices, 1);
//     initArrayInt(&uvIndices, 1);
//     initArrayInt(&normalIndices, 1);
//     int triangles_count =0;
//     int vertex_count = 0;
//     while (true)
//     {
//         char lineHeader[128];
//         int res = fscanf(file, "%s", lineHeader);
//         if (res == EOF)
//             break;
//         if (strcmp(lineHeader, "v") == 0)
//         {
//             vertex_count++;
//             vec3_t vertex;
//             fscanf(file, "%lf %lf %lf\n", &vertex.x, &vertex.y, &vertex.z);
//             insertArrayVec3(&position, vertex);
//         }
//         else if (strcmp(lineHeader, "vt") == 0)
//         {
//             vec2_t uv;
//             fscanf(file, "%lf %lf\n", &uv.x, &uv.y);
//             insertArrayVec2(&textureCoords, uv);
//         }
//         else if (strcmp(lineHeader, "vn") == 0)
//         {
//             vec3_t normal;
//             fscanf(file, "%lf %lf %lf\n", &normal.x, &normal.y, &normal.z);
//             insertArrayVec3(&normals, normal);
//         }
//         else if (strcmp(lineHeader, "f") == 0)
//         {
//             triangles_count++;
//             int vertexIndex[3], uvIndex[3], normalIndex[3];
//             if (fullVersion) {
//                 int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n",
//                                 &vertexIndex[0], &uvIndex[0], &normalIndex[0],
//                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1],
//                                 &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
//                 if (matches != 9) return false;
//             }

//             else {
//                 int matches = fscanf(file, "%d %d %d\n",
//                     &vertexIndex[0],// &uvIndex[0], &normalIndex[0],
//                     &vertexIndex[1],// &uvIndex[1], &normalIndex[1],
//                     &vertexIndex[2]);// &uvIndex[2], &normalIndex[2]);
//                 if (matches != 3) return false;
//             }

//             insertArrayInt(&vertexIndices, vertexIndex[0]-1);
//             insertArrayInt(&vertexIndices, vertexIndex[1]-1);
//             insertArrayInt(&vertexIndices, vertexIndex[2]-1);

//             if(fullVersion) {
//                 insertArrayInt(&uvIndices, uvIndex[0] - 1);
//                 insertArrayInt(&uvIndices, uvIndex[1] - 1) ;
//                 insertArrayInt(&uvIndices, uvIndex[2] - 1);

//                 insertArrayInt(&normalIndices, normalIndex[0] - 1);
//                 insertArrayInt(&normalIndices, normalIndex[1] - 1);
//                 insertArrayInt(&normalIndices, normalIndex[2] - 1);
//             }
//         }
//     }

//     mesh = (mesh_t *)malloc(sizeof(mesh_t));
//     mesh->triangle_count = triangles_count;
//     mesh->triangles = (triangle_t*)malloc(sizeof(triangle_t)*triangles_count);
//     mesh->vertex_count = vertex_count;
//     mesh->vertices = (vertex_t*)malloc(sizeof(vertex_t)*vertex_count);
//     for (int i = 0; i < vertex_count; i++) {
//         mesh->vertices[i] = make_vertex(position.array[i], vec2(0,0), vec3(0,0,0));
//     }
//     cnt = 0;
//     for (int i = 0; i < vertexIndices.used-2; i+=3) {
//         mesh->triangles[cnt++] = make_triangle(&mesh->vertices[vertexIndices.array[i]], &mesh->vertices[vertexIndices.array[i+1]], &mesh->vertices[vertexIndices.array[i+2]]);
//     }
//     printf("Mesh's triangle counts is: %d\n", triangles_count);
//     printf("Mesh's vertices counts is: %d\n", vertex_count);
//     for (int i = 0; i < mesh->triangle_count; i++) {
//         printf("Vertex %d: %lf %lf %lf %lf %lf %lf %lf %lf %lf\n", i,
//             mesh->triangles[i].vertices[0]->position.x, mesh->triangles[i].vertices[0]->position.y, mesh->triangles[i].vertices[0]->position.z,
//             mesh->triangles[i].vertices[1]->position.x, mesh->triangles[i].vertices[1]->position.y, mesh->triangles[i].vertices[1]->position.z,
//             mesh->triangles[i].vertices[2]->position.x, mesh->triangles[i].vertices[2]->position.y, mesh->triangles[i].vertices[2]->position.z);
//     }
//     return mesh;
// }

mesh_t* convert_from_fast_object_mesh(fastObjMesh obj, mesh_t* mesh) {
    mesh = (mesh_t*)malloc(sizeof(mesh_t));
    mesh->triangle_count = obj.face_count;
    mesh->triangles = (triangle_t*)malloc(sizeof(mesh));
    mesh->vertex_count = obj.index_count+1;
    mesh->vertices = (vertex_t*) malloc(mesh->vertex_count*sizeof(vertex_t));
    int cnt = 0, cnt2 = 0;
    int surface_count = 0;
    for (int i = 0; i < (obj.index_count); i++) {
        surface_count ++;
        
        vec3_t positions = vec3(
            (double)obj.positions[(obj.indices[i].p)*(3)],
            (double)obj.positions[(obj.indices[i].p)*(3)+1],
            (double)obj.positions[(obj.indices[i].p)*3+2]
        );
        vec2_t uv = obj.texcoord_count > 1 ? vec2(
            (double)obj.texcoords[(obj.indices[i].t)*(2)],
            (double)obj.positions[(obj.indices[i].t)*(2)+1]
        ) : vec2(0,0);
        
        vec3_t normal = obj.normal_count > 1 ? vec3(
            (double)obj.normals[(obj.indices[i].n)*(3)],
            (double)obj.normals[(obj.indices[i].n)*(3)+1],
            (double)obj.normals[(obj.indices[i].n)*3+2]
        ) : vec3(0,0,0);
        mesh->vertices[cnt++] = make_vertex(positions, uv, normal);
        printf("error at %d, surface count: %d, cnt: %d, cnt2: %d\n",i, surface_count, cnt, cnt2);
        if (surface_count == 3) {
            surface_count = 0;
            mesh->triangles[cnt2++] = make_triangle(&mesh->vertices[cnt-3],&mesh->vertices[cnt-2],&mesh->vertices[cnt-1]);
        }
    }
    return mesh;
}
mesh_t *load_object_from_file(char *filePath, mesh_t *mesh, bool fullVersion)
{
    fastObjMesh* meshRead = fast_obj_read(filePath);
    
    mesh = convert_from_fast_object_mesh(*meshRead, mesh);
    
    return mesh;
}