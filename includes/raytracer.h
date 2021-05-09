#pragma once
#include "camera.h"
#include "framebuff.h"
class rayTracer
{
public:
    static void initRays(camera &);
    static void updateCam(camera &cam);
    frameBuff trace(const camera &, std::vector<triangle> &);
    frameBuff trace(const camera &, std::vector<mesh> &);

private:
    glm::vec3 trace(ray &, std::vector<triangle> &);
};