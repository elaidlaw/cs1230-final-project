#ifndef CUBEFACESURFACE_H
#define CUBEFACESURFACE_H

#include "implicitshape.h"

class CubeFaceSurface: public ImplicitShape
{
public:
    CubeFaceSurface(int axis, double level);
    ~CubeFaceSurface();

    double getTValue(glm::vec3 point, glm::vec3 direction) override;
    glm::vec3 getNormal(double x, double y, double z) override;
    glm::vec2 getUV(double x, double y, double z) override;

private:
    //which axis the face is on (0->x, 1->y, 2->z)
    int m_axis;
    //at what value of the particular axis the surface exists (-0.5 or 0.5 for our purposes)
    double m_level;

};

#endif // CUBEFACESURFACE_H
