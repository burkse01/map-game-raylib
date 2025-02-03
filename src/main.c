/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"
#include <math.h>

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

#define PHI 1.61803398875f
#define E 0.46364760899965618f
#define PI 3.141592653589f

typedef struct  {
	int edges[30][2];
	Vector3 vertices[12];
} Icosahedron;
Vector3 sphericalToCartesian(Vector3 spherical) {
	float radius = spherical.x;
	float alpha = spherical.y;
	float polar = spherical.z;
	float x = radius * cosf(polar) * cosf(alpha);
	float y = radius * sinf(polar);
	float z = radius * cosf(polar) * sinf(alpha);
	
	return (Vector3) {x, y, z};
}
Icosahedron makeIcosahedron(float radius){
	Icosahedron icosahedron;
	Vector3 cart_vertices_middle[10];
	Vector3 poles_cart[2] = { sphericalToCartesian((Vector3) { radius, 0, PI / 2}), sphericalToCartesian((Vector3) {radius, 0, -PI / 2}) };
	icosahedron.vertices[10] = poles_cart[0];
	icosahedron.vertices[11] = poles_cart[1];
	for (int i = 0; i < 10; i++) {
		Vector3 spherical = (Vector3){ radius, i * PI / 5, (1 - (2 * (i % 2))) * E };
		cart_vertices_middle[i] = sphericalToCartesian(spherical);
		icosahedron.vertices[i] = cart_vertices_middle[i];
		Vector3 point = cart_vertices_middle[i];
		printf("%f , %f , %f\n", point.x, point.y, point.z);
	}
	printf("%f , %f , %f\n", poles_cart[0].x, poles_cart[0].y, poles_cart[0].z);
	printf("%f , %f , %f\n", poles_cart[1].x, poles_cart[1].y, poles_cart[0].z);
	int index = 0;
	for (int i = 0; i < 10; i++) {
		icosahedron.edges[index][0] = i;
		icosahedron.edges[index][1] = 10 + (i % 2);
		index++;
		icosahedron.edges[index][0] = i;
		icosahedron.edges[index][1] = (i + 2) % 10;
		index++;
		icosahedron.edges[index][0] = i;
		icosahedron.edges[index][1] = (i + 1) % 10;
		index++;
	}
	return icosahedron;
}
drawIcosahedron(Icosahedron icosahedron){
	Color color = RED;
	for (int i = 0; i < 30; i++) {
		Vector3 p1 = icosahedron.vertices[icosahedron.edges[i][0]];
		Vector3 p2 = icosahedron.vertices[icosahedron.edges[i][1]];
		//printf("%f , %f \n", p1, p2);
		DrawLine3D(p1, p2, color);
	}
	/*for (int i = 0; i < 12; i++) {
		DrawPoint3D(icosahedron.vertices[i], BLACK);
	}*/
}


int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1280, 800, "Map Game");

	Camera3D camera = { 0 };
	camera.position = (Vector3){ 60, 60, 30 };  // Camera position
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };      // Camera looking at point
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	camera.fovy = 45.0f;                                // Camera field-of-view Y
	camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type
	Icosahedron icosahedron = makeIcosahedron(2);
	DisableCursor();
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();
		ClearBackground(RAYWHITE);

		camera.position = (Vector3){ 0.0f, 2.0f, 10.0f };
		camera.target = (Vector3){ 0.0f, 2.0f, 0.0f };
		camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
		camera.projection = CAMERA_PERSPECTIVE;
		camera.fovy = 60.0f;

		UpdateCamera(&camera, CAMERA_THIRD_PERSON);
		BeginMode3D(camera);

		drawIcosahedron(icosahedron);

		EndMode3D();

		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
	}

	// cleanup
	// unload our texture so it can be cleaned up

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
