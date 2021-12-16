#ifndef BEZIERCURVE_H
#define BEZIERCURVE_H

#include <glm.hpp>

class BezierCurve
{
public:
    BezierCurve();
    BezierCurve(glm::vec2 p0, glm::vec2 direction, float interPointDistance);

    glm::vec2 getPosition(float t);
    glm::vec2 getDerivative(float t);
    glm::vec2 lastPoint();
    glm::vec2 lastDirection();

private:
    glm::vec2 m_p0;
    glm::vec2 m_p1;
    glm::vec2 m_p2;

};

#endif // BEZIERCURVE_H
