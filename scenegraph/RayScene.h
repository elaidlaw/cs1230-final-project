#ifndef RAYSCENE_H
#define RAYSCENE_H

#include "Scene.h"
#include "shapes/implicitshape.h"
#include <vector>


/**
 * @class RayScene
 *
 *  Students will implement this class as necessary in the Ray project.
 */
class RayScene : public Scene {
public:
    RayScene(Scene &scene);
    virtual ~RayScene();
    std::vector<glm::vec4> rayTrace(int width, int height);

private:
    std::vector<std::shared_ptr<ImplicitShape>> m_implicitshapes;
    std::vector<std::shared_ptr<glm::mat4x4>> m_inverseTransforms;
    int m_maxrecursiondepth;
//    std::vector<std::shared_ptr<glm::mat4x4>> m_inverseRotations;
    //the index of the intersected object
    std::vector<glm::vec4> viewPlanePointsCameraSpace(int width, int height);
    std::vector<glm::vec4> viewPlanePointsWorldSpace(int width, int height);
    std::tuple<double, int> getTValue(glm::vec4 point, glm::vec4 viewPlanePoint);
    glm::vec4 getPosition(glm::vec4 point, glm::vec4 viewPlanePoint, double t);
    glm::vec4 getNormal(glm::vec4 point, int objectIndex);
    glm::vec4 computeLighting(glm::vec4 point, glm::vec4 viewPlanePoint, double t, int objectIndex, int recursionDepth);
    glm::vec4 computeTextureColor(glm::vec4 position, int objectIndex);
    bool shadowRay(glm::vec4 surfacePoint, glm::vec4 lightDirection, float lightDist);
    static glm::vec4 QrgbToVec4(QRgb color);
    static glm::vec4 reflectRay(glm::vec4 ray, glm::vec4 normal);
    glm::vec4 rayLighting(glm::vec4 point, glm::vec4 viewPlanePoint, int recursionDepth);
};

#endif // RAYSCENE_H
