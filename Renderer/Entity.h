#pragma once
#include <vector>
#include "mathfunctions.h"
#include "rasterization.h"

class Entity
{
public:
	Entity();
	~Entity();
	void setVertices(const std::vector < Vec <float, 3> > vertices);
	void setFaces(const std::vector < Vec <int, 3> > faces);
	void setColours(const std::vector <uint32_t> colours);
	std::vector <uint32_t> getColours();
	int getVerticesCount();
	int getFacesCount();
	Vec <float, 3> getVertexByIndex(int i);
	Vec <int, 3> getFaceByIndex(int i);
private:
	std::vector < Vec <float, 3> > vertices;
	std::vector < Vec <int, 3> > faces;
	std::vector <uint32_t> colours;
};

