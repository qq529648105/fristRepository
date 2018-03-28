#ifndef _VIEWER_H_
#define _VIEWER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <QWidget.h>

//#include <GL/glew.h>
#include <QMap>
#include <AIS_InteractiveContext.hxx>
#include <OpenGl_GraphicDriver.hxx>
#include <V3d_View.hxx>
#include <OpenGl_View.hxx>
//#include <Graphic3d_WNTGraphicDevice.hxx>
#include <WNT_Window.hxx>
#include <Handle_AIS_Shape.hxx>
#include <Prs3d_Presentation.hxx>

#include <TopoDS_Edge.hxx>
#include <Draw_Text3D.hxx>

#include <QMenu>
#include "ISession_Text.h"
#include "FloatTool.h"
class Viewer : public QWidget 
{
	Q_OBJECT
public:
    Viewer(QWidget *parent = 0);
	virtual ~Viewer();
    virtual QPaintEngine* paintEngine() const;

signals:
    void moduleAllLoaded( int count );
    void updateAxisValue ( QString axisSign, double value );
    void shapeSelected(QStringList currentModuleIdList, bool isMulti);
    //void shapeSelected(QString currentModuleId);
    void mousePressSignal(QMouseEvent *e);
    void updateProgram ( QString programText );
    void setMeasurePntInfoSignal();
    void setPntInfoSignal();
    void cancelButtonDownSignal();

public slots:
	//void syncPosition(double axis1, double axis2, double axis3, double axis4, double axis5, double axis6 );
    void syncPosition( QStringList posList, int index );
	void updateUnitData( QByteArray data );
	void syncDisplayState( int displayState, int pickCount );
	void moduleLoadFinish( QString id );
    void moduleImportFinish( QString id );

	void showAll();
    void openTransformTool();
    void showToolWidget(int type);
    void toolClose();

    void importModuleFile(QString filename, QString id, QString name);
    void stationChanged(QString stationId);
    void moduleChanged(QString id, bool isRemoved);

    void nurbsGenerate(QList<TopoDS_Edge> edges);
    void nurbsGenerate2(QList<TopoDS_Edge> edges);

    void showText(char* str);

    int drawText(const TCollection_AsciiString& aText,
                                         const Standard_Real            anX         = 0   ,
                                         const Standard_Real            anY         = 0   ,
                                         const Standard_Real            aZ          = 0   ,
                                         const Aspect_TypeOfText        aType       = Aspect_TOT_SOLID,
                                         const Quantity_PlaneAngle      anAngle     = 0.0 ,
                                         const Standard_Real            aSlant      = 0.0 ,
                                         const Standard_Integer         aColorIndex = 1   ,
                                         const Standard_Integer         aFontIndex  = 1   ,
                                         const Quantity_Factor          aScale      = 0.1   );
        //(Standard_CString& aText, gp_Pnt& pnt, Quantity_Color& aColor, const int& hor_align, const int& ver_align, const Standard_Real angle, const Standard_Boolean zoom, const Standard_Real, const Font_FontAspect aspect, TCollection_AsciiString font);
    void setViewUpdateState(bool state);
    void setGeometricElementSelectMode(int selectMode);
    void cancelGeometricElementSelectMode();
    void cancelButtonDownSlot();
    void createActions();
    void showArea(QString curArea);
    void removeArea(QString curArea);
    void moveRect(QString uuid, int x, int y, int z);
    void createArea(QString uuid,int len1,int len2,int len3,int x,int y,int z);
protected:
    virtual void paintEvent			( QPaintEvent* e );
    virtual void resizeEvent		( QResizeEvent* e );
    virtual void mousePressEvent    ( QMouseEvent* e );
    virtual void contextMenuEvent   (QContextMenuEvent* e);
    virtual void mouseReleaseEvent  ( QMouseEvent* e );
    virtual void mouseMoveEvent     ( QMouseEvent* e );
    virtual void mouseDoubleClickEvent ( QMouseEvent* e );
    virtual void wheelEvent         ( QWheelEvent* e );
    virtual void timerEvent         ( QTimerEvent* e );
    virtual void keyPressEvent		( QKeyEvent * event );
    virtual void keyReleaseEvent	( QKeyEvent * event );
    virtual bool event				(QEvent *event);

    void changeEvent(QEvent *e);

private:
    Handle_V3d_View                 myView;
    Handle_V3d_Viewer               myViewer;
    Handle_AIS_InteractiveContext   myContext;
    Handle_WNT_Window				myWindow;
    FloatTool *m_pFloatTool;
    QPoint startPos;
    Standard_Real rotX;
    Standard_Real rotY;
    bool mouseLeftPressed;
    bool mouseRightPressed;
    bool mouseMiddleBtn;
    bool toolSelectMode;

//	int viewMouseMode;

    double spindleStep;
    double spindleRotate;
    int rotateInterval;
    int rotateTimerId;
    int redrawInterval;
    int redrawTimerId;

    QString currentStationId;
    int moduleLoadedCount;
    int moduleLoadCount;
    bool viewUpdateState;//选择点或测量时，视图模型不更新。
    Handle_Prs3d_Presentation loadingText,text1;


    static int paintCallBack (Aspect_Drawable, void*, Aspect_GraphicCallbackStruct*);
    void paintOpenGL();
    bool loadAllModule();
    void axisMove(QString axisId, double value);

    QMap<QString,Handle_AIS_Shape> myRect;
    QMenu *pop_menu;
    //Handle(Graphic3d_WNTGraphicDevice) device;

    //Handle(AIS_InteractiveContext) myIC;
    //Handle(V3d_View)               myView;
 //   Handle(OpenGl_GraphicDriver)   myGraphicDriver;

    //bool myRotate;
    //bool myZoom;
    //bool myPan;

//	QPoint myStartPnt;

};

#endif // _VIEWER_H_
