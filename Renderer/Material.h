#pragma once
#include "Entity.h"
#define RED 0xff0011

class Material
{
public:
	Material(uint32_t* t, int w, int h);
	~Material();
	uint32_t returnColourAtUV(const float& u, const float& v);
	
private:
	int width, height;
	uint32_t* texture;
};

