#ifndef CANVAS2D_H
#define CANVAS2D_H

#include <memory>

#include "SupportCanvas2D.h"
#include "RGBA.h"
#include "Brush.h"

class RayScene;

class CS123SceneCameraData;

/**
 * @class Canvas2D
 *
 * 2D canvas that students will implement in the Brush and Filter assignments. The same canvas
 * will be used to display raytraced images in the Intersect and Ray assignments.
 */
class Canvas2D : public SupportCanvas2D {
    Q_OBJECT
public:
    Canvas2D();
    virtual ~Canvas2D();

    void setScene(RayScene *scene);

    // UI will call this from the button on the "Ray" dock
    void renderImage(CS123SceneCameraData *camera, int width, int height);

    // This will be called when the settings have changed
    virtual void settingsChanged();

    QImage* getImage() { return m_image; }


public slots:
    // UI will call this from the button on the "Ray" dock
    void cancelRender();

    // UI will call this from the button on the "Filter" dock
    void filterImage();

protected:
    virtual void paintEvent(QPaintEvent *);  // Overridden from SupportCanvas2D.
    virtual void mouseDown(int x, int y);    // Called when left mouse button is pressed on canvas
    virtual void mouseDragged(int x, int y); // Called when left mouse button is dragged on canvas
    virtual void mouseUp(int x, int y);      // Called when left mouse button is released

    // Called when the size of the canvas has been changed
    virtual void notifySizeChanged(int w, int h);


private:

    std::unique_ptr<RayScene> m_rayScene;

    //TODO: [BRUSH, INTERSECT, RAY] Put your member variables here.

    //Brush
    int m_currBrushType;
    int m_currBrushRadius;
    RGBA m_currBrushColor;
    std::array<std::unique_ptr<Brush>, 4> m_allBrushes;


};

#endif // CANVAS2D_H
