#include "cylindercapsurface.h"

CylinderCapSurface::CylinderCapSurface(double level):
    ImplicitShape(),
    m_level(level)
{

}

CylinderCapSurface::~CylinderCapSurface()
{

}

double CylinderCapSurface::getTValue(glm::vec3 point, glm::vec3 direction){
    double px = point[0];
    double py = point[1];
    double pz = point[2];
    double dx = direction[0];
    double dy = direction[1];
    double dz = direction[2];
    //epsilon check if dy is zero because there will be no intersection in this case
    if (abs(dy) < 0.000001) {
        return -1.0;
    }

    //compute t
    double t = (m_level - py) / dy;

    //compute intersection x and z values
    double z_int = pz + dz*t;
    double x_int = px + dx*t;

    //check if the intersection is within our circle bounds
    if ((pow(z_int, 2.0)+pow(x_int, 2.0)) <= 0.25){
        return t;
    }

    return -1.0;
}

glm::vec3 CylinderCapSurface::getNormal(double x, double y, double z){
    glm::vec3 normal = glm::vec3(0.0, m_level/abs(m_level), 0.0);
    return normal;
}

glm::vec2 CylinderCapSurface::getUV(double x, double y, double z){
    if (m_level > 0.0){
        return glm::vec2(0.5+x, 0.5+z);
    } else {
        return glm::vec2(0.5+x, 0.5-z);
    }
};

