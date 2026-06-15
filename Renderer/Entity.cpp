#include "Entity.h"

Entity::Entity(Vec<float, 3> position)
{
	transform.position = position;
	transform.scale = { 1,1,1 };
	transform.rotation = { 1, 0, 0, 0 };
}

Entity::~Entity()
{
}

void Entity::setVertices(const std::vector<Vec<float, 3>> vertices)
{
	this->mesh.vertices = vertices;
}

void Entity::setFaces(const std::vector<Vec<int, 3>> faces)
{
	this->mesh.faces = faces;
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
	return this->mesh.vertices.size();
}

int Entity::getFacesCount()
{
	return this->mesh.faces.size();
}

Transform& Entity::getTransform()
{
	return this->transform;
}

Vec<float, 3> Entity::getVertexByIndex(int i)
{
	return mesh.vertices[i];
}

Vec<int, 3> Entity::getFaceByIndex(int i)
{
	return mesh.faces[i];
}

void Entity::move(Vec<float, 3> offset)
{
	transform.position = transform.position + offset;
}

void Entity::rotate(Vec<float, 3> d)
{
	Quaternion qx = axisAngle({ 1, 0, 0 }, d.x);
	Quaternion qy = axisAngle({ 0, 1, 0 }, d.y);
	Quaternion qz = axisAngle({ 0, 0, 1 }, d.z);

	Quaternion delta = multiply(qz, multiply(qy, qx));

	transform.rotation = multiply(delta, transform.rotation);
	normalizeQuaternion(transform.rotation);
}