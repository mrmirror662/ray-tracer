#include "raytracer.h"
#include <iostream>
#include "imgui.h"

#define CLIP_DISTANCE 10000.f
static bool operator>(const glm::vec3 &a, const glm::vec3 &b)
{
    return (glm::dot(a, a) > glm::dot(b, b));
}
static bool operator<(const glm::vec3 &a, const glm::vec3 &b)
{
    return (glm::dot(a, a) < glm::dot(b, b));
}
struct intersect_info
{
    float t;
    glm::vec3 pos;
    uint index;
};
glm::vec3 rayTracer::trace(ray &r, std::vector<triangle> &tris)
{
    using namespace glm;
    std::vector<intersect_info> intersections;
    uint counter = 0;
    for (auto t : tris)
    {
        if (r.doesIntersect(t))
        {

            auto [c_t, intersection] = r.getIntersect(t);

            intersections.push_back({c_t, intersection, counter});
        }
        counter++;
    }
    if (intersections.size() == 0)
    {
        return vec3(clamp(-dot(r.dir, vec3(0.f, 1.f, 0.f)), 0.f, 0.5f), 0.f, 0.f);
    }
    intersect_info min = {CLIP_DISTANCE, vec3(CLIP_DISTANCE, CLIP_DISTANCE, CLIP_DISTANCE), 0};
    for (auto intersection : intersections)
    {
        if (intersection.t < min.t)
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
    return frameBuff(fBuff, cam.w * cam.resolutionFactor, cam.h * cam.resolutionFactor, cam.c);
}

void rayTracer::initRays(camera &cam)
{
    std::vector<ray> rays;
    for (float r = 0; r < cam.h; r += 1)
    {
        float dy = 2 * (r / cam.h - 0.5);
        for (float c = 0; c < cam.w; c += 1)
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
