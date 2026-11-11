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
        Entity* e = &s.getSceneObjectByIndex(i);
        for(int i=0;i<e->getInstancesCount();i++)
        {
            updateRenderable(*e, i, s.getSceneCamera(), triangles, projection, width, height);
        }

		rasterize(s.getSceneObjectByIndex(i));
	}
}

void Renderer::clearDepthBuffer() {
    for (int i = 0; i < width * height; i++)
        depthbuffer[i] = 300.f;//for now checking against hardcoded huge number
}

void calculateUV(float& u, float& v, Vertex t[], const Vec<float, 3>& weights) {
    float uInter = weights.x * (t[0].vt.x / t[0].v.w) + weights.y * (t[1].vt.x / t[1].v.w) + weights.z * (t[2].vt.x / t[2].v.w);
    float vInter = weights.x * (t[0].vt.y / t[0].v.w) + weights.y * (t[1].vt.y / t[1].v.w) + weights.z * (t[2].vt.y / t[2].v.w);

    float wInter = weights.x * (1 / t[0].v.w) + weights.y * (1 / t[1].v.w) + weights.z * (1 / t[2].v.w);
    /*
    float uInter = weights.x * (t[0].vt.x / t[0].v.z) + weights.y * (t[1].vt.x / t[1].v.z) + weights.z * (t[2].vt.x / t[2].v.z);
    float vInter = weights.x * (t[0].vt.y / t[0].v.z) + weights.y * (t[1].vt.y / t[1].v.z) + weights.z * (t[2].vt.y / t[2].v.z);

    float zInter = weights.x * (1 / t[0].v.z) + weights.y * (1 / t[1].v.z) + weights.z * (1 / t[2].v.z);
    */
    u = uInter / wInter;
    v = 1.0f - ( vInter / wInter);
}

void Renderer::printTriangle(Vertex triangle[], uint32_t colour, Material* mat) {

    coordinateBlock c = getRBlock(triangle);

    int startY = std::max(0, c.minY);
    int endY = std::min(height, c.maxY);
    int startX = std::max(0, c.minX);
    int endX = std::min(width, c.maxX);

    Vec<float, 3> weights;

    for (int i = startY; i < endY; i++) {
        for (int j = startX; j < endX; j++) {

            if (pointInTriangle(triangle[0].v, triangle[1].v, triangle[2].v, { j + 0.5f,i + 0.5f }, weights)) {

                float depth = dot({ triangle[0].v.z, triangle[1].v.z, triangle[2].v.z }, weights);


                if (depth < depthbuffer[width * i + j]) {
                    uint32_t pixelcolour = colour;

                    float u, v;
                    calculateUV(u, v, triangle, weights);

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
        if (isClockwise(triangles[i].data()[0].v, triangles[i].data()[1].v, triangles[i].data()[2].v))
        {

            printTriangle(triangles[i].data(), colours[i % 2], mat);
        }

    }

    triangles.clear();
}
