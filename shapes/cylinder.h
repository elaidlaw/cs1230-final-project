#ifndef CYLINDER_H
#define CYLINDER_H

#include "OpenGLShape.h"


class Cylinder: public OpenGLShape
{
public:
    Cylinder(int param1, int param2);
    void update();
    ~Cylinder();

private:
    std::vector<float> GenVertexData();
};

#endif // CYLINDER_H
