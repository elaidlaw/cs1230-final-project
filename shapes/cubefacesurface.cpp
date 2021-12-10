#include "cubefacesurface.h"

CubeFaceSurface::CubeFaceSurface(int axis, double level):
    ImplicitShape(),
    m_axis(axis),
    m_level(level)
{

}

CubeFaceSurface::~CubeFaceSurface(){

}


double CubeFaceSurface::getTValue(glm::vec3 point, glm::vec3 direction){

    double p = point[m_axis];
    double d = direction[m_axis];

    //epsilon check if d is zero because there will be no intersection in this case
    if (abs(d) < 0.000001) {
        return -1.0;
    }

    //compute t
    double t = (m_level - p) / d;

    //check that the other intersection dimensions are within the cube face bounds
    for (int i = 0; i < 3; i++){
        if (i != m_axis && abs(point[i] + direction[i]*t) > 0.5){
            return -1.0;
        }
    }

    return t;
}

glm::vec3 CubeFaceSurface::getNormal(double x, double y, double z){
    glm::vec3 normal = glm::vec3(0.0);
    normal[m_axis] = m_level/abs(m_level);
    return normal;
}

glm::vec2 CubeFaceSurface::getUV(double x, double y, double z){
    glm::vec2 uv = glm::vec2(0.0, 0.0);
    if(m_axis == 2){
        uv = glm::vec2(0.5+x, 0.5-y);
    }else if(m_axis == 1){
        uv = glm::vec2(0.5+x, 0.5+z);
    }else if(m_axis == 0){
        uv = glm::vec2(0.5-z, 0.5-y);
    }
    //if we are on the opposite face the v value is the same but the u value goes the opposite direction
    if (m_level < 0.0){
        uv[0] = 1.0 - uv[0];
    }

    if (m_level < 0.0 && m_axis == 1){
        uv = glm::vec2(0.5+x, 0.5-z);
    }
    return uv;
};

