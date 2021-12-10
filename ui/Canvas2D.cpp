/**
 * @file Canvas2D.cpp
 *
 * CS123 2-dimensional canvas. Contains support code necessary for Brush, Filter, Intersect, and
 * Ray.
 *
 * YOU WILL NEED TO FILL THIS IN!
 *
 */

// For your convenience, a few headers are included for you.
#include <assert.h>
#include <iostream>
#include <math.h>
#include <memory>
#include <unistd.h>
#include "Canvas2D.h"
#include "Settings.h"
#include "RayScene.h"

#include <QCoreApplication>
#include <QPainter>
#include "brush/Brush.h"
#include "brush/ConstantBrush.h"
#include "brush/LinearBrush.h"
#include "brush/QuadraticBrush.h"
#include "brush/SmudgeBrush.h"
#include "filter/filterutils.h"

Canvas2D::Canvas2D() :
    // @TODO: Initialize any pointers in this class here.
    m_rayScene(nullptr),
    m_currBrushType(settings.brushType),
    m_currBrushRadius(settings.brushRadius),
    m_currBrushColor(settings.brushColor),
    m_allBrushes{std::unique_ptr<Brush>(new ConstantBrush(settings.brushColor, settings.brushRadius)),
                 std::unique_ptr<Brush>(new LinearBrush(settings.brushColor, settings.brushRadius)),
                 std::unique_ptr<Brush>(new QuadraticBrush(settings.brushColor, settings.brushRadius)),
                 std::unique_ptr<Brush>(new SmudgeBrush(settings.brushColor, settings.brushRadius))}
{
}

Canvas2D::~Canvas2D()
{
}

// This is called when the canvas size is changed. You can change the canvas size by calling
// resize(...). You probably won't need to fill this in, but you can if you want to.
void Canvas2D::notifySizeChanged(int w, int h) {
}

void Canvas2D::paintEvent(QPaintEvent *e) {
    // You probably won't need to fill this in, but you can if you want to override any painting
    // events for the 2D canvas. For now, we simply call the superclass.
    SupportCanvas2D::paintEvent(e);

}

void Canvas2D::settingsChanged() {
    // TODO: Process changes to the application settings.
    std::cout << "Canvas2d::settingsChanged() called. Settings have changed" << std::endl;
    if(settings.brushType == BRUSH_SPECIAL_1 || settings.brushType == BRUSH_SPECIAL_2){
        std::cout << "No special brushes were implemented, please make a different selection" << std::endl;
        settings.brushType = m_currBrushType;
    }
    m_currBrushType = settings.brushType;
    m_currBrushRadius = settings.brushRadius;
    m_currBrushColor = settings.brushColor;
    m_allBrushes[m_currBrushType]->setRGBA(m_currBrushColor);
    m_allBrushes[m_currBrushType]->setRadius(m_currBrushRadius);
}

// ********************************************************************************************
// ** BRUSH
// ********************************************************************************************


void Canvas2D::mouseDown(int x, int y) {
    // @TODO: [BRUSH] Mouse interaction for brush. You will probably want to create a separate
    //        class for each of your brushes. Remember that you can use the static Settings
    //        object to get the currently selected brush and its parameters.

    // You're going to need to leave the alpha value on the canvas itself at 255, but you will
    // need to use the actual alpha value to compute the new color of the pixel

    std::cout << "Canvas2d::mouseDown() called" << std::endl;
    m_allBrushes[m_currBrushType]->brushDown(x,y,this);


//    bool fixAlphaBlending = settings.fixAlphaBlending; // for extra/half credit

}

void Canvas2D::mouseDragged(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseDragged() called" << std::endl;
    m_allBrushes[m_currBrushType]->brushDragged(x,y,this);

}

void Canvas2D::mouseUp(int x, int y) {
    // TODO: [BRUSH] Mouse interaction for Brush.
    std::cout << "Canvas2d::mouseUp() called" << std::endl;
}



// ********************************************************************************************
// ** FILTER
// ********************************************************************************************

void Canvas2D::filterImage() {
    // TODO: [FILTER] Filter the image. Some example code to get the filter type is provided below.
    std::cout << settings.filterType << std::endl;
    switch(settings.filterType) {
//        case FILTER_BLUR:
//            // ...
//            FilterUtils::applyGaussianBlur(this, settings.blurRadius);
//            // fill in the rest
//        case FILTER_EDGE_DETECT:
//            FilterUtils::applyEdgeDetection(this, settings.edgeDetectSensitivity);
    }
    if (settings.filterType == FILTER_BLUR){
        FilterUtils::applyGaussianBlur(this, settings.blurRadius);
    } else if (settings.filterType == FILTER_EDGE_DETECT){
        FilterUtils::applyEdgeDetection(this, settings.edgeDetectSensitivity);
    } else if (settings.filterType == FILTER_SCALE){
        FilterUtils::applyScaling(this, settings.scaleX, settings.scaleY);
    }



    // Leave this code here! This code ensures that the Canvas2D will be completely wiped before
    // drawing the new image.
    repaint(rect());
    QCoreApplication::processEvents();
    this->update();

}

// ********************************************************************************************
// ** RAY
// ********************************************************************************************

void Canvas2D::setScene(RayScene *scene) {
    m_rayScene.reset(scene);
}

void Canvas2D::renderImage(CS123SceneCameraData *camera, int width, int height) {
    if (m_rayScene) {
        // @TODO: raytrace the scene based on settings
        //        YOU MUST FILL THIS IN FOR INTERSECT/RAY

        // If you want the interface to stay responsive, make sure to call
        // QCoreApplication::processEvents() periodically during the rendering.

        //change the dimensions of the canvas to match the passed dimensions
        this->resize(width, height);

        //update the camera in the scene
        m_rayScene->setCamera(*camera);

        std::vector<glm::vec4> colors = m_rayScene->rayTrace(width, height);

        RGBA *pix = this->data();

        for (int i = 0; i < this->height(); i++){
            QCoreApplication::processEvents();
            for (int j = 0; j < this->width(); j++){
//                double val = colors[i*this->width() + j];
                glm::vec4 color = colors[i*this->width() + j];
                color = glm::max(color, glm::vec4(0.,0.,0.,1.));
                pix[i*this->width() + j] = RGBA(color[0]*255., color[1]*255., color[2]*255., color[3]*255.);
            }
        }

    }
}


void Canvas2D::cancelRender() {
    // TODO: cancel the raytracer (optional)
}




