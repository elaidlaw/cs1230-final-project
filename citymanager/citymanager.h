#ifndef CITYMANAGER_H
#define CITYMANAGER_H

#include "scenegraph/SceneviewScene.h"
#include <memory>
#include "citytile.h"

class CityManager
{
public:
    CityManager(float bufferDistance, float tileSize, int blockSize);

    OpenGLScene* getScene();
    void updateTiles();
    void addRoadsToScene();
    glm::vec4 getCameraTranslation();

private:
    float m_bufferDistance; //max distance at which city is rendered
    float m_tileSize; //the dimensions of each square tile
    int m_blockSize; //the dimension of each square in city blocks
    std::shared_ptr<SceneviewScene> m_scene;
    std::vector<glm::vec2> m_roadVertices;
    std::vector<Edge> m_roadEdges;
    std::vector<CityTile> m_tiles;

    glm::vec3 m_lastCameraCenter;
    glm::vec3 m_cameraCenter;
    glm::vec4 m_cameraTrajectory;

//    void generateRoads();
    void initializeTiles();
    void setUpScene();
    void addTilesToScene();
};

#endif // CITYMANAGER_H
