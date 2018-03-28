//#include <GL/glew.h>

#include "WorkView.h"
//#include "ui_WorkView.h"
#include <V3d_View.hxx>
#include <V3d_Viewer.hxx>
#include <Handle_V3d_View.hxx>
#include <Handle_V3d_Viewer.hxx>
#include <V3d_OrthographicView.hxx>
#include "V3d_DirectionalLight.hxx"
#include "V3d_AmbientLight.hxx"

#include "mesh/MeshTranslate.h"
#include "SysUtils.h"

#include <QApplication>
#include <QSqlQuery>
#include <QMessageBox>

WorkView::WorkView(QWidget *parent) :
    QGLWidget(QGLFormat(QGL::DoubleBuffer|QGL::DirectRendering|QGL::Rgba|QGL::StencilBuffer|QGL::SampleBuffers), parent)
{

    qApp->setOverrideCursor(Qt::WaitCursor);

//    glClearColor(0.58f, 0.65f, 0.76f, 1.0f);
//    setAttribute( Qt::WA_NoSystemBackground );

//this->hide();

    rotX = 0.0;
    rotY = 0.0;
    mouseLeftPressed = false;
    mouseRightPressed = false;
    mouseMiddleBtn = false;

//        QSqlQuery query;
////    query.exec("CREATE TABLE test(id INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR(40) NOT NULL)");
////    query.exec("INSERT INTO test(name) VALUES('abcd中文efg')");
//    query.exec("SELECT * FROM test");
//    while(query.next()){
//        QMessageBox::information(NULL, QObject::tr("Coll中国ection"), query.value(1).toString());
//    }

}

WorkView::~WorkView()
{
}

void WorkView::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    default:
        break;
    }
}

int WorkView::paintCallBack (Aspect_Drawable /* drawable */,
                                void* aPointer,
                                Aspect_GraphicCallbackStruct* /* data */)
{
    static GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};
    static GLfloat diffuse[] = {0.65, 0.65, 0.65, 1.0};
    static GLfloat specular[] = {0.0, 0.0, 0.0, 1.0};
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.039f);
    glEnable( GL_LIGHTING );

    ((WorkView *) aPointer)->paintOpenGL();
    return 0;
}

#include <BRepTools_ShapeSet.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <Standard_Stream.hxx>

//using namespace std;

bool WorkView::loadAllModel()
{
    QSqlQuery query;
    query.exec("SELECT name,type,fileId,material,color,parentId FROM Module WHERE isUse=1");
    while(query.next()) {
        QByteArray file = SysUtils::getFile(query.value(2).toString());
        BRep_Builder builder = BRep_Builder();
        BRepTools_ShapeSet SS(builder);
        TopoDS_Shape shape;

//        ifstream mf;
        filebuf fic;
        fic.sgetn(file.data(), strlen(file.data()));
// file >> fic;
//        fic = (filebuf)file.constData();
  istream in(&fic);
//  filebuf fic;

        SS.Read(in);
        SS.Read(shape, in);
        myContext->Display(new AIS_Shape(shape));
    }
	return true;
}

