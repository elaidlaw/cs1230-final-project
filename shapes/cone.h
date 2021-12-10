#ifndef CONE_H
#define CONE_H

#include "OpenGLShape.h"


class Cone: public OpenGLShape
{
public:
    Cone(int param1, int param2);
    void update();
    ~Cone();

private:
    std::vector<float> GenVertexData();
};

#endif // CONE_H
