#pragma once
#include "camera.h"
#include "framebuff.h"
#include <thread>
class rayTracer
{
public:
    static void initRays(camera &);
    static void updateCam(camera &cam);
    static std::vector<std::thread> trace_tris(camera *, std::vector<mesh> *, frameBuff *);
    static void trace_work(int, int, frameBuff *, std::vector<mesh> *, camera *);
    void trace(const camera &, std::vector<mesh> &, frameBuff *);

private:
    static glm::vec3 trace_tris_impl(ray &, std::vector<mesh> &);
    static glm::vec3 trace_tris_impl(ray, std::vector<mesh> &, glm::vec3, int);
};