static Handle_Graphic3d_WNTGraphicDevice myGraphicDevice;
void WorkView::initializeGL()
{
//    setMouseTracking(true);
  printf("init %d %d\n", this->width(), this->height());

//    setAttribute( Qt::WA_OpaquePaintEvent );
//    setAttribute( Qt::WA_WState_Visible, false );

//    setFocusPolicy(Qt::StrongFocus);
//    setFocus();

    if(myGraphicDevice.IsNull())
    {
        myGraphicDevice = new Graphic3d_WNTGraphicDevice();
        if (!myGraphicDevice->GraphicDriver().IsNull())
        {
//            myGraphicDriver = Handle(OpenGl_GraphicDriver)::DownCast(myGraphicDevice->GraphicDriver());
        }
    }
	//Handle(V3d_Viewer) myViewer = new V3d_Viewer(myGraphicDevice, TCollection_ExtendedString("Visu3D").ToExtString(), "",
 //                                               1000, V3d_XposYnegZpos,
 //                                               Quantity_NOC_GRAY30, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
 //                                               true, true, V3d_TEX_NONE);
		Handle(V3d_Viewer) myViewer = new V3d_Viewer(myGraphicDevice, TCollection_ExtendedString("Visu3D").ToExtString(), "",
                                                1000, V3d_XposYnegZpos,
                                                Quantity_NOC_GRAY30, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
                                                true, true, V3d_TEX_NONE);

    myViewer->Init();

	myViewer->SetDefaultBackgroundColor(Quantity_NOC_BLACK);
/*
    myView = aViewer->CreateView();
    myIC = new AIS_InteractiveContext(aViewer);
    myIC->SetDeviationCoefficient(1.e-3);


	TCollection_ExtendedString a3DName("Visu3D");
	myViewer = new V3d_Viewer( myGraphicDevice, a3DName.ToExtString(),"", 1000.0, 
									V3d_XposYnegZpos, Quantity_NOC_GRAY30,
									V3d_ZBUFFER,V3d_GOURAUD,V3d_WAIT, 
									Standard_True, Standard_False);

	myViewer->Init();
*/
//	myViewer->SetDefaultLights();
//	myViewer->SetLightOn();
	myView = myViewer->CreateView();

    int       windowHandle = (int) winId();
    short     hi, lo;

    lo = (short) windowHandle;
    hi = (short) (windowHandle >> 16);
    Handle(WNT_Window) aWNTWindow =
        new WNT_Window(Handle(Graphic3d_WNTGraphicDevice)::DownCast(myViewer->Device()), (int) hi, (int) lo);

//	Handle(WNT_Window) aWNTWindow = new WNT_Window(myGraphicDevice, reinterpret_cast<HWND> (winId()));
	myView->SetWindow(aWNTWindow);
	if (!aWNTWindow->IsMapped()) 
		 aWNTWindow->Map();
	myContext = new AIS_InteractiveContext(myViewer);
	myContext->UpdateCurrentViewer();
	myView->Redraw();
	myView->MustBeResized();
	return;


    try {

 //       glewInit();

        #ifdef WNT
            Handle(Graphic3d_WNTGraphicDevice) myDevice=new Graphic3d_WNTGraphicDevice();
            myWindow=new WNT_Window(myDevice,winId());
            myWindow->SetFlags( WDF_NOERASEBKGRND );
        #else
            Handle(Graphic3d_GraphicDevice) myDevice=new Graphic3d_GraphicDevice(getenv("DISPLAY"));
            myWindow=new Xw_Window(new Graphic3d_GraphicDevice(getenv("DISPLAY")),winId(),Xw_WQ_SAMEQUALITY);
        #endif
        myWindow->SetBackground(Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB));
//        myWindow->SetDoubleBuffer(Standard_True);

        myViewer=new V3d_Viewer(myDevice, Standard_ExtString("Visual3D"), Standard_CString("DBMReflex"), 1000.0 ,V3d_Yneg,
                Quantity_NOC_BLACK, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT, Standard_False,
                Standard_False, V3d_TEX_NONE);
        myViewer->SetDefaultBackgroundColor(Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB));
        myViewer->SetLightOff();
        myViewer->SetLightOn( new V3d_DirectionalLight(myViewer, V3d_XposYnegZneg, Quantity_NOC_GRAY45, Standard_True) );
        myViewer->SetLightOn( new V3d_DirectionalLight(myViewer, V3d_XnegYnegZneg, Quantity_NOC_GRAY85, Standard_True) );
        myViewer->SetLightOn( new V3d_DirectionalLight(myViewer, V3d_Zneg, Quantity_NOC_GRAY10, Standard_True) );
        myViewer->SetLightOn( new V3d_AmbientLight(myViewer));	//, Quantity_NOC_GRAY15));

        myView=new V3d_OrthographicView(myViewer);
        myView->SetWindow(myWindow);
//        myView->SetWindow( myWindow, 0, paintCallBack, this );
//        myView->ZBufferTriedronSetup();
//        myView->TriedronDisplay(Aspect_TOTP_RIGHT_UPPER,Quantity_NOC_YELLOW,0.08, V3d_ZBUFFER);
//        myView->SetTransparency(Standard_True);
//        myView->SetSurfaceDetail(V3d_TEX_ALL);
        myView->SetComputedMode (Standard_False);

        myContext = new AIS_InteractiveContext( myViewer );

        myContext->SetDisplayMode(AIS_Shaded);

        rotX= 0.0;
        rotY= -1.570796327;
        myView->Rotate(rotX, rotY, 0, 418.5, 250, 989, Standard_True);

//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));

        loadAllModel();

        myView->FitAll();
        myView->ZFitAll();

        qApp->restoreOverrideCursor();
    }
    catch (Standard_Failure) {
    }

}

bool mustBeResized = false;
void WorkView::paintGL()
{
//    glClearColor(0.58f, 0.65f, 0.76f, 1.0f);
//    printf("paint %d %d\n", this->width(), this->height());
 //   if(mustBeResized) myView->MustBeResized();
 //   myView->Redraw();
}

