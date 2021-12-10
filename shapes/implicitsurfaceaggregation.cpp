#include "implicitsurfaceaggregation.h"
#include "conebodysurface.h"
#include "cylinderbodysurface.h"
#include "cylindercapsurface.h"
#include "spheresurface.h"
#include "cubefacesurface.h"

ImplicitSurfaceAggregation::ImplicitSurfaceAggregation(PrimitiveType type):
    ImplicitShape(),
    m_surfaces(std::vector<std::shared_ptr<ImplicitShape>>()),
    m_surfaceIndex(0)
{
    if (type == PrimitiveType::PRIMITIVE_CONE){
        m_surfaces.push_back(std::make_shared<ConeBodySurface>(ConeBodySurface()));
        m_surfaces.push_back(std::make_shared<CylinderCapSurface>(CylinderCapSurface(-0.5)));
    }else if (type == PrimitiveType::PRIMITIVE_CYLINDER){
        m_surfaces.push_back(std::make_shared<CylinderBodySurface>(CylinderBodySurface()));
        m_surfaces.push_back(std::make_shared<CylinderCapSurface>(CylinderCapSurface(-0.5)));
        m_surfaces.push_back(std::make_shared<CylinderCapSurface>(CylinderCapSurface(0.5)));
    }else if (type == PrimitiveType::PRIMITIVE_SPHERE || type == PrimitiveType::PRIMITIVE_MESH || type == PrimitiveType::PRIMITIVE_TORUS){
        m_surfaces.push_back(std::make_shared<SphereSurface>(SphereSurface()));
    }else if (type == PrimitiveType::PRIMITIVE_CUBE){
        m_surfaces.push_back(std::make_shared<CubeFaceSurface>(CubeFaceSurface(0, 0.5)));
        m_surfaces.push_back(std::make_shared<CubeFaceSurface>(CubeFaceSurface(0, -0.5)));
        m_surfaces.push_back(std::make_shared<CubeFaceSurface>(CubeFaceSurface(1, 0.5)));
        m_surfaces.push_back(std::make_shared<CubeFaceSurface>(CubeFaceSurface(1, -0.5)));
        m_surfaces.push_back(std::make_shared<CubeFaceSurface>(CubeFaceSurface(2, 0.5)));
        m_surfaces.push_back(std::make_shared<CubeFaceSurface>(CubeFaceSurface(2, -0.5)));
    }
}

ImplicitSurfaceAggregation::~ImplicitSurfaceAggregation(){

}

double ImplicitSurfaceAggregation::getTValue(glm::vec3 point, glm::vec3 direction){
    //Tracks the closest surface we've encountered
    double firstIntersection = -1.0;
    //variable to hold the current t value
    double currSurfaceT = 0.0;
    for (int i = 0; i < m_surfaces.size(); i++){
        currSurfaceT = m_surfaces[i]->getTValue(point, direction);
        if (currSurfaceT > 0.0){
            if (currSurfaceT < firstIntersection || firstIntersection < 0.0){
                firstIntersection = currSurfaceT;
                m_surfaceIndex = i;
            }
        }
    }
    return firstIntersection;
}

glm::vec3 ImplicitSurfaceAggregation::getNormal(double x, double y, double z){
    //This function relies on getTValue having already been called to set the m_surfaceIndex variable
    return m_surfaces[m_surfaceIndex]->getNormal(x,y,z);
}

glm::vec2 ImplicitSurfaceAggregation::getUV(double x, double y, double z){
    return m_surfaces[m_surfaceIndex]->getUV(x,y,z);
};

