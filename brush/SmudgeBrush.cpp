/**
 * @file   Brush.cpp
 *
 * Implementation of a brush that smudges the image on the canvas as the mouse is dragged.
 *
 * You should fill this file in for the Brush assignment.
 */

#include "SmudgeBrush.h"
#include "iostream"

#include "Canvas2D.h"
#include "math.h"


SmudgeBrush::SmudgeBrush(RGBA color, int radius) :
    QuadraticBrush(color, radius)
{
    // @TODO: [BRUSH] Initialize any memory you are going to use here. Hint - you are going to
    //       need to store temporary image data in memory. Remember to use automatically managed memory!
    for(int r = 0; r <= 2*m_radius; r++){
        for(int c = 0; c <= 2*m_radius; c++){
            paintBuffer.push_back(RGBA());
        }
    }
    makeMask();
}


SmudgeBrush::~SmudgeBrush()
{
    // @TODO: [BRUSH] Be sure not to leak memory!  If you're using automatic memory management everywhere,
    //       this will be empty.
}

void SmudgeBrush::brushUp(int x, int y, Canvas2D* canvas) {
}


//! create a mask with a distribution of your choice (probably want to use quadratic for best results)
void SmudgeBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your brush here. For this brush you will probably want
    //        to use a quadratic distribution for the best results. Linear or Gaussian would
    //        work too, however. Feel free to paste your code from the Linear or Quadratic brushes
    //        or modify the class inheritance to be able to take advantage of one of those class's
    //        existing implementations. The choice is yours! 
    m_mask.clear();
    for(int r = 0; r <= m_radius; r++){
        for(int c = 0; c <= m_radius; c++){
            float dist = std::max(0.,1.-sqrt(r*r + c*c)/m_radius);
            m_mask.push_back(dist*dist);
        }
    }
}

void SmudgeBrush::brushDown(int x, int y, Canvas2D *canvas) {
    pickUpPaint(x, y, canvas);
}

//! Picks up paint from the canvas before drawing begins.
void SmudgeBrush::pickUpPaint(int x, int y, Canvas2D* canvas) {
    paintBuffer.clear();
    int width = canvas->width();
    int height = canvas->height();
    RGBA *pix = canvas->data();
    // @TODO: [BRUSH] Perform the "pick up paint" step described in the assignment handout here.
    //        In other words, you should store the "paint" under the brush mask in a temporary
    //        buffer (which you'll also have to figure out where and how to allocate). Then,
    //        in the paintOnce() method, you'll paste down the paint that you picked up here.
    //
    for(int r = y - m_radius; r <= y + m_radius; r++){
        for(int c = x - m_radius; c <= x + m_radius; c++){
            if(r < 0 || c < 0 || r >= height || c >= width){
                paintBuffer.push_back(RGBA());
            } else {
                paintBuffer.push_back(pix[r*width + c]);
            }
        }
    }

}

void SmudgeBrush::brushDragged(int mouseX, int mouseY, Canvas2D* canvas) {
    // @TODO: [BRUSH] Here, you'll need to paste down the paint you picked up in
    //        the previous method. Be sure to take the mask into account! You can
    //        ignore the alpha parameter, but you can also use it (smartly) if you
    //        would like to.

    RGBA *pix = canvas->data();

    int low_x = std::max(0,mouseX-m_radius);
    int high_x = std::min(canvas->width()-1, mouseX+m_radius);
    int low_y = std::max(0,mouseY-m_radius);
    int high_y = std::min(canvas->height()-1, mouseY+m_radius);

    for (int r = low_y; r <= high_y; r++){
        for (int c = low_x; c <= high_x; c++){
            int bufferRow = r - (mouseY - m_radius);
            int bufferCol = c - (mouseX - m_radius);
            RGBA bufferColor = paintBuffer[bufferRow*(2*m_radius + 1) + bufferCol];
            float brush_ratio = m_mask[std::abs(r-mouseY)*(m_radius+1) + std::abs(c-mouseX)] * bufferColor.a/255.f;
            int red = (1-brush_ratio) * pix[r*canvas->width() + c].r + brush_ratio * bufferColor.r + 0.5;
            int green = (1-brush_ratio) * pix[r*canvas->width() + c].g + brush_ratio * bufferColor.g + 0.5;
            int blue = (1-brush_ratio) * pix[r*canvas->width() + c].b + brush_ratio * bufferColor.b + 0.5;
            RGBA newColor = RGBA((int)red,(int)green,(int)blue);
            pix[r*canvas->width() + c] = newColor;
        }
    }
    canvas->update();

    // now pick up paint again...
    pickUpPaint(mouseX, mouseY, canvas);

}


