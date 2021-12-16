#include "buildingcolor.h"
#include <iostream>
glm::vec4 glassyBlueColor(){
    glm::vec3 baseline = glm::vec3(116.f/255.f, 123.f/255.f, 150.f/255.f);
    float variation = 0.1f;
    float multiplier = static_cast<float>(rand() % 1000) / 500.f + 0.5f;
    glm::vec3 color = baseline + glm::vec3(variation*multiplier);
    return glm::vec4(color, 1.f);
}

glm::vec4 darkBrownColor(){
    glm::vec3 baseline = glm::vec3(41.f/255.f, 28.f/255.f, 26.f/255.f);
    float variation = 0.05f;
    float multiplier1 = static_cast<float>(rand() % 1000) / 500.f - 1.f;
    float multiplier2 = static_cast<float>(rand() % 1000) / 500.f - 1.f;
    float multiplier3 = static_cast<float>(rand() % 1000) / 500.f - 1.f;
    glm::vec3 color = baseline + glm::vec3(variation*multiplier1, variation*multiplier2, variation*multiplier3);
    return glm::vec4(color, 1.f);
}

glm::vec4 brickColor(){
    glm::vec3 baseline = glm::vec3(87.f/255.f, 32.f/255.f, 25.f/255.f);
    float variation = 0.08f;
    float multiplier = static_cast<float>(rand() % 1000) / 500.f - 1.f;
    glm::vec3 color = baseline + glm::vec3(variation*multiplier);
    return glm::vec4(color, 1.f);
}

glm::vec4 concreteColor(){
    glm::vec3 baseline = glm::vec3(196.f/255.f, 184.f/255.f, 183.f/255.f);
    float variation = 0.2f;
    float multiplier = static_cast<float>(rand() % 1000) / 500.f - 1.f;
    glm::vec3 color = baseline + glm::vec3(variation*multiplier);
    return glm::vec4(color, 1.f);
}

glm::vec4 chooseBuildingColor(float buildingHeight){
    buildingHeight = buildingHeight / 4.f;
    std::cout << buildingHeight << std::endl;
    float randNum = static_cast<float>(rand()%10000)/10000.f;
    //Tall buildings are mostly glass, some concrete
    if (buildingHeight > 1.6f){
        if (randNum < 0.6){
            return glassyBlueColor();
        } else {
            return concreteColor();
        }
    } //Short buildings are usually brick, stone, wood
    else if (buildingHeight < 0.3f) {
        if (randNum < 0.3){
            return brickColor();
        } else if (randNum < 0.7) {
            return darkBrownColor();
        }else {
            return concreteColor();
        }
    } else { //Medium height buildings are mostly concrete
        if (randNum < 0.9){
            return concreteColor();
        } else if (randNum < 0.95) {
            return darkBrownColor();
        }else {
            return brickColor();
        }
    }

}
