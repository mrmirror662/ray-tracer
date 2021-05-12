#include "raytracer.h"
#include <iostream>
#include "imgui.h"
#include <thread>
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
glm::vec3 rayTracer::trace_tris_impl(ray &r, std::vector<triangle> &tris)
{
    using namespace glm;
    std::vector<intersect_info> intersections;
    uint counter = 0;
    for (auto t : tris)
    {
        float t_t;
        glm::vec3 pos_t;
        if (r.doesIntersect(t, t_t, pos_t))
        {

            intersections.push_back({t_t, pos_t, counter});
        }
        counter++;
    }
    if (intersections.size() == 0)
    {

        auto b_val = clamp((-dot(r.dir, vec3(0.f, 1.f, 0.f))), 0.f, 1.f);

        return vec3(b_val, .20f, .20f);
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
    auto reflect_dir = glm::reflect(r.dir, -norm);
    ray r_new(min.pos, reflect_dir);
    auto bright = abs(dot(raytointer, norm));
    auto col = bright * triHit.col;
    glm::vec3 f_col = f_col = f_col + (0.7f * trace_tris_impl(r_new, tris, col));
    return f_col;
}
glm::vec3 rayTracer::trace_tris_impl(ray r, std::vector<triangle> &tris, glm::vec3 b_col)
{
    using namespace glm;
    std::vector<intersect_info> intersections;
    uint counter = 0;
    for (auto t : tris)
    {
        float t_t;
        glm::vec3 pos_t;
        if (r.doesIntersect(t, t_t, pos_t))
        {

            intersections.push_back({t_t, pos_t, counter});
        }
        counter++;
    }
    if (intersections.size() == 0)
    {
        auto b_val = clamp((dot(r.dir, vec3(0.f, 1.f, 0.f))), 0.0f, 1.f);

        return vec3(b_val, .20f, .20f);
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
    auto reflect_dir = glm::reflect(r.dir, norm);
    ray r_new(min.pos, reflect_dir);
    auto bright = abs(dot(raytointer, norm));
    auto col = ((bright * triHit.col) + (b_col / 1.f)) / 2.f;
    glm::vec3 f_col = f_col + (0.7f * trace_tris_impl(r_new, tris, col));
    return f_col;
}
void rayTracer::trace_work(int start, int end, frameBuff *displayFrame, std::vector<triangle> *tris, camera *cam)
{
    for (int i = start; i >= end; i--)
    {
        auto r = cam->rays[i];
        auto traced = trace_tris_impl(r, *tris);
        displayFrame->pushRGBPixel(
            i,
            (ub)(traced.r * 255.0f),
            (ub)(traced.g * 255.0f),
            (ub)(traced.b * 255.0f));
    }
}
std::vector<std::thread> rayTracer::trace_tris(camera *cam, std::vector<triangle> *tris, frameBuff *displayFrame)
{
    int raysLength = cam->rays.size();
    int cores = std::thread::hardware_concurrency();
    int dividedRays = raysLength / cores;
    int startIndex = raysLength - 1;

    std::vector<std::thread> multiTaskingThreads;
    multiTaskingThreads.reserve(cores);

    for (int i = 0; i < cores; i++)
    {
        multiTaskingThreads.push_back(
            std::thread(
                trace_work,
                startIndex,
                std::max(startIndex - dividedRays, 0),
                displayFrame,
                tris,
                cam));
        startIndex -= (dividedRays + 1);
    }

    return multiTaskingThreads;
}

void rayTracer::initRays(camera &cam)
{
    std::vector<ray> rays;
    for (float r = 0; r < cam.h * cam.resolutionFactor; r += 1)
    {
        float dy = 2 * (r / cam.h - 0.5);
        for (float c = 0; c < cam.w * cam.resolutionFactor; c += 1)
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
