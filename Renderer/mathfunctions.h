#pragma once
#include <algorithm>
#include <cmath>

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

struct coordinateBlock {
	float minX, minY, maxX, maxY;
};

bool pointInTriangle(const Vec<float,2>  a, const Vec<float, 2> b, const Vec<float, 2> c, const Vec<float, 2> p);

coordinateBlock getRBlock(Vec<float, 2> t[]);//rasterization block

void translateZ(Vec<float, 3>& p, float offset);

void translateCoordinates(Vec<float, 2>& c, int wWidth, int wHeight);

Vec<float, 2> project(Vec<float, 3>& c);

Vec<float, 2> updatePoint(Vec<float, 3>& p, float offset, int wWidth, int wHeight, float angleX, float angleY, float angleZ);
