#include "pixel_grid.h"
#include "triangles.h"
#include <stdbool.h>
#include "cube.h"

Tigr* window;
Tigr* gScreen;

vec3_t camera_position;
double camera_x_rotation;
double camera_y_rotation;

mesh_t cube;

#include <stdio.h>
// Runs 60 times per second
// Can be used for timers, simulation state updates, etc.
void tick_state() {
	vec2_t impulse = vec2(0, 0);

	if (tigrKeyHeld(window, 83)){
		impulse.y += 0.01;
	} else if (tigrKeyHeld( window, 87)){
		impulse.y -= 0.01;
	} else if (tigrKeyHeld( window, 68)){
		impulse.x += 0.01;
	} else if (tigrKeyHeld( window, 65)){
		impulse.x -= 0.01;
	}

	camera_position.x += impulse.x * cos(camera_y_rotation) + impulse.y * sin(camera_y_rotation);
	camera_position.z += impulse.y * cos(camera_y_rotation) - impulse.x * sin(camera_y_rotation);

	if (tigrKeyHeld(window, TK_LEFT)) {
		camera_y_rotation += 0.01;
	} else if (tigrKeyHeld(window, TK_RIGHT)) {
		camera_y_rotation -= 0.01;
	}
}

void render_state() {
	set_view_matrix(&camera_position, camera_x_rotation, camera_y_rotation);

	render_mesh(&cube);
}

void initialize(){
	camera_position = vec3(0, 0, 2);
	camera_x_rotation = 0;
	camera_y_rotation = 0;

	initialize_matrices();

	cube = make_cube();
}

void cleanup() {
	free_mesh(&cube);
}

int main(int argc, char* argv[]) {
	// Create window
	window = tigrWindow(PIXEL_GRID_WIDTH, PIXEL_GRID_HEIGHT, "Software Renderer", TIGR_FIXED);

	//initialize shit 
	initialize();
	

	// Create externally-accessible 320x240 bitmap
	gScreen = tigrBitmap(PIXEL_GRID_WIDTH, PIXEL_GRID_HEIGHT);

    while (!tigrClosed(window) && !tigrKeyDown(window, TK_ESCAPE)) {
        tigrClear(gScreen, tigrRGB(0, 0, 0));

		tick_state();
		render_state();

		tigrBlit(window, gScreen, 0, 0, 0, 0, PIXEL_GRID_WIDTH, PIXEL_GRID_HEIGHT);

        tigrUpdate(window);
    }

	cleanup();

    tigrFree(window);

	return 0;
}