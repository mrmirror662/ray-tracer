#pragma once
#include <glm/glm.hpp>
#include <tuple>
#include "shape.h"
class ray
{
public:
    glm::vec3 origin, dir;
    ray(glm::vec3 o, glm::vec3 d) : origin(o), dir(d){};
    ray(){};
    bool doesIntersect(const triangle &);
    std::tuple<float, glm::vec3> getIntersect(const triangle &);
    glm::vec3 doesIntersect(mesh &);
};
