#include "filterutils.h"
#include "math.h"

namespace FilterUtils {

    inline unsigned char REAL2byte(float f) {
        int i = static_cast<int>((f * 255.0 + 0.5));
        return (i < 0) ? 0 : (i > 255) ? 255 : i;
    }

    unsigned char RGBAToGray(const RGBA &pixel) {
        // TODO: Task 5
        int val = static_cast<int>(0.299*pixel.r+0.587*pixel.g+0.114*pixel.b);
        return static_cast<unsigned char>(val);
    }

    //unified triangle filter
    float g(float x, float a){
        float radius;
        if (a < 1.f){
            radius = 1.f/a;
        } else {
            radius = 1.f;
        }
        if (x < -radius || x > radius){
            return 0.f;
        } else {
            return (1.f - abs(x)/radius)/radius;
        }
    }

    RGBA scaleSinglePixelSampling(RGBA* data, int width, int height, int row, int col, float a, bool vertical){
        int k = vertical ? row : col;
        int upperLimit = vertical ? height : width;
        float redSum = 0.f;
        float greenSum = 0.f;
        float blueSum = 0.f;
        float weightSum = 0.f;
        float support = (a > 1.f) ? 1.f : 1.f/a;
        float center = k/a + (1.f-a)/(2.f*a);
        int left = ceil(center-support);
        int right = floor(center+support);

        for (int i = left; i <=right; i++){
            if (i < 0 || i >= upperLimit){
                continue;
            }
            int coord = vertical ? (i*width + col) : (row*width + i);
            RGBA imgVal = data[coord];
            float weight = g(i-center, a);
            redSum += imgVal.r/255.f*weight;
            greenSum += imgVal.g/255.f*weight;
            blueSum += imgVal.b/255.f*weight;

            weightSum += weight;
        }
        return RGBA(REAL2byte(redSum/weightSum), REAL2byte(greenSum/weightSum), REAL2byte(blueSum/weightSum));
    }

    std::vector<RGBA> TriangleScale(RGBA* data, int width, int height, int newWidth, int newHeight, float a, bool vertical){
        std::vector<RGBA> newImage = std::vector<RGBA>();

        for (int r=0; r<newHeight; r++){
            for (int c=0; c<newWidth; c++){
                RGBA newPixel = scaleSinglePixelSampling(data, width, height, r, c, a, vertical);
                newImage.push_back(newPixel);
            }
        }
        return newImage;
    }

