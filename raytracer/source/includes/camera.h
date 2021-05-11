#pragma once
#include <vector>
#include <glm/glm.hpp>
#include "ray.h"
struct camera
{
    glm::vec3 pos;
    glm::vec3 rotation;
    float w, h, c, resolutionFactor;
    std::vector<ray> rays;
};