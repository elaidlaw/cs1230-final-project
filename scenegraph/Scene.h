#ifndef SCENE_H
#define SCENE_H

#include "CS123SceneData.h"

#include <memory>
#include <QImage>
#include <QString>

class Camera;
class CS123ISceneParser;


/**
 * @class Scene
 *
 * @brief This is the base class for all scenes. Modify this class if you want to provide
 * common functionality to all your scenes.
 */
class Scene {
public:
    Scene();
    Scene(Scene &scene);
    virtual ~Scene();

    virtual void settingsChanged() {}

    static void parse(Scene *sceneToFill, CS123ISceneParser *parser);

    static void parseTree(Scene *sceneToFill, glm::mat4x4 cumMatrix, CS123SceneNode* currNode);

    static glm::mat4x4 parseTransform(CS123SceneTransformation *transform);

    // Adds a primitive to the scene.
    virtual void addPrimitive(const CS123ScenePrimitive &scenePrimitive, const glm::mat4x4 &matrix);

    //Clears all primitives from the scene
    void clearPrimitives();

    //Loads primitives from tiles
//    void loadTiles(std::vector<CityTile> tiles);
    void loadPrimitives(std::vector<CS123ScenePrimitive> primitives, std::vector<glm::mat4x4> transforms);

    // Adds a light to the scene.
    virtual void addLight(const CS123SceneLightData &sceneLight);

    // Sets the global data for the scene.
    virtual void setGlobal(const CS123SceneGlobalData &global);

    //Sets the camera data for the scene
    virtual void setCamera(const CS123SceneCameraData &camera);

protected:

    std::shared_ptr<CS123SceneGlobalData> m_globalData;
    std::shared_ptr<CS123SceneCameraData> m_cameraData;
    std::vector<std::shared_ptr<CS123SceneLightData>> m_lightData;
    std::vector<std::shared_ptr<CS123ScenePrimitive>> m_primitives;
    std::vector<std::shared_ptr<glm::mat4x4>> m_transforms;
    std::vector<std::shared_ptr<QImage>> m_primitiveTextures;
    std::vector<float> m_blendValues;
};

#endif // SCENE_H
