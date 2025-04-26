#ifndef _CUBE_H
#define _CUBE_H

#include "triangles.h"
#include <stdlib.h>

static inline mesh_t make_cube(vec3_t center) {
    mesh_t mesh;
	mesh.vertex_count = 5*4;
	mesh.triangle_count = 5*2;

    mesh.vertices = (vertex_t*) malloc(sizeof(vertex_t) * mesh.vertex_count);
    mesh.triangles = (triangle_t*) malloc(sizeof(triangle_t) * mesh.triangle_count);

    // back face
    mesh.vertices[0] = make_vertex(
        vec3(-0.5, 0.5, 0.5),
        vec2(0, 0),
        vec3(0, 0, 1)
    );
    mesh.vertices[1] = make_vertex(
        vec3(-0.5, -0.5, 0.5),
        vec2(0, 1),
        vec3(0, 0, 1)
    );
    mesh.vertices[2] = make_vertex(
        vec3(0.5, -0.5, 0.5),
        vec2(1, 1),
        vec3(0, 0, 1)
    );
    mesh.vertices[3] = make_vertex(
        vec3(0.5, 0.5, 0.5),
        vec2(1, 0),
        vec3(0, 0, 1)
    );

    // front face
    mesh.vertices[4] = make_vertex(
        vec3(0.5, 0.5, -0.5),
        vec2(0, 0),
        vec3(0, 0, -1)
    );
    mesh.vertices[5] = make_vertex(
        vec3(0.5, -0.5, -0.5),
        vec2(0, 1),
        vec3(0, 0, -1)
    );
    mesh.vertices[6] = make_vertex(
        vec3(-0.5, -0.5, -0.5),
        vec2(1, 1),
        vec3(0, 0, -1)
    );
    mesh.vertices[7] = make_vertex(
        vec3(-0.5, 0.5, -0.5),
        vec2(1, 0),
        vec3(0, 0, -1)
    );

    // left face
    mesh.vertices[8] = make_vertex(
        vec3(-0.5, 0.5, -0.5),
        vec2(0, 0),
        vec3(-1, 0, 0)
    );
    mesh.vertices[9] = make_vertex(
        vec3(-0.5, -0.5, -0.5),
        vec2(0, 1),
        vec3(-1, 0, 0)
    );
    mesh.vertices[10] = make_vertex(
        vec3(-0.5, -0.5, 0.5),
        vec2(1, 1),
        vec3(-1, 0, 0)
    );
    mesh.vertices[11] = make_vertex(
        vec3(-0.5, 0.5, 0.5),
        vec2(1, 0),
        vec3(-1, 0, 0)
    );

    // right face
    mesh.vertices[12] = make_vertex(
        vec3(0.5, 0.5, 0.5),
        vec2(0, 0),
        vec3(1, 0, 0)
    );
    mesh.vertices[13] = make_vertex(
        vec3(0.5, -0.5, 0.5),
        vec2(0, 1),
        vec3(1, 0, 0)
    );
    mesh.vertices[14] = make_vertex(
        vec3(0.5, -0.5, -0.5),
        vec2(1, 1),
        vec3(1, 0, 0)
    );
    mesh.vertices[15] = make_vertex(
        vec3(0.5, 0.5, -0.5),
        vec2(1, 0),
        vec3(1, 0, 0)
    );

    // top face
    mesh.vertices[16] = make_vertex(
        vec3(-0.5, 0.5, -0.5),
        vec2(0, 0),
        vec3(0, 1, 0)
    );
    mesh.vertices[17] = make_vertex(
        vec3(-0.5, 0.5, 0.5),
        vec2(0, 1),
        vec3(0, 1, 0)
    );
    mesh.vertices[18] = make_vertex(
        vec3(0.5, 0.5, 0.5),
        vec2(1, 1),
        vec3(0, 1, 0)
    );
    mesh.vertices[19] = make_vertex(
        vec3(0.5, 0.5, -0.5),
        vec2(1, 0),
        vec3(0, 1, 0)
    );

    for (int i = 0; i < mesh.vertex_count; i++) {
        mesh.vertices[i].position.x += center.x;
        mesh.vertices[i].position.y += center.y;
        mesh.vertices[i].position.z += center.z;
    }

    mesh.triangles[0] = make_triangle(&mesh.vertices[0], &mesh.vertices[1], &mesh.vertices[2]);
    mesh.triangles[1] = make_triangle(&mesh.vertices[2], &mesh.vertices[3], &mesh.vertices[0]);

    mesh.triangles[2] = make_triangle(&mesh.vertices[4], &mesh.vertices[5], &mesh.vertices[6]);
    mesh.triangles[3] = make_triangle(&mesh.vertices[6], &mesh.vertices[7], &mesh.vertices[4]);

    mesh.triangles[4] = make_triangle(&mesh.vertices[8], &mesh.vertices[9], &mesh.vertices[10]);
    mesh.triangles[5] = make_triangle(&mesh.vertices[10], &mesh.vertices[11], &mesh.vertices[8]);

    mesh.triangles[6] = make_triangle(&mesh.vertices[12], &mesh.vertices[13], &mesh.vertices[14]);
    mesh.triangles[7] = make_triangle(&mesh.vertices[14], &mesh.vertices[15], &mesh.vertices[12]);

    mesh.triangles[8] = make_triangle(&mesh.vertices[16], &mesh.vertices[17], &mesh.vertices[18]);
    mesh.triangles[9] = make_triangle(&mesh.vertices[18], &mesh.vertices[19], &mesh.vertices[16]);

    return mesh;
}

static inline mesh_t make_ground() {
    mesh_t mesh;
    mesh.vertex_count = 4;
    mesh.triangle_count = 2;

    mesh.vertices = (vertex_t*) malloc(sizeof(vertex_t) * mesh.vertex_count);
    mesh.triangles = (triangle_t*) malloc(sizeof(triangle_t) * mesh.triangle_count);

    const double size = 3;

    mesh.vertices[0] = make_vertex(
        vec3(-size, -0.5, -size),
        vec2(0, 0),
        vec3(0, 1, 0)
    );
    mesh.vertices[1] = make_vertex(
        vec3(-size, -0.5, size),
        vec2(0, 1),
        vec3(0, 1, 0)
    );
    mesh.vertices[2] = make_vertex(
        vec3(size, -0.5, size),
        vec2(1, 1),
        vec3(0, 1, 0)
    );
    mesh.vertices[3] = make_vertex(
        vec3(size, -0.5, -size),
        vec2(1, 0),
        vec3(0, 1, 0)
    );

    mesh.triangles[0] = make_triangle(&mesh.vertices[0], &mesh.vertices[1], &mesh.vertices[2]);
    mesh.triangles[1] = make_triangle(&mesh.vertices[2], &mesh.vertices[3], &mesh.vertices[0]);

    return mesh;
}

#endif