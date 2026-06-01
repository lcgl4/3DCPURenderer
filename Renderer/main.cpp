#define _USE_MATH_DEFINES

#include <iostream>
#include "Window.h"
#include "rasterization.h"
#include "Entity.h"
#include "objReader.h"
#include <chrono>

#define W_WIDTH 1366
#define W_HEIGHT 768


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
	float zOffset = -10.f;
	float angle = 0.1 * M_PI;

	Quaternion orientation = { 1, 0, 0, 0 };

	using clock = std::chrono::high_resolution_clock;

	auto last = clock::now();

	bool running = true;
	while (running) {

		auto now = clock::now();

		float delta = std::chrono::duration<float>(now - last	).count();

		last = now;

		if (!window->processMessages()) {
			running = false;
		}

		zOffset += 0.5 * delta;

		updateQuaternion(orientation, { 0, angle*delta, angle / 2 * delta });

		window->clearScreen();

		updateRenderable(cube, triangles, translation, orientation, { xOffset, 0,zOffset }, projection, width, height);
		
		rasterize(cube, triangles, window->getBuffer(), width, height);

		window->render();
	}

	delete window;

	return 0;
}