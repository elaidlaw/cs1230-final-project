#include "utilities.h"
#include <memory>
#include <glm/gtx/transform.hpp>
#include <cmath>
#include <iostream>

const float PI = std::atan(1.0)*4.f;

void AddVertexAndNormal(std::unique_ptr<std::vector<float>>& vertList, glm::vec3 vertex, glm::vec3 normal){
    for(int i=0; i<3; i++){
        vertList->push_back(vertex[i]);
    }
    for(int i=0; i<3; i++){
        vertList->push_back(normal[i]);
    }
}

std::vector<float> GenCubeFace(glm::vec3 corner, glm::vec3 uDir, glm::vec3 vDir, glm::vec3 normal, int param)
{
/*
 * corner1 is the top left corner of the square
 * corner2 is the bottom right corner of the square
 * normal is the unit normal vector for the face'
 * fixedAxis is that axis with a constant value
 * param specifies the number of triangle faces to use
 * */
    std::unique_ptr<std::vector<float>> vertList = std::make_unique<std::vector<float>>();
    for(int i=0; i < param; i++){
        for(int j=0; j < param; j++){
            //define vertices
            glm::vec3 upperLeft = corner + static_cast<float>(i)/static_cast<float>(param) * uDir + static_cast<float>(j)/static_cast<float>(param) * vDir;
            glm::vec3 upperRight = corner + static_cast<float>(i+1)/static_cast<float>(param) * uDir + static_cast<float>(j)/static_cast<float>(param) * vDir;
            glm::vec3 lowerLeft = corner + static_cast<float>(i)/static_cast<float>(param) * uDir + static_cast<float>(j+1)/static_cast<float>(param) * vDir;
            glm::vec3 lowerRight = corner + static_cast<float>(i+1)/static_cast<float>(param) * uDir + static_cast<float>(j+1)/static_cast<float>(param) * vDir;

            //add vertices and normals to the list
            //one triangle in the square
            AddVertexAndNormal(vertList, upperLeft, normal);
            AddVertexAndNormal(vertList, lowerRight, normal);
            AddVertexAndNormal(vertList, upperRight, normal);
            //second triangle in the square
            AddVertexAndNormal(vertList, upperLeft, normal);
            AddVertexAndNormal(vertList, lowerLeft, normal);
            AddVertexAndNormal(vertList, lowerRight, normal);
        }
    }
    return *vertList;
}

std::vector<float> GenCircleFace(glm::vec3 center, glm::vec3 normal, glm::vec3 zeroAngleVec, int param1, int param2){
/*
 * center is the center point of the circular face
 * normal is the normal vector of the face
 * zeroAngleVec is a direction from the center that corresponds to an angle of zero
 * param1 and param2 define the tessalation resolution
 * */
    std::unique_ptr<std::vector<float>> vertList = std::make_unique<std::vector<float>>();
    int safeParam2 = std::max(param2, 3);
    //iterate over different radii
    for(int i = 0; i < param1; i++){
        //iterate over different angles
        for(int j=0; j < safeParam2; j++){
            glm::mat4 lowRotationMatrix = glm::rotate(static_cast<float>(j)/static_cast<float>(safeParam2)*2.f*PI, center);
            glm::mat4 highRotationMatrix = glm::rotate(static_cast<float>(j+1)/static_cast<float>(safeParam2)*2.f*PI, center);

            float lowRadius = static_cast<float>(i)/static_cast<float>(param1)*0.5;
            float highRadius = static_cast<float>(i+1)/static_cast<float>(param1)*0.5;

            //rotate the zeroangle vector around the center to the two relevant positions for a given "square" on the cirle surface
            glm::vec3 highRot = glm::vec3(highRotationMatrix*glm::vec4(zeroAngleVec, 1.f));
            glm::vec3 lowRot = glm::vec3(lowRotationMatrix*glm::vec4(zeroAngleVec, 1.f));

            //first triangle
            AddVertexAndNormal(vertList, center+highRot*highRadius, normal);
            AddVertexAndNormal(vertList, center+lowRot*lowRadius, normal);
            AddVertexAndNormal(vertList, center+lowRot*highRadius, normal);
            //second triangle (not done when i=0 because it would duplicate the first triangle)
            if(i > 0){
                AddVertexAndNormal(vertList, center+(highRot*highRadius), normal);
                AddVertexAndNormal(vertList, center+(highRot*lowRadius), normal);
                AddVertexAndNormal(vertList, center+(lowRot*lowRadius), normal);
            }
        }
    }
    return *vertList;
}

