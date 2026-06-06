#include "mathfunctions.h"
#include "Entity.h"


bool pointInTriangle(const Vec<float, 3>& a, const Vec<float, 3>& b, const Vec<float, 3>& c, const Vec<float, 2>& p, Vec<float, 3>& w)
{
    Vec<float, 2> ab =  {b.x - a.x, b.y - a.y };
    Vec<float, 2> ac = { c.x - a.x, c.y - a.y };
    Vec<float, 2> ap = { p.x - a.x, p.y - a.y };

    float d00 = dot(ab, ab);
    float d01 = dot(ab, ac);
    float d11 = dot(ac, ac);
    float d20 = dot(ap, ab);
    float d21 = dot(ap, ac);

    float denom = d00 * d11 - d01 * d01;

    float u = (d11 * d20 - d01 * d21) / denom;
    float v = (d00 * d21 - d01 * d20) / denom;

    w.x = 1.f - u - v;//a 
    w.y = u;//b
    w.z = v;//c

    return (u >= 0.0) && (v >= 0.0) && (u + v <= 1.0);
}

bool isClockwise(const Vec<float, 3>& a, const Vec<float, 3>& b, const Vec<float, 3>& c) {

    Vec<float, 2> ab = { b.x - a.x, b.y - a.y };
    Vec<float, 2> ac = { c.x - a.x, c.y - a.y };


    if (cross(ab, ac) < 0.f) {
        return true;
    }
    return false;
}

coordinateBlock getRBlock(Vec<float, 3> t[])
{
    return {
        std::min({t[0].x, t[1].x, t[2].x}),
        std::min({t[0].y, t[1].y, t[2].y}),
        std::max({t[0].x, t[1].x, t[2].x}),
        std::max({t[0].y, t[1].y, t[2].y})
    };
}

Vec<float, 4> multiply(const Mat4& M, const Vec<float, 4>& v)
{
    return {
        M.m[0] * v.x + M.m[1] * v.y + M.m[2] * v.z + M.m[3] * v.w,
        M.m[4] * v.x + M.m[5] * v.y + M.m[6] * v.z + M.m[7] * v.w,
        M.m[8] * v.x + M.m[9] * v.y + M.m[10] * v.z + M.m[11] * v.w,
        M.m[12] * v.x + M.m[13] * v.y + M.m[14] * v.z + M.m[15] * v.w
    };
}

Quaternion multiply(const Quaternion& a, const Quaternion& b)
{
    return {
        a.s * b.s - a.x * b.x - a.y * b.y - a.z * b.z,
        a.s * b.x + a.x * b.s + a.y * b.z - a.z * b.y,
        a.s * b.y - a.x * b.z + a.y * b.s + a.z * b.x,
        a.s * b.z + a.x * b.y - a.y * b.x + a.z * b.s
    };
}

void rotate(Vec<float, 3>& p, Mat4& r) {

    Vec<float, 4> rotated = multiply(r, { p.x, p.y, p.z, 1 });
    p = { rotated.x,rotated.y,rotated.z };
}

void translateCoordinates(Vec<float, 3>& p, Mat4& translation) {
    Vec<float, 4> translated = multiply(translation, { p.x, p.y, p.z, 1 });
    p = { translated.x, translated.y, translated.z };
}

Vec<float, 3> normalizeCoordinates(Vec<float, 3>& c, int width, int height) {
    return Vec<float, 3>((c.x + 1) / 2 * width, (1 - (c.y + 1) / 2) * height, c.z);
}

void project(Vec<float, 3>& c, Mat4& projection) {

    Vec<float, 4> result = multiply(projection, { c.x, c.y, c.z, 1.f });

    c.x = result.x / result.w;
    c.y = result.y / result.w;
    c.z = result.z / result.w;
}

Vec<float, 3> updatePoint(Vec<float, 3> p, Mat4& r, Mat4& translation, Mat4& projection, int width, int height) {

    rotate(p, r);
    translateCoordinates(p, translation);
    project(p, projection);
    return normalizeCoordinates(p, width, height);
}

void updateRenderable(Entity& object, std::vector<std::array<Vec<float, 3>, 3>>& triangles, Mat4& translation, Quaternion q, Vec<float, 3> offset, Mat4& projection, int width, int height)
{

    std::vector <Vec<float, 3>> projected;
    projected.reserve(object.getVerticesCount());
    triangles.reserve(object.getFacesCount());

    translation.m[T_X] = offset.x;
    translation.m[T_Y] = offset.y;
    translation.m[T_Z] = offset.z;

    Mat4 rotation = {
        1 - 2 * q.y * q.y - 2 * q.z * q.z,         2 * q.x * q.y - 2 * q.s * q.z,                   2 * q.x * q.z + 2 * q.s * q.y,                   0,
        2 * q.x * q.y + 2 * q.s * q.z,              1 - 2 * q.x * q.x - 2 * q.z * q.z,             2 * q.y * q.z - 2 * q.s * q.x,                    0,
        2 * q.x * q.z - 2 * q.s * q.y,              2 * q.y * q.z + 2 * q.s * q.x,                   1 - 2 * q.x * q.x - 2 * q.y * q.y,              0,
        0,                                                    0,                                                         0,                                                          1
    };


    for (int i = 0; i < object.getVerticesCount(); i++) {
        projected.push_back(updatePoint(object.getVertexByIndex(i), rotation, translation, projection, width, height));
    }


    for (int i = 0; i < object.getFacesCount(); i++) {
        Vec <int, 3> cFace = object.getFaceByIndex(i);
        std::array<Vec<float, 3>, 3> tri = { projected[cFace.x], projected[cFace.y], projected[cFace.z] };

        triangles.push_back(tri);
    }
}

void normalizeQuaternion(Quaternion& q)
{
    float len = std::sqrt(
        q.s * q.s +
        q.x * q.x +
        q.y * q.y +
        q.z * q.z
    );

    if (len == 0) {
        q = { 1,0,0,0 };
        return;
    }

    q.s /= len;
    q.x /= len;
    q.y /= len;
    q.z /= len;
}

Quaternion axisAngle(Vec<float, 3> p, float angle)
{

    float len = std::sqrt(p.x * p.x + p.y * p.y + p.z * p.z);
    if (len == 0.0f)
        return { 1, 0, 0, 0 };

    float inv = 1.0f / len;
    p.x *= inv;
    p.y *= inv;
    p.z *= inv;

    float half = angle * 0.5f;
    float s = std::sin(half);

    return {
        std::cos(half),
        p.x * s,
        p.y * s,
        p.z * s
    };
}

void updateQuaternion(Quaternion& orientation, Vec<float, 3> d)
{
    Quaternion qx = axisAngle({ 1, 0, 0 }, d.x);
    Quaternion qy = axisAngle({ 0, 1, 0 }, d.y);
    Quaternion qz = axisAngle({ 0, 0, 1 }, d.z);

    Quaternion delta = multiply(qz, multiply(qy, qx));

    orientation = multiply(delta, orientation);
    normalizeQuaternion(orientation);
}