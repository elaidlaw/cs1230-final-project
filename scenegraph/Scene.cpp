#include "Scene.h"
#include "Camera.h"
#include "CS123ISceneParser.h"

#include "glm/gtx/transform.hpp"
#include <memory>


Scene::Scene()
{
}

Scene::Scene(Scene &scene)
{
    // We need to set the global constants to one when we duplicate a scene,
    // otherwise the global constants will be double counted (squared)
//    CS123SceneGlobalData global = { 1, 1, 1, 1};
//    setGlobal(global);

    // TODO [INTERSECT]
    // Make sure to copy over the lights and the scenegraph from the old scene,
    // as well as any other member variables your new scene will need.

    //set the global data
    CS123SceneGlobalData global = *scene.m_globalData;
    this->setGlobal(global);

    //set the camera data
    CS123SceneCameraData camera = *scene.m_cameraData;
    this->setCamera(camera);

    //add lights
    std::vector<std::shared_ptr<CS123SceneLightData>> lights = scene.m_lightData;
    for(int i = 0; i < lights.size(); i++){
        this->addLight(*lights[i]);
    }

    //add primitives and their transforms
    std::vector<std::shared_ptr<CS123ScenePrimitive>> primitives = scene.m_primitives;
    std::vector<std::shared_ptr<glm::mat4x4>> transforms = scene.m_transforms;
    for(int i = 0; i < primitives.size(); i++){
        this->addPrimitive(*primitives[i], *transforms[i]);
    }

}

Scene::~Scene()
{
    // Do not delete m_camera, it is owned by SupportCanvas3D
}

void Scene::parse(Scene *sceneToFill, CS123ISceneParser *parser) {
    // TODO: load scene into sceneToFill using setGlobal(), addLight(), addPrimitive(), and
    CS123SceneGlobalData globalData;
    parser->getGlobalData(globalData);
    sceneToFill->setGlobal(globalData);

    CS123SceneCameraData cameraData;
    parser->getCameraData(cameraData);
    sceneToFill->setCamera(cameraData);


    //Save the light data
    int numLights = parser->getNumLights();
    CS123SceneLightData lightData;
    for (int i=0; i<numLights;i++){
        parser->getLightData(i,lightData);
        sceneToFill->addLight(lightData);
    }

    //Save the object data

    CS123SceneNode *root = parser->getRootNode();
    parseTree(sceneToFill, glm::mat4(1.f), root);


    std::vector<CS123ScenePrimitive> m_primitives;
    std::vector<glm::mat4x4> m_transforms;
}


void Scene::parseTree(Scene *sceneToFill, glm::mat4x4 cumMatrix, CS123SceneNode* currNode){
    //add matrices to our cumulative matrix
    for (int i = 0; i < currNode->transformations.size(); i++){
        cumMatrix = cumMatrix*parseTransform(currNode->transformations[i]);
    }
    //add primitives to the scene
    for (int i = 0; i < currNode->primitives.size(); i++){
        sceneToFill->addPrimitive(*(currNode->primitives[i]), cumMatrix);
    }
    //recur for children
    for (int i = 0; i < currNode->children.size(); i++){
        parseTree(sceneToFill, cumMatrix, currNode->children[i]);
    }
}

glm::mat4x4 Scene::parseTransform(CS123SceneTransformation *transform){
if(transform->type == TRANSFORMATION_TRANSLATE){
    return glm::translate(transform->translate);
}
if(transform->type == TRANSFORMATION_ROTATE){
    return glm::rotate(transform->angle, transform->rotate);
}
if(transform->type == TRANSFORMATION_SCALE){
    return glm::scale(transform->scale);
}
if(transform->type == TRANSFORMATION_MATRIX){
    return transform->matrix;
}
return glm::mat4(1.f);
}

void Scene::setCamera(const CS123SceneCameraData &camera) {
    m_cameraData = std::make_shared<CS123SceneCameraData>(camera);
}

void Scene::addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix) {
    m_primitives.push_back(std::make_shared<CS123ScenePrimitive>(scenePrimitive));
    m_transforms.push_back(std::make_shared<glm::mat4x4>(matrix));
    //also add the QImage for texture mapping
//    QString filePath = QString::fromStdString(scenePrimitive.material.textureMap.filename);
//    QImage textureImage = QImage(filePath);
    QImage textureImage = QImage();
    m_primitiveTextures.push_back(std::make_shared<QImage>(textureImage));
    m_blendValues.push_back(scenePrimitive.material.blend);
}

void Scene::clearPrimitives(){
    m_primitives = std::vector<std::shared_ptr<CS123ScenePrimitive>>();
    m_transforms = std::vector<std::shared_ptr<glm::mat4x4>>();
    m_primitiveTextures = std::vector<std::shared_ptr<QImage>>();
    m_blendValues = std::vector<float>();
}

void Scene::addLight(const CS123SceneLightData &sceneLight) {
    m_lightData.push_back(std::make_shared<CS123SceneLightData>(sceneLight));
}

void Scene::setGlobal(const CS123SceneGlobalData &global) {
    m_globalData = std::make_shared<CS123SceneGlobalData>(global);
}

