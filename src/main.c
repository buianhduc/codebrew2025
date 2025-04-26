#include "pixel_grid.h"
#include "triangles.h"
#include <stdbool.h>
#include "cube.h"
#include "texture.h"
#include "lighting.h"

Tigr* window;
Tigr* gScreen;

vec3_t camera_position;
double camera_x_rotation;
double camera_y_rotation;

mesh_t cube1;
mesh_t cube2;
mesh_t ground;
texture_t cobblestone;

#include <stdio.h>
// Runs 60 times per second
// Can be used for timers, simulation state updates, etc.
void tick_state() {
	vec2_t impulse = vec2(0, 0);

	if (tigrKeyHeld(window, 'S')){
		impulse.y += 0.01;
	} else if (tigrKeyHeld( window, 'W')){
		impulse.y -= 0.01;
	}
	if (tigrKeyHeld(window, 'D')){
		impulse.x += 0.01;
	} else if (tigrKeyHeld( window, 'A')){
		impulse.x -= 0.01;
	}

	double y_add = 0;
	if (tigrKeyHeld(window, TK_SPACE)) {
		y_add = 0.01;
	} else if (tigrKeyHeld(window, TK_SHIFT)) {
		y_add = -0.01;
	}

	camera_position.x += impulse.x * cos(camera_y_rotation) + impulse.y * sin(camera_y_rotation);
	camera_position.z += impulse.y * cos(camera_y_rotation) - impulse.x * sin(camera_y_rotation);
	camera_position.y += y_add;

	if (tigrKeyHeld(window, TK_LEFT)) {
		camera_y_rotation += 0.01;
	} else if (tigrKeyHeld(window, TK_RIGHT)) {
		camera_y_rotation -= 0.01;
	}
	if (tigrKeyHeld(window, TK_UP)) {
		camera_x_rotation += 0.01;
	} else if (tigrKeyHeld(window, TK_DOWN)) {
		camera_x_rotation -= 0.01;
	}
}

// Reduce colours to RGB332 and add retro dithering
void post_process() {
	for (int y = 0; y < PIXEL_GRID_HEIGHT; y++) {
		for (int x = 0; x < PIXEL_GRID_WIDTH; x++) {
			int dither = ((x + y) % 2) << 0;
			TPixel pix = tigrGet(gScreen, x, y);
			pix.r = (pix.r + dither) >> 3 << 3;
			pix.g = (pix.g + dither) >> 3 << 3;
			pix.b = (pix.b + dither) >> 2 << 2;
			tigrPlot(gScreen, x, y, pix);
		}
	}
}

void render_state() {
	clear_z_buffer();
	set_view_matrix(&camera_position, camera_x_rotation, camera_y_rotation);

	render_mesh(&ground, &cobblestone);
	render_mesh(&cube1, &cobblestone);
	render_mesh(&cube2, &cobblestone);

	post_process();
}

void initialize(){
	camera_position = vec3(0, 0, 2);
	camera_x_rotation = 0;
	camera_y_rotation = 0;

	initialize_rendering();
	initialize_lighting();

	cube1 = make_cube(vec3(0, 0, 0));
	cube2 = make_cube(vec3(-3, 0, -2));
	ground = make_ground();

	load_texture(&cobblestone, "assets/cobblestone.png");
}

void cleanup() {
	free_mesh(&cube1);
	free_mesh(&cube2);
	free_mesh(&ground);
	free_texture(&cobblestone);
}

int main(int argc, char* argv[]) {
	// Create window
	window = tigrWindow(PIXEL_GRID_WIDTH, PIXEL_GRID_HEIGHT, "Software Renderer", TIGR_FIXED);

	//initialize shit 
	initialize();
	

	// Create externally-accessible 320x240 bitmap
	gScreen = tigrBitmap(PIXEL_GRID_WIDTH, PIXEL_GRID_HEIGHT);

    while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE)) {
		// ensure that tick_state() is called 60 times per second
		float unprocessed_time = tigrTime();
		while (unprocessed_time > 0) {
			tick_state();
			unprocessed_time -= 1.0f / 60.0f;
		}

        tigrClear(gScreen, tigrRGB(0, 0, 0));
		render_state();

		tigrBlit(window, gScreen, 0, 0, 0, 0, PIXEL_GRID_WIDTH, PIXEL_GRID_HEIGHT);

        tigrUpdate(window);
    }

	cleanup();

    tigrFree(window);

	return 0;
}