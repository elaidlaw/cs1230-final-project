#ifndef OPENGLSHAPE_H
#define OPENGLSHAPE_H

/** imports the OpenGL math library https://glm.g-truc.net/0.9.2/api/a00001.html */
#include <glm/glm.hpp>
#include "GL/glew.h"

#include<memory>
#include <vector>

/**
 *
 * inserts a glm::vec3 into a vector of floats
 * this will come in handy if you want to take advantage of vectors to build your shape
 * make sure to call reserve beforehand to speed this up
 */
inline void insertVec3(std::vector<float> &data, glm::vec3 v){
    data.push_back(v.x);
    data.push_back(v.y);
    data.push_back(v.z);
}

namespace CS123 { namespace GL {
class VAO;
}}

class OpenGLShape
{
public:
    OpenGLShape();
    OpenGLShape(int param1, int param2);
    virtual ~OpenGLShape();
    void draw();
    virtual void update() = 0;
    virtual std::vector<float> GenVertexData() = 0;
    void setParam1(int param1){m_param1 = param1;}
    void setParam2(int param2){m_param2 = param2;}

protected:
    /**
     * initializes the relavant openGL properties for the shape
     * don't worry about what exactly this function is doing, you'll learn more about that later in the course!
     * look at ExampleShape.cpp for it's demonstrated usage
     */
    void initializeOpenGLShapeProperties();

    std::vector<GLfloat> m_vertexData;
    std::unique_ptr<CS123::GL::VAO> m_VAO;
    int m_param1;
    int m_param2;
};

#endif // OPENGLSHAPE_H
