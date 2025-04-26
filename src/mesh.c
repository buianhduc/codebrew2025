#include "mesh.h"
#include "fast_obj.h"
#include <stdio.h>

void convert_from_fast_object_mesh(fastObjMesh obj, double scale, mesh_t* mesh) {
    mesh->triangle_count = obj.face_count;
    mesh->triangles = (triangle_t*)malloc(sizeof(triangle_t)*obj.face_count);
    mesh->vertex_count = obj.index_count+1;
    mesh->vertices = (vertex_t*) malloc(mesh->vertex_count*sizeof(vertex_t));
    int cnt = 0, cnt2 = 0;
    int surface_count = 0;
    for (int i = 0; i < (obj.index_count); i++) {
        surface_count ++;
        
        vec3_t positions = vec3(
            (double)obj.positions[(obj.indices[i].p)*(3)]*scale,
            (double)obj.positions[(obj.indices[i].p)*(3)+1]*scale,
            (double)obj.positions[(obj.indices[i].p)*3+2]*scale
        );
        vec2_t uv = obj.texcoord_count > 1 ? vec2(
            (double)obj.texcoords[(obj.indices[i].t)*(2)],
            (double)obj.texcoords[(obj.indices[i].t)*(2)+1]
        ) : vec2(0,0);
        
        vec3_t normal = obj.normal_count > 1 ? vec3(
            (double)obj.normals[(obj.indices[i].n)*(3)],
            (double)obj.normals[(obj.indices[i].n)*(3)+1],
            (double)obj.normals[(obj.indices[i].n)*3+2]
        ) : vec3(0,0,0);
        mesh->vertices[cnt++] = make_vertex(positions, uv, normal);
        if (surface_count == 3) {
            surface_count = 0;
            mesh->triangles[cnt2++] = make_triangle(&mesh->vertices[cnt-3],&mesh->vertices[cnt-2],&mesh->vertices[cnt-1]);
        }
    }
}

void load_mesh(mesh_t* mesh, double scale, const char* path) {
    fastObjMesh* meshRead = fast_obj_read(path);
    convert_from_fast_object_mesh(*meshRead, scale, mesh);
    fast_obj_destroy(meshRead);
}

void free_mesh(mesh_t *mesh) {
    free(mesh->vertices);
    free(mesh->triangles);
}