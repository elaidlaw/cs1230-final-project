#include "cube.h"
#include "utilities.h"
#include <iostream>

Cube::Cube(int param1, int param2) :
    OpenGLShape(param1, param2)
{

    m_vertexData = GenVertexData();

    initializeOpenGLShapeProperties();

}

void Cube::update(){
    m_vertexData = GenVertexData();
    initializeOpenGLShapeProperties();
}

std::vector<float> Cube::GenVertexData(){
    std::vector<float> newVerts;
    glm::vec3 corner = glm::vec3(0.5, 0.5, 0.5);
    float multipliers[] = {-1., 1.};
    //each loop draws three faces of the cube. One set starts at (0.5,0.5,0.5) the other at the negative of that
    for(float m: multipliers){
        //each of these loops draws one face of the cube
        for(int i = 0; i < 3; i++){
            // normal vector
            glm::vec3 normal = glm::vec3(0.f);
            normal[i] = 1.0*m;
            // calculating u and v directions on the cube face
            glm::vec3 vDir = glm::vec3(0.f);
            //we want i+1 if starting at the positive corner, i-1 if starting at negative corner
            vDir[(i+static_cast<int>(m) + 3)%3] = -1.0*m;
            glm::vec3 uDir = glm::cross(normal, vDir);
            std::vector<float> faceVerts = GenCubeFace(corner*m, uDir, vDir, normal, m_param1);
            newVerts.insert(newVerts.end(), faceVerts.begin(), faceVerts.end());
        }
    }
    return newVerts;
}

Cube::~Cube(){

}
