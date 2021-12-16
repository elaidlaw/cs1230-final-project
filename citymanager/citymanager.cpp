#include "citymanager.h"

#include <glm/gtx/transform.hpp>
#include <cmath>
#include <iostream>
#include "glm/ext.hpp"

CityManager::CityManager(float bufferDistance, float tileSize, int blockSize) :
m_bufferDistance(bufferDistance),
m_tileSize(tileSize),
m_blockSize(blockSize),
m_tiles(std::vector<CityTile>()),
m_cameraCenter(glm::vec3(5.5f,2.5f,5.5f)),
m_cameraTranslation(glm::vec4(-0.1f,-0.0000001f,-0.1f, 0.0f)),
m_cameraTrajectory(BezierCurve()),
m_curvePosition(0.f)

//m_cameraTrajectory(glm::vec4(-0.01f,-0.000001f,-0.01f, 0.0f))
{
    m_scene = std::make_shared<SceneviewScene>();
    m_lastCameraCenter = glm::vec3(m_cameraCenter);
    setUpScene();
    //generateRoads();
    initializeTiles();
//    addRoadsToScene();
}

OpenGLScene* CityManager::getScene() {
    return m_scene.get();
}

void CityManager::setUpScene() {
    CS123SceneGlobalData global;
    global.ka = 0.5;
    global.kd = 0.5;

    CS123SceneLightData light;
    light.type = LightType::LIGHT_DIRECTIONAL;
    light.dir = glm::normalize(glm::vec4(-1, -1, -1, 0));
    light.color = glm::vec4(1, 1, 1, 1);

    m_scene->setGlobal(global);
    m_scene->addLight(light);
}

//void CityManager::generateRoads() {
//    m_roadVertices.push_back(glm::vec2(0, 0));
//    m_roadVertices.push_back(glm::vec2(1, 1));
//    m_roadVertices.push_back(glm::vec2(0, 1));
//    m_roadVertices.push_back(glm::vec2(1, 0));
//    m_roadVertices.push_back(glm::vec2(2, 0));
//    m_roadEdges.push_back(Edge(0, 2));
//    m_roadEdges.push_back(Edge(2, 1));
//    m_roadEdges.push_back(Edge(0, 3));
//    m_roadEdges.push_back(Edge(3, 1));
//    m_roadEdges.push_back(Edge(1, 4));
//    m_roadEdges.push_back(Edge(3, 4));
//}

//RoadMap CityManager::returnRoads() {
//    std::vector<Edge> roadEdges = std::vector<Edge>();
//    std::vector<glm::vec2> roadVertices= std::vector<glm::vec2>();
//    roadVertices.push_back(glm::vec2(0, 0));
//    roadVertices.push_back(glm::vec2(1, 1));
//    roadVertices.push_back(glm::vec2(0, 1));
//    roadVertices.push_back(glm::vec2(1, 0));
//    roadVertices.push_back(glm::vec2(2, 0));
//    roadEdges.push_back(Edge(0, 2));
//    roadEdges.push_back(Edge(2, 1));
//    roadEdges.push_back(Edge(0, 3));
//    roadEdges.push_back(Edge(3, 1));
//    roadEdges.push_back(Edge(1, 4));
//    roadEdges.push_back(Edge(3, 4));
//    return RoadMap(roadEdges, roadVertices);
//}

void CityManager::initializeTiles(){
    int xMax = floor((m_cameraCenter.x+m_bufferDistance)/m_tileSize);
    int xMin = floor((m_cameraCenter.x-m_bufferDistance)/m_tileSize);
    int zMax = floor((m_cameraCenter.z+m_bufferDistance)/m_tileSize);
    int zMin = floor((m_cameraCenter.z-m_bufferDistance)/m_tileSize);

    for(int xi = xMin; xi < xMax; xi++){
        for(int zi = zMin; zi < zMax; zi++){
            float xCenter = (static_cast<float>(xi)+0.5)*m_tileSize;
            float zCenter = (static_cast<float>(zi)+0.5)*m_tileSize;
            CityTile newTile = CityTile(xCenter, zCenter, m_tileSize, m_blockSize);
            m_tiles.push_back(newTile);
        }
    }
//    addRoadsToScene();
    addTilesToScene();
}

void CityManager::updateTiles(){

    std::vector<CityTile> validTiles = std::vector<CityTile>();
//    remove all of the tiles that are now further than buffer distance
    for (CityTile tile: m_tiles) {
        if (abs(tile.centerX - m_cameraCenter.x) < (m_bufferDistance + 0.5*m_tileSize) && abs(tile.centerZ - m_cameraCenter.z) < (m_bufferDistance + 0.5*m_tileSize)){
            validTiles.push_back(tile);
        }
    }
    m_tiles = validTiles;

    //add new tiles that weren't within range at our last camera position
    int lastxMax = floor((m_lastCameraCenter.x+m_bufferDistance)/m_tileSize);
    int lastxMin = floor((m_lastCameraCenter.x-m_bufferDistance)/m_tileSize);
    int lastzMax = floor((m_lastCameraCenter.z+m_bufferDistance)/m_tileSize);
    int lastzMin = floor((m_lastCameraCenter.z-m_bufferDistance)/m_tileSize);
    int xMax = floor((m_cameraCenter.x+m_bufferDistance)/m_tileSize);
    int xMin = floor((m_cameraCenter.x-m_bufferDistance)/m_tileSize);
    int zMax = floor((m_cameraCenter.z+m_bufferDistance)/m_tileSize);
    int zMin = floor((m_cameraCenter.z-m_bufferDistance)/m_tileSize);

    for(int xi = xMin; xi <= xMax; xi++){
        for(int zi = zMin; zi <= zMax; zi++){
            if (xi < lastxMin || xi > lastxMax || zi < lastzMin || zi > lastzMax){
                float xCenter = (static_cast<float>(xi)+0.5)*m_tileSize;
                float zCenter = (static_cast<float>(zi)+0.5)*m_tileSize;
                CityTile newTile = CityTile(xCenter, zCenter, m_tileSize, m_blockSize);
                m_tiles.push_back(newTile);
            }
        }
    }
//    addRoadsToScene();
    addTilesToScene();
}

