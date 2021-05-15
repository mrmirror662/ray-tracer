#pragma once
#include "camera.h"
#include "framebuff.h"
#include "skybox.h"
#include <thread>
class rayTracer
{
public:
    static void initAll(camera &);
    static void initRays(camera &);
    static void updateCam(camera &cam);
    static std::vector<std::thread> trace_tris(camera *, std::vector<mesh> *, SkyBox *, frameBuff *);
    static void trace_work(int, int, frameBuff *, std::vector<mesh> *, SkyBox *, camera *);
    void trace(const camera &, std::vector<mesh> &, frameBuff *);

private:
    static glm::vec3 trace_meshes_impl(ray, std::vector<mesh> &, SkyBox *sb);
    static glm::vec3 trace_meshes_impl(ray, std::vector<mesh> &, SkyBox *sb, glm::vec3, int);
};