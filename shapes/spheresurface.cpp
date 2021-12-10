#include "spheresurface.h"

SphereSurface::SphereSurface():
    ImplicitShape()
{

}

SphereSurface::~SphereSurface(){

}

double SphereSurface::getTValue(glm::vec3 point, glm::vec3 direction){
    double px = point[0];
    double py = point[1];
    double pz = point[2];
    double dx = direction[0];
    double dy = direction[1];
    double dz = direction[2];

    //Implicit surface intersection derived in algo assignment
    double a = pow(dx, 2.0) + pow(dz, 2.0) + pow(dy, 2.0);
    double b = 2.0* (px*dx + pz*dz + py*dy);
    double c = pow(px, 2.0) + pow(pz, 2.0) + pow(py, 2.0) - 0.25;

    //Calculate the term inside the square root of the quadratic equation
    double innerTerm = pow(b, 2.0) - 4.0*a*c;

    //return a negative value (meaning no intersection) if our t value is imaginary
    if (innerTerm < 0.){
        return -1.0;
    }

    double lowT = (-b - sqrt(innerTerm)) / (2.0*a);

    return lowT;
}

glm::vec3 SphereSurface::getNormal(double x, double y, double z){
    glm::vec3 normal = glm::vec3(2.0*x, 2.0*y, 2.0*z);
    normal = glm::normalize(normal);
    return normal;
}

glm::vec2 SphereSurface::getUV(double x, double y, double z){
    float PI = 3.1415926535898;
    if (y == 0.5) {
        return glm::vec2(0.5, 1.0);
    }
    if (y == -0.5) {
        return glm::vec2(0.0, 0.0);
    }
    float u = atan2(z,x) / (2*PI) + 0.5;
    float opposite = sqrt(pow(x, 2.0) + pow(z,2.0));
    float v = 1.0 - (atan(opposite/y) / PI + 0.5);
    return glm::vec2(u, v);
};
