#include "Entity.h"

Entity::Entity(Material* material)
{
	this->material = material;
}

Entity::~Entity()
{
}

void Entity::setMesh(const std::vector < Vec <int, 3> >& faces, const std::vector < Vec<float, 3> >& vertices, const std::vector < Vec <int, 3> >& vtfaces, const std::vector < Vec<float, 2> >& vts)
{
	this->mesh.v = vertices;
	this->mesh.f = faces;
	this->mesh.vt = vts;
	this->mesh.fvt = vtfaces;
}

int Entity::getVerticesCount()
{
	return this->mesh.v.size();
}

int Entity::getFacesCount()
{
	return this->mesh.f.size();
}

int Entity::getInstancesCount()
{
	return instances.size();
}

void Entity::addInstance(Vec<float, 3> position)
{
	Transform transform;
	transform.position = position;
	transform.scale = { 1,1,1 };
	transform.rotation = { 1, 0, 0, 0 };
	instances.push_back(transform);
}

Transform& Entity::getInstanceTransformByIndex(int i)
{
	return this->instances[i];
}


Vec <float, 3> Entity::getVertexByIndex(int i)
{
	return mesh.v[i];
}

Vec<float, 2> Entity::getVtByIndex(int i)
{
	return mesh.vt[i];
}

Vec<int, 3> Entity::getFaceByIndex(int i)
{
	return mesh.f[i];
}

Vec<int, 3> Entity::getVtFaceByIndex(int i)
{
	return mesh.fvt[i];
}

Material* Entity::getMaterial()
{
	return this->material;
}

void Entity::moveInstance(Vec<float, 3> offset, int i)
{
	instances[i].position = instances[i].position + offset;
}

void Entity::rotateInstance(Vec<float, 3> d, int i)
{
	Quaternion qx = axisAngle({ 1, 0, 0 }, d.x);
	Quaternion qy = axisAngle({ 0, 1, 0 }, d.y);
	Quaternion qz = axisAngle({ 0, 0, 1 }, d.z);

	Quaternion delta = multiply(qz, multiply(qy, qx));

	instances[i].rotation = multiply(delta, instances[i].rotation);
	normalizeQuaternion(instances[i].rotation);
}

void Entity::setInstanceScale(Vec<float, 3> d, int i)
{
	instances[i].scale = d;
}

Mat4 Entity::getInstanceModelMatrix(int i)
{
	return getInstanceTranslationMatrix(i) * getInstanceRotationMatrix(i) * getInstanceScaleMatrix(i);
}

Mat4 Entity::getInstanceRotationMatrix(int i) {
	Quaternion q = instances[i].rotation;
	return {
		1 - 2 * q.y * q.y - 2 * q.z * q.z,         2 * q.x * q.y - 2 * q.s * q.z,                   2 * q.x * q.z + 2 * q.s * q.y,                   0,
		2 * q.x * q.y + 2 * q.s * q.z,              1 - 2 * q.x * q.x - 2 * q.z * q.z,             2 * q.y * q.z - 2 * q.s * q.x,                    0,
		2 * q.x * q.z - 2 * q.s * q.y,              2 * q.y * q.z + 2 * q.s * q.x,                   1 - 2 * q.x * q.x - 2 * q.y * q.y,              0,
		0,                                                    0,                                                         0,                                                          1
	};
}

Mat4 Entity::getInstanceTranslationMatrix(int i)
{
	Mat4 t;
	t.m[T_X] = instances[i].position.x;
	t.m[T_Y] = instances[i].position.y;
	t.m[T_Z] = instances[i].position.z;
	return t;
}

Mat4 Entity::getInstanceScaleMatrix(int i)
{
	Mat4 s = {
		instances[i].scale.x, 0, 0, 0,
		0, instances[i].scale.y, 0, 0,
		0, 0, instances[i].scale.z, 0,
		0, 0, 0, 1
	};
	return s;
}

