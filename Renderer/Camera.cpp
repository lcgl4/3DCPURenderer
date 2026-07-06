#include "Camera.h"

Camera::Camera()
{
	transform.position = {0, 0, -4.f};
	transform.rotation = { 1, 0, 0, 0 };
}

Camera::~Camera()
{
}

cameraTransform& Camera::getTransform() {
	return this->transform;
}

Mat4 Camera::getViewMatrix() {

    float s = transform.rotation.s;
    float x = transform.rotation.x;
    float y = transform.rotation.y;
    float z = transform.rotation.z;

    float xx = x * x; float xy = x * y; float xz = x * z; float xs = x * s;
    float yy = y * y; float yz = y * z; float ys = y * s;
    float zz = z * z; float zs = z * s;

    float r00 = 1.0f - 2.0f * (yy + zz);
    float r10 = 2.0f * (xy - zs);
    float r20 = 2.0f * (xz + ys);

    float r01 = 2.0f * (xy + zs);
    float r11 = 1.0f - 2.0f * (xx + zz);
    float r21 = 2.0f * (yz - xs);

    float r02 = 2.0f * (xz - ys);
    float r12 = 2.0f * (yz + xs);
    float r22 = 1.0f - 2.0f * (xx + yy);

    float px = transform.position.x;
    float py = transform.position.y;
    float pz = transform.position.z;

    Mat4 view;

    view.m[0] = r00;
    view.m[1] = r01;
    view.m[2] = r02;
    view.m[3] = -(r00 * px + r01 * py + r02 * pz);

    view.m[4] = r10;
    view.m[5] = r11;
    view.m[6] = r12;
    view.m[7] = -(r10 * px + r11 * py + r12 * pz);

    view.m[8] = r20;
    view.m[9] = r21;
    view.m[10] = r22;
    view.m[11] = -(r20 * px + r21 * py + r22 * pz);

    view.m[12] = 0.0f;
    view.m[13] = 0.0f;
    view.m[14] = 0.0f;
    view.m[15] = 1.0f;

    return view;
}

void Camera::move(Vec<float, 3> offset) {
    Vec<float, 3> worldOffset = rotateVector( transform.rotation, offset );

    transform.position = transform.position + worldOffset;
}

void Camera::rotate(Vec<float, 3> d) {
	Quaternion qx = axisAngle({ 1, 0, 0 }, d.x);
	Quaternion qy = axisAngle({ 0, 1, 0 }, d.y);
	Quaternion qz = axisAngle({ 0, 0, 1 }, d.z);

	Quaternion delta = multiply(qz, multiply(qy, qx));

	transform.rotation = multiply(delta, transform.rotation);
	normalizeQuaternion(transform.rotation);
}