#include "../lib/tigr.h"
#include "ray.h"
#include "triangles.h"
#include <math.h>
#include <stdlib.h>

#define WIDTH 320
#define HEIGHT 240
TPixel ray_color(ray_t r){
    vec3_t unit_direction = unit_vector(r.direction);
    double a = 0.5*(unit_direction.y + 1.0);
    return tigrRGBA(0xff, 0xff, 0xff, a);
}

void DrawTriangle(Tigr *bmp, int x1, int y1, int x2, int y2, int x3, int y3, TPixel color) {
  tigrLine(bmp, x1, y1, x2, y2,color);
  tigrLine(bmp, x2, y2, x3, y3,color);
  tigrLine(bmp, x3, y3, x1, y1,color);
}

int main(int argc, char *argv[])
{
    Tigr *screen = tigrWindow(WIDTH, HEIGHT, "Hello", 0);
    double aspect_ratio = WIDTH/HEIGHT;
    double viewport_height = 2.0;
    double viewport_width = viewport_height * (WIDTH/HEIGHT);


    // Set up the cameras
    // Camera

    double focal_length = 1.0;
    vec3_t camera_center = (vec3_t){.x=0, .y=0, .z=0};

    // Calculate the vectors across the horizontal and down the vertical viewport edges.
    vec3_t viewport_u = (vec3_t){.x=viewport_width, .y=0, .z=0};
    vec3_t viewport_v = (vec3_t){.x=0,.y=-viewport_height, .z=0};

    // Calculate the horizontal and vertical delta vectors from pixel to pixel.
    vec3_t pixel_delta_u = vec3_div(viewport_u, WIDTH);
    vec3_t pixel_delta_v = vec3_div(viewport_v, HEIGHT);

    // Calculate the location of the upper left pixel.
    vec3_t viewport_upper_left = sub(sub(sub(camera_center, (vec3_t){.x=0, .y=0, .z=focal_length}), vec3_div(viewport_u,2.0)),vec3_div(viewport_v,2.0));
    vec3_t pixel00_loc = add(viewport_upper_left, mul(add(pixel_delta_u, pixel_delta_v), 0.5));
    triangle_t cube_verticies []  = {
    // South
    triangle(vertex(vec3(0.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    triangle(vertex(vec3(0.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    // East
    triangle(vertex(vec3(1.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    triangle(vertex(vec3(1.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    // North
    triangle(vertex(vec3(1.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    triangle(vertex(vec3(1.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    // West
    triangle(vertex(vec3(0.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    triangle(vertex(vec3(0.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),

    // Top
    triangle(vertex(vec3(0.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    triangle(vertex(vec3(0.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 1.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    // Bottom
    triangle(vertex(vec3(1.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D)),
    triangle(vertex(vec3(1.0f, 0.0f, 1.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(0.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D),
             vertex(vec3(1.0f, 0.0f, 0.0f), EMPTY_VEC2D, EMPTY_VEC3D))
    };
    mesh_t meshCube = create_mesh(cube_verticies, 12);

    // Projection Matrix
    float fNear = 0.1f;
    float fFar = 1000.0f;
    float fFov = 90.0f;
    float fAspectRatio = WIDTH / HEIGHT;
    float fFovRad = 1.0f / tanf(fFov * 0.5f / 180.0f * 3.14159f);
    float fTheta = 0.5;
    float fElapsedTime = 0;
    mat4x4_t matProj = (mat4x4_t){.m={0}};
    matProj.m[0][0] = fAspectRatio * fFovRad;
    matProj.m[1][1] = fFovRad;
    matProj.m[2][2] = fFar / (fFar - fNear);
    matProj.m[3][2] = (-fFar * fNear) / (fFar - fNear);
    matProj.m[2][3] = 1.0f;
    matProj.m[3][3] = 0.0f;
    
    while (!tigrClosed(screen))
    {
      fElapsedTime = tigrTime();
        tigrClear(screen, tigrRGB(0x80, 0x90, 0xa0));
        // Set up rotation matrices
        mat4x4_t matRotZ, matRotX;
        fTheta += 1.0f * fElapsedTime;

        // Rotation Z
        matRotZ.m[0][0] = cosf(fTheta);
        matRotZ.m[0][1] = sinf(fTheta);
        matRotZ.m[1][0] = -sinf(fTheta);
        matRotZ.m[1][1] = cosf(fTheta);
        matRotZ.m[2][2] = 1;
        matRotZ.m[3][3] = 1;

        // Rotation X
        matRotX.m[0][0] = 1;
        matRotX.m[1][1] = cosf(fTheta * 0.5f);
        matRotX.m[1][2] = sinf(fTheta * 0.5f);
        matRotX.m[2][1] = -sinf(fTheta * 0.5f);
        matRotX.m[2][2] = cosf(fTheta * 0.5f);
        matRotX.m[3][3] = 1;

        // Draw Triangles
        for (int i = 0; i < meshCube.length; i++)
        {
          triangle_t tri = meshCube.tris[i];
          triangle_t triProjected, triTranslated, triRotatedZ, triRotatedZX;

          // Rotate in Z-Axis
          triRotatedZ.vertices[0].position = multiply_matrix_vector(tri.vertices[0].position , matRotZ);
          triRotatedZ.vertices[1].position = multiply_matrix_vector(tri.vertices[1].position, matRotZ);
          triRotatedZ.vertices[2].position = multiply_matrix_vector(tri.vertices[2].position,  matRotZ);

          // Rotate in X-Axis
          triRotatedZX.vertices[0].position = multiply_matrix_vector(triRotatedZ.vertices[0].position,  matRotX);
          triRotatedZX.vertices[1].position = multiply_matrix_vector(triRotatedZ.vertices[1].position,  matRotX);
          triRotatedZX.vertices[2].position = multiply_matrix_vector(triRotatedZ.vertices[2].position,  matRotX);

          // Offset into the screen
          triTranslated = triRotatedZX;
          triTranslated.vertices[0].position.z = triRotatedZX.vertices[0].position.z + 3.0f;
          triTranslated.vertices[1].position.z = triRotatedZX.vertices[1].position.z + 3.0f;
          triTranslated.vertices[2].position.z = triRotatedZX.vertices[2].position.z + 3.0f;

          // Project triangles from 3D --> 2D
          triProjected.vertices[0].position = multiply_matrix_vector(triTranslated.vertices[0].position, matProj);
          triProjected.vertices[1].position = multiply_matrix_vector(triTranslated.vertices[1].position,  matProj);
          triProjected.vertices[2].position = multiply_matrix_vector(triTranslated.vertices[2].position, matProj);

          // Scale into view
          triProjected.vertices[0].position.x += 1.0f; triProjected.vertices[0].position.y += 1.0f;
          triProjected.vertices[1].position.x += 1.0f; triProjected.vertices[1].position.y += 1.0f;
          triProjected.vertices[2].position.x += 1.0f; triProjected.vertices[2].position.y += 1.0f;
          triProjected.vertices[0].position.x *= 0.5f * (float)WIDTH;
          triProjected.vertices[0].position.y *= 0.5f * (float)HEIGHT;
          triProjected.vertices[1].position.x *= 0.5f * (float)WIDTH;
          triProjected.vertices[1].position.y *= 0.5f * (float)HEIGHT;
          triProjected.vertices[2].position.x *= 0.5f * (float)WIDTH;
          triProjected.vertices[2].position.y *= 0.5f * (float)HEIGHT;

          // Rasterize triangle
          DrawTriangle(screen,
                       triProjected.vertices[0].position.x, triProjected.vertices[0].position.y,
                       triProjected.vertices[1].position.x, triProjected.vertices[1].position.y,
                       triProjected.vertices[2].position.x, triProjected.vertices[2].position.y,
                       tigrRGB(255, 255, 255));

        }
        tigrUpdate(screen);
    }
    free_mesh(&meshCube);
    tigrFree(screen);
    return 0;
}