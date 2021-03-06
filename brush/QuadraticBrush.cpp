/**
 * @file   QuadraticBrush.cpp
 *
 * Implementation of a brush with a quadratic mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "QuadraticBrush.h"

#include <math.h>

QuadraticBrush::QuadraticBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}

QuadraticBrush::~QuadraticBrush()
{
}

void QuadraticBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Quadratic brush here...
    m_mask.clear();
    for(int r = 0; r <= m_radius; r++){
        for(int c = 0; c <= m_radius; c++){
            float dist = std::max(0.,1.-sqrt(r*r + c*c)/m_radius);
            m_mask.push_back(dist*dist);
        }
    }
}


