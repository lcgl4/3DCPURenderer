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
	Vec<float, 3> vn;
	Vertex(Vec<float, 4> iv, Vec<float, 2> ivt, Vec<float, 3> ivn) : v(iv), vt(ivt), vn(ivn) {}
};

struct Mesh
{
	std::vector<Vec<float, 3>> v;
	std::vector<Vec<float, 2>> vt;
	std::vector<Vec<float, 3>> vn;

	std::vector<Vec<int, 3>> f;
	std::vector<Vec<int, 3>> fvt;
	std::vector<Vec<int, 3>> fvn;
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
	Entity(Material* material);
	~Entity();
	void setMesh(const std::vector < Vec <int, 3> >& faces, const std::vector < Vec<float, 3> >& vertices, const std::vector < Vec <int, 3> >& vtfaces, const std::vector < Vec<float, 2> >& vts, const std::vector < Vec <int, 3> >& vnfaces, const std::vector < Vec<float, 3> >& vns);
	int getVerticesCount();
	int getFacesCount();
	int getInstancesCount();
	void addInstance(Vec<float, 3> position);
	Transform& getInstanceTransformByIndex(int i);
	Vec <float, 3> getVertexByIndex(int i);
	Vec <float, 2> getVtByIndex(int i);
	Vec <float, 3> getVnByIndex(int i);
	Vec <int, 3> getFaceByIndex(int i);
	Vec <int, 3> getVtFaceByIndex(int i);
	Vec <int, 3> getVnFaceByIndex(int i);
	Material* getMaterial();
	void moveInstance(Vec<float, 3> offset, int i);
	void rotateInstance(Vec<float, 3> d, int i);
	void setInstanceScale(Vec<float, 3> d, int i);
	Mat4 getInstanceModelMatrix(int i);
private:
	Mat4 getInstanceRotationMatrix(int i);
	Mat4 getInstanceTranslationMatrix(int i);
	Mat4 getInstanceScaleMatrix(int i);

	Material* material;
	Mesh mesh;
	std::vector<Transform> instances;
};

