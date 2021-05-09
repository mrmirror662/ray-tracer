#pragma once
#include "framebuff.h"
class view
{
private:
    unsigned int vb, ib, tid, pid;
    float ver[8] = {
        -1,
        -1,

        -1,
        1,

        1,
        1,

        1,
        -1,
    };
    unsigned int indices[6] = {0, 1, 2, 2, 3, 0};
    unsigned int loadShaders(const char *, const char *);

public:
    view();
    void loadBuff(frameBuff buff);
    void display();
};
