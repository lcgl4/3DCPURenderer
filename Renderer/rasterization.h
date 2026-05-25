#pragma once
#include <cstdint>
#include "mathfunctions.h"

#define BLACK 0x000000
#define GREEN 0x00ff26
#define BLUE 0x00fbff
#define DBLUE 0x0055ff
#define PURPLE 0xc300ff
#define PINK 0xff00d9
#define RED 0xff0011
#define YELLOW 0xff0011
#define ORANGE 0xff6f00


#define NUMBER_OF_COLORS 9

uint32_t getNextColor();

void rasterize(Vec<float, 2> triangle[], uint32_t buffer[], int width, int height, uint32_t colour);


