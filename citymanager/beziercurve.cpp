#include "beziercurve.h"

#include "math.h"

glm::vec2 randomDirection(){
    float pi = 3.1415926532384626;
    int randomInteger = rand() % 1000;
    float randomFloat = static_cast<float>(randomInteger) / 1000.f;
    return glm::vec2(cos(randomFloat*2.f*pi), sin(randomFloat*2.f*pi));
}

BezierCurve::BezierCurve() :
    m_p0(glm::vec2(0.f, 0.f)),
    m_p1(glm::vec2(1.f, 1.f)),
    m_p2(glm::vec2(2.f, 2.f))
{
}

BezierCurve::BezierCurve(glm::vec2 p0, glm::vec2 direction, float interPointDistance) :
    m_p0(p0),
    m_p1(p0 + glm::normalize(direction)*interPointDistance)
{
    float momentum = 0.7; //smooth the direction
    m_p2 = m_p1 + momentum*interPointDistance*direction + (1.f-momentum)*interPointDistance*randomDirection();
}


glm::vec2 BezierCurve::getPosition(float t) {
    //t should be in range 0->1
    return (1.f-t)*((1.f-t)*m_p0 + t*m_p1) + t*((1.f-t)*m_p1 + t*m_p2);
//    return glm::vec2(1.f, 1.f);
}

glm::vec2 BezierCurve::getDerivative(float t) {
    //t should be in range 0->1
    return 2.f*(1.f-t)*(m_p1-m_p0) + 2.f*t*(m_p2-m_p1);
//    return glm::vec2(0.5f,0.5f);
}

glm::vec2 BezierCurve::lastPoint(){
    return m_p2;
}

glm::vec2 BezierCurve::lastDirection() {
    return glm::normalize(m_p2-m_p1);
}
