#ifndef IMPLICITSURFACEAGGREGATION_H
#define IMPLICITSURFACEAGGREGATION_H

#include "implicitshape.h"
#include "CS123SceneData.h"
#include <vector>
#include <memory>


class ImplicitSurfaceAggregation: public ImplicitShape
{
public:
    ImplicitSurfaceAggregation(PrimitiveType type);
    ~ImplicitSurfaceAggregation();

    double getTValue(glm::vec3 point, glm::vec3 direction) override;
    glm::vec3 getNormal(double x, double y, double z) override;
    glm::vec2 getUV(double x, double y, double z) override;

private:
    std::vector<std::shared_ptr<ImplicitShape>> m_surfaces;
    //the index of the surface that was first intersected by a ray
    int m_surfaceIndex;
};

#endif // IMPLICITSURFACEAGGREGATION_H
