#pragma once
#define _USE_MATH_DEFINES

#include "Scene.h"

#define FOV 45.f * (M_PI / 180.0)
#define FRONT 1.f
#define BACK 100.f


class Renderer
{
public:

	Renderer(int width, int height, uint32_t* buffer);
	~Renderer();
	void renderScene(Scene& s);
private:
	void rasterize(Entity& object);
	void clearDepthBuffer();
	void printTriangle(Vec<float, 3> triangle[], uint32_t colour);


	std::vector<std::array<Vec<float, 3>, 3>> triangles;

	Mat4 translation;

	Mat4 view;
	Mat4 projection;

	float* depthbuffer;
	uint32_t* buffer;

	int width;
	int height;

};

