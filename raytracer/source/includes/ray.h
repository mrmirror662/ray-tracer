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
    bool doesIntersect(const triangle &, float &t, glm::vec3 &);
    /*
    depreciated
    */
    // std::tuple<float, glm::vec3> getIntersect(const triangle &);
};
