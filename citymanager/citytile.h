#ifndef CITYTILE_H
#define CITYTILE_H

#include "scenegraph/SceneviewScene.h"
#include <vector>
#include <glm.hpp>


struct Edge {
    int v1;
    int v2;

    Edge(int v1, int v2) : v1(v1), v2(v2) {}

};

struct RoadMap {
RoadMap(std::vector<Edge> edges, std::vector<glm::vec2> vertices) : edges(edges), vertices(vertices) {}
RoadMap() : edges(std::vector<Edge>()), vertices(std::vector<glm::vec2>()) {}

std::vector<Edge> edges;
std::vector<glm::vec2> vertices;


};

struct Building {

    Building(glm::vec2 center, glm::vec2 size, float angle, float height):
        center(center),
        size(size),
        angle(angle),
        height(height) {}

    glm::vec2 center;
    glm::vec2 size;
    float angle;
    float height;
};

class CityTile
{
public:
    CityTile(float centerX, float centerZ, float sideLength, int blocks);
    RoadMap getRoads();
    std::vector<CS123ScenePrimitive> getPrimitives();
    std::vector<glm::mat4x4> getTransforms();
    glm::mat4 m_tileTransform;
    float centerX;
    float centerZ;

private:
//    float m_centerX;
//    float m_centerZ;
    float m_sideLength;
    int m_blocks; //side length in city blocks
    RoadMap m_map;
    std::vector<CS123ScenePrimitive> m_primitives;
    std::vector<glm::mat4x4> m_transforms;
    std::vector<Building> buildings;
    void generateRoads();
    void buildPrimitives();
    void loadTile();
};

#endif // CITYTILE_H
