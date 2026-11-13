#pragma once
#include <fstream>
#include <sstream>
#include <string>
#include "Entity.h"
#include "Camera.h"
#include "objReader.h"
#include "mathfunctions.h"

class Scene
{
public:
	Scene();
	~Scene();
	void loadSceneFromFile(const std::string name);
	Entity& getSceneObjectByIndex(const int i);
	Camera& getSceneCamera();
	void rotateLightDir(const float x, const float z);
	const Vec<float, 3>& getLightDir();
	int getNumberOfObjectsContained();
private:
	std::vector <Entity*> objects;
	Camera camera;
	Vec<float, 3> lightDir;
};