void WorkView::paintOpenGL()
{

//    glMatrixMode(GL_MODELVIEW);
//        glLoadIdentity();
     glBegin(GL_LINES);
     //红色X轴
//     glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCheTou);
     glVertex3f(0.0f,0.0f,0.0f);
     glVertex3f(25.5f,0.0f,0.0f);
     glVertex3f(25.5f,0.0f,0.0f);
     glVertex3f(24.0f,1.0f,0.0f);
     glVertex3f(25.5f,0.0f,0.0f);
     glVertex3f(24.0f,-1.0f,0.0f);
 //  outputfont(26.5,0.0,"X");
     // 绿色Y轴
//     glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaoPei);
     glVertex3f(0.0f,0.0f,0.0f);
     glVertex3f(0.0f,25.5f,0.0f);
     glVertex3f(0.0f,25.5f,0.0f);
     glVertex3f(1.0f,24.0f,0.0f);
     glVertex3f(0.0f,25.5f,0.0f);
     glVertex3f(-1.0f,24.0f,0.0f);
     // 蓝色Z轴
//     glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseJiaTou);
     glVertex3f(0.0f,0.0f,0.0f);
     glVertex3f(0.0f,0.0f,25.5f);
     glVertex3f(0.0f,0.0f,25.5f);
     glVertex3f(0.0f,1.0f,24.0f);
     glVertex3f(0.0f,0.0f,25.5f);
     glVertex3f(0.0f,-1.0f,24.0f);
     glEnd();
}

void WorkView::resizeGL(int width, int height)
{
    mustBeResized = true;
    printf("%d %d\n", width, height);
}

#include <QTranslator>
#include <QApplication>
//extern                QTranslator translator;

void WorkView::mousePressEvent ( QMouseEvent* e )
{
        startPos = e->pos();
        if(e->button() == Qt::LeftButton) {
                mouseLeftPressed = true;
        }
        if(e->button() == Qt::RightButton) {
                mouseRightPressed = true;
        }
        if(e->button() == Qt::MidButton) {
                mouseMiddleBtn = true;
        }
}

void WorkView::mouseReleaseEvent( QMouseEvent* e )
{
        Q_UNUSED(e);
        mouseLeftPressed = false;
        mouseRightPressed = false;
        mouseMiddleBtn = false;
}

void WorkView::mouseMoveEvent(QMouseEvent* e)
{
    if( !myView.IsNull() )
    {
        if(mouseLeftPressed){
            rotX += (- startPos.x() + e->x())*0.00314159;//*0.5;
            rotY += ( startPos.y() - e->y())*0.00314159;//*0.5;

            if (rotX>-0.000002 && rotX<0.000002) rotX = 0.0;
            if (rotY>-0.000002 && rotY<0.000002) rotY = 0.0;

            myView->Rotate(rotX, rotY, 0, 989, 250, 418.5, Standard_False);
            startPos = e->pos();
        }
        if(mouseMiddleBtn){
            myView->Pan( e->x() - startPos.x(), startPos.y() - e->y() );
            startPos = e->pos();
        }
        if(mouseRightPressed){

        }
    }
}

void WorkView::wheelEvent( QWheelEvent* e )
{
    if( !myView.IsNull() )
    {
        Standard_Real currentScale = myView->Scale();
        if (e->delta() > 0)
        {
            currentScale *= 1.2; // +5%
        }
        else
        {
            currentScale /= 1.2; // -5%
        }
        myView->SetScale( currentScale );
    }
}


