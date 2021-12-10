#include "cylinderbodysurface.h"
#include <math.h>

CylinderBodySurface::CylinderBodySurface():
    ImplicitShape()
{

}

CylinderBodySurface::~CylinderBodySurface(){

}

double CylinderBodySurface::getTValue(glm::vec3 point, glm::vec3 direction){
    double px = point[0];
    double py = point[1];
    double pz = point[2];
    double dx = direction[0];
    double dy = direction[1];
    double dz = direction[2];

    //Implicit surface intersection derived in algo assignment
    double a = pow(dx, 2.0) + pow(dz, 2.0);
    double b = 2.0*px*dx + 2.0*pz*dz;
    double c = pow(px, 2.0) + pow(pz, 2.0) - 0.25;

    //Calculate the term inside the square root of the quadratic equation
    double innerTerm = pow(b, 2.0) - 4.0*a*c;

    //return a negative value (meaning no intersection) if our t value is imaginary
    if (innerTerm < 0.){
        return -1.0;
    }

    double lowT = (-b - sqrt(innerTerm)) / (2.0*a);
    double highT = (-b + sqrt(innerTerm)) / (2.0*a);

    //Return the lowest positive t value that respects the constraints. If none are positive, return -1.
    if (lowT > 0. && (py + lowT*dy) <= 0.5 && (py + lowT*dy) >=-0.5){
        return lowT;
    } else if (highT > 0. && (py + highT*dy) <= 0.5 && (py + highT*dy) >=-0.5) {
        return highT;
    }
    return -1.0;
}

glm::vec3 CylinderBodySurface::getNormal(double x, double y, double z){
    glm::vec3 normal = glm::vec3(2.0*x, 0.0, 2.0*z);
    normal = glm::normalize(normal);
    return normal;
}

glm::vec2 CylinderBodySurface::getUV(double x, double y, double z){
    float PI = 3.1415926535898;
    float u = atan2(z, -x) / (2*PI) + 0.5;
    float v =  1.0 - (y + 0.5);
    return glm::vec2(u, v);
};

