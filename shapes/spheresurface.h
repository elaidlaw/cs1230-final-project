#ifndef SPHERESURFACE_H
#define SPHERESURFACE_H

#include "implicitshape.h"


class SphereSurface: public ImplicitShape
{
public:
    SphereSurface();
    ~SphereSurface();

    double getTValue(glm::vec3 point, glm::vec3 direction) override;
    glm::vec3 getNormal(double x, double y, double z) override;
    glm::vec2 getUV(double x, double y, double z) override;
};

#endif // SPHERESURFACE_H
