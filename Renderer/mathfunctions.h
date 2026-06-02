#pragma once
#define _USE_MATH_DEFINES

#include <algorithm>
#include <array>
#include <cmath>
#include <vector>
#include <iostream>

#define FOV 45.f * (M_PI / 180.0)
#define FRONT 1.f
#define BACK 100.f

#define MAT_SIZE 16
#define T_X 3
#define T_Y 7
#define T_Z 11

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
	float minX, minY, maxX, maxY;
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
};

class Entity;

bool pointInTriangle(const Vec<float,2>&  a, const Vec<float, 2>& b, const Vec<float, 2>& c, const Vec<float, 2>& p);

bool isClockwise(const Vec<float, 2>& a, const Vec<float, 2>& b, const Vec<float, 2>& c);

coordinateBlock getRBlock(Vec<float, 2> t[]);//rasterization block

void updateRenderable(Entity& object, std::vector<std::array<Vec<float, 2>, 3>>& triangles, Mat4& translation, Quaternion q, Vec<float, 3> offset, Mat4& projection, int width, int height);

void updateQuaternion(Quaternion & orientation, Vec<float, 3> d);
