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

Mat4 Entity::getModelMatrix()
{
	return getTranslationMatrix() * getRotationMatrix() * getScaleMatrix();
}

Mat4 Entity::getRotationMatrix() {
	Quaternion q = transform.rotation;
	return {
		1 - 2 * q.y * q.y - 2 * q.z * q.z,         2 * q.x * q.y - 2 * q.s * q.z,                   2 * q.x * q.z + 2 * q.s * q.y,                   0,
		2 * q.x * q.y + 2 * q.s * q.z,              1 - 2 * q.x * q.x - 2 * q.z * q.z,             2 * q.y * q.z - 2 * q.s * q.x,                    0,
		2 * q.x * q.z - 2 * q.s * q.y,              2 * q.y * q.z + 2 * q.s * q.x,                   1 - 2 * q.x * q.x - 2 * q.y * q.y,              0,
		0,                                                    0,                                                         0,                                                          1
	};
}

Mat4 Entity::getTranslationMatrix()
{
	Mat4 t;
	t.m[T_X] = transform.position.x;
	t.m[T_Y] = transform.position.y;
	t.m[T_Z] = transform.position.z;
	return t;
}

Mat4 Entity::getScaleMatrix()
{
	Mat4 s = {
		transform.scale.x, 0, 0, 0,
		0, transform.scale.y, 0, 0,
		0, 0, transform.scale.z, 0,
		0, 0, 0, 1
	};
	return s;
}

