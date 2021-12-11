#include "SceneviewScene.h"
#include "GL/glew.h"
#include <QGLWidget>
#include "Camera.h"

#include "Settings.h"
#include "SupportCanvas3D.h"
#include "ResourceLoader.h"
#include "gl/shaders/CS123Shader.h"
#include <iostream>
using namespace CS123::GL;


SceneviewScene::SceneviewScene():
    m_cone(std::make_unique<Cone>(settings.shapeParameter1,settings.shapeParameter2)),
    m_sphere(std::make_unique<Sphere>(settings.shapeParameter1,settings.shapeParameter2)),
    m_cube(std::make_unique<Cube>(settings.shapeParameter1,settings.shapeParameter2)),
    m_cylinder(std::make_unique<Cylinder>(settings.shapeParameter1,settings.shapeParameter2))
{
    // TODO: [SCENEVIEW] Set up anything you need for your Sceneview scene here...
    loadPhongShader();
    loadWireframeShader();
    loadNormalsShader();
    loadNormalsArrowShader();
}

SceneviewScene::~SceneviewScene()
{
}

void SceneviewScene::loadPhongShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/default.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/default.frag");
    m_phongShader = std::make_unique<CS123Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadWireframeShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.vert");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/wireframe.frag");
    m_wireframeShader = std::make_unique<Shader>(vertexSource, fragmentSource);
}

void SceneviewScene::loadNormalsShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normals.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normals.frag");
    m_normalsShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::loadNormalsArrowShader() {
    std::string vertexSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.vert");
    std::string geometrySource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.gsh");
    std::string fragmentSource = ResourceLoader::loadResourceFileToString(":/shaders/normalsArrow.frag");
    m_normalsArrowShader = std::make_unique<Shader>(vertexSource, geometrySource, fragmentSource);
}

void SceneviewScene::render(SupportCanvas3D *context) {
    setClearColor();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    m_phongShader->bind();
    setGlobalData();
    setSceneUniforms(context);
    setLights();
    renderGeometry();
    glBindTexture(GL_TEXTURE_2D, 0);
    m_phongShader->unbind();

}

void SceneviewScene::setGlobalData(){
    // [TODO] pass global data to shader.vert using m_phongShader
    if (m_globalData){
        m_phongShader->setUniform("ka", m_globalData->ka);
        m_phongShader->setUniform("kd", m_globalData->kd);
        m_phongShader->setUniform("ks", m_globalData->ks);
    }
}

void SceneviewScene::setSceneUniforms(SupportCanvas3D *context) {
    Camera *camera = context->getCamera();
    m_phongShader->setUniform("useLighting", settings.useLighting);
    m_phongShader->setUniform("useArrowOffsets", false);
    m_phongShader->setUniform("isShapeScene", false);
    m_phongShader->setUniform("p" , camera->getProjectionMatrix());
    m_phongShader->setUniform("v", camera->getViewMatrix());
}

void SceneviewScene::setMatrixUniforms(Shader *shader, SupportCanvas3D *context) {
    shader->setUniform("p", context->getCamera()->getProjectionMatrix());
    shader->setUniform("v", context->getCamera()->getViewMatrix());
}

void SceneviewScene::setLights()
{
    // TODO: [SCENEVIEW] Fill this in...
    //
    // Set up the lighting for your scene using m_phongShader.
    // The lighting information will most likely be stored in CS123SceneLightData structures.
    //

    for (int i = 0; i < m_lightData.size(); i++){
        m_phongShader->setLight(*m_lightData[i]);
    }
}

void SceneviewScene::renderGeometry() {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // TODO: [SCENEVIEW] Fill this in...
    // You shouldn't need to write *any* OpenGL in this class!
    //
    //
    // This is where you should render the geometry of the scene. Use what you
    // know about OpenGL and leverage your Shapes classes to get the job done.
    //

    for (int i = 0; i < static_cast<int>(m_primitives.size()); i++){
        m_phongShader->setUniform("m", *m_transforms[i]);
        m_phongShader->applyMaterial(m_primitives[i]->material);
        PrimitiveType type = m_primitives[i]->type;
        if (type == PrimitiveType::PRIMITIVE_CONE){
            m_cone->draw();
        }
        if (type == PrimitiveType::PRIMITIVE_CUBE){
            m_cube->draw();
        }
        if (type == PrimitiveType::PRIMITIVE_SPHERE){
            m_sphere->draw();
        }
        if (type == PrimitiveType::PRIMITIVE_CYLINDER){
            m_cylinder->draw();
        }
    }

}

void SceneviewScene::settingsChanged() {
    //Set upper bound tesselation parameters as an LOD technique
    //The bound is scaled by the inverse fourth root of the number of scene objects
    int boundedP1 = std::max(1, static_cast<int>(settings.shapeParameter1/sqrt(sqrt(m_primitives.size()))));
    int boundedP2 = std::max(1, static_cast<int>(settings.shapeParameter2/sqrt(sqrt(m_primitives.size()))));

    // TODO: [SCENEVIEW] Fill this in if applicable.
    m_cone = std::make_unique<Cone>(boundedP1, boundedP2);
    m_cylinder = std::make_unique<Cylinder>(boundedP1, boundedP2);
    m_cube = std::make_unique<Cube>(boundedP1, boundedP2);
    m_sphere = std::make_unique<Sphere>(boundedP1, boundedP2);
}

