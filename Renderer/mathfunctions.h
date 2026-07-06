#pragma once
#define _USE_MATH_DEFINES

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>
#include <iostream>


#define MAT_SIZE 16
#define PLANES_NUMBER 4


template<typename T, int N>
struct Vec;

template<typename T>
struct Vec<T, 2>
{
    T x, y;

    Vec() : x(0), y(0) {}
    Vec(T x, T y) : x(x), y(y) {}

    T& operator[](int i) {
        return (&x)[i];
    }

    const T& operator[](int i) const {
        return (&x)[i];
    }
};

template<typename T>
struct Vec<T, 3>
{
    T x, y, z;

    Vec() : x(0), y(0), z(0) {}
    Vec(T x, T y, T z) : x(x), y(y), z(z) {}
    Vec(const Vec<T, 4>& v) : x(v.x), y(v.y), z(v.z) {}

    T& operator[](int i) {
        return (&x)[i];
    }

    const T& operator[](int i) const {
        return (&x)[i];
    }
};

template<typename T>
struct Vec<T, 4>
{
    T x, y, z, w;

    Vec() : x(0), y(0), z(0), w(0) {}
    Vec(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
    Vec(const Vec<T, 3>& v, T w) : x(v.x), y(v.y), z(v.z), w(w) {}

    T& operator[](int i) {
        return (&x)[i];
    }

    const T& operator[](int i) const {
        return (&x)[i];
    }
};

template<typename T, int N>
Vec<T, N> operator+(const Vec<T, N>& a, const Vec<T, N>& b)
{
    Vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = a[i] + b[i];
    return result;
}

template<typename T, int N>
Vec<T, N> operator-(const Vec<T, N>& a, const Vec<T, N>& b)
{
    Vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = a[i] - b[i];
    return result;
}

template<typename T, int N>
Vec<T, N> operator*(const Vec<T, N>& v, T s)//scalar mult
{
    Vec<T, N> result;
    for (int i = 0; i < N; i++)
        result[i] = v[i] * s;
    return result;
}

template<typename T, int N>
T dot(const Vec<T, N>& a, const Vec<T, N>& b)
{
    T result = 0;
    for (int i = 0; i < N; i++)
        result += a[i] * b[i];

    return result;
}

template<typename T>
T cross(const Vec<T, 2>& a, const Vec<T, 2>& b)
{
    return a.x * b.y - a.y * b.x;
}


struct coordinateBlock {
	int minX, minY, maxX, maxY;
};

struct Quaternion {
    float s, x, y, z;
};

struct Mat4 {
    float m[MAT_SIZE] = {
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    0, 0, 0, 1
    };

    Mat4 operator*(const Mat4& rhs) const {
        Mat4 result;

        result.m[0] = m[0] * rhs.m[0] + m[1] * rhs.m[4] + m[2] * rhs.m[8] + m[3] * rhs.m[12];
        result.m[1] = m[0] * rhs.m[1] + m[1] * rhs.m[5] + m[2] * rhs.m[9] + m[3] * rhs.m[13];
        result.m[2] = m[0] * rhs.m[2] + m[1] * rhs.m[6] + m[2] * rhs.m[10] + m[3] * rhs.m[14];
        result.m[3] = m[0] * rhs.m[3] + m[1] * rhs.m[7] + m[2] * rhs.m[11] + m[3] * rhs.m[15];

        result.m[4] = m[4] * rhs.m[0] + m[5] * rhs.m[4] + m[6] * rhs.m[8] + m[7] * rhs.m[12];
        result.m[5] = m[4] * rhs.m[1] + m[5] * rhs.m[5] + m[6] * rhs.m[9] + m[7] * rhs.m[13];
        result.m[6] = m[4] * rhs.m[2] + m[5] * rhs.m[6] + m[6] * rhs.m[10] + m[7] * rhs.m[14];
        result.m[7] = m[4] * rhs.m[3] + m[5] * rhs.m[7] + m[6] * rhs.m[11] + m[7] * rhs.m[15];

        result.m[8] = m[8] * rhs.m[0] + m[9] * rhs.m[4] + m[10] * rhs.m[8] + m[11] * rhs.m[12];
        result.m[9] = m[8] * rhs.m[1] + m[9] * rhs.m[5] + m[10] * rhs.m[9] + m[11] * rhs.m[13];
        result.m[10] = m[8] * rhs.m[2] + m[9] * rhs.m[6] + m[10] * rhs.m[10] + m[11] * rhs.m[14];
        result.m[11] = m[8] * rhs.m[3] + m[9] * rhs.m[7] + m[10] * rhs.m[11] + m[11] * rhs.m[15];

        result.m[12] = m[12] * rhs.m[0] + m[13] * rhs.m[4] + m[14] * rhs.m[8] + m[15] * rhs.m[12];
        result.m[13] = m[12] * rhs.m[1] + m[13] * rhs.m[5] + m[14] * rhs.m[9] + m[15] * rhs.m[13];
        result.m[14] = m[12] * rhs.m[2] + m[13] * rhs.m[6] + m[14] * rhs.m[10] + m[15] * rhs.m[14];
        result.m[15] = m[12] * rhs.m[3] + m[13] * rhs.m[7] + m[14] * rhs.m[11] + m[15] * rhs.m[15];

        return result;
    }
};

enum ClipPlane {
    Left,
    Right,
    Bottom,
    Top,
    Near
};

class Entity;
class Camera;

bool pointInTriangle(const Vec<float,3>&  a, const Vec<float, 3>& b, const Vec<float, 3>& c, const Vec<float, 2>& p, Vec<float, 3>& w);

bool isClockwise(const Vec<float, 3>& a, const Vec<float, 3>& b, const Vec<float, 3>& c);

coordinateBlock getRBlock(Vec<float, 3> t[]);//rasterization block

void updateRenderable(Entity& object, Camera camera, std::vector<std::array<Vec<float, 3>, 3>>& triangles, Mat4& projection, int width, int height);

void normalizeQuaternion(Quaternion& q);

Quaternion axisAngle(Vec<float, 3> p, float angle);

Vec<float, 4> multiply(const Mat4& M, const Vec<float, 4>& v);

Quaternion multiply(const Quaternion& a, const Quaternion& b);

Vec<float, 3> rotateVector(Quaternion q, Vec<float, 3> v);