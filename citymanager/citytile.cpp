#include "citytile.h"
#include <glm/gtx/transform.hpp>

CityTile::CityTile(float centerX, float centerZ, float sideLength = 20, int blocks = 10) :
    centerX(centerX),
    centerZ(centerZ),
    m_sideLength(sideLength),
    m_blocks(blocks),
    m_primitives(std::vector<CS123ScenePrimitive>()),
    m_transforms(std::vector<glm::mat4x4>())
{
    generateRoads();
    buildPrimitives();
}

void CityTile::generateRoads()
{
    std::vector<Edge> roadEdges = std::vector<Edge>();
    std::vector<glm::vec2> roadVertices= std::vector<glm::vec2>();

    float lowX = centerX - (m_sideLength / 2.f);
    float lowZ = centerZ - (m_sideLength / 2.f);
    float blockLength = m_sideLength / static_cast<float>(m_blocks);

    int vertexIndex = 0;

    //add z parallel roads
    for (int zi = 0; zi <= m_blocks; zi++){
        for (int xi = 0; xi < m_blocks; xi++){
            float zVal = lowZ + zi*blockLength;
            float xVal1 = lowX + xi*blockLength;
            float xVal2 = lowX + (xi+1)*blockLength;
            roadVertices.push_back(glm::vec2(xVal1, zVal));
            roadVertices.push_back(glm::vec2(xVal2, zVal));
            roadEdges.push_back(Edge(vertexIndex, vertexIndex+1));
            vertexIndex += 2;
        }
    }

    //add x parallel roads
    for (int xi = 0; xi <= m_blocks; xi++){
        for (int zi = 0; zi < m_blocks; zi++){
            float xVal = lowX + xi*blockLength;
            float zVal1 = lowZ + zi*blockLength;
            float zVal2 = lowZ + (zi+1)*blockLength;
            roadVertices.push_back(glm::vec2(xVal, zVal1));
            roadVertices.push_back(glm::vec2(xVal, zVal2));
            roadEdges.push_back(Edge(vertexIndex, vertexIndex+1));
            vertexIndex += 2;
        }
    }

    m_map = RoadMap(roadEdges, roadVertices);
}

void CityTile::buildPrimitives(){
    for (Edge e : m_map.edges) {
        glm::vec2 v1 = m_map.vertices[e.v1];
        glm::vec2 v2 = m_map.vertices[e.v2];
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

        m_primitives.push_back(primitive);
        m_transforms.push_back(translate*rotate*scale);
    }
}

RoadMap CityTile::getRoads(){
    return m_map;
}

std::vector<CS123ScenePrimitive> CityTile::getPrimitives(){
    return m_primitives;
}

std::vector<glm::mat4x4> CityTile::getTransforms(){
    return m_transforms;
}
