#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QButtonGroup>
#include <QMainWindow>

class CS123XmlSceneParser;
class SupportCanvas3D;

namespace Ui {
    class MainWindow;
}

class DataBinding;

/**
 * @class MainWindow
 *
 * The main graphical user interface class (GUI class) for our application.
 */
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:

    // Overridden from QWidget. Handles the window resize event.
    virtual void changeEvent(QEvent *e);

    // Overridden from QWidget. Handles the window close event.
    virtual void closeEvent(QCloseEvent *e);

private:

    // Helper function for checkAllRayFeatures() and uncheckAllRayFeatures(), either check or
    // clear all checkboxes associated with rendering features for Ray, depending on 'checked'.
    void setAllRayFeatures(bool checked);

    // Enables or disables the UI controls.
    void setAllEnabled(bool enabled);

    // Sets up the data bindings between the UI and app settings
    void dataBind();

    // initializes settings and ui for camtrans viewing frustum
    void initializeCamtransFrustum();

    // [C++ Note] private members start with m_
    QList<DataBinding*> m_bindings;
    QList<QButtonGroup*> m_buttonGroups;
    SupportCanvas3D *m_canvas3D;
    std::unique_ptr<CS123XmlSceneParser> m_sceneParser;

    // EXCEPTION: 'ui' member which is auto-generated by Qt. DO NOT RENAME!
    Ui::MainWindow *ui;

public slots:
    // Used internally to keep the aspect ratio up to date.
    void updateAspectRatio();

    // Used internally to keep data bindings and the user interface in sync.
    void settingsChanged();

    // Copy the contents of the 3D tab to the 2D tab
    void fileCopy3Dto2D();

    // Clears the current 2D image and 3D scene.
    void fileNew();

    // Displays a dialog box to open a 2D image or 3D scene file.
    void fileOpen();

    // Displays a dialog box to save the current 2D image. Can be extended (for extra credit) to
    // save the current 3D scene.
    void fileSave();

    // Checks all (enables all) Ray features.
    void checkAllRayFeatures();

    // Unchecks all (disables all) Ray features.
    void uncheckAllRayFeatures();

    // Called when the user presses the "Filter Image" button in the Filter panel
    void filterImage();

    // Called when the user presses the "Render Image" button in the Ray panel
    void renderImage();

    // Clears the 2D canvas
    void clearImage();

    // Reverts the 2D canvas to the last opened image
    void revertImage();

    // Sets the current tab on the UI to the 2D canvas view.
    void activateCanvas2D();

    // Sets the current tab on the UI to the 3D canvas view.
    void activateCanvas3D();

    // These methods are update different aspects of the 3D camera, and delegate to the Canvas3D.
    void setCameraAxisX();
    void setCameraAxisY();
    void setCameraAxisZ();
    void updateCameraTranslation();
    void updateCameraRotationN();
    void updateCameraRotationV();
    void updateCameraRotationU();
    void resetUpVector();
    void resetSliders();
    void updateCameraClip();
    void updateCameraHeightAngle();
    void setCameraAxonometric();
};

#endif // MAINWINDOW_H