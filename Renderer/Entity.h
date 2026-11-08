#pragma once
#include <vector>
#include "mathfunctions.h"

//for translation matrix
#define T_X 3
#define T_Y 7
#define T_Z 11

class Material;

struct Vertex {
	Vec<float, 4> v;
	Vec<float, 2> vt;

	Vertex(Vec<float, 4> iv, Vec<float, 2> ivt) : v(iv), vt(ivt) {}
};

struct Mesh
{
	std::vector<Vec<float, 3>> v;
	std::vector<Vec<float, 2>> vt;
	std::vector<Vec<int, 3>> f;
	std::vector<Vec<int, 3>> fvt;
};

struct Transform
{
	Vec<float, 3> position;
	Quaternion rotation;
	Vec<float, 3> scale;
};

class Entity
{
public:
	Entity(Vec<float, 3> position, Material* material);
	~Entity();
	void setMesh(const std::vector < Vec <int, 3> >& faces, const std::vector < Vec<float, 3> >& vertices, const std::vector < Vec <int, 3> >& vtfaces, const std::vector < Vec<float, 2> >& vts);
	int getVerticesCount();
	int getFacesCount();
	Transform& getTransform();
	Vec <float, 3> getVertexByIndex(int i);
	Vec <float, 2> getVtByIndex(int i);
	Vec <int, 3> getFaceByIndex(int i);
	Vec <int, 3> getVtFaceByIndex(int i);
	Material* getMaterial();
	void move(Vec<float, 3> offset);
	void rotate(Vec<float, 3> d);
	Mat4 getModelMatrix();
private:
	Mat4 getRotationMatrix();
	Mat4 getTranslationMatrix();
	Mat4 getScaleMatrix();

	Material* material;
	Mesh mesh;
	Transform transform;
};

