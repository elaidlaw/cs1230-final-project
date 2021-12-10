#include "sphere.h"
#include "utilities.h"

Sphere::Sphere(int param1, int param2) :
    OpenGLShape(param1, param2)
{

    m_vertexData = GenVertexData();

    initializeOpenGLShapeProperties();
}

void Sphere::update(){
    m_vertexData = GenVertexData();
    initializeOpenGLShapeProperties();
}

std::vector<float> Sphere::GenVertexData(){
    return GenSphere(glm::vec3(0.0f,-1.f,0.0f), glm::vec3(1.0f,0.0f,0.0f), m_param1, m_param2);
}

Sphere::~Sphere(){

}
