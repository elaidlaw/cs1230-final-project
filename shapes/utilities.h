#include "glm.hpp"
#include <vector>

#ifndef UTILITIES_H
#define UTILITIES_H

void AddVertexAndNormal(std::vector<float> *vertList, glm::vec3 vertex, glm::vec3 normal);
std::vector<float> GenCubeFace(glm::vec3 corner, glm::vec3 uDir, glm::vec3 vDir, glm::vec3 normal, int param);
std::vector<float> GenCircleFace(glm::vec3 center, glm::vec3 normal, glm::vec3 zeroAngleVec, int param1, int param2);
std::vector<float> GenConeUpper(glm::vec3 point, glm::vec3 axis, glm::vec3 baseZeroAngleVec, float height, float topRadius, float bottomRadius, int param1, int param2);
std::vector<float> GenSphere(glm::vec3 axis, glm::vec3 zeroAngleVec, int latParam, int longParam);

#endif // UTILITIES_H


