#include "Renderer.h"


Renderer::Renderer(int width, int height, uint32_t* buffer)
{
	this->width = width;
	this->height = height;
	this->buffer = buffer;

	view.m[T_Z] = -3.f;

	float tangent = std::tan(FOV / 2);
	float right = FRONT * tangent;
	float aspect = (float)width / (float)height;
	float top = right / aspect;

	projection = {
	FRONT / right, 0, 0, 0,
	0, FRONT / top, 0, 0,
	0, 0, -(BACK + FRONT) / (BACK - FRONT), -1,
	0, 0, -(2 * BACK * FRONT) / (BACK - FRONT), 0
	};

	depthbuffer = new float[width * height];

}

Renderer::~Renderer()
{
	delete[] depthbuffer;
}

void Renderer::renderScene(Scene& s)
{
	for (int i = 0; i < s.getNumberOfObjectsContained(); i++) {

		updateRenderable(s.getSceneObjectByIndex(i), triangles, translation, projection, width, height);

		rasterize(s.getSceneObjectByIndex(i));
	}
}

void Renderer::clearDepthBuffer() {
    for (int i = 0; i < width * height; i++)
        depthbuffer[i] = 300.f;//for now checking against huge number
}

void Renderer::printTriangle(Vec<float, 3> triangle[], uint32_t colour) {

    coordinateBlock c = getRBlock(triangle);

    //temp out of bounds guard
    c.minX = std::max(0.f, c.minX);
    c.minY = std::max(0.f, c.minY);

    c.maxX = std::min((float)width, c.maxX);
    c.maxY = std::min((float)height, c.maxY);

    Vec<float, 3> weights;

    for (int i = c.minY; i < c.maxY; i++) {
        for (int j = c.minX; j < c.maxX; j++) {

            if (pointInTriangle(triangle[0], triangle[1], triangle[2], { j + 0.5f,i + 0.5f }, weights)) {

                float depth = dot({ triangle[0].z, triangle[1].z, triangle[2].z }, weights);
                if (depth < depthbuffer[width * i + j]) {
                    buffer[width * i + j] = colour;
                    depthbuffer[width * i + j] = depth;
                }

            }
        }

    }
}

void Renderer::rasterize(Entity& object)
{
    clearDepthBuffer();
    std::vector <uint32_t> colours = object.getColours();

    for (int i = 0; i < object.getFacesCount(); i++) {

        //backface culling for counterclockwise
        if (isClockwise(triangles[i].data()[0], triangles[i].data()[1], triangles[i].data()[2]))
        {
            printTriangle(triangles[i].data(), colours[i]);
        }

    }

    triangles.clear();
}
