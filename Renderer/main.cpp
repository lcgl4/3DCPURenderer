#define _USE_MATH_DEFINES

#include <iostream>
#include "Window.h"
#include "Entity.h"
#include "objReader.h"
#include <chrono>
#include "Scene.h"
#include "Renderer.h"

#define W_WIDTH 1366
#define W_HEIGHT 768


int main() {

	Scene scene;
	scene.loadSceneFromFile("scene.txt");


	Window* window = new Window(W_WIDTH, W_HEIGHT);
	int width = window->getWidth();
	int height = window->getHeight();

	Renderer renderer(width, height, window->getBuffer());

	using clock = std::chrono::high_resolution_clock;

	auto last = clock::now();

	bool running = true;
	while (running) {

		auto now = clock::now();

		float delta = std::chrono::duration<float>(now - last	).count();

		last = now;

		float zMove = 0.5 * delta;

		float angle = 0.1 * M_PI * delta;

		if (!window->processMessages()) {
			running = false;
		}

		window->clearScreen();

		//scene.getSceneObjectByIndex(0).move({ 0,0, zMove });

		scene.getSceneObjectByIndex(0).rotate({ angle * 3, angle, angle / 2 });

		//scene.getSceneCamera().rotate({ 0, angle, 0 });


		renderer.renderScene(scene);

		window->render();
	}

	delete window;

	return 0;
}