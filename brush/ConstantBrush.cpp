/**
 * @file   ConstantBrush.cpp
 *
 * Implementation of a brush with a constant mask distribution.
 *
 * You should fill this file in while completing the Brush assignment.
 */

#include "ConstantBrush.h"

ConstantBrush::ConstantBrush(RGBA color, int radius)
    : Brush(color, radius)
{
    // @TODO: [BRUSH] You'll probably want to set up the mask right away.
    makeMask();
}


ConstantBrush::~ConstantBrush()
{
}

void ConstantBrush::makeMask() {
    // @TODO: [BRUSH] Set up the mask for your Constant brush here...
    m_mask.clear();
    for (int r = 0; r <= m_radius; r++){
        for (int c = 0; c <= m_radius; c++){
            if ((r*r + c*c) <= (m_radius*m_radius)){
                m_mask.push_back(1.0);
            } else {
                m_mask.push_back(0.0);
            }
        }
    }
}

