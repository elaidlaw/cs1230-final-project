#ifndef CONEBODYSURFACE_H
#define CONEBODYSURFACE_H

#include "implicitshape.h"

class ConeBodySurface: public ImplicitShape
{
public:
    ConeBodySurface();
    ~ConeBodySurface();
    double getTValue(glm::vec3 point, glm::vec3 direction) override;
    glm::vec3 getNormal(double x, double y, double z) override;
    glm::vec2 getUV(double x, double y, double z) override;
};

#endif // CONEBODYSURFACE_H
