/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include <math.h>
#include <glm/gtx/transform.hpp>

CamtransCamera::CamtransCamera()
{
    // @TODO: [CAMTRANS] Fill this in...
    setClip(1.f, 30.f);
    setHeightAngle(60.f);
    setAspectRatio(1.f);
    translate(glm::vec4(2.f,2.f,2.f,0.f));
    orientLook(glm::vec4(2.f, 2.f, 2.f, 1.f), glm::vec4(-1.f,-1.f,-1.f,0.f), glm::vec4(0.f,1.f,0.f,0.f));

}

void CamtransCamera::setAspectRatio(float a)
{
    m_aspectRatio = a;
    updateProjectionMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    return m_perspectiveTransformation*m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    return m_rotationMatrix*m_translationMatrix;
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    return m_eye;
}

glm::vec4 CamtransCamera::getLook() const {
    return -1.f*m_w;
}

glm::vec4 CamtransCamera::getUp() const {
    return m_up;
}

float CamtransCamera::getAspectRatio() const {
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
    m_eye = eye;
    m_up = up;
    //TODO: Do we have to handle -y, +y look vectors?
    m_w = glm::vec4(-1.f * glm::normalize(glm::vec3(look)), 0.f);
//    m_u = glm::vec4(glm::normalize(glm::cross(glm::vec3(up), glm::vec3(m_w))), 0.f);
//    m_v = glm::vec4(glm::cross(glm::vec3(m_w), glm::vec3(m_u)), 0.f);
    m_v = up - glm::dot(glm::vec3(up), glm::vec3(m_w))*m_w;
    m_u = glm::vec4(glm::cross(glm::vec3(m_v), glm::vec3(m_w)), 0.f);
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    m_thetaH = h;
    updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    m_eye = m_eye + v;
    updateViewMatrix();
}

void CamtransCamera::rotateU(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float radians = glm::radians(-degrees);
    glm::mat4 invView = glm::inverse(getViewMatrix());
    glm::vec3 newVCamSpace = cos(radians)*glm::vec3(0.f,1.f,0.f) - sin(radians)*glm::vec3(0.f,0.f,1.f);
    glm::vec3 newWCamSpace = cos(radians)*glm::vec3(0.f,0.f,1.f) + sin(radians)*glm::vec3(0.f,1.f,0.f);
    m_v = invView * glm::vec4(newVCamSpace, 1.f) - m_eye;
    m_w = invView * glm::vec4(newWCamSpace, 1.f) - m_eye;
    updateRotationMatrix();
}

void CamtransCamera::rotateV(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float radians = glm::radians(-degrees);
    glm::mat4 invView = glm::inverse(getViewMatrix());
    glm::vec3 newUCamSpace = cos(radians)*glm::vec3(1.f,0.f,0.f) + sin(radians)*glm::vec3(0.f,0.f,1.f);
    glm::vec3 newWCamSpace = cos(radians)*glm::vec3(0.f,0.f,1.f) - sin(radians)*glm::vec3(1.f,0.f,0.f);
    m_u = invView * glm::vec4(newUCamSpace, 1.f) - m_eye;
    m_w = invView * glm::vec4(newWCamSpace, 1.f) - m_eye;
    updateRotationMatrix();
}

void CamtransCamera::rotateW(float degrees) {
    // @TODO: [CAMTRANS] Fill this in...
    float radians = glm::radians(-degrees);
    glm::mat4 invView = glm::inverse(getViewMatrix());
    glm::vec3 newVCamSpace = cos(radians)*glm::vec3(0.f,1.f,0.f) + sin(radians)*glm::vec3(1.f,0.f,0.f);
    glm::vec3 newUCamSpace = cos(radians)*glm::vec3(1.f,0.f,0.f) - sin(radians)*glm::vec3(0.f,1.f,0.f);
    m_v = invView * glm::vec4(newVCamSpace, 1.f) - m_eye;
    m_u = invView * glm::vec4(newUCamSpace, 1.f) - m_eye;
//    m_v = invView * glm::vec4(0.f, 1.f, 0.f, 0.f);
//    m_u = invView * glm::vec4(1.f, 0.f, 0.f, 0.f);
    updateRotationMatrix();
}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    m_near = nearPlane;
    m_far = farPlane;
}

