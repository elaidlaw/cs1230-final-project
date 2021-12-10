#include "cone.h"
#include "utilities.h"

#include <iostream>

Cone::Cone(int param1, int param2) :
    OpenGLShape(param1, param2)
{
    m_vertexData = GenVertexData();
    initializeOpenGLShapeProperties();
}

void Cone::update(){
    m_vertexData = GenVertexData();
    initializeOpenGLShapeProperties();
}

std::vector<float> Cone::GenVertexData()
{
    std::vector<float> newVerts;

    //Generate vertices for the cone base
    glm::vec3 baseCenter = glm::vec3(0.0, -0.5,0.0);
    glm::vec3 baseNormal = glm::vec3(0.0,-1.0,0.0);
    glm::vec3 zeroVec = glm::vec3(0.0,0.0,1.0);
    std::vector<float> baseVerts = GenCircleFace(baseCenter, baseNormal, zeroVec, m_param1, m_param2);
    newVerts.insert(newVerts.end(), baseVerts.begin(), baseVerts.end());

    //Generate vertices for the cone upper
    glm::vec3 conePoint = glm::vec3(0.f,0.5,0.f);
    glm::vec3 coneAxis = glm::vec3(0.f,-1.f,0.f);
    std::vector<float> upperVerts = GenConeUpper(conePoint, coneAxis, zeroVec, 1.f, 0.0f, 0.5f, m_param1, m_param2);
    newVerts.insert(newVerts.end(), upperVerts.begin(), upperVerts.end());
    return newVerts;
}

Cone::~Cone(){
}
