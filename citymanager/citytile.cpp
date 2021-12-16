#include "citytile.h"
#include "buildingcolor.h"
#include <glm/gtx/transform.hpp>
#include <fstream>
#include <iostream>

#include <iostream>
#include "math.h"

/**
 * Returns a pseudo-random value between -1.0 and 1.0 for the given row and
 * column.
 */
float randValue(int row, int col)
{
    float output = glm::fract(sin(row * 127.1f + col * 311.7f) * 43758.5453123f);
    return pow(output/0.94, 4.0);
}

float perlinNoise(float x, float z){
    float y = 0.f;
    int lowOctave = 0;
    int highOctave = 3;

    float dispersionTerm = 5.;

    for(int octave = lowOctave; octave <= highOctave; octave++){
        float octavePower = glm::exp2(static_cast<float>(octave));
        float rowPosition = x / (dispersionTerm / octavePower) + 100. * octave;
        float colPosition = z / (dispersionTerm / octavePower) + 100. * octave;
        float A = randValue(glm::floor(rowPosition), glm::floor(colPosition));
        float B = randValue(glm::floor(rowPosition), glm::ceil(colPosition));
        float C = randValue(glm::ceil(rowPosition), glm::floor(colPosition));
        float D = randValue(glm::ceil(rowPosition), glm::ceil(colPosition));

    //    Bilinear Interpolation
    //        float abMix = glm::mix(A, B, glm::fract(colPosition));
    //        float cdMix = glm::mix(C, D, glm::fract(colPosition));
    //        position.y += glm::mix(abMix, cdMix, glm::fract(rowPosition)) / octavePower;

    //    Bicubic Interpolation
        float x = glm::fract(colPosition);
        float abMix = glm::mix(A, B, x*x*(3-2*x));
        float cdMix = glm::mix(C, D, x*x*(3-2*x));
        x = glm::fract(rowPosition);
        y += glm::mix(abMix, cdMix, x*x*(3-2*x)) / sqrt(octavePower);
    }
    return y;
}

CityTile::CityTile(float centerX, float centerZ, float sideLength = 20, int blocks = 10) :
    centerX(centerX),
    centerZ(centerZ),
    m_sideLength(sideLength),
    m_blocks(blocks),
    m_primitives(std::vector<CS123ScenePrimitive>()),
    m_transforms(std::vector<glm::mat4x4>()),
    m_tileTransform(glm::translate(glm::vec3(centerX - sideLength / 2, 0, centerZ - sideLength / 2)) * glm::scale(glm::vec3(sideLength / 100, 1, sideLength / 100)))
{
    loadTile();
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

void CityTile::loadTile() {
    std::ifstream infile("F:/Brown/cs1230/2.txt");

    std::vector<Edge> roadEdges = std::vector<Edge>();
    std::vector<glm::vec2> roadVertices = std::vector<glm::vec2>();

    char type;
    while(infile >> type) {
        if (type == 'v') {
            float x, y;
            infile >> x >> y;
            roadVertices.push_back(glm::vec2(x, y));
        }
        if (type == 'e') {
            int x, y;
            infile >> x >> y;
            roadEdges.push_back(Edge(x, y));
        }
        if (type == 'b') {
            float x, y, a, w, h;
            infile >> x >> y >> a >> w >> h;
            glm::vec4 pos = m_tileTransform * glm::vec4(x, 0, y, 1);
            buildings.push_back(Building(glm::vec2(x, y), glm::vec2(w, h), a, perlinNoise(pos.x, pos.z) * 5));
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
        primitive.material.cDiffuse = glm::vec4(0.5, 0.5, 0.5, 0.5);
        primitive.material.cAmbient = glm::vec4(0.2, 0.2, 0.2, 0.1);

        // make the roads .05 thick, .2 wide, and the length of the distance between v1 and v2 (plus a little extra so it doesn't look weird)
        glm::mat4 scale = glm::scale(glm::vec3(glm::length(diff) + 1, 0.1f, 1));
        glm::mat4 rotate = glm::rotate(-atan2f(diff.y, diff.x), glm::vec3(0, 1, 0));
        glm::mat4 translate = glm::translate(glm::vec3(midpoint.x, 0, midpoint.y));

        m_primitives.push_back(primitive);
        m_transforms.push_back(m_tileTransform * translate*rotate*scale);
    }

    for (Building b : buildings) {
        CS123ScenePrimitive primitive;
        primitive.type = PrimitiveType::PRIMITIVE_CUBE;
        glm::vec4 buildingColor = chooseBuildingColor(b.height);
        primitive.material.cDiffuse = buildingColor;
        primitive.material.cAmbient = buildingColor;

        glm::mat4 scale = glm::scale(glm::vec3(b.size.y, b.height, b.size.x));
        glm::mat4 rotate = glm::rotate(-b.angle, glm::vec3(0, 1, 0));
        glm::mat4 translate = glm::translate(glm::vec3(b.center.x, b.height / 2, b.center.y));
        m_primitives.push_back(primitive);
        m_transforms.push_back(m_tileTransform * translate*rotate*scale);
    }

    CS123ScenePrimitive primitive;
    primitive.type = PrimitiveType::PRIMITIVE_CUBE;
    primitive.material.cDiffuse = glm::vec4(0, 0, 0, 0.1);
    primitive.material.cAmbient = glm::vec4(0.1, 0.1, 0.1, 0.9);

    // make the roads .05 thick, .2 wide, and the length of the distance between v1 and v2 (plus a little extra so it doesn't look weird)
    glm::mat4 scale = glm::scale(glm::vec3(100, 1, 100));
    glm::mat4 translate = glm::translate(glm::vec3(50, -0.5, 50));

    m_primitives.push_back(primitive);
    m_transforms.push_back(m_tileTransform * translate*scale);

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
