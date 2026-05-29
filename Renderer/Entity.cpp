#include "Entity.h"

Entity::Entity()
{
}

Entity::~Entity()
{
}

void Entity::setVertices(const std::vector<Vec<float, 3>> vertices)
{
	this->vertices = vertices;
}

void Entity::setFaces(const std::vector<Vec<int, 3>> faces)
{
	this->faces = faces;
}

void Entity::setColours(const std::vector<uint32_t> colours)
{
	this->colours = colours;
}

std::vector<uint32_t> Entity::getColours()
{
	return this->colours;
}

int Entity::getVerticesCount()
{
	return this->vertices.size();
}

int Entity::getFacesCount()
{
	return this->faces.size();
}

Vec<float, 3> Entity::getVertexByIndex(int i)
{
	return vertices[i];
}

Vec<int, 3> Entity::getFaceByIndex(int i)
{
	return faces[i];
}
