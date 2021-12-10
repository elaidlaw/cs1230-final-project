#include "cylinder.h"
#include "utilities.h"

Cylinder::Cylinder(int param1, int param2) :
    OpenGLShape(param1, param2)
{
    m_vertexData = GenVertexData();

    initializeOpenGLShapeProperties();
}

void Cylinder::update(){
    m_vertexData = GenVertexData();
    initializeOpenGLShapeProperties();
}

Cylinder::~Cylinder(){

}

std::vector<float> Cylinder::GenVertexData()
{
    std::vector<float> newVerts;

    //Generate vertices for the bottom cap
    glm::vec3 bottomCenter = glm::vec3(0.0, -0.5,0.0);
    glm::vec3 bottomNormal = glm::vec3(0.0,-1.0,0.0);
    glm::vec3 zeroVec = glm::vec3(1.0,0.0,0.0);
    std::vector<float> bottomVerts = GenCircleFace(bottomCenter, bottomNormal, zeroVec, m_param1, m_param2);
    newVerts.insert(newVerts.end(), bottomVerts.begin(), bottomVerts.end());

    //Generate vertices for the top cap
    glm::vec3 topCenter = glm::vec3(0.0, 0.5, 0.0);
    glm::vec3 topNormal = glm::vec3(0.0, 1.0, 0.0);
    std::vector<float> topVerts = GenCircleFace(topCenter, topNormal, zeroVec, m_param1, m_param2);
    newVerts.insert(newVerts.end(), topVerts.begin(), topVerts.end());

    glm::vec3 conePoint = glm::vec3(0.f,0.5,0.f);
    glm::vec3 coneAxis = glm::vec3(0.f,-1.f,0.f);
    std::vector<float> upperVerts = GenConeUpper(conePoint, coneAxis, zeroVec, 1.f, 0.5f, 0.5f, m_param1, m_param2);
    newVerts.insert(newVerts.end(), upperVerts.begin(), upperVerts.end());
    return newVerts;
}