void WorkView::timerEvent( QTimerEvent* e)
{
 Q_UNUSED(e);
//
//
// killTimer(timerId);
//// printf("aaa\n");
//
//// spindleRotate = spindleRotate + 2.0;
//////	trsfSpindle.SetRotation(gp::OX(), spindleRotate);
////	trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), spindleRotate);
////	trsfSpindle.SetTranslationPart(gp_Vec(418.5, 250, 989));
////	topoSpindle.Location(TopLoc_Location(trsfSpindle));
////	aisSpindle->Set(topoSpindle);
////	aisSpindle->Redisplay();
////	myView->Redraw();
//
//        spindleRotate = spindleRotate + spindleStep;	//0.632153093;
//        trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), spindleRotate);
//        trsfSpindle.SetTranslationPart(gp_Vec(418.5, 250, 989));
//        topoSpindle = Handle_AIS_Shape::DownCast(aisCheTou)->Shape();
//        topoSpindle.Location(TopLoc_Location(trsfSpindle));
//        aisCheTou->Set(topoSpindle);
////aisCheTou->SetLocation(TopLoc_Location(trsfSpindle));
//
//        trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(418.5, 250, 989), gp_Dir(1, 0, 0)), spindleRotate);
//        topoSpindle = Handle_AIS_Shape::DownCast(aisJiaTou)->Shape();
//        topoSpindle.Location(TopLoc_Location(trsfSpindle));
//        aisJiaTou->Set(topoSpindle);
////aisJiaTou->SetLocation(TopLoc_Location(trsfSpindle));
//
////	trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(0, 0, 0), gp_Dir(1, 0, 0)), spindleRotate);
////	trsfSpindle.SetTranslationPart(gp_Vec(418.5 + 67, 250, 989));
//
////	trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(418.5, 250, 989), gp_Dir(1, 0, 0)), spindleRotate);
////	topoSpindle = Handle_AIS_Shape::DownCast(aisMaoPei)->Shape();
////	topoSpindle.Location(TopLoc_Location(trsfSpindle * trsfMaoPei));
////	aisMaoPei->Set(topoSpindle);
//
////aisMaoPei->SetLocation(TopLoc_Location(trsfSpindle));
//
////   double start,finish;
////   double totaltime;
////   start=clock();
//
//        aisCheTou->Redisplay(Standard_False);
////	myContext->Redisplay(aisCheTou, Standard_False, Standard_False);
//        aisJiaTou->Redisplay(Standard_False);
////	myContext->Redisplay(aisJiaTou, Standard_False, Standard_False);
//
////	aisMaoPei->Redisplay(Standard_False);
//
//
//
//
////
////			topoSpindle = Handle_AIS_Shape::DownCast(aisDaoPian)->Shape();
////			TopOpeBRep_ShapeIntersector si;
//////			si.InitIntersection(Handle_AIS_Shape::DownCast(aisJiaTou)->Shape(), topoSpindle);
//////			if (si.MoreIntersection()) {
//////				update();
//////				printf("发生碰刀，请检查后再继续!\n");
//////
//////				topoSpindle = Handle_AIS_Shape::DownCast(aisCheDao)->Shape();
//////				trsfAxisX.SetTranslationPart(gp_Vec(2, 0, 0));
//////				topoSpindle.Move(TopLoc_Location(trsfAxisX));
//////				aisCheDao->Set(topoSpindle);
//////				aisCheDao->Redisplay();
//////
//////				topoSpindle = Handle_AIS_Shape::DownCast(aisDaoPian)->Shape();
//////				trsfAxisX.SetTranslationPart(gp_Vec(2, 0, 0));
//////				topoSpindle.Move(TopLoc_Location(trsfAxisX));
//////				aisDaoPian->Set(topoSpindle);
//////				aisDaoPian->Redisplay();
//////
//////				update();
//////				timerId = startTimer(50);
//////				return;
//////			}
//////
////			si.InitIntersection(Handle_AIS_Shape::DownCast(aisMaoPei)->Shape(), topoSpindle);
////			if (si.MoreIntersection()) {
////				TopoDS_Shape topoMaoPei;
////
////				topoMaoPei = Handle_AIS_Shape::DownCast(aisMaoPei)->Shape();
////
////			   double start,finish;
////			   double totaltime;
////			   start=clock();
////
////				BRepAlgoAPI_BooleanOperation* tt = new BRepAlgoAPI_Cut(topoMaoPei, Handle_AIS_Shape::DownCast(aisDaoPian)->Shape());
////
////			   finish=clock();
////			   totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
////			   printf("\n切削用时为 %f 毫秒！", totaltime * 1000);
////
////				if (tt->IsDone()) {
////					topoMaoPei = tt->Shape();
////					trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(418.5, 250, 989), gp_Dir(1, 0, 0)), spindleRotate);
////					trsfMaoPei.SetRotation(gp_Ax1(gp_Pnt(418.5, 250, 989), gp_Dir(1, 0, 0)), -spindleRotate);
////					topoMaoPei.Location(TopLoc_Location(trsfSpindle * trsfMaoPei));
////
////					aisMaoPei->Set(topoMaoPei);
////					aisMaoPei->Redisplay();
////
////if (totaltime > 2) {
////		MeshTranslate meshTranslate;
////	meshTranslate.saveModel(tr("resource/model/maopei22.brep"), topoMaoPei);
////}
////
////					} else {
////					printf("切削出错: %d\n", tt->ErrorStatus());
////				}
////
////			}
//
//
//
//
////    double start,finish;
////   double totaltime;
////   start=clock();
//
//
////	update();
//
//
//
//
////   finish=clock();
////   totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
////   printf("\n旋转用时为 %f 毫秒！", totaltime * 1000);
//
//        myView->Redraw();
//
//timerId = startTimer(40);
//
//
////   finish=clock();
////   totaltime=(double)(finish-start)/CLOCKS_PER_SEC;
////   printf("\n旋转用时为 %f 毫秒！", totaltime * 1000);

}
