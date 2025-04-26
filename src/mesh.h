#ifndef _MESH_H
#define _MESH_H

#include "triangles.h"

void load_mesh(mesh_t* mesh, double scale, const char* path);
void free_mesh(mesh_t* mesh);

#endif