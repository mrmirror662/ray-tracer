#pragma once
#include <vector>
using ub = unsigned char;

class view;
class frameBuff
{
private:
    int w, h, c;

public:
    std::vector<unsigned char> data;
    frameBuff(std::vector<unsigned char> &other, int w, int h, int c) : w(w), h(h), c(c) { data = other; }
    frameBuff(){};
    frameBuff(int width, int height, int c_)
    {
        int pixels = width * height * 3;
        data.reserve(pixels);
        for (int i = 0; i < pixels; i++)
        {
            data.emplace_back(0);
        }
        w = width;
        h = height;
        c = c_;
    };
    friend view;
    inline int getW() { return w; };
    inline int getH() { return h; };
    inline int getChannel() { return h; };
    void pushRGBPixel(int index, ub r, ub g, ub b)
    {
        data[index * 3] = r;
        data[index * 3 + 1] = g;
        data[index * 3 + 2] = b;
    }
    ub &operator[](size_t index)
    {
        return data[index];
    }
};