void CityManager::addTilesToScene(){
    std::vector<CS123ScenePrimitive> allPrimitives;
    std::vector<glm::mat4x4> allTransforms;
    for (CityTile tile: m_tiles) {
        std::vector<CS123ScenePrimitive> primitives = tile.getPrimitives();
        std::vector<glm::mat4x4> transforms = tile.getTransforms();
        for (int i=0; i < primitives.size(); i++){
            allPrimitives.push_back(primitives[i]);
            allTransforms.push_back(transforms[i]);
        }
    }
    m_scene->loadPrimitives(allPrimitives, allTransforms);
}

void CityManager::addRoadsToScene() {
    m_scene->clearPrimitives();
    for (CityTile tile : m_tiles){
        RoadMap map = tile.getRoads();
        for (Edge e : map.edges) {
            glm::vec2 v1 = map.vertices[e.v1];
            glm::vec2 v2 = map.vertices[e.v2];
            glm::vec2 diff = v2 - v1;
            glm::vec2 midpoint = (v1 + v2) / 2.0f;
            CS123ScenePrimitive primitive;
            primitive.type = PrimitiveType::PRIMITIVE_CUBE;
            primitive.material.cDiffuse = glm::vec4(1, 1, 1, 1);
            primitive.material.cAmbient = glm::vec4(1, 1, 1, 1);

            // make the roads .05 thick, .2 wide, and the length of the distance between v1 and v2 (plus a little extra so it doesn't look weird)
            glm::mat4 scale = glm::scale(glm::vec3(glm::length(diff) + 0.2, 0.05, 0.2));
            glm::mat4 rotate = glm::rotate(-atan2f(diff.y, diff.x), glm::vec3(0, 1, 0));
            glm::mat4 translate = glm::translate(glm::vec3(midpoint.x, 0, midpoint.y));

            m_scene->addPrimitive(primitive, translate * rotate * scale);
        }
    }
}

//void CityManager::addRoadsToScene(RoadMap map) {
//    for (Edge e : map.edges) {
//        glm::vec2 v1 = map.vertices[e.v1];
//        glm::vec2 v2 = map.vertices[e.v2];
//        glm::vec2 diff = v2 - v1;
//        glm::vec2 midpoint = (v1 + v2) / 2.0f;
//        CS123ScenePrimitive primitive;
//        primitive.type = PrimitiveType::PRIMITIVE_CUBE;
//        primitive.material.cDiffuse = glm::vec4(1, 1, 1, 1);
//        primitive.material.cAmbient = glm::vec4(1, 1, 1, 1);

//        // make the roads .05 thick, .2 wide, and the length of the distance between v1 and v2 (plus a little extra so it doesn't look weird)
//        glm::mat4 scale = glm::scale(glm::vec3(glm::length(diff) + 0.2, 0.05, 0.2));
//        glm::mat4 rotate = glm::rotate(-atan2f(diff.y, diff.x), glm::vec3(0, 1, 0));
//        glm::mat4 translate = glm::translate(glm::vec3(midpoint.x, 0, midpoint.y));

//        m_scene->addPrimitive(primitive, translate * rotate * scale);
//    }
//}

glm::vec4 CityManager::nextCameraPosition() {
    float bezierIncrement = 0.02f;
    float bezierSegmentLength = 1.5f;
    m_curvePosition = m_curvePosition + bezierIncrement;

    //new bezier curve if we got to the end of the last one
    if (m_curvePosition > 1.f) {
        m_curvePosition -= 1.f;
        m_cameraTrajectory = BezierCurve(m_cameraTrajectory.lastPoint(), m_cameraTrajectory.lastDirection(), bezierSegmentLength);
    }

    //get camera center from curve
    glm::vec2 newCenter = m_cameraTrajectory.getPosition(m_curvePosition);

    m_lastCameraCenter = glm::vec3(m_cameraCenter);
    m_cameraCenter = glm::vec3(newCenter[0], m_cameraCenter[1], newCenter[1]);
    return glm::vec4(m_cameraCenter, 1.f);
}

glm::vec4 CityManager::getLookVector() {
    float verticalComponent = -0.02;
    glm::vec2 planarLook = m_cameraTrajectory.getDerivative(m_curvePosition);
    glm::vec4 look = glm::vec4(glm::normalize(glm::vec3(planarLook[0], verticalComponent, planarLook[1])), 0.f);
    return look;
}

glm::vec4 CityManager::getCameraTranslation() {
    m_lastCameraCenter = glm::vec3(m_cameraCenter);
    m_cameraCenter += m_cameraTranslation;
    return glm::vec4(m_cameraTranslation, 0.f);
}
