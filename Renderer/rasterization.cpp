#include "rasterization.h"

uint32_t getNextColor()
{
    static int c = -1;
    c++;
    if (c >= NUMBER_OF_COLORS-1) {// -1 bc not taking black into account
        c = 0;
    }

    switch (c) {
    case 0:
        return GREEN;
        break;
    case 1:
        return BLUE;
        break;
    case 2:
        return DBLUE;
        break;
    case 3:
        return PURPLE;
        break;
    case 4:
        return PINK;
        break;
    case 5:
        return RED;
        break;
    case 6:
        return YELLOW;
        break;
    case 7:
        return ORANGE;
        break;
    }
}

void clearDepthBuffer(float depthbuffer[], int width, int height) {
    for (int i = 0; i < width * height; i++)
        depthbuffer[i] = 300.f;//for now checking against huge number
}

void printTriangle (Vec<float, 3> triangle[], uint32_t buffer[], float depthbuffer[], int width, int height, uint32_t colour) {

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

                float depth = dot({ triangle[0].z, triangle[1].z, triangle[2].z },weights );
                if (depth < depthbuffer[width * i + j]) {
                    buffer[width * i + j] = colour;
                    depthbuffer[width * i + j] = depth;
                }
                
            }
        }

    }
}

void rasterize(Entity& object, std::vector<std::array<Vec<float, 3>, 3>>& triangles, uint32_t buffer[], float depthbuffer[], int width, int height)
{
    clearDepthBuffer(depthbuffer, width, height);
    std::vector <uint32_t> colours = object.getColours();

    for (int i = 0; i < object.getFacesCount(); i++) {

        //backface culling for counterclockwise
        if (isClockwise(triangles[i].data()[0], triangles[i].data()[1], triangles[i].data()[2]))
        {
            printTriangle(triangles[i].data(), buffer, depthbuffer, width, height, colours[i]);
        }

    }

    triangles.clear();
}

