#include "citytile.h"

CityTile::CityTile(float centerX, float centerZ, float sideLength = 20, int blocks = 10) :
    centerX(centerX),
    centerZ(centerZ),
    m_sideLength(sideLength),
    m_blocks(blocks)
{
    generateRoads();
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

RoadMap CityTile::getRoads(){
    return m_map;
}
