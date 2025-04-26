#include "pixel_grid.h"
#include "triangles.h"
#include <stdbool.h>
#include "hardcoded_geometry.h"
#include "texture.h"
#include "lighting.h"
#include "mesh.h"

Tigr* window;
Tigr* gScreen;

vec3_t camera_position;
double camera_x_rotation;
double camera_y_rotation;

mesh_t cube;
mesh_t ground;
mesh_t cat;
mesh_t fish;
texture_t cobblestone;
texture_t floor_tex;
texture_t cat_tex;
texture_t fish_tex;

double time_seconds = 0;

#include <stdio.h>
// Runs 60 times per second
// Can be used for timers, simulation state updates, etc.
void tick_state() {
	const double move_speed = 0.02;
	const double rot_speed = 0.01;

	vec2_t impulse = vec2(0, 0);

	if (tigrKeyHeld(window, 'S')){
		impulse.y += move_speed;
	} else if (tigrKeyHeld( window, 'W')){
		impulse.y -= move_speed;
	}
	if (tigrKeyHeld(window, 'D')){
		impulse.x += move_speed;
	} else if (tigrKeyHeld( window, 'A')){
		impulse.x -= move_speed;
	}

	double y_add = 0;
	if (tigrKeyHeld(window, TK_SPACE)) {
		y_add = move_speed;
	} else if (tigrKeyHeld(window, TK_SHIFT)) {
		y_add = -move_speed;
	}

	camera_position.x += impulse.x * cos(camera_y_rotation) + impulse.y * sin(camera_y_rotation);
	camera_position.z += impulse.y * cos(camera_y_rotation) - impulse.x * sin(camera_y_rotation);
	camera_position.y += y_add;

	if (tigrKeyHeld(window, TK_LEFT)) {
		camera_y_rotation += rot_speed;
	} else if (tigrKeyHeld(window, TK_RIGHT)) {
		camera_y_rotation -= rot_speed;
	}
	if (tigrKeyHeld(window, TK_UP)) {
		camera_x_rotation += rot_speed;
	} else if (tigrKeyHeld(window, TK_DOWN)) {
		camera_x_rotation -= rot_speed;
	}

	time_seconds += 1 / 60.0;
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

	render_mesh(&ground, &floor_tex, vec3(0, 0, 0), vec3(0, 0, 0));
	// render_mesh(&cube, &cobblestone, vec3(3, 0, 3), vec3(0, 0, 0));
	// render_mesh(&cube, &cobblestone, vec3(0, 0, 0), vec3(0, time_seconds, 0));
	render_mesh(&cat, &cat_tex, vec3(0, -0.5, 0), vec3(0, time_seconds, 0));

	for (int i = 0; i < 5; i++) {
		double t = time_seconds + i * 2.5;
		vec3_t fish_pos = vec3(0, 0, 0);
		fish_pos.x = cos(t * 0.5) * 1.5;
		fish_pos.z = sin(t * 0.5) * 1.5;
		render_mesh(&fish, NULL, fish_pos, vec3(0, -t * 0.5, 0));
	}

	post_process();
}

void initialize(){
	camera_position = vec3(0, 0, 2);
	camera_x_rotation = 0;
	camera_y_rotation = 0;

	initialize_rendering();
	initialize_lighting();

	cube = make_cube();
	ground = make_ground();

	load_mesh(&cat, 0.05, "assets/maxwell.obj");
	load_mesh(&fish, 0.1, "assets/fish.obj");

	load_texture(&cobblestone, "assets/cobblestone.png");
	load_texture(&floor_tex, "assets/floor.png");
	load_texture(&cat_tex, "assets/dingus_nowhiskers.png");
	load_texture(&fish_tex, "assets/fish.png");
}

void cleanup() {
	free_mesh(&cube);
	free_mesh(&ground);
	free_mesh(&cat);
	free_mesh(&fish);
	free_texture(&cobblestone);
	free_texture(&floor_tex);
	free_texture(&cat_tex);
	free_texture(&fish_tex);
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