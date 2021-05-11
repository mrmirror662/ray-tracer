#include "shape.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <unistd.h>
bool mesh::loadFromObj(std::string filep)
{
    std::ifstream read;
    read.open(filep);
    if (!read.is_open())
        return false;
    char line[128];
    std::vector<glm::vec3> temp;

    while (read.getline(line, 128))
    {
        std::stringstream ss;
        ss << line;
        char prefix;
        ss >> prefix;
        if (prefix == 'v')
        {
            glm::vec3 vert;
            ss >> vert.x >> vert.y >> vert.z;
            vert.x *= -1;
            vert.y *= -1;
            // vert.z *= -1;
            temp.push_back(vert);
        }
        else if (prefix == 'f')
        {
            char SLASH;
            int INT;
            int index[3];
            for (int i = 0; i < 3; i++)
            {
                ss >> index[i] >> SLASH >> INT;
            }
            tris.push_back({temp[index[0] - 1], temp[index[1] - 1], temp[index[2] - 1], {1., 1., 1.}});
        }
    }
    return true;
}