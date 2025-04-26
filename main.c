#define FAST_OBJ_IMPLEMENTATION
#include "include/fast_obj.h"
#include <stdio.h>

int main() {
    fastObjMesh* obj = fast_obj_read("kitty.obj");
    printf("Mesh Face count %u", obj->face_count);
    int cnt = 0, cnt2 = 0;
    int surface_count = 0;
    for (int i = 0; i < (obj->index_count)+1; i++) {
        surface_count ++;
        
        printf("Positions %lf %lf %lf\n", 
            (double)obj->positions[(obj->indices[i].p)*(3)],
            (double)obj->positions[(obj->indices[i].p)*(3)+1],
            (double)obj->positions[(obj->indices[i].p)*3+2]
        );
        printf("Tex %lf %lf\n",
            (double)obj->texcoords[(obj->indices[i].t)*(2)],
            (double)obj->positions[(obj->indices[i].t)*(2)+1]
        );
        
        printf("normal = %lf %lf %lf\n",
            (double)obj->normals[(obj->indices[i].n)*(3)],
            (double)obj->normals[(obj->indices[i].n)*(3)+1],
            (double)obj->normals[(obj->indices[i].n)*3+2]
        );
        
        printf("error at %d, surface count: %d, cnt: %d, cnt2: %d\n",i, surface_count, cnt, cnt2);
        if (surface_count == 3) {
            surface_count = 0;
            printf("triangle made");
        }
    }
    return 0;
}