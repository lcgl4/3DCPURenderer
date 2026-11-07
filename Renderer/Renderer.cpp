#include "Renderer.h"


Renderer::Renderer(int width, int height, uint32_t* buffer)
{
	this->width = width;
	this->height = height;
	this->buffer = buffer;


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
    clearDepthBuffer();
	for (int i = 0; i < s.getNumberOfObjectsContained(); i++) {

		updateRenderable(s.getSceneObjectByIndex(i), s.getSceneCamera(), triangles, projection, width, height);

		rasterize(s.getSceneObjectByIndex(i));
	}
}

void Renderer::clearDepthBuffer() {
    for (int i = 0; i < width * height; i++)
        depthbuffer[i] = 300.f;//for now checking against hardcoded huge number
}

//move to mathfunctions
void calculateUV(int& u, int& v, Vec<float, 3> triangle[], const Vec<float, 3>& weights, Vec<float, 2> vt[]) {
    float uInter = 0;
}

void Renderer::printTriangle(Vec<float, 3> triangle[], uint32_t colour, Material* mat, Vec<float, 2> vt[]) {

    coordinateBlock c = getRBlock(triangle);

    int startY = std::max(0, c.minY);
    int endY = std::min(height, c.maxY);
    int startX = std::max(0, c.minX);
    int endX = std::min(width, c.maxX);

    Vec<float, 3> weights;

    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {

            if (pointInTriangle(triangle[0], triangle[1], triangle[2], { j + 0.5f,i + 0.5f }, weights)) {

                float depth = dot({ triangle[0].z, triangle[1].z, triangle[2].z }, weights);


                if (depth < depthbuffer[width * i + j]) {
                    uint32_t pixelcolour = colour;
                    int u, v;
                    calculateUV(u, v, triangle, weights, vt);
                    if (mat != nullptr) {//has a texture
                        pixelcolour = mat->returnColourAtUV(u, v);
                    }
                    buffer[width * i + j] = pixelcolour;

                    depthbuffer[width * i + j] = depth;
                }

            }
        }

    }
}

void Renderer::rasterize(Entity& object)
{
    uint32_t colours[] = { GREEN, BLUE };
    Material* mat = object.getMaterial();

    for (int i = 0; i < triangles.size(); i++) {

        //backface culling for counterclockwise
        if (isClockwise(triangles[i].data()[0], triangles[i].data()[1], triangles[i].data()[2]))
        {

            ///to jest zle bo te tez musisz clippowac w update renderable
            Vec<float, 2> vt[] = { object.getVtByIndex(object.getVtFaceByIndex(i)[0]), object.getVtByIndex(object.getVtFaceByIndex(i)[1]) , object.getVtByIndex(object.getVtFaceByIndex(i)[2]) };

            printTriangle(triangles[i].data(), colours[i % 2], mat, vt);
        }

    }

    triangles.clear();
}
