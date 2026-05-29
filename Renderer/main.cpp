#define _USE_MATH_DEFINES

#include <iostream>
#include "Window.h"
#include "rasterization.h"
#include "Entity.h"
#include "objReader.h"

#define W_WIDTH 800
#define W_HEIGHT 600
#define FPS 60
#define DELTA 1.f/FPS


int main() {

	Entity cube;
	loadEntityFromFile("blendercube.obj", cube);

	Window* window = new Window(W_WIDTH, W_HEIGHT);
	int width = window->getWidth();
	int height = window->getHeight();

	std::vector<std::array<Vec<float, 2>, 3>> triangles;

	Mat4 translation;

	Mat4 view;
	view.m[T_Z] = -3.f;

	float tangent = std::tan(FOV / 2);
	float right = FRONT * tangent;
	float aspect = (float)width / (float)height;
	float top = right / aspect;

	Mat4 projection = {
	FRONT / right, 0, 0, 0,
	0, FRONT / top, 0, 0,
	0, 0, -(BACK + FRONT) / (BACK - FRONT), -1,
	0, 0, -(2 * BACK * FRONT) / (BACK - FRONT), 0
	};

	float xOffset = 0.f;
	float zOffset = -5.f;
	float angle = 0.01 * M_PI;

	Quaternion orientation = { 1, 0, 0, 0 };


	bool running = true;
	while (running) {
		if (!window->processMessages()) {
			running = false;
		}

		zOffset += 1.5 * DELTA;

		updateQuaternion(orientation, { 0, angle, angle / 2 });

		window->clearScreen();

		updateRenderable(cube, triangles, translation, orientation, { xOffset, 0,zOffset }, projection, width, height);
		
		rasterize(cube, triangles, window->getBuffer(), width, height);

		window->render();
		Sleep(1000 / FPS);
	}

	delete window;

	return 0;
}