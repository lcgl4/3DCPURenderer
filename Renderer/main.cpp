#include <iostream>
#include "Window.h"
#include "rasterization.h"
#include "Entity.h"
#include "objReader.h"

#define W_WIDTH 800
#define W_HEIGHT 600

int main() {

	Entity cube;
	loadEntityFromFile("blendercube.obj", cube);



	Window* window = new Window(W_WIDTH, W_HEIGHT);
	int width = window->getWidth();
	int height = window->getHeight();


	float zOffset = 3.f;
	Vec<float, 3>* vs = cube.getVertices()->data();
	Vec<int, 3>* faces = cube.getFaces()->data();
	std::vector <uint32_t> colours = cube.getColours();
	Vec<float, 2> projected[8];

	bool running = true;
	while (running) {
		if (!window->processMessages()) {
			running = false;
		}

		window->clearScreen();


		zOffset += 0.1f;

		for (int i = 0; i <8; i++) {//number of vetrices  8
			projected[i] = placePoint(vs[i], zOffset, width, height);
		}


		//turn inot a function
		for (int i = 0; i < 12; i++) {//12 faces
			Vec<float, 2> tri[] = { {projected[faces[i].x].x, projected[faces[i].x].y}, {projected[faces[i].y].x, projected[faces[i].y].y},  {projected[faces[i].z].x, projected[faces[i].z].y} };
			rasterize(tri, window->getBuffer(), width, height, colours[i]);
		}

		window->render();
		Sleep(10);
	}

	delete window;

	return 0;
}