    std::vector<RGBA> Convolve2D(RGBA *data, int width, int height, const std::vector<float> &kernel, int kernelHeight, int kernelWidth, bool normalize) {
        // TODO: Task 9 Create buffer to store new image data
        std::vector<RGBA> result;

        // TODO: Task 10 Obtain kernel dimension
//        int kernelDimension = sqrt(kernel.size());
//        int kernelCenter = kernelDimension / 2;

        int verticalCenter = kernelHeight / 2;
        int horizontalCenter = kernelWidth / 2;

        float kernelSum = 0.f;
        for (int i = 0; i < static_cast<int>(kernel.size()); i++){
            kernelSum += kernel[i];
        }

        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {

                // TODO: Task 11 Initialize color to accumulate convolution data
                float red_acc = 0.f;
                float green_acc = 0.f;
                float blue_acc = 0.f;

                // TODO: Task 12
                // Iterate over the kernel using the value from task 10
                // Obtain the value at current index of kernel
                // Find correct index in original image data
                // Accumulate the kernel applied to pixel value in color_acc
                for (int rk = 0; rk < kernelHeight; rk++){
                    for (int ck = 0; ck < kernelWidth; ck++){
                        float kernelWeight = kernel[rk*kernelWidth+ck];
                        int sampleRow = r - (verticalCenter-rk);
                        int sampleCol = c - (horizontalCenter-ck);
                        sampleRow = std::max(0, std::min(sampleRow, height-1));
                        sampleCol = std::max(0, std::min(sampleCol, width-1));
//                        if (sampleRow < 0 || sampleRow >= height || sampleCol < 0 || sampleCol >= width){
//                            continue;
//                        }
                        red_acc += (data[sampleRow*width+sampleCol].r/255.f) * kernelWeight;
                        green_acc += (data[sampleRow*width+sampleCol].g/255.f) * kernelWeight;
                        blue_acc += (data[sampleRow*width+sampleCol].b/255.f) * kernelWeight;
                    }
                }

                // TODO: Task 13 Ignore outside bounds


                // TODO: Task 14 Update buffer with accumulated color
                RGBA newPixel = RGBA(REAL2byte(red_acc), REAL2byte(green_acc), REAL2byte(blue_acc));
                if (normalize){
                    newPixel = RGBA(REAL2byte(red_acc/kernelSum), REAL2byte(green_acc/kernelSum), REAL2byte(blue_acc/kernelSum));
                }
                result.push_back(newPixel);
    //            result[r*width+c]->g = ;
    //            result[r*width+c]->b = ;
            }
        }
        return result;
    }

    std::vector<RGBA> Convolve1D(RGBA *data, int width, int height, const std::vector<float> &kernel, bool vertical, bool normalize){
        int kernelHeight = 1;
        int kernelWidth = 1;
        if (vertical){
            kernelHeight = kernel.size();
        } else {
            kernelWidth = kernel.size();
        }

        return Convolve2D(data, width, height, kernel, kernelHeight, kernelWidth, normalize);
    }

    void applyGrayscale(Canvas2D *canvas) {
        int width = canvas->width();

        RGBA* current_pixel = nullptr;

        /* Initialize the first pixel of the first row */
        RGBA* current_row = canvas->data();

        RGBA* data = canvas->data();
        size_t currentIndex = 0;

        for (int r = 0; r < canvas->height(); r++) {
            current_pixel = current_row;
            currentIndex = r * width;

            for (int c = 0; c < canvas->width(); c++) {
                // TODO: Task 4
                unsigned char lightIntensity = RGBAToGray(data[currentIndex]);
                // TODO: Task 6
                current_pixel->r = lightIntensity;
                current_pixel->g = lightIntensity;
                current_pixel->b = lightIntensity;


                /* Advance to the next pixel */
                current_pixel++;
                currentIndex++;
            }
            current_row += width;
        }
    }

    void updateData(RGBA *data, std::vector<RGBA> newData, int height, int width){
//        RGBA* data = canvas->data();
//        int height = canvas->height();
//        int width = canvas->width();
        for (int r = 0; r < height; r++) {
            for (int c = 0; c < width; c++) {
                data[r*width+c] = newData[r*width+c];
            }
        }
    }

    std::vector<RGBA> applySobel(Canvas2D *canvas, bool vertical) {
        std::vector<float> kernel = std::vector<float>();
        if (vertical){
            kernel.push_back(-1.f);
            kernel.push_back(-2.f);
            kernel.push_back(-1.f);
            kernel.push_back(0.f);
            kernel.push_back(0.f);
            kernel.push_back(0.f);
            kernel.push_back(1.f);
            kernel.push_back(2.f);
            kernel.push_back(1.f);
        } else {
            kernel.push_back(-1.f);
            kernel.push_back(0.f);
            kernel.push_back(1.f);
            kernel.push_back(-2.f);
            kernel.push_back(0.f);
            kernel.push_back(2.f);
            kernel.push_back(-1.f);
            kernel.push_back(0.f);
            kernel.push_back(1.f);
        }
        std::vector<RGBA> newData = Convolve2D(canvas->data(), canvas->width(), canvas->height(), kernel, 3, 3, false);
        return newData;
    }

    void applyEdgeDetection(Canvas2D *canvas, float sensitivity){
        applyGrayscale(canvas);
        std::vector<RGBA> yGrad = applySobel(canvas, true);
        std::vector<RGBA> xGrad = applySobel(canvas, false);

        //combine the x and y gradients
        std::vector<RGBA> totalGrad = std::vector<RGBA>();
        for (int i=0; i < yGrad.size(); i++){
            float fy = static_cast<float>(yGrad[i].r);
            float fx = static_cast<float>(xGrad[i].r);
            float newVal = std::min(255.f, sqrt(pow(fx, 2.f) + pow(fy, 2.f))*sensitivity)/255.f;
            newVal = std::max(newVal, 0.f);
            totalGrad.push_back(RGBA(REAL2byte(newVal), REAL2byte(newVal), REAL2byte(newVal)));
        }

        updateData(canvas->data(), totalGrad, canvas->height(), canvas->width());
    }

    void applyGaussianBlur(Canvas2D *canvas, int radius, bool separate) {
        std::vector<float> gaussianKernel = std::vector<float>();
        float stddev = static_cast<float>(radius)/3.f;
        float pi = 3.1415926535897932384626;
        std::vector<RGBA> newData;

        //You can choose whether to separate the Gaussian filter into two 1D filters or not (defaults to separate)
        if (!separate){
            for (int r=0; r<2*radius+1; r++){
                for (int c=0; c<2*radius+1; c++){
                    float xDiff = static_cast<float>(c) - static_cast<float>(radius);
                    float yDiff = static_cast<float>(r) - static_cast<float>(radius);
                    float dist = sqrt(pow(xDiff, 2.f) + pow(yDiff, 2.f));
                    float gaussianVal = 1.f/(stddev*sqrt(2*pi))*exp(-0.5f*pow(dist,2.f)/pow(stddev,2.f));
                    gaussianKernel.push_back(gaussianVal);
                }
            }
            newData = Convolve2D(canvas->data(), canvas->width(), canvas->height(), gaussianKernel, 2*radius+1, 2*radius+1, true);
        } else {
            for (int i=0; i<2*radius+1; i++){
                float dist = static_cast<float>(i) - static_cast<float>(radius);
                float gaussianVal = 1.f/(stddev*sqrt(2*pi))*exp(-0.5f*pow(dist,2.f)/pow(stddev,2.f));
                gaussianKernel.push_back(gaussianVal);
            }
            int width = canvas->width();
            int height = canvas->height();
            newData = Convolve1D(canvas->data(), width, height, gaussianKernel, true, true);
            newData = Convolve1D(newData.data(), width, height, gaussianKernel, false, true);
        }

        updateData(canvas->data(), newData, canvas->height(), canvas->width());
    }

    void applyScaling(Canvas2D *canvas, float scaleX, float scaleY){
        int newWidth = static_cast<int>(scaleX*canvas->width());
        int newHeight = static_cast<int>(scaleY*canvas->height());
        std::vector<RGBA> newData = TriangleScale(canvas->data(), canvas->width(), canvas->height(), canvas->width(), newHeight, scaleY, true);
        newData = TriangleScale(newData.data(), canvas->width(), newHeight, newWidth, newHeight, scaleX, false);
        canvas->resize(newWidth, newHeight);
        updateData(canvas->data(), newData, canvas->height(), canvas->width());
    }

}
