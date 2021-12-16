#include "OpenGLScene.h"

#include <GL/glew.h>

#include "Settings.h"

OpenGLScene::~OpenGLScene()
{
}

void OpenGLScene::setClearColor() {
    if (settings.drawWireframe || settings.drawNormals) {
        glClearColor(0.5f, 0.5f, 0.5f, 0.5f);
    } else {
        glClearColor(.9, .96, .995, 1);
    }
}

void OpenGLScene::settingsChanged() {

}
