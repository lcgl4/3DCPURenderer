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

Window* g_WindowInstance = nullptr;


int main() {

	Scene scene;
	scene.loadSceneFromFile("scene.txt");
	Scene scene2;
	scene2.loadSceneFromFile("scene2.txt");
	Scene* currentScene = &scene;

	float sceneSwitchLock = 1.f;


	Window* window = new Window(W_WIDTH, W_HEIGHT);
	Window* g_WindowInstance = window;
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

		if (sceneSwitchLock > 0) {
			sceneSwitchLock -= delta;
		}

		float zMove = 2.5 * delta;

		float angle = 0.2 * M_PI * delta;

		if (!window->processMessages()) {
			running = false;
		}


		if (window->isKeyPressed(VK_ESCAPE)) {
			running = false;
		}
		if (window->isKeyPressed('W')) {
			currentScene->getSceneCamera().move({ 0, 0, -zMove });
		}
		if (window->isKeyPressed('S')) {
			currentScene->getSceneCamera().move({ 0, 0, zMove });
		}
		if (window->isKeyPressed('A')) {
			currentScene->getSceneCamera().move({ -zMove, 0, 0 });
		}
		if (window->isKeyPressed('D')) {
			currentScene->getSceneCamera().move({ zMove, 0, 0 });
		}
		if (window->isKeyPressed('Q')) {
			currentScene->getSceneCamera().rotate({ 0, angle, 0 });
		}
		if (window->isKeyPressed('E')) {
			currentScene->getSceneCamera().rotate({ 0, -angle, 0 });
		}
		if (window->isKeyPressed('H')) {
			if(sceneSwitchLock <=0)
			{
				if (currentScene == &scene) {
					currentScene = &scene2;
				}
				else {
					currentScene = &scene;
				}
				sceneSwitchLock = 1.f;
			}
		}
		if (window->isKeyPressed(VK_SPACE)) {
			currentScene->getSceneCamera().move({ 0, zMove, 0 });
		}
		if (window->isKeyPressed(VK_SHIFT)) {
			currentScene->getSceneCamera().move({ 0, -zMove, 0 });
		}

		window->clearScreen();

		//example how to manipulate an object
		//scene.getSceneObjectByIndex(0).rotate({ angle * 3, angle, angle / 2 });


		renderer.renderScene(*currentScene);

		window->render();
	}

	delete window;

	return 0;
}