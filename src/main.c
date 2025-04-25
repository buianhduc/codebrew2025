#include "pixel_grid.h"
#include "triangles.h"
#include <stdbool.h>

Tigr* window;
Tigr* gScreen;

// Where to draw the red pixel
int red_pixel_x = 240, red_pixel_y = 200;

triangle_t tri;

vec3_t camera_position;
vec3_t camera_rotation;

// Runs 60 times per second
// Can be used for timers, simulation state updates, etc.
void tick_state() {
	if (tigrKeyHeld(window, TK_DOWN)){
		camera_position.z += 0.01;
	}
}

void render_state() {

	
	set_view_matrix(&camera_position, 0, 0);


	render_triangle(&tri);
}

void initialize(){
	camera_position = vec3(0, 0, 0);
	camera_rotation = vec3(0, 0, 0);

	initialize_matrices();

	vec3_t p1 = vec3(0.0, 1.0 , -2.0);
	vec3_t p2 = vec3(1.0, -1.0 , -2.0);
	vec3_t p3 = vec3(-1.0, -1.0 , -2.0);

	vertex_t v1 = make_basic_vertex(&p1);
	vertex_t v2 = make_basic_vertex(&p2);
	vertex_t v3 = make_basic_vertex(&p3);

	tri = make_triangle(&v1, &v2, &v3);
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

    tigrFree(window);

	return 0;
