#include "mathfunctions.h"
#include "Entity.h"
#include "Camera.h"


bool pointInTriangle(const Vec<float, 3>& a, const Vec<float, 3>& b, const Vec<float, 3>& c, const Vec<float, 2>& p, Vec<float, 3>& w)
{
    Vec<float, 2> ab = { b.x - a.x, b.y - a.y };
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

bool isClockwise(const Vec<float, 4>& a, const Vec<float, 4>& b, const Vec<float, 4>& c) {

    Vec<float, 2> ab = { b.x - a.x, b.y - a.y };
    Vec<float, 2> ac = { c.x - a.x, c.y - a.y };


    if (cross(ab, ac) < 0.f) {
        return true;
    }
    return false;
}

coordinateBlock getRBlock(Vertex t[])
{
    return {
        (int)std::floor(std::min({t[0].v.x, t[1].v.x, t[2].v.x})),
        (int)std::floor(std::min({t[0].v.y, t[1].v.y, t[2].v.y})),
        (int)std::ceil(std::max({t[0].v.x, t[1].v.x, t[2].v.x})),
        (int)std::ceil(std::max({t[0].v.y, t[1].v.y, t[2].v.y}))
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

void calculateUV(float& u, float& v, Vertex t[], const Vec<float, 3>& weights) {
    float uInter = weights.x * (t[0].vt.x / t[0].v.w) + weights.y * (t[1].vt.x / t[1].v.w) + weights.z * (t[2].vt.x / t[2].v.w);
    float vInter = weights.x * (t[0].vt.y / t[0].v.w) + weights.y * (t[1].vt.y / t[1].v.w) + weights.z * (t[2].vt.y / t[2].v.w);

    float wInter = weights.x * (1 / t[0].v.w) + weights.y * (1 / t[1].v.w) + weights.z * (1 / t[2].v.w);

    u = uInter / wInter;
    v = 1.0f - (vInter / wInter);
}

Vec<float, 3> NDCtoPixels(Vec<float, 4>& vertex, int width, int height) {
    return Vec<float, 3>((vertex.x + 1.f) / 2.f * width, (1 - (vertex.y + 1.f) / 2.f) * height, vertex.z);
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

bool isInside(const Vertex& p, ClipPlane plane) {
    switch (plane) {
    case Left:
        return p.v.x >= -1.0f;
    case Right:
        return p.v.x <= 1.0f;
    case Bottom:
        return p.v.y >= -1.0f;
    case Top:
        return p.v.y <= 1.0f;
    case Near:
        return p.v.w >= 0.1f;
    }
    return false;
}

Vertex getIntersection(const Vertex& s, const Vertex& p, ClipPlane plane) {

    float t = 0.0f;
    switch (plane) {
    case Left:
        t = (-1.0f - s.v.x) / (p.v.x - s.v.x);
        break;
    case Right:
        t = (1.0f - s.v.x) / (p.v.x - s.v.x);
        break;
    case Bottom:
        t = (-1.0f - s.v.y) / (p.v.y - s.v.y);
        break;
    case Top:
        t = (1.0f - s.v.y) / (p.v.y - s.v.y);
        break;
    case Near:
        t = (0.1f - s.v.w) / (p.v.w - s.v.w);
        break;
    }

    Vec<float, 4> intersect = {
        s.v.x + t * (p.v.x - s.v.x),
        s.v.y + t * (p.v.y - s.v.y),
        s.v.z + t * (p.v.z - s.v.z),
        s.v.w + t * (p.v.w - s.v.w)
    };

    if (plane == Left)   intersect.x = -1.0f;
    if (plane == Right)  intersect.x = 1.0f;
    if (plane == Bottom) intersect.y = -1.0f;
    if (plane == Top)    intersect.y = 1.0f;
    if (plane == Near)   intersect.w = 0.1f;

    Vec<float, 2> intersectVt = { s.vt.x + t * (p.vt.x - s.vt.x), s.vt.y + t * (p.vt.y - s.vt.y) };

    return { intersect, intersectVt };
}

std::vector<Vertex> clipTriangle(const std::vector<Vertex>& triangle) {
    std::vector<Vertex> outputs = triangle;
    ClipPlane planes[] = { Left, Right, Bottom, Top };

    for (int i = 0; i < PLANES_NUMBER; i++) {
        if (outputs.empty()) break;

        std::vector<Vertex> inputs = outputs;
        outputs.clear();

        for (int j = 0; j < inputs.size(); j++) {
            bool a = isInside(inputs[j], planes[i]);
            int ind = j + 1;
            if (ind > inputs.size() - 1) {
                ind = 0;
            }

            bool b = isInside(inputs[ind], planes[i]);

            if (a && b) {
                outputs.push_back(inputs[ind]);
            }
            else if (!a && b) {
                outputs.push_back(getIntersection(inputs[j], inputs[ind], planes[i]));
                outputs.push_back(inputs[ind]);
            }
            else if (a && !b) {
                outputs.push_back(getIntersection(inputs[j], inputs[ind], planes[i]));
            }
        }

    }
    return outputs;
}

std::vector<Vertex> clipNear(const std::vector<Vertex>& polygon) {
    std::vector<Vertex> outputs;
    for (size_t j = 0; j < polygon.size(); j++) {
        bool a = isInside(polygon[j], Near);
        size_t ind = (j + 1) % polygon.size();
        bool b = isInside(polygon[ind], Near);

        if (a && b) { outputs.push_back(polygon[ind]); }
        else if (!a && b) {
            outputs.push_back(getIntersection(polygon[j], polygon[ind], Near));
            outputs.push_back(polygon[ind]);
        }
        else if (a && !b) {
            outputs.push_back(getIntersection(polygon[j], polygon[ind], Near));
        }
    }

    return outputs;
}

void updateRenderable(Entity& object, int i, Camera& camera, std::vector<std::array<Vertex, 3>>& triangles, Mat4& projection, int width, int height)
{

    std::vector <Vec<float, 4>> projected;
    projected.reserve(object.getVerticesCount());
    triangles.reserve(object.getFacesCount());

    Mat4 model = object.getInstanceModelMatrix(i);
    Mat4 view = camera.getViewMatrix();
    Mat4 MVP = projection * view * model;


    for (int i = 0; i < object.getVerticesCount(); i++) {

        Vec<float, 4> vertex = { object.getVertexByIndex(i) , 1 };
        //Vec<float, 4> Vworld = multiply(model, vertex);
        //Vec<float, 4> Vview = multiply(view, Vworld);
        Vec<float, 4> Vclip = multiply(MVP, vertex);

        projected.push_back(Vclip);
    }


    for (int i = 0; i < object.getFacesCount(); i++) {
        Vec <int, 3> cFace = object.getFaceByIndex(i);

        Vec<int, 3> vtFace = object.getVtFaceByIndex(i);

        Vertex v0(projected[cFace.x], object.getVtByIndex(vtFace.x));
        Vertex v1(projected[cFace.y], object.getVtByIndex(vtFace.y));
        Vertex v2(projected[cFace.z], object.getVtByIndex(vtFace.z));

        std::vector<Vertex> polygon = { v0, v1, v2 };

        polygon = clipNear(polygon);

        if (polygon.empty()) continue;

        bool clip = false;
        for (int j = 0; j < polygon.size(); j++) {
            normalizeCoordinates(polygon[j].v);
            if (!keepVertex(polygon[j].v)) {
                clip = true;
            }
        }

        if (clip) {
            polygon = clipTriangle(polygon);
        }


        std::vector<Vertex> pixelPoints;
        for (int k = 0; k < polygon.size(); k++) {
            pixelPoints.push_back({ { NDCtoPixels(polygon[k].v, width, height), polygon[k].v.w },  polygon[k].vt });
        }

        if (pixelPoints.size() >= 3) {

            for (int k = 1; k < pixelPoints.size() - 1; ++k) {
                std::array<Vertex, 3> tri = { pixelPoints[0], pixelPoints[k], pixelPoints[k + 1] };
                triangles.push_back(tri);
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

Vec<float, 3> rotateVector(Quaternion q, Vec<float, 3> v)
{
    Quaternion vq = { 0, v.x, v.y, v.z };

    Quaternion qConj = { q.s, -q.x, -q.y, -q.z };

    Quaternion result = multiply(multiply(q, vq), qConj);

    return { result.x, result.y, result.z };
}