void CamtransCamera::updateProjectionMatrix(){
    updatePerspectiveMatrix();
    updateScaleMatrix();
}

void CamtransCamera::updatePerspectiveMatrix(){
    float c = -m_near/m_far;
    m_perspectiveTransformation = glm::mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, -1.f/(1.f+c), -1.f, 0.f, 0.f, c/(1.f+c), 0.f);
}

void CamtransCamera::updateScaleMatrix(){

    float height = m_far*tan(glm::radians(m_thetaH)/2.f);
    float width = m_aspectRatio*height;
    m_scaleMatrix = glm::mat4(1.f/width, 0.f, 0.f, 0.f, 0.f, 1.f/height, 0.f, 0.f, 0.f, 0.f, 1.f/m_far, 0.f, 0.f, 0.f, 0.f, 1.f);
}

void CamtransCamera::updateViewMatrix(){
    updateTranslationMatrix();
    updateRotationMatrix();
}

void CamtransCamera::updateRotationMatrix(){
//    float PI = 3.1415926535897932384626f;
//    glm::vec3 vec1 = glm::vec3(0.f,0.f,1.f);
//    glm::vec3 vec2 = glm::vec3(m_w);
//    glm::vec3 rotation_axis = glm::normalize(glm::cross(vec1, vec2));
//    float rotation_angle = atan2(glm::dot(vec1, vec2), glm::length(glm::cross(vec1, vec2)));
//    if(rotation_angle < 0.f){
//            rotation_angle = 2.f*PI - rotation_angle;
//    }

//    //Rodrigues Rotation Formula
//    glm::mat3 rCross = glm::mat3(0.f, rotation_axis[2], -rotation_axis[1], -rotation_axis[2], 0.f, -rotation_axis[0], rotation_axis[1], -rotation_axis[0], 0.f);
//    glm::mat3 firstTerm = glm::mat3(rotation_axis[0]*rotation_axis[0], rotation_axis[0]*rotation_axis[1], rotation_axis[0]*rotation_axis[2],\
//                                    rotation_axis[1]*rotation_axis[0], rotation_axis[1]*rotation_axis[1], rotation_axis[1]*rotation_axis[2],\
//                                    rotation_axis[2]*rotation_axis[0], rotation_axis[2]*rotation_axis[0], rotation_axis[2]*rotation_axis[0]);
//    glm::mat3 firstTerm = glm::outerProduct(rotation_axis, rotation_axis);
//    glm::mat3 secondTerm = sin(rotation_angle)*rCross;
//    glm::mat3 thirdTerm = -1.f * cos(rotation_angle) * rCross * rCross;
//    m_rotationMatrix = glm::mat4(firstTerm + secondTerm + thirdTerm);
//    m_rotationMatrix = glm::rotate(glm::degrees(rotation_angle), rotation_axis);
    m_rotationMatrix = glm::mat4(m_u[0], m_v[0], m_w[0], 0.f, m_u[1], m_v[1],m_w[1], 0.f, m_u[2], m_v[2], m_w[2], 0.f, 0.f, 0.f, 0.f, 1.f);
}

void CamtransCamera::updateTranslationMatrix(){
    m_translationMatrix = glm::mat4(1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, -m_eye[0], -m_eye[1], -m_eye[2], 1.f);
}

glm::vec4 CamtransCamera::getU() const {
    return m_u;
}

glm::vec4 CamtransCamera::getV() const{
    return m_v;
}

glm::vec4 CamtransCamera::getW() const{
    return m_w;
}


