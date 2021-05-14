#pragma once
#include "stb_image.h"
#include <string>
#include <glm/glm.hpp>
#include "ray.h"
class SkyBox
{

public:
    SkyBox(std::string image_path);
    SkyBox();
    void LoadFromImage(const std::string &image_path);
    glm::vec3 SampleSkyBox(const ray &);
    ~SkyBox();

private:
    u_char *image_data;
    int channels;
    int w, h;
};