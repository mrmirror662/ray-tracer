#include "ray.h"
#include <glm/glm.hpp>

bool ray::doesIntersect(const triangle &tri)
{
    using namespace glm;
    auto norm = tri.normal;
    auto raydotnorm = dot(norm, dir);
    if (abs(raydotnorm) < 0.00001f)
    {
        return false;
    }
    auto t = (dot(norm, tri.v1) - dot(norm, origin)) / (raydotnorm);
    if (t < 0)
        return false;
    auto pos = origin + dir * t;
    auto e0 = tri.v2 - tri.v1;
    auto vp0 = pos - tri.v1;
    auto c = cross(e0, vp0);
    if (dot(norm, c) < 0.f)
        return false;
    auto e1 = tri.v3 - tri.v2;
    auto vp1 = pos - tri.v2;
    c = cross(e1, vp1);
    if (dot(norm, c) < 0.f)
        return false;
    auto e2 = tri.v1 - tri.v3;
    auto vp2 = pos - tri.v3;
    c = cross(e2, vp2);
    if (dot(norm, c) < 0.f)
        return false;
    return true;
}
//to be called after doesIntersect (no error checks)
glm::vec3 ray::getIntersect(const triangle &tri)
{
    using namespace glm;
    auto norm = tri.normal;
    auto raydotnorm = dot(norm, dir);

    auto t = ((dot(norm, tri.v1) - dot(norm, origin))) / (raydotnorm);

    auto pos = origin + dir * t;

    return pos;
}