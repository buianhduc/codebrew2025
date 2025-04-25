#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "triangles.h"
#include "tigr.h"
#include "pixel_grid.h"

mat4x4_t projection_matrix;
mat4x4_t view_matrix;

vec3_t sun;

void initialize_matrices() {
    double a = 320/240.0;
    double fov = 70.0 * M_PI/180;
    double z_near = 0.01;
    double z_far = 100;
    projection_matrix = make_projection_matrix(a, fov, z_near, z_far);

    load_identity(&view_matrix);

    sun = vec3(-6, -10, 8);
    normalize(&sun);
}

void free_mesh(mesh_t *mesh) {
    free(mesh->vertices);
    free(mesh->triangles);
}

void render_mesh(mesh_t *mesh) {
    for (unsigned int i = 0; i < mesh->triangle_count; i++) {
        render_triangle(&mesh->triangles[i]);
    }
}

double edge_function(vec2_t a, vec2_t b, vec2_t c){
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

static inline vec2_t to_pixel_space(vec4_t *p){
    vec2_t pix = vec2(p->x / p->z, p->y / p->z);
    pix.x = pix.x * PIXEL_GRID_WIDTH / 2 + PIXEL_GRID_WIDTH / 2;
    pix.y = pix.y * PIXEL_GRID_HEIGHT / 2 + PIXEL_GRID_HEIGHT / 2;
    return pix;
}

void render_triangle(triangle_t *tri){

    mat4x4_t transformation = mat4x4_multiply_mat4x4(&projection_matrix, &view_matrix);

    //get position of the vertices on screen 
    vec4_t p1 = mat4x4_multiply(&transformation,
       tri->vertices[0]->position);
    vec4_t p2 = mat4x4_multiply(&transformation,
        tri->vertices[1]->position);
    vec4_t p3 = mat4x4_multiply(&transformation,
        tri->vertices[2]->position);

    // clip the triangle
    if (p1.z <= 0 || p2.z <= 0 || p3.z <= 0) {
        return;
    }

    // points on the screen in pixels
    vec2_t A = to_pixel_space(&p1);
    vec2_t B = to_pixel_space(&p2);
    vec2_t C = to_pixel_space(&p3);
    const double ABC = edge_function(A, B, C);

    vec2_t p = vec2(0, 0);

    for (p.y = 0; p.y < PIXEL_GRID_HEIGHT; p.y++) {
        for (p.x = 0; p.x < PIXEL_GRID_WIDTH; p.x++) {
            double BCP = edge_function(B, C, p);
            double CAP = edge_function(C, A, p);
            double ABP = edge_function(A, B, p);

            if (BCP >= 0 && CAP >= 0 && ABP >= 0) {
                const double weight_a = BCP / ABC;
                const double weight_b = CAP / ABC;
                const double weight_c = ABP / ABC;
                vec3_t *normal_a = &tri->vertices[0]->normal;
                vec3_t *normal_b = &tri->vertices[1]->normal;
                vec3_t *normal_c = &tri->vertices[2]->normal;
                vec3_t normal = vec3(
                    weight_a * normal_a->x + weight_b * normal_b->x + weight_c * normal_c->x,
                    weight_a * normal_a->y + weight_b * normal_b->y + weight_c * normal_c->y,
                    weight_a * normal_a->z + weight_b * normal_b->z + weight_c * normal_c->z
                );

                double dot = sun.x * normal.x + sun.y * normal.y + sun.z * normal.z;
                double brightness = 0.25;
                if (dot > 0) {
                    brightness = dot;
                }

                // Draw the pixel
                tigrPlot(gScreen, (int) p.x, (int) p.y, tigrRGB((int) (255 * brightness), 0, (int) (255 * brightness)));
            }

        }
    }
}

void set_view_matrix(vec3_t* p, double x_rotation, double y_rotation) {
    mat4x4_t translate = make_translation_matrix(vec3(
        -p->x,
        -p->y,
        -p->z
    ));
    mat4x4_t x_rotate = make_rotation_matrix_x(-x_rotation);
    mat4x4_t y_rotate = make_rotation_matrix_y(-y_rotation);

    view_matrix = mat4x4_multiply_mat4x4(&y_rotate, &translate);
    view_matrix = mat4x4_multiply_mat4x4(&x_rotate, &view_matrix);
}