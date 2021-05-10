#include "raytracer.h"
#include <iostream>
#include "imgui.h"

#define CLIP_DISTANCE 10000.f
static bool operator>(glm::vec3 &a, glm::vec3 &b)
{
    return (glm::dot(a, a) > glm::dot(b, b));
}
static bool operator<(glm::vec3 &a, glm::vec3 &b)
{
    return (glm::dot(a, a) < glm::dot(b, b));
}
struct pair
{
    glm::vec3 pos;
    uint index;
};
glm::vec3 rayTracer::trace(ray &r, std::vector<triangle> &tris)
{
    using namespace glm;
    std::vector<pair> intersections;
    uint counter = 0;
    for (auto t : tris)
    {
        if (r.doesIntersect(t))
        {
            auto intersection = r.getIntersect(t);
            intersections.push_back({intersection, counter});
        }
        counter++;
    }
    if (intersections.size() == 0)
    {
        return vec3(0.f, 0.f, 0.f);
    }
    pair min = {vec3(CLIP_DISTANCE, CLIP_DISTANCE, CLIP_DISTANCE), 0};
    for (auto intersection : intersections)
    {
        if (intersection.pos < min.pos)
            min = intersection;
    }
    auto raytointer = min.pos - r.origin;
    raytointer = glm::normalize(raytointer);
    auto triHit = tris[min.index];
    auto norm = glm::normalize(triHit.normal);
    auto bright = abs(dot(raytointer, norm));
    // ImGui::Text("bright:%f", bright);
    auto col = bright * triHit.col;
    return col;
}
frameBuff rayTracer::trace(const camera &cam, std::vector<triangle> &tris)
{
    std::vector<ub> fBuff;
    for (int i = cam.rays.size() - 1; i >= 0; i--)
    {
        auto r = cam.rays[i];
        auto traced = trace(r, tris);
        fBuff.push_back((ub)(traced.r * 255.0f));
        fBuff.push_back((ub)(traced.g * 255.0f));
        fBuff.push_back((ub)(traced.b * 255.0f));
    }
    return frameBuff(fBuff, cam.w, cam.w, cam.c);
}

void rayTracer::initRays(camera &cam)
{
    std::vector<ray> rays;
    for (float r = 0; r < cam.h; r += 1.f)
    {
        float dy = 2 * (r / cam.h - 0.5);
        for (float c = 0; c < cam.w; c += 1.f)
        {
            // auto index = cam.w * r + c;
            ray ra;
            float dz = 2 * (c / cam.w - 0.5);
            ra.origin = cam.pos + glm::vec3(0.f, r, c);
            ra.dir = {1.f, dy, dz};
            ra.dir = glm::normalize(ra.dir);
            rays.push_back(ra);
        }
    }
    cam.rays = std::move(rays);
}
void rayTracer::updateCam(camera &cam)
{
    for (auto &r : cam.rays)
    {
        r.origin += cam.pos;
    }
}
