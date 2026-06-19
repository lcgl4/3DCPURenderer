#include "mathfunctions.h"
#include "Entity.h"
#include "Camera.h"


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

void rotate(Vec<float, 4>& vertex, const Mat4& r) {

    vertex = multiply(r, vertex);
}

Vec<float, 3> NDCtoPixels(Vec<float, 4>& vertex, int width, int height) {
    return Vec<float, 3>((vertex.x + 1) / 2 * width, (1 - (vertex.y + 1) / 2) * height, vertex.z);
}

void normalizeCoordinates(Vec<float, 4>& vertex) {

    vertex.x = vertex.x / vertex.w;
    vertex.y = vertex.y / vertex.w;
    vertex.z = vertex.z / vertex.w;
}

int keepVertex(const Vec<float, 4>& t) {
    if (t.x < -1.0f || t.x > 1.0f) {
        return 0;
    }

    if (t.y < -1.0f || t.y > 1.0f) {
        return 0;
    }

    return 1;
}

bool isInside(const Vec<float, 4>& p, ClipPlane plane) {
    switch (plane) {
    case Left:
        return p.x >= -1.0f;
    case Right:
        return p.x <= 1.0f;
    case Bottom:
        return p.y >= -1.0f;
    case Top:
        return p.y <= 1.0f;
    }
    return false;
}

Vec<float, 4> getIntersection(const Vec<float, 4>& s, const Vec<float, 4>& p, ClipPlane plane) {

    float t = 0.0f;
    switch (plane) {
    case Left:
        t = (-1.0f - s.x) / (p.x - s.x);
        break;
    case Right:
        t = (1.0f - s.x) / (p.x - s.x);
        break;
    case Bottom:
        t = (-1.0f - s.y) / (p.y - s.y);
        break;
    case Top:
        t = (1.0f - s.y) / (p.y - s.y);
        break;
    }

    return Vec<float, 4>(
        s.x + t * (p.x - s.x),
        s.y + t * (p.y - s.y),
        s.z + t * (p.z - s.z),
        s.w + t * (p.w - s.w)
    );
}

std::vector<Vec<float, 4>> clipTriangle(const std::vector<Vec<float, 4>>& triangle) {
    std::vector<Vec<float, 4>> outputs = triangle;
    ClipPlane planes[] = {Left, Right, Bottom, Top};

    for (ClipPlane plane : planes) {
        if (outputs.empty()) break;

        std::vector<Vec<float, 4>> inputs = outputs;
        outputs.clear();


        Vec<float, 4> s = inputs.back();

        for (const auto& p : inputs) {
            if (isInside(p, plane)) {
                if (!isInside(s, plane)) {

                    outputs.push_back(getIntersection(s, p, plane));
                }
                outputs.push_back(p);
            }
            else if (isInside(s, plane)) {

                outputs.push_back(getIntersection(s, p, plane));
            }
            s = p;
        }
    }
    return outputs;
}

void updateRenderable(Entity& object, Camera camera, std::vector<std::array<Vec<float, 3>, 3>>& triangles, Mat4& projection, int width, int height)
{

    std::vector <Vec<float, 4>> projected;
    projected.reserve(object.getVerticesCount());
    triangles.reserve(object.getFacesCount());

    Mat4 model = object.getModelMatrix();
    Mat4 view = camera.getViewMatrix();
    Mat4 MVP = projection * view * model;


    for (int i = 0; i < object.getVerticesCount(); i++) {

        Vec<float, 4> vertex = { object.getVertexByIndex(i) , 1 };
        Vec<float, 4> Vworld = multiply(model, vertex);
        Vec<float, 4> Vview = multiply(view, Vworld);
        Vec<float, 4> Vclip = multiply(MVP, vertex);

        projected.push_back(Vclip);
    }


    for (int i = 0; i < object.getFacesCount(); i++) {
        Vec <int, 3> cFace = object.getFaceByIndex(i);

        Vec<float, 4> a = projected[cFace.x];
        normalizeCoordinates(a);
        int av = keepVertex(a);

        Vec<float, 4> b = projected[cFace.y];
        normalizeCoordinates(b);
        int bv = keepVertex(b);

        Vec<float, 4> c = projected[cFace.z];
        normalizeCoordinates(c);
        int cv = keepVertex(c);

        //i use Sutherland-Hodgman algorithm
        if (av + bv + cv == 3) {//none clipped

            std::array<Vec<float, 3>, 3> tri = { NDCtoPixels(a, width, height), NDCtoPixels(b, width, height), NDCtoPixels(c, width, height) };

            triangles.push_back(tri);
        }
        else if (av + bv + cv > 0) {

            std::vector<Vec<float, 4>> inputTriangle = { a, b, c };
            std::vector<Vec<float, 4>> clippedPoints = clipTriangle(inputTriangle);


            std::vector<Vec<float, 3>> pixelPoints;
            for (int j = 0; j < clippedPoints.size(); j++) {
                pixelPoints.push_back(NDCtoPixels(clippedPoints[j], width, height));
            }


            if (pixelPoints.size() >= 3) {
                for (int i = 1; i < pixelPoints.size() - 1; ++i) {
                    std::array<Vec<float, 3>, 3> tri = {
                        pixelPoints[0],
                        pixelPoints[i],
                        pixelPoints[i + 1]
                    };
                    triangles.push_back(tri);
                }
            }
        }

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
