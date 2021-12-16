/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    setClip(1, 100);
    setHeightAngle(60);
    setAspectRatio(1);
    orientLook(glm::vec4(2, 2, 2, 1), glm::vec4(-1, -1, -1, 0), glm::vec4(0, 1, 0, 0));
}

void CamtransCamera::setAspectRatio(float a)
{
    // @TODO: [CAMTRANS] Fill this in...
    m_aspectRatio = a;
    m_thetaWTan = a * glm::tan(glm::radians(m_thetaH / 2));
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...

    return m_perspectiveTransformation * m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_rotationMatrix * m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    // @TODO: [CAMTRANS] Fill this in...
    return -m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // @TODO: [CAMTRANS] Fill this in...
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye = eye;
    m_up = up;
    m_w = glm::normalize(-look);
    m_v = glm::normalize(up - glm::dot(up, m_w) * m_w);
    m_u = glm::vec4(glm::cross(m_v.xyz(), m_w.xyz()), 0);
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    // @TODO: [CAMTRANS] Fill this in...
    m_thetaH = h;
    m_thetaWTan = m_aspectRatio * glm::tan(glm::radians(h / 2));
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    // @TODO: [CAMTRANS] Fill this in...
    m_eye += glm::vec4(v.xyz(), 0);
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 v = glm::cos(glm::radians(-degrees)) * m_v - glm::sin(glm::radians(-degrees)) * m_w;
    glm::vec4 w = glm::sin(glm::radians(-degrees)) * m_v + glm::cos(glm::radians(-degrees)) * m_w;
    m_v = v;
    m_w = w;
    updateViewMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 u = glm::cos(glm::radians(-degrees)) * m_u - glm::sin(glm::radians(-degrees)) * -m_w;
    glm::vec4 w = glm::sin(glm::radians(-degrees)) * m_u + glm::cos(glm::radians(-degrees)) * -m_w;
    m_u = u;
    m_w = -w;
    updateViewMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    glm::vec4 u = glm::cos(glm::radians(-degrees)) * m_u - glm::sin(glm::radians(-degrees)) * m_v;
    glm::vec4 v = glm::sin(glm::radians(-degrees)) * m_u + glm::cos(glm::radians(-degrees)) * m_v;
    m_u = u;
    m_v = v;
    updateViewMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // @TODO: [CAMTRANS] Fill this in...
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

void CamtransCamera::updateProjectionMatrix() {
    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix() {
    float c = -m_near / m_far;
    m_perspectiveTransformation = glm::transpose(glm::mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1 / (1 + c), c / (1 + c), 0, 0, -1, 0));
}

void CamtransCamera::updateScaleMatrix() {
    float scaleX = 1 / (m_far * m_thetaWTan);
    float scaleY = 1 / (m_far * glm::tan(glm::radians(m_thetaH / 2)));
    m_scaleMatrix = glm::transpose(glm::mat4(scaleX, 0, 0, 0, 0, scaleY, 0, 0, 0, 0, 1 / m_far, 0, 0, 0, 0, 1));
}

void CamtransCamera::updateViewMatrix() {
    updateTranslationMatrix();
    updateRotationMatrix();
}

void CamtransCamera::updateRotationMatrix() {
    m_rotationMatrix = glm::transpose(glm::mat4(m_u.x, m_u.y, m_u.z, 0, m_v.x, m_v.y, m_v.z, 0, m_w.x, m_w.y, m_w.z, 0, 0, 0, 0, 1));
}

void CamtransCamera::updateTranslationMatrix() {
    m_translationMatrix = glm::transpose(glm::mat4(1, 0, 0, -m_eye.x, 0, 1, 0, -m_eye.y, 0, 0, 1, -m_eye.z, 0, 0, 0, 1));
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    return m_w;
}
