#pragma once
#include "Entity.h"
#define RED 0xff0011

class Material
{
public:
	Material(uint32_t* t, int w, int h);
	~Material();
	uint32_t returnColourAtUV(const int& u, const int& v);
	
private:
	int width, height;
	uint32_t* texture;
};

