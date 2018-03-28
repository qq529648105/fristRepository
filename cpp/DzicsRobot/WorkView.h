#ifndef WORKVIEW_H
#define WORKVIEW_H

#include <QWidget>
#include <QGLWidget>
#include <QMouseEvent>
#include <Handle_V3d_View.hxx>
#include <Handle_V3d_Viewer.hxx>
#include <AIS_InteractiveContext.hxx>
#include <Aspect_Drawable.hxx>
#include <Aspect_GraphicCallbackProc.hxx>

    #include <Handle_WNT_Window.hxx>
    #include <WNT_Window.hxx>
    #include <Handle_Graphic3d_WNTGraphicDevice.hxx>
    #include <Graphic3d_WNTGraphicDevice.hxx>
#ifdef WNT
//    #include <Handle_WNT_Window.hxx>
//    #include <WNT_Window.hxx>
//    #include <Handle_Graphic3d_WNTGraphicDevice.hxx>
//    #include <Graphic3d_WNTGraphicDevice.hxx>
#else
    #include <Xw_Window.hxx>
    #include <Handle_Xw_Window.hxx>
    #include <Graphic3d_GraphicDevice.hxx>
    #include <Handle_Graphic3d_GraphicDevice.hxx>
#endif

namespace Ui {
    class WorkView;
}

class WorkView : public QGLWidget {
    Q_OBJECT
public:
    WorkView(QWidget *parent = 0);
    ~WorkView();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL           (int width, int height);
    void mousePressEvent    ( QMouseEvent* e );
    void mouseReleaseEvent  ( QMouseEvent* e );
    void mouseMoveEvent     ( QMouseEvent* e );
    void wheelEvent         ( QWheelEvent* e );
    void timerEvent         ( QTimerEvent* e );

    void changeEvent(QEvent *e);

private:
    Ui::WorkView *m_ui;

    Handle_V3d_View                 myView;
    Handle_V3d_Viewer               myViewer;
    Handle_AIS_InteractiveContext   myContext;
    #ifdef WNT
        Handle_WNT_Window           myWindow;
    #else
        Handle_Xw_Window            myWindow;
    #endif // WNT

    QPoint startPos;
    Standard_Real rotX;
    Standard_Real rotY;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    bool mouseMiddleBtn;

    static int paintCallBack (Aspect_Drawable, void*, Aspect_GraphicCallbackStruct*);
    void paintOpenGL();
    bool loadAllModel();
};

#endif // WORKVIEW_H
