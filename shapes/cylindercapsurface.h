#ifndef CYLINDERCAPSURFACE_H
#define CYLINDERCAPSURFACE_H

#include "implicitshape.h"

class CylinderCapSurface: public ImplicitShape
{
public:
    CylinderCapSurface(double level);
    ~CylinderCapSurface();

    double getTValue(glm::vec3 point, glm::vec3 direction) override;
    glm::vec3 getNormal(double x, double y, double z) override;
    glm::vec2 getUV(double x, double y, double z) override;

private:
    //the y value at which the cap exists
    //should be either 0.5 or -0.5 for our cylinder and cone
    double m_level;
};

#endif // CYLINDERCAPSURFACE_H
