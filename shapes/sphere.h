#ifndef SPHERE_H
#define SPHERE_H

#include "OpenGLShape.h"

class Sphere:  public OpenGLShape
{
public:
public:
    Sphere(int param1, int param2);
    void update();
    ~Sphere();

private:
    std::vector<float> GenVertexData();};

#endif // SPHERE_H
