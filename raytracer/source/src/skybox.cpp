#include "skybox.h"
#include <glm/gtc/constants.hpp>
SkyBox::SkyBox(std::string image_path)
{
    image_data = stbi_load(image_path.c_str(), &w, &h, &channels, 0);
    if (image_data == nullptr)
    {
        throw std::runtime_error("cant open image");
    }
}
void SkyBox::LoadFromImage(const std::string &image_path)
{
    image_data = stbi_load(image_path.c_str(), &w, &h, &channels, 0);
}
SkyBox::SkyBox()
{
}
SkyBox::~SkyBox()
{
    delete[] image_data;
}

glm::vec3 SkyBox::SampleSkyBox(const ray &r)
{
    float u, v;
    u = 0.5f + (glm::atan(r.dir.x, r.dir.z) / (2.f * glm::pi<float>()));
    v = 0.5 - (glm::asin(r.dir.y) / (glm::pi<float>()));
    int wu = w * u;
    int hv = h * v;
    int index = (wu + hv * w) * 3;
    float red = image_data[index];
    float green = image_data[index + 1];
    float blue = image_data[index + 2];
    return {red / 255, green / 255, blue / 255};
}