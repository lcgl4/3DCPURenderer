#pragma once
#include <vector>
#include "mathfunctions.h"

struct Mesh
{
	std::vector<Vec<float, 3>> vertices;
	std::vector<Vec<int, 3>> faces;
};

struct Transform
{
	Vec<float, 3> position;
	Quaternion rotation;
	Vec<float, 3> scale;//not scaling now todo
};

class Entity
{
public:
	Entity(Vec<float, 3> position);
	~Entity();
	void setVertices(const std::vector < Vec <float, 3> > vertices);
	void setFaces(const std::vector < Vec <int, 3> > faces);
	void setColours(const std::vector <uint32_t> colours);
	std::vector <uint32_t> getColours();
	int getVerticesCount();
	int getFacesCount();
	Transform& getTransform();
	Vec <float, 3> getVertexByIndex(int i);
	Vec <int, 3> getFaceByIndex(int i);
	void move(Vec<float, 3> offset);
	void rotate(Vec<float, 3> d);
private:
	Mesh mesh;
	Transform transform;
	std::vector <uint32_t> colours;//for no texture triangle coloring
};

