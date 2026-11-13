#include "Renderer.h"


Renderer::Renderer(int width, int height, uint32_t* buffer)
{
	this->width = width;
	this->height = height;
	this->buffer = buffer;
    this->primitivesCount = 0;


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

    primitivesCount = 0;

	for (int i = 0; i < s.getNumberOfObjectsContained(); i++) {
        Entity* e = &s.getSceneObjectByIndex(i);
        for(int i=0;i<e->getInstancesCount();i++)
        {
            updateRenderable(*e, i, s.getSceneCamera(), triangles, projection, width, height);
        }

		rasterize(s.getSceneObjectByIndex(i), s.getLightDir());
	}
}

int Renderer::getPrimitivesCount()
{
    return primitivesCount;
}

void Renderer::clearDepthBuffer() {
    for (int i = 0; i < width * height; i++)
        depthbuffer[i] = 300.f;//for now checking against hardcoded huge number
}

void Renderer::printTriangle(Vertex triangle[], uint32_t colour, Material* mat, const Vec<float, 3>& LightDir) {

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
                    depthbuffer[width * i + j] = depth;
                    //pseudo fragment shading below
                    uint32_t pixelcolour = colour;


                    if (mat != nullptr) {//has a texture
                        float u, v;

                        calculateUV(u, v, triangle, weights);

                        pixelcolour = mat->returnColourAtUV(u, v);
                    }
                    Vec<float, 3> normals;
                    interpolateNormals(normals, triangle, weights);
                    float diffuse = dot(normals, LightDir);
                    if (diffuse < 0) {
                        diffuse = 0.0f;
                    }
                    float intensity = AMBIENT_LIGHT + (1.0f - AMBIENT_LIGHT) * diffuse;
                    applyLighting(pixelcolour, intensity);

                    buffer[width * i + j] = pixelcolour;

                }

            }
        }

    }
}

void Renderer::rasterize(Entity& object, const Vec<float, 3>& LightDir)
{
    uint32_t colours[] = { GREEN, BLUE };
    Material* mat = object.getMaterial();

    for (int i = 0; i < triangles.size(); i++) {

        //backface culling for counterclockwise
        if (isClockwise(triangles[i].data()[0].v, triangles[i].data()[1].v, triangles[i].data()[2].v))
        {
            primitivesCount++;
            printTriangle(triangles[i].data(), colours[i % 2], mat, LightDir);
        }

    }

    triangles.clear();
}
