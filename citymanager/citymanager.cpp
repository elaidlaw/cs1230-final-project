#include "citymanager.h"

#include <glm/gtx/transform.hpp>
#include <cmath>
#include <iostream>

CityManager::CityManager()
{
    m_scene = std::make_shared<SceneviewScene>();
    setUpScene();
    generateRoads();
    addRoadsToScene();
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

void CityManager::generateRoads() {
    m_roadVertices.push_back(glm::vec2(0, 0));
    m_roadVertices.push_back(glm::vec2(1, 1));
    m_roadVertices.push_back(glm::vec2(0, 1));
    m_roadVertices.push_back(glm::vec2(1, 0));
    m_roadVertices.push_back(glm::vec2(2, 0));
    m_roadEdges.push_back(Edge(0, 2));
    m_roadEdges.push_back(Edge(2, 1));
    m_roadEdges.push_back(Edge(0, 3));
    m_roadEdges.push_back(Edge(3, 1));
    m_roadEdges.push_back(Edge(1, 4));
    m_roadEdges.push_back(Edge(3, 4));
}

void CityManager::addRoadsToScene() {
    for (Edge e : m_roadEdges) {
        glm::vec2 v1 = m_roadVertices[e.v1];
        glm::vec2 v2 = m_roadVertices[e.v2];
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
