#pragma once
#include "mathfunctions.h"

struct cameraTransform
{
	Vec<float, 3> position;
	Quaternion rotation;
};

class Camera
{
public:
	Camera();
	~Camera();
	cameraTransform& getTransform();
	Mat4 getViewMatrix();
	void move(Vec<float, 3> offset);
	void rotate(Vec<float, 3> d);
private:
	cameraTransform transform;
};

