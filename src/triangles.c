#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "triangles.h"
#include "tigr.h"
#include "pixel_grid.h"

#define min3(a, b, c) (a) < (b) ? ((a) < (c) ? (a) : (c)) : ((b) < (c) ? (b) : (c))
#define max3(a, b, c) (a) > (b) ? ((a) > (c) ? (a) : (c)) : ((b) > (c) ? (b) : (c))

double zBuffer[PIXEL_GRID_WIDTH * PIXEL_GRID_HEIGHT];

mat4x4_t projection_matrix;
mat4x4_t view_matrix;

vec3_t sun;

void clear_z_buffer() {
    for (int i = 0; i < PIXEL_GRID_WIDTH * PIXEL_GRID_HEIGHT; i++) {
        zBuffer[i] = 9999999999;
    }
}

void initialize_matrices() {
    clear_z_buffer();

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

void render_mesh(mesh_t *mesh, texture_t *texture) {
    for (unsigned int i = 0; i < mesh->triangle_count; i++) {
        render_triangle(&mesh->triangles[i], texture);
    }
}

double triangle_signed_area(vec2_t a, vec2_t b, vec2_t c){
    return ((b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x));
}

static inline vec2_t to_pixel_space(vec4_t *p){
    vec2_t pix = vec2(p->x / p->z, p->y / p->z);
    pix.x = pix.x * PIXEL_GRID_WIDTH / 2 + PIXEL_GRID_WIDTH / 2;
    pix.y = PIXEL_GRID_HEIGHT - (pix.y * PIXEL_GRID_HEIGHT / 2 + PIXEL_GRID_HEIGHT / 2);
    return pix;
}

void render_clipped_triangle(vertex_t *v1, vertex_t *v2, vertex_t *v3, texture_t *texture) {
    // points on the screen in pixels
    vec2_t A = to_pixel_space(&v1->position);
    vec2_t B = to_pixel_space(&v2->position);
    vec2_t C = to_pixel_space(&v3->position);
    const double ABC = triangle_signed_area(A, B, C);

    vec2_t p = vec2(0, 0);

    double x_min = min3(A.x, B.x, C.x);
    double x_max = max3(A.x, B.x, C.x);
    double y_min = min3(A.y, B.y, C.y);
    double y_max = max3(A.y, B.y, C.y);
    
    x_min = x_min < 0 ? 0 : x_min;
    x_max = x_max >= PIXEL_GRID_WIDTH ? PIXEL_GRID_WIDTH : x_max;
    y_min = y_min < 0 ? 0 : y_min;
    y_max = y_max >= PIXEL_GRID_HEIGHT ? PIXEL_GRID_HEIGHT : y_max;

    for (p.y = y_min; p.y < y_max; p.y++) {
        for (p.x = x_min; p.x < x_max; p.x++) {
            double BCP = triangle_signed_area(B, C, p);
            double CAP = triangle_signed_area(C, A, p);
            double ABP = triangle_signed_area(A, B, p);

            if (BCP <= 0 && CAP <= 0 && ABP <= 0) {
                const double weight_a = BCP / ABC;
                const double weight_b = CAP / ABC;
                const double weight_c = ABP / ABC;
                vec3_t *normal_a = &v1->normal;
                vec3_t *normal_b = &v2->normal;
                vec3_t *normal_c = &v3->normal;
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
                brightness *= 1.5;
                if (brightness > 1) {
                    brightness = 1;
                }

                int xp = (int) p.x;
                int yp = (int) p.y;

                double inv_z = weight_a / v1->position.z + weight_b / v2->position.z + weight_c / v3->position.z;
                double z = 1.0 / inv_z;
                if (z > zBuffer[xp + yp * PIXEL_GRID_WIDTH]) {
                    continue;
                }

                // default pink colour
                unsigned char r = 255;
                unsigned char g = 0;
                unsigned char b = 255;

                if (texture != NULL) {
                    double u_z = weight_a * v1->textureCoords.x / v1->position.z + weight_b * v2->textureCoords.x / v2->position.z + weight_c * v3->textureCoords.x / v3->position.z;
                    double v_z = weight_a * v1->textureCoords.y / v1->position.z + weight_b * v2->textureCoords.y / v2->position.z + weight_c * v3->textureCoords.y / v3->position.z;
                    int up = (int) (u_z / inv_z * texture->width) % texture->width;
                    int vp = (int) (v_z / inv_z * texture->height) % texture->height;
                    if (up < 0) {
                        up += texture->width;
                    }
                    if (vp < 0) {
                        vp += texture->height;
                    }
                    int offset = (up + vp * texture->width) * 4;

                    r = texture->pixels[offset];
                    g = texture->pixels[offset + 1];
                    b = texture->pixels[offset + 2];
                }

                // Draw the pixel
                tigrPlot(gScreen, (int) p.x, (int) p.y, tigrRGB((int) (r * brightness), (int) (g * brightness), (int) (b * brightness)));
                zBuffer[xp + yp * PIXEL_GRID_WIDTH] = z;
            }

        }
    }
}

#include <stdbool.h>

void render_triangle(triangle_t *tri, texture_t *texture){

    mat4x4_t transformation = mat4x4_multiply_mat4x4(&projection_matrix, &view_matrix);

    //get position of the vertices on screen 
    vec4_t p1 = mat4x4_multiply(&transformation,
       tri->vertices[0]->position);
    vec4_t p2 = mat4x4_multiply(&transformation,
        tri->vertices[1]->position);
    vec4_t p3 = mat4x4_multiply(&transformation,
        tri->vertices[2]->position);
    
    vertex_t v1 = make_vertex(vec3(p1.x, p1.y, p1.z), tri->vertices[0]->textureCoords, tri->vertices[0]->normal);
    vertex_t v2 = make_vertex(vec3(p2.x, p2.y, p2.z), tri->vertices[1]->textureCoords, tri->vertices[1]->normal);
    vertex_t v3 = make_vertex(vec3(p3.x, p3.y, p3.z), tri->vertices[2]->textureCoords, tri->vertices[2]->normal);

    const double clip_distance = 0.1;
    const bool clip1 = v1.position.z <= clip_distance;
    const bool clip2 = v2.position.z <= clip_distance;
    const bool clip3 = v3.position.z <= clip_distance;
    if (clip1 && clip2 && clip3) {
        return;
    }

    // clip triangle (bad code, but at least it's all concentrated here)
    if (clip1 || clip2 || clip3) {
        if (clip1 && clip2) {
            double pr13 = -(v1.position.z - clip_distance) / (v3.position.z - v1.position.z);
            double pr23 = -(v2.position.z - clip_distance) / (v3.position.z - v2.position.z);
            vec3_t pos1 = vec4_interpolate(&v1.position, &v3.position, pr13);
            vec3_t pos2 = vec4_interpolate(&v2.position, &v3.position, pr23);
            v1.position = vec4(pos1.x, pos1.y, pos1.z, 1);
            v2.position = vec4(pos2.x, pos2.y, pos2.z, 1);
            v1.textureCoords = vec2_interpolate(&v1.textureCoords, &v3.textureCoords, pr13);
            v2.textureCoords = vec2_interpolate(&v2.textureCoords, &v3.textureCoords, pr23);
            v1.normal = vec3_interpolate(&v1.normal, &v3.normal, pr13);
            v2.normal = vec3_interpolate(&v2.normal, &v3.normal, pr23);
        } else if (clip1 && clip3) {
            double pr32 = -(v3.position.z - clip_distance) / (v2.position.z - v3.position.z);
            double pr12 = -(v1.position.z - clip_distance) / (v2.position.z - v1.position.z);
            vec3_t pos3 = vec4_interpolate(&v3.position, &v2.position, pr32);
            vec3_t pos1 = vec4_interpolate(&v1.position, &v2.position, pr12);
            v3.position = vec4(pos3.x, pos3.y, pos3.z, 1);
            v1.position = vec4(pos1.x, pos1.y, pos1.z, 1);
            v3.textureCoords = vec2_interpolate(&v3.textureCoords, &v2.textureCoords, pr32);
            v1.textureCoords = vec2_interpolate(&v1.textureCoords, &v2.textureCoords, pr12);
            v3.normal = vec3_interpolate(&v3.normal, &v2.normal, pr32);
            v1.normal = vec3_interpolate(&v1.normal, &v2.normal, pr12);
        } else if (clip2 && clip3) {
            double pr21 = -(v2.position.z - clip_distance) / (v1.position.z - v2.position.z);
            double pr31 = -(v3.position.z - clip_distance) / (v1.position.z - v3.position.z);
            vec3_t pos2 = vec4_interpolate(&v2.position, &v1.position, pr21);
            vec3_t pos3 = vec4_interpolate(&v3.position, &v1.position, pr31);
            v2.position = vec4(pos2.x, pos2.y, pos2.z, 1);
            v3.position = vec4(pos3.x, pos3.y, pos3.z, 1);
            v2.textureCoords = vec2_interpolate(&v2.textureCoords, &v1.textureCoords, pr21);
            v3.textureCoords = vec2_interpolate(&v3.textureCoords, &v1.textureCoords, pr31);
            v2.normal = vec3_interpolate(&v2.normal, &v1.normal, pr21);
            v3.normal = vec3_interpolate(&v3.normal, &v1.normal, pr31);
        } else if (clip1) {
            double pr12 = -(v1.position.z - clip_distance) / (v2.position.z - v1.position.z);
            double pr13 = -(v1.position.z - clip_distance) / (v3.position.z - v1.position.z);
            vertex_t v12 = make_vertex(
                vec4_interpolate(&v1.position, &v2.position, pr12),
                vec2_interpolate(&v1.textureCoords, &v2.textureCoords, pr12),
                vec3_interpolate(&v1.normal, &v2.normal, pr12)
            );
            vertex_t v13 = make_vertex(
                vec4_interpolate(&v1.position, &v3.position, pr13),
                vec2_interpolate(&v1.textureCoords, &v3.textureCoords, pr13),
                vec3_interpolate(&v1.normal, &v3.normal, pr13)
            );
            render_clipped_triangle(&v13, &v12, &v2, texture);
            render_clipped_triangle(&v2, &v3, &v13, texture);
            return;
        } else if (clip2) {
            double pr23 = -(v2.position.z - clip_distance) / (v3.position.z - v2.position.z);
            double pr21 = -(v2.position.z - clip_distance) / (v1.position.z - v2.position.z);
            vertex_t v23 = make_vertex(
                vec4_interpolate(&v2.position, &v3.position, pr23),
                vec2_interpolate(&v2.textureCoords, &v3.textureCoords, pr23),
                vec3_interpolate(&v2.normal, &v3.normal, pr23)
            );
            vertex_t v21 = make_vertex(
                vec4_interpolate(&v2.position, &v1.position, pr21),
                vec2_interpolate(&v2.textureCoords, &v1.textureCoords, pr21),
                vec3_interpolate(&v2.normal, &v1.normal, pr21)
            );
            render_clipped_triangle(&v21, &v23, &v3, texture);
            render_clipped_triangle(&v3, &v1, &v21, texture);
            return;
        } else if (clip3) {
            double pr31 = -(v3.position.z - clip_distance) / (v1.position.z - v3.position.z);
            double pr32 = -(v3.position.z - clip_distance) / (v2.position.z - v3.position.z);
            vertex_t v31 = make_vertex(
                vec4_interpolate(&v3.position, &v1.position, pr31),
                vec2_interpolate(&v3.textureCoords, &v1.textureCoords, pr31),
                vec3_interpolate(&v3.normal, &v1.normal, pr31)
            );
            vertex_t v32 = make_vertex(
                vec4_interpolate(&v3.position, &v2.position, pr32),
                vec2_interpolate(&v3.textureCoords, &v2.textureCoords, pr32),
                vec3_interpolate(&v3.normal, &v2.normal, pr32)
            );
            render_clipped_triangle(&v32, &v31, &v1, texture);
            render_clipped_triangle(&v1, &v2, &v32, texture);
            return;
        }
    }

    render_clipped_triangle(&v1, &v2, &v3, texture);
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