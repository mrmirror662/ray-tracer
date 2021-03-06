#pragma once
#include <glm/glm.hpp>
#include <vector>
#include <string>
enum class MatType
{
    Metallic,
    Plastic,
    Dielectric
};
struct triangle
{

    glm::vec3 v1, v2, v3;
    glm::vec3 col;
    glm::vec3 normal;
    triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3) : v1(v1), v2(v2), v3(v3)
    {
        col = {0.1f, 0.1f, 0.1f};
        normal = glm::cross(v2 - v1, v3 - v1);
        normal = glm::normalize(normal);
    };
    triangle(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 col) : v1(v1), v2(v2), v3(v3), col(col)
    {
        normal = glm::cross(v2 - v1, v3 - v1);
        normal = glm::normalize(normal);
    };
};
struct mesh
{
    MatType SurfaceType;
    std::vector<triangle> tris;
    float absorption;
    mesh() : SurfaceType(MatType::Metallic){};
    mesh(MatType type) : SurfaceType(type){};
    bool loadFromObj(std::string filePath, glm::vec3 col);
};