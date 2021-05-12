#include "ray.h"
#include <glm/glm.hpp>
#include <iostream>
#define STAND

const float kEpsilon = 1e-8;
bool ray::doesIntersect(const triangle &tri, float &f_t, glm::vec3 &f_pos)
{
    using namespace glm;
#ifdef STAND
    auto norm = tri.normal;
    auto raydotnorm = dot(norm, dir);
    if (abs(raydotnorm) < kEpsilon)
    {
        return false;
    }
    auto t = (dot(norm, tri.v1) - dot(norm, origin)) / (raydotnorm);
    if (t < 0.1f)
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
    f_pos = pos;
    f_t = t;
    return true;
#else

    vec3 v0v1 = tri.v2 - tri.v1;
    vec3 v0v2 = tri.v3 - tri.v1;
    vec3 pvec = cross(dir, v0v2);
    float det = dot(v0v1, pvec);

    if ((double)fabs(det) < (double)kEpsilon)
    {
        return false;
    }
    float invDet = 1 / det;
    vec3 tvec = origin - tri.v1;
    float u, v;
    u = dot(tvec, pvec) * invDet;
    if ((double)u < (double)0.f || (double)u > (double)1.f)
    {
        return false;
    }
    vec3 qvec = cross(tvec, v0v1);
    v = dot(dir, qvec) * invDet;
    if ((double)v < (double)0.f || (double)v > (double)1.f)
    {
        return false;
    }
    f_t = dot(v0v2, qvec) * invDet;
    f_pos = origin + dir * f_t;

    return true;
#endif
}
/* 
depreciated
*/
//to be called after doesIntersect (no error checks)
// std::tuple<float, glm::vec3> ray::getIntersect(const triangle &tri)
// {
//     using namespace glm;
// #ifdef STAND
//     auto norm = tri.normal;
//     auto raydotnorm = dot(norm, dir);

//     auto t = ((dot(norm, tri.v1) - dot(norm, origin))) / (raydotnorm);

//     auto pos = origin + dir * t;
// #else
//     vec3 v0v1 = tri.v2 - tri.v1;
//     vec3 v0v2 = tri.v3 - tri.v1;
//     vec3 pvec = cross(dir, v0v2);
//     float det = dot(v0v1, pvec);
//     float invDet = 1 / det;
//     vec3 tvec = origin - tri.v1;
//     vec3 qvec = cross(tvec, v0v1);
//     float t = dot(v0v2, qvec) * invDet;
//     vec3 pos = origin + dir * t;

// #endif
//     return {t, pos};
// }