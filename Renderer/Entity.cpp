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

std::vector<Vec<float, 3>>* Entity::getVertices()
{
	return &this->vertices;
}

std::vector<Vec<int, 3>>* Entity::getFaces()
{
	return &this->faces;
}

std::vector<uint32_t> Entity::getColours()
{
	return this->colours;
}