std::vector<float> GenConeUpper(glm::vec3 point, glm::vec3 axis, glm::vec3 baseZeroAngleVec, float height, float topRadius, float bottomRadius, int param1, int param2){
    /*
     * center is the center point of the circular face
     * normal is the normal vector of the face
     * zeroAngleVec is a direction from the center that corresponds to an angle of zero
     * param1 and param2 define the tessalation resolution
     * */
    std::unique_ptr<std::vector<float>> vertList = std::make_unique<std::vector<float>>();
    int safeParam2 = std::max(param2, 3);
    float marginalRadius = bottomRadius - topRadius;

    //iterate over different radii/heights
    for(int i = 0; i < param1; i++){
        //iterate over different angles
        for(int j=0; j < safeParam2; j++){
            glm::mat4 lowRotationMatrix = glm::rotate(static_cast<float>(j)/static_cast<float>(safeParam2)*2.f*PI, point);
            glm::mat4 highRotationMatrix = glm::rotate(static_cast<float>(j+1)/static_cast<float>(safeParam2)*2.f*PI, point);

            float lowRadius = topRadius + static_cast<float>(i)/static_cast<float>(param1)*marginalRadius;
            float highRadius = topRadius + static_cast<float>(i+1)/static_cast<float>(param1)*marginalRadius;

            //rotate the zeroangle vector around the center to the two relevant positions for a given "square" on the cirle surface
            glm::vec3 highRot = glm::vec3(highRotationMatrix*glm::vec4(baseZeroAngleVec, 1.f));
            glm::vec3 lowRot = glm::vec3(lowRotationMatrix*glm::vec4(baseZeroAngleVec, 1.f));
            glm::vec3 lowHeight = axis * static_cast<float>(i)/static_cast<float>(param1);
            glm::vec3 highHeight = axis * static_cast<float>(i+1)/static_cast<float>(param1);

            //calculate vectors and their normals
            glm::vec3 upperLeft = point + highRot*highRadius + highHeight;
            glm::vec3 upperRight = point + lowRot*highRadius + highHeight;
            glm::vec3 lowerLeft = point + highRot*lowRadius + lowHeight;
            glm::vec3 lowerRight = point + lowRot*lowRadius + lowHeight;
            glm::vec3 lowRotNormal = glm::normalize(-1.f * axis*marginalRadius + lowRot * height);
            glm::vec3 highRotNormal = glm::normalize(-1.f*axis*marginalRadius + highRot * height);


            //first triangle
            AddVertexAndNormal(vertList, upperLeft, highRotNormal);
            AddVertexAndNormal(vertList, lowerRight, lowRotNormal);
            AddVertexAndNormal(vertList, upperRight, lowRotNormal);
            //second triangle (not done when i=0 because it would duplicate the first triangle)
            if(i > 0 || topRadius > 0.f){
                AddVertexAndNormal(vertList, upperLeft, highRotNormal);
                AddVertexAndNormal(vertList, lowerLeft, highRotNormal);
                AddVertexAndNormal(vertList, lowerRight, lowRotNormal);
            }
        }
    }
    return *vertList;

}

std::vector<float> GenSphere(glm::vec3 axis, glm::vec3 zeroAngleVec, int latParam, int longParam){
/*
 * axis is the axis of the sphere, pointing south
 * zeroAngle vec defines the direction from the sphere center that corresponds to a longitudinal angle of 0
 * latParam and longParam control the sphere tesselation resolution
 * */

    std::unique_ptr<std::vector<float>> vertList = std::make_unique<std::vector<float>>();
    int safeLatParam = std::max(latParam, 2);
    int safeLongParam = std::max(longParam, 3);

    //iterate over different radii/heights
    for(int i = 0; i < safeLatParam; i++){
        //iterate over different longitude angles
        for(int j=0; j < safeLongParam; j++){
            glm::mat4 lowRotationMatrix = glm::rotate(static_cast<float>(j)/static_cast<float>(safeLongParam)*2.f*PI, -1.f * axis);
            glm::mat4 highRotationMatrix = glm::rotate(static_cast<float>(j+1)/static_cast<float>(safeLongParam)*2.f*PI, -1.f * axis);

            float lowLat = cos((static_cast<float>(i)/static_cast<float>(safeLatParam)) * PI) / 2.f;
            float highLat = cos((static_cast<float>(i+1)/static_cast<float>(safeLatParam)) * PI) / 2.f;

            float lowRadius =  sqrtf(0.5f*0.5f - pow(lowLat, 2));
            float highRadius =  sqrtf(0.5f*0.5f - pow(highLat, 2));

            //rotate the zeroangle vector around the center to the two relevant positions for a given "square" on the cirle surface
            glm::vec3 highRot = glm::vec3(highRotationMatrix*glm::vec4(zeroAngleVec, 1.f));
            glm::vec3 lowRot = glm::vec3(lowRotationMatrix*glm::vec4(zeroAngleVec, 1.f));
            glm::vec3 lowHeight = -1.f * axis * lowLat;
            glm::vec3 highHeight = -1.f * axis * highLat;

            //calculate vectors and their normals
            glm::vec3 upperLeft = highRot*highRadius + highHeight;
            glm::vec3 upperRight = lowRot*highRadius + highHeight;
            glm::vec3 lowerLeft = highRot*lowRadius + lowHeight;
            glm::vec3 lowerRight = lowRot*lowRadius + lowHeight;


            //first triangle
            if (i != 0){
                AddVertexAndNormal(vertList, upperLeft, glm::normalize(upperLeft));
                AddVertexAndNormal(vertList, lowerLeft, glm::normalize(lowerLeft));
                AddVertexAndNormal(vertList, lowerRight, glm::normalize(lowerRight));
            }
            //second triangle (not done when i=0 because it would duplicate the first triangle)
            if(i != (safeLatParam-1)){
                AddVertexAndNormal(vertList, upperLeft, glm::normalize(upperLeft));
                AddVertexAndNormal(vertList, lowerRight, glm::normalize(lowerRight));
                AddVertexAndNormal(vertList, upperRight, glm::normalize(upperRight));
            }
        }
    }

    return * vertList;
}


