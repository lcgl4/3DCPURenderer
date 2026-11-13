#pragma once
#define _USE_MATH_DEFINES

#include "Scene.h"
#include <algorithm>

#define FOV 45.f * (M_PI / 180.0)
#define FRONT 1.f
#define BACK 100.f


class Renderer
{
public:

	Renderer(int width, int height, uint32_t* buffer);
	~Renderer();
	void renderScene(Scene& s);
	int getPrimitivesCount();
private:
	void rasterize(Entity& object);
	void clearDepthBuffer();
	void printTriangle(Vertex triangle[], uint32_t colour, Material* mat);


	std::vector<std::array<Vertex, 3>> triangles;
	int primitivesCount;

	Mat4 projection;

	float* depthbuffer;
	uint32_t* buffer;

	int width;
	int height;

};

