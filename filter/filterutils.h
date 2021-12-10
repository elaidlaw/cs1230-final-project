#ifndef FILTERUTILS_H
#define FILTERUTILS_H

#include <vector>

#include "RGBA.h"
#include "Canvas2D.h"


namespace FilterUtils
{
    inline unsigned char REAL2byte(float f);
    unsigned char RGBAToGray(const RGBA &pixel);
    float g(float x, float a);
    RGBA scaleSinglePixelSampling(RGBA* data, int width, int height, int row, int col, float a, bool vertical);

    std::vector<RGBA> Convolve2D(RGBA* data, int width, int height, const std::vector<float> &kernel, int kernelHeight, int kernelWidth, bool normalize);
    std::vector<RGBA> Convolve1D(RGBA* data, int width, int height, const std::vector<float> &kernel);
    std::vector<RGBA> TriangleScale(RGBA* data, int width, int height, int newWidth, int newHeight, float a, bool vertical);

    void applyGrayscale(Canvas2D *canvas);
    std::vector<RGBA> applySobel(Canvas2D *canvas, bool vertical);

    void updateData(RGBA *data, std::vector<RGBA> newData, int height, int width);

    void applyEdgeDetection(Canvas2D *canvas, float sensitivity);
    void applyGaussianBlur(Canvas2D *canvas, int radius, bool separate = true);
    void applyScaling(Canvas2D *canvas, float scaleX, float scaleY);

};

#endif // FILTERUTILS_H
