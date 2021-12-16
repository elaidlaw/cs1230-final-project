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

class CityTile
{
public:
    CityTile(float centerX, float centerZ, float sideLength, int blocks);
    RoadMap getRoads();
    std::vector<CS123ScenePrimitive> getPrimitives();
    std::vector<glm::mat4x4> getTransforms();
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
    void generateRoads();
    void buildPrimitives();
};

#endif // CITYTILE_H
