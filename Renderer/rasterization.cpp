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

void printTriangle (Vec<float, 2> triangle[], uint32_t buffer[], int width, int height, uint32_t colour) {

    coordinateBlock c = getRBlock(triangle);

    //temp out of bounds guard
    c.minX = std::max(0.f, c.minX);
    c.minY = std::max(0.f, c.minY);

    c.maxX = std::min((float)width, c.maxX);
    c.maxY = std::min((float)height, c.maxY);

    for (int i = c.minY; i < c.maxY; i++) {
        for (int j = c.minX; j < c.maxX; j++) {

            if (pointInTriangle(triangle[0], triangle[1], triangle[2], { j + 0.5f,i + 0.5f })) {

                buffer[width * i + j] = colour;
            }
        }

    }
}

void rasterize(Entity& object, std::vector<std::array<Vec<float, 2>, 3>>& triangles, uint32_t buffer[], int width, int height)
{
    std::vector <uint32_t> colours = object.getColours();

    for (int i = 0; i < object.getFacesCount(); i++) {
        printTriangle(triangles[i].data(), buffer, width, height, colours[i]);
    }

    triangles.clear();
}
