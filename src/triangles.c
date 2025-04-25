#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "triangles.h"
#include "tigr.h"
#include "pixel_grid.h"


mat4x4_t projection_matrix;
mat4x4_t view_matrix; 

void initialize_matrices() {
    double a = 320/240.0;
    double fov = 70.0 * M_PI/180;
    double z_near = 0.01;
    double z_far = 100;
    projection_matrix = make_projection_matrix(a, fov, z_near, z_far);

    load_identity(&view_matrix);
}

void draw_point(vec4_t *p){
    // if (p->z <= 0) {
    //     return;
    // }

    int pixel_x = p->x / p->z * PIXEL_GRID_HEIGHT / 2 + PIXEL_GRID_WIDTH / 2;
    int pixel_y = p->y / p->z * PIXEL_GRID_HEIGHT / 2 + PIXEL_GRID_HEIGHT / 2;

    const int square_size = 8;
    tigrFill(gScreen, pixel_x - square_size / 2, pixel_y - square_size / 2, square_size, square_size, tigrRGB(255, 0, 255));
    tigrPlot(gScreen, pixel_x, pixel_y, tigrRGB(255, 255, 0));
}

void render_triangle(triangle_t *tri){

    //here

    //get position of the vertices on screen 
    vec4_t p1 = mat4x4_multiply(&projection_matrix,
       tri->vertices[0].position);
    vec4_t p2 = mat4x4_multiply(&projection_matrix,
        tri->vertices[1].position);
    vec4_t p3 = mat4x4_multiply(&projection_matrix,
        tri->vertices[2].position);

    draw_point(&p1);
    draw_point(&p2);
    draw_point(&p3);

}

void set_view_matrix(vec3_t * p, double x_rotation, double y_rotation) {
    mat4x4_t translate = make_translation_matrix(vec3(
        -p->x,
        -p->y,
        -p->z
    ));
    mat4x4_t x_rotate = make_rotation_matrix_x(x_rotation);
    mat4x4_t y_rotate = make_rotation_matrix_x(y_rotation);

    //view_matrix = y_rotate * x_rotate * translate (write multiply matrices method)
}