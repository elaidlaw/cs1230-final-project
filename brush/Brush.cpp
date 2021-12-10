/**
 * @file   Brush.cpp
 *
 * Implementation of common functionality of bitmap brushes.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "Brush.h"
#include "Canvas2D.h"
# include "Settings.h"

static unsigned char lerp(unsigned char a, unsigned char b, float percent) {
    float fa = static_cast<float>(a) / 255.0f;
    float fb = static_cast<float>(b) / 255.0f;
    return static_cast<unsigned char>((fa + (fb - fa) * percent) * 255.0f + 0.5f);
}



Brush::Brush(RGBA color, int radius) :
    // Pro-tip: Initialize all variables in the initialization list
    m_color(color),
    m_radius(radius)
{
    // Pro-tip: By the time you get to the constructor body, all of the
    // member variables have already been initialized.
}


Brush::~Brush()
{
    // Pro-tip: The destructor (here) is where you free all the resources
    // you allocated during the lifetime of the class
    // Pro-tip: If you use std::unique_ptr or std::vector, you probabably
    // won't need to put anything in here.
}

int Brush::getAlpha() const {
    return m_color.a;
}

RGBA Brush::getRGBA() const {
    return m_color;
}

int Brush::getBlue() const {
    return m_color.b;
}

int Brush::getGreen() const {
    return m_color.g;
}

int Brush::getRadius() const {
    return m_radius;
}

int Brush::getRed() const {
    return m_color.r;
}

void Brush::setAlpha(int alpha) {
    m_color.a = alpha;
}

void Brush::setRGBA(const RGBA &rgba) {
    m_color = rgba;
}

void Brush::setBlue(int blue) {
    m_color.b = blue;
}

void Brush::setGreen(int green) {
    m_color.g = green;
}

void Brush::setRed(int red) {
    m_color.r = red;
}

void Brush::setRadius(int radius) {
    m_radius = radius;
    makeMask();
}

void Brush::brushDown(int x, int y, Canvas2D *canvas){
    if(settings.fixAlphaBlending){
        RGBA *pix = canvas->data();
        m_alphaMask.clear();
        m_lastCanvas.clear();
        for(int r=0;r<canvas->height();r++){
            for(int c=0; c<canvas->width();c++){
                m_alphaMask.push_back(0.0);
                m_lastCanvas.push_back(pix[r*canvas->width() + c]);
            }
        }
    }
}


void Brush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] You can do any painting on the canvas here. Or, you can
    //        override this method in a subclass and do the painting there.
    //
    // Example: You'll want to delete or comment out this code, which
    // sets all the pixels on the canvas to red.
    //

    RGBA *pix = canvas->data();

    int low_x = std::max(0,mouseX-m_radius);
    int high_x = std::min(canvas->width()-1, mouseX+m_radius);
    int low_y = std::max(0,mouseY-m_radius);
    int high_y = std::min(canvas->height()-1, mouseY+m_radius);


    for (int r = low_y; r <= high_y; r++){
        for (int c = low_x; c <= high_x; c++){
            float brush_ratio = m_mask[std::abs(r-mouseY)*(m_radius+1) + std::abs(c-mouseX)] * m_color.a/255.f;
            if(settings.fixAlphaBlending){
                if(m_alphaMask[r*canvas->width() + c] < brush_ratio){
                    m_alphaMask[r*canvas->width() + c] = brush_ratio;
                    int red = (1-brush_ratio) * m_lastCanvas[r*canvas->width() + c].r + brush_ratio * m_color.r + 0.5f;
                    int green = (1-brush_ratio) * m_lastCanvas[r*canvas->width() + c].g + brush_ratio * m_color.g + 0.5f;
                    int blue = (1-brush_ratio) * m_lastCanvas[r*canvas->width() + c].b + brush_ratio * m_color.b + 0.5f;
                    RGBA newColor = RGBA((int)red,(int)green,(int)blue);
                    pix[r*canvas->width() + c] = newColor;
                }
            } else {
                int red = (1-brush_ratio) * pix[r*canvas->width() + c].r + brush_ratio * m_color.r + 0.5f;
                int green = (1-brush_ratio) * pix[r*canvas->width() + c].g + brush_ratio * m_color.g + 0.5f;
                int blue = (1-brush_ratio) * pix[r*canvas->width() + c].b + brush_ratio * m_color.b + 0.5f;
                RGBA newColor = RGBA((int)red,(int)green,(int)blue);
                pix[r*canvas->width() + c] = newColor;
            }
        }
    }
    canvas->update();

}
