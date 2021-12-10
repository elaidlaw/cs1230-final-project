#include "conebodysurface.h"

#include <cmath>

ConeBodySurface::ConeBodySurface():
    ImplicitShape()
{

}

ConeBodySurface::~ConeBodySurface(){

}

double ConeBodySurface::getTValue(glm::vec3 point, glm::vec3 direction){
    double px = point[0];
    double py = point[1];
    double pz = point[2];
    double dx = direction[0];
    double dy = direction[1];
    double dz = direction[2];

    //Implicit surface intersection derived in algo assignment
    double a = pow(dx, 2.0) + pow(dz, 2.0) - 0.25 * pow(dy,2.0);
    double b = 2.0*px*dx + 2.0*pz*dz - 0.5*py*dy + 0.25*dy;
    double c = pow(px, 2.0) + pow(pz, 2.0) - 0.25 * pow(py, 2.0) + 0.25 * py - 1.0/16.0;

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

glm::vec3 ConeBodySurface::getNormal(double x, double y, double z){
    glm::vec3 normal = glm::vec3(2.0*x,0.0, 2.0*z);
    normal = glm::normalize(normal);
    //since the cone has base radius of 0.5 and height of 1.0, the ratio of the y component of the normal to the rest of the normal should be 0.5/1.0
    normal[1] = 0.5;
    normal = glm::normalize(normal);
    return normal;
}

glm::vec2 ConeBodySurface::getUV(double x, double y, double z){
    float PI = 3.1415926535898;
    if (y == 0.5) {
        return glm::vec2(0.5, 1.0);
    }
    //z, -x matches the TA demo
    float u = atan2(z, -x) / (2*PI) + 0.5;
    float v =  1.0 - (y + 0.5);
    return glm::vec2(u, v);
};
