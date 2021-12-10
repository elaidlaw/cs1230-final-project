#ifndef CYLINDERBODYSURFACE_H
#define CYLINDERBODYSURFACE_H

#include "implicitshape.h"

class CylinderBodySurface: public ImplicitShape
{
public:
    CylinderBodySurface();
    ~CylinderBodySurface();

    double getTValue(glm::vec3 point, glm::vec3 direction) override;
    glm::vec3 getNormal(double x, double y, double z) override;
    glm::vec2 getUV(double x, double y, double z) override;
};

#endif // CYLINDERBODYSURFACE_H
