#include "mathfunctions.h"

bool pointInTriangle(const Vec<float, 2> a, const Vec<float, 2> b, const Vec<float, 2> c, const Vec<float, 2> p)
{
    Vec<float, 2> ab =  {b.x - a.x, b.y - a.y };
    Vec<float, 2> ca = { c.x - a.x, c.y - a.y };
    Vec<float, 2> ap = { p.x - a.x, p.y - a.y };

    float d00 = dot(ab, ab);
    float d01 = dot(ab, ca);
    float d11 = dot(ca, ca);
    float d20 = dot(ap, ab);
    float d21 = dot(ap, ca);

    float denom = d00 * d11 - d01 * d01;

    float u = (d11 * d20 - d01 * d21) / denom;
    float v = (d00 * d21 - d01 * d20) / denom;

    return (u >= 0.0) && (v >= 0.0) && (u + v <= 1.0);
}

coordinateBlock getRBlock(Vec<float, 2> t[])
{
    return {
        std::min({t[0].x, t[1].x, t[2].x}),
        std::min({t[0].y, t[1].y, t[2].y}),
        std::max({t[0].x, t[1].x, t[2].x}),
        std::max({t[0].y, t[1].y, t[2].y})
    };
}


void translateZ(Vec<float, 3>& p, float offset) {
    p.z += offset;
}

void translateCoordinates(Vec<float, 2>& c, int wWidth, int wHeight) {
    c = { (c.x + 1) / 2 * wWidth, (1 - (c.y + 1) / 2) * wHeight };
}

Vec<float, 2> project(Vec<float, 3>& c) {

    return { c.x * 2.f / c.z, c.y * 2.f / c.z };
}


Vec<float, 2> placePoint(Vec<float, 3>& p, float offset, int wWidth, int wHeight) {
    Vec<float, 3> temp = p;

    translateZ(temp, offset);

    Vec<float, 2 > point = project(temp);
    translateCoordinates(point, wWidth, wHeight);
    return point;
}