#ifndef CUBE_H
#define CUBE_H

#include "OpenGLShape.h"


class Cube: public OpenGLShape
{
public:
    Cube(int param1, int param2);
    void update();
    ~Cube();

private:
    std::vector<float> GenVertexData();
};

#endif // CUBE_H
