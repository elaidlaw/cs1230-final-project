#ifndef IMPLICITSHAPE_H
#define IMPLICITSHAPE_H

#include <glm/glm.hpp>

class ImplicitShape
{
public:
    ImplicitShape();
    ~ImplicitShape();
    virtual double getTValue(glm::vec3 point, glm::vec3 direction) =0;
    virtual glm::vec3 getNormal(double x, double y, double z) =0;
    virtual glm::vec2 getUV(double x, double y, double z) = 0;
};

#endif // IMPLICITSHAPE_H
