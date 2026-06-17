#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Entity.h"
#include "Camera.h"
#include "objReader.h"

class Scene
{
public:
	Scene();
	~Scene();
	void loadSceneFromFile(const std::string name);
	Entity& getSceneObjectByIndex(const int i);
	Camera& getSceneCamera();
	int getNumberOfObjectsContained();
private:
	std::vector <Entity*> objects;
	Camera camera;
};

