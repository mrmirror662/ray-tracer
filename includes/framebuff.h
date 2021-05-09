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
    friend view;
    inline int getW() { return w; };
    inline int getH() { return h; };
    inline int getChannel() { return h; };

    ub &operator[](size_t index)
    {
        return data[index];
    }
};