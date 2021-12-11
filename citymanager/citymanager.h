#ifndef CITYMANAGER_H
#define CITYMANAGER_H

#include "scenegraph/SceneviewScene.h"
#include <memory>

struct Edge {
    int v1;
    int v2;

    Edge(int v1, int v2) : v1(v1), v2(v2) {}

};

class CityManager
{
public:
    CityManager();

    OpenGLScene* getScene();
    glm::vec4 getCameraTranslation();

private:
    std::shared_ptr<SceneviewScene> m_scene;
    std::vector<glm::vec2> m_roadVertices;
    std::vector<Edge> m_roadEdges;
    glm::vec3 m_cameraCenter;
    glm::vec4 m_cameraTrajectory;

    void generateRoads();
    void addRoadsToScene();
    void setUpScene();
};

#endif // CITYMANAGER_H
