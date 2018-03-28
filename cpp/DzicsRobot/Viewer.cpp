#include "Viewer.h"
//#include "Timer.h"

#include <QApplication.h>
#include <QCursor.h>
#include <QMessagebox.h>

#include <QMouseEvent>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>

#include <AIS_ListOfInteractive.hxx>
#include <AIS_ListIteratorOfListOfInteractive.hxx>
#include <V3d_DirectionalLight.hxx>
#include <V3d_AmbientLight.hxx>
//#include <V3d_OrthographicView.hxx>
#include <TopTools_HSequenceOfShape.hxx>
#include <gp_Quaternion.hxx>

//#include <Graphic3d.hxx>
#include <Graphic3d_GraphicDriver.hxx>

#include <Geom_Conic.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <TopoDS_Edge.hxx>
#include <TopoDS.hxx>
#include <Geom_Line.hxx>

#include <Prs3d_Presentation.hxx>
#include <Prs3d_TextAspect.hxx>
#include <Prs3d_Text.hxx>
#include <Resource_Unicode.hxx>

#include <Draw_Text3D.hxx>
#include <OSD_Environment.hxx>

#include <BRepAdaptor_Curve.hxx>
#include <GeomAdaptor.hxx>
#include <AIS_ConnectedInteractive.hxx>
#include <QDockWidget>
#include "Global.h"



#include <BRepPrimAPI_MakeBox.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <BRepBuilderAPI_NurbsConvert.hxx>
#include <BRepPrimAPI_MakeCone.hxx>

//#include <QPushButton>

//#include <vector>
//#include <map>

//using namespace std;

//struct Axis
//{
//    QString id;
//    QString sign;
//    QString name;
//    QString parentId;
//    int type;
//    double start[3];
//    double end[3];
//    double stepValue[3];
//    double move[3];
//    double originOffset;    // 零坐标偏移值
//    double value;
//    int symbol;
//	double minValue;
//	double maxValue;
//    std::vector<QString> subAxisList;
//    std::vector<QString> moduleList;
//};
//std::map<QString, Axis*> axisMap;
//std::map<QString, QString> axisSignMap;

//struct Module
//{
//    QString id;
//    QString code;
//    QString name;
//    int type;
//    QString fileId;
//    QString bindAxis;
//    double pos[3];
//    double rotatePoint[3];
//    double rotateDirection[3];
//    double rotateAngle;
//	double transparency;
//    int material;
//    int color;
//	bool isShow;
//    QString parentId;
//    gp_Trsf transform;
//    Handle_AIS_Shape aisShape;
//};
//std::map<QString, Module*> moduleMap;

//#include "mesh/MeshTranslate.h"
#include "SysUtils.h"
#include "ModuleLoadThread.h"
#include "ModuleImportThread.h"
#include "AxisObject.h"
#include "ModuleObject.h"
#include "Global.h"

static Handle(Graphic3d_GraphicDriver) myDevice;

QList<int> glDrawList;

//static inline void DrawTriChip( MeshTranslate::STL_FORMAT face )
//{
////	if (face.attribute & 0x8000) {
////		glColor3b((face.attribute >> 10) & 0x1F, (face.attribute >> 5) & 0x1F, (face.attribute) & 0x1F);
////	}
//	glBegin(GL_TRIANGLES);
//		glNormal3d(face.normal[0],face.normal[1],face.normal[2]);
//		glVertex3d(face.v1[0],face.v1[1],face.v1[2]);
//		glVertex3d(face.v2[0],face.v2[1],face.v2[2]);
//		glVertex3d(face.v3[0],face.v3[1],face.v3[2]);
//	glEnd();
//}

#include <QSettings>
int robotType;
//extern QString robotId, robotPath;
//extern QMap<QString, AxisObject*> axisMap;
//extern QMap<QString, ModuleObject*> moduleMap;

qreal totalScaleFactor = 1;

Viewer::Viewer(QWidget* parent):QWidget(parent)
{
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

	setAttribute(Qt::WA_AcceptTouchEvents);

    OSD_Environment CasRootEnv("CASROOT");
    TCollection_AsciiString CasRootString(CasRootEnv.Value());
    if(CasRootString.Length() <= 0){
        CasRootEnv.SetValue("D:\\DzicsRobot\\support");
        CasRootEnv.Build();
    }

//	glewInit();

//printf("MSAA: buffers = %d samples = %d\n", bufs, samples);

//    myDevice = new Graphic3d_WNTGraphicDevice();
	  if (myDevice.IsNull())
	  {
			Handle(Aspect_DisplayConnection) aDisplayConnection;
		#if !defined(_WIN32) && !defined(__WIN32__) && (!defined(__APPLE__) || defined(MACOSX_USE_GLX))
			aDisplayConnection = new Aspect_DisplayConnection (aDisplay);
		#endif
//			myDevice = Graphic3d::InitGraphicDriver (aDisplayConnection);   // before 6.7
            myDevice = new OpenGl_GraphicDriver(aDisplayConnection);
	  }

    myViewer = new V3d_Viewer(myDevice, Standard_ExtString("Visual3D"), Standard_CString("DBMReflex"), 3000.0 ,/*V3d_XposYnegZneg*/ V3d_XposYneg,
                Quantity_NOC_BLACK, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT, Standard_False,
                Standard_False, V3d_TEX_NONE);
	//myViewer = new V3d_Viewer(myDevice, TCollection_ExtendedString("Visu3D").ToExtString(), "",
 //                                               1000, V3d_XposYnegZpos,
 //                                               Quantity_NOC_GRAY30, V3d_ZBUFFER, V3d_GOURAUD, V3d_WAIT,
 //                                               true, true, V3d_TEX_NONE);
 
//    myViewer->Init();

   // myViewer->SetDefaultBackgroundColor(Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB));
//	myViewer->SetDefaultBgGradientColors(Quantity_NOC_GRAY10, Quantity_NOC_GRAY90, Aspect_GFM_VER);
//	myViewer->SetDefaultBgGradientColors(Quantity_Color(Standard_Real(0.78f), Standard_Real(0.85f), Standard_Real(0.96f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB), Aspect_GFM_VER);
    myViewer->SetDefaultBgGradientColors(Quantity_Color(Standard_Real(0.59f), Standard_Real(0.75f), Standard_Real(0.86f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.90f), Standard_Real(0.90f), Standard_Real(0.90f), Quantity_TOC_RGB), Aspect_GFM_VER);

//    myViewer->SetDefaultLights();
    myViewer->SetLightOff();
    myViewer->SetLightOn( new V3d_DirectionalLight(myViewer, V3d_XposYnegZneg, Quantity_NOC_GRAY45, Standard_True) );
    myViewer->SetLightOn( new V3d_DirectionalLight(myViewer, V3d_XnegYnegZneg, Quantity_NOC_GRAY85, Standard_True) );
    myViewer->SetLightOn( new V3d_DirectionalLight(myViewer, V3d_Zneg, Quantity_NOC_GRAY10, Standard_True) );
    myViewer->SetLightOn( new V3d_AmbientLight(myViewer) );	//, Quantity_NOC_GRAY15));
    myView = myViewer->CreateView();
//   myView = new V3d_OrthographicView( myViewer );
    //myContext = new AIS_InteractiveContext(myViewer);
    //myIC->SetDeviationCoefficient(1.e-3);

    myContext = new AIS_InteractiveContext( myViewer );
    myContext->SetDisplayMode(AIS_Shaded);   // AIS_WireFrame   // AIS_Shaded
	

    //int       windowHandle = (int) winId();
    //short     hi, lo;
    //lo = (short) windowHandle;
    //hi = (short) (windowHandle >> 16);
//    myWindow = new WNT_Window(Handle(Graphic3d_WNTGraphicDevice)::DownCast(myViewer->Device()), winId());
    myWindow = new WNT_Window(winId());
//	myWindow->SetFlags( WDF_NOERASEBKGRND );

//	myWindow->SetBackground(Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB));
//	myWindow->SetBackground(Quantity_Color(Standard_Real(0.68f), Standard_Real(0.75f), Standard_Real(0.86f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB), Aspect_GFM_VER);
//ug
//    myWindow->SetBackground(Quantity_Color(Standard_Real(0.59f), Standard_Real(0.75f), Standard_Real(0.86f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.90f), Standard_Real(0.90f), Standard_Real(0.90f), Quantity_TOC_RGB), Aspect_GFM_VER);
//    myWindow->SetBackground(Quantity_Color(Standard_Real(0.90f), Standard_Real(0.90f), Standard_Real(0.90f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.59f), Standard_Real(0.75f), Standard_Real(0.86f), Quantity_TOC_RGB), Aspect_GFM_VER);
//abb
//    myWindow->SetBackground(Quantity_Color(Standard_Real(0.89f), Standard_Real(0.96f), Standard_Real(1.0f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.7f), Standard_Real(0.76f), Standard_Real(0.84f), Quantity_TOC_RGB), Aspect_GFM_VER);
    myWindow->SetBackground(Quantity_Color(Standard_Real(0.89f), Standard_Real(0.96f), Standard_Real(1.0f), Quantity_TOC_RGB), Quantity_Color(Standard_Real(0.58f), Standard_Real(0.65f), Standard_Real(0.76f), Quantity_TOC_RGB), Aspect_GFM_VER);

   myView->SetWindow(myWindow);
//   myView->SetWindow( myWindow, 0, paintCallBack, this );
   if(!myWindow->IsMapped())
        myWindow->Map();

    myView->SetComputedMode (Standard_False);

	myView->MustBeResized();

	myView->ZBufferTriedronSetup();
    myView->TriedronDisplay(Aspect_TOTP_LEFT_LOWER, Quantity_NOC_BLACK, 0.05, V3d_ZBUFFER);

//	myView->SetAntialiasingOn();

    rotX= SysUtils::getSysParam("viewRotateX").toDouble();//0.0;
    rotY= SysUtils::getSysParam("viewRotateY").toDouble() - 0.5;//0.0;//-1.570796327;
    //myView->Rotate(rotX, rotY, 0, 418.5, 250, 989, Standard_True);
	
//    myView->Rotate(rotX, rotY, 0, SysUtils::getSysParam("viewCenterX").toDouble(), SysUtils::getSysParam("viewCenterY").toDouble(), SysUtils::getSysParam("viewCenterZ").toDouble(), Standard_True);
    myView->Rotate(rotX, rotY, 0, 0,0,0, Standard_True);
 //   myView->Rotate(rotX, rotY, 0, 374, 0, 630, Standard_True);

//    myView->Pan(0, -3.0, 0.8);  // before 6.7
//	myView->Pan(0, -10, 0.8);
//	myView->Pan(0, -10, 0.6);

    myView->SetScale(1.0);	//2.0
    //设置三维显示长度
    myView->SetZSize( 100000.0);//20000.0 );

	//MeshTranslate::saveModel(tr("resource/robot/IRB120_3-58_IRC5_rev0.brep"), MeshTranslate::loadSTEP(tr("resource/robot/IRB120_3-58_IRC5_rev0.STEP")));
//	MeshTranslate::saveModel(tr("resource/robot/irb260.brep"), MeshTranslate::loadSTEP(tr("resource/robot/irb260.STEP")));
//	MeshTranslate::saveModel(tr("resource/robot/stl/base.brep"), MeshTranslate::loadSTL(tr("resource/robot/stl/BASE.STL")));
//	MeshTranslate::saveModel(tr("resource/robot/stl/link1.brep"), MeshTranslate::loadSTL(tr("resource/robot/stl/LINK1.STL")));
//	MeshTranslate::saveModel(tr("resource/robot/stl/link2.brep"), MeshTranslate::loadSTL(tr("resource/robot/stl/LINK2.STL")));
// MeshTranslate::loadSTL(tr("resource/robot/BASE.STL"));


	//	myContext->Display(MeshTranslate::importModel(tr("resource/robot/IRB120.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/robot/irb260.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));
//        myContext->Display(MeshTranslate::importModel(tr("resource/model/base.brep")));

//	QSettings *configIniRead = new QSettings("D:\\DzicsRobot\\ROBOT.INI", QSettings::IniFormat);
//    robotId = configIniRead->value("/Robot/SYSID").toString();
//	robotPath = configIniRead->value("/Robot/PATH").toString();
//	robotType = configIniRead->value("/Robot/TYPE").toString().toInt();
//	delete configIniRead;

//	myContext->SetDisplayMode(AIS_WireFrame);
//myView->SetComputedMode(true);

//    loadAllModule();    // old


	//for(int i=0;i<stlList.count();i++) {
	//	DrawTriChip(stlList[i]);
	//}

    //myView->FitAll();
    //myView->ZFitAll();


    setAttribute( Qt::WA_PaintOnScreen );
    setAttribute( Qt::WA_NoSystemBackground );
	setFocusPolicy( Qt::StrongFocus );
    setViewUpdateState(true);

//    qApp->restoreOverrideCursor();

    mouseLeftPressed = false;
    mouseRightPressed = false;
    mouseMiddleBtn = false;
//	viewMouseMode = 1;
    toolSelectMode = false;

	this->setMouseTracking(true);

// 捕捉组件
//myContext->CloseAllContexts ( true );
//myContext->OpenLocalContext ( true , true , false , false );
//myContext->ActivateStandardMode ( TopAbs_COMPOUND );	//TopAbs_EDGE, TopAbs_VERTEX

// 捕捉面
//myContext->CloseAllContexts ( true );
//myContext->OpenLocalContext ( true , true , false , false );
//myContext->ActivateStandardMode ( TopAbs_FACE );	//TopAbs_EDGE, TopAbs_VERTEX

// 捕捉顶点
//myContext->CloseAllContexts ( true );
//myContext->OpenLocalContext ( true , true , false , false );
//myContext->ActivateStandardMode ( TopAbs_VERTEX );	//TopAbs_EDGE, TopAbs_FACE

//捕捉边
//myContext->CloseAllContexts ( true );
//myContext->OpenLocalContext ( true , true , false , false );
//myContext->ActivateStandardMode ( TopAbs_EDGE );	//TopAbs_FACE, TopAbs_VERTEX

// 捕捉边和顶点
//myContext->CloseAllContexts ( true );
//myContext->OpenLocalContext ( true , true , false , false );
//myContext->ActivateStandardMode ( TopAbs_VERTEX );	//TopAbs_EDGE, TopAbs_FACE
//myContext->ActivateStandardMode ( TopAbs_EDGE );	//TopAbs_FACE, TopAbs_VERTEX

//    myContext->CloseAllContexts ( true );
//    myContext->OpenLocalContext ( true , true , false , false );
//    myContext->ActivateStandardMode(TopAbs_WIRE);

	spindleRotate = 0.0;

//	startTimer(50);

//	connect(parent->parent()->parent(), SIGNAL(syncPosition(QStringList)), this, SLOT(syncPosition(QStringList)), Qt::DirectConnection);
//	connect(parent->parent()->parent(), SIGNAL(syncDisplayState( int, int )), this, SLOT(syncDisplayState( int, int )), Qt::DirectConnection);
////	connect(parent->parent()->parent(), SIGNAL(updateUnitData(QByteArray)), this, SLOT(updateUnitData(QByteArray)), Qt::QueuedConnection);

//	connect(parent->parent()->parent(), SIGNAL(showAll()), this, SLOT(showAll()), Qt::QueuedConnection);


//    QString lastStationId = SysUtils::getSysParam( "lastStationId" ).toString();
//    if (!lastStationId.isEmpty()) {
//        this->stationChanged( lastStationId );
//    }

//    QPushButton *button = new QPushButton("TEST", this);
//    button->move(100,100);
//    button->show();


    loadingText = new Prs3d_Presentation(myViewer->Viewer());
    text1=new Prs3d_Presentation(myViewer->Viewer());
    Handle_Prs3d_TextAspect aTextAspect= new Prs3d_TextAspect();
    aTextAspect->SetFont("Monospace");//Graphic3d_NOF_ASCII_COMPLEX);
//    aTextAspect->SetFont("simhei");// "Monospace");//Graphic3d_NOF_ASCII_COMPLEX);
    aTextAspect->SetHeight(30);
    aTextAspect->SetColor(Quantity_NOC_YELLOW);
    gp_Pnt p;
//    p.SetX(0);
//    p.SetY(0);
//    p.SetZ(0);
//    TCollection_ExtendedString name;
//    Resource_Unicode::ConvertGBToUnicode("正在加载方案...", name);
//    Prs3d_Text::Draw(loadingText,aTextAspect,name, p);

    Prs3d_Text::Draw(loadingText, aTextAspect, "Loading...", p);
//    Draw_Text3D t(p, "Loading...", Draw_orange);
    loadingText->Display();

    m_pFloatTool = NULL;
}

Viewer::~Viewer()
{

}

void Viewer::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    //switch (e->type()) {
    //default:
    //    break;
    //}
}

int Viewer::paintCallBack (Aspect_Drawable /* drawable */,
                                void* aPointer,
                                Aspect_GraphicCallbackStruct* /* data */)
{

////    static GLfloat ambient[] = {0.3, 0.3, 0.3, 1.0};
////    static GLfloat diffuse[] = {0.65, 0.65, 0.65, 1.0};
////    static GLfloat diffuse[] = {0.15, 0.15, 0.15, 1.0};
//    static GLfloat specular[] = {0.0, 0.0, 0.0, 1.0};

//       static GLfloat ambient[] = {1.0, 1.0, 1.0, 0.15};
//       static GLfloat dl[] = {1.0, 1.0, 1.0, 0.45};
//       static GLfloat dr[] = {1.0, 1.0, 1.0, 0.85};

//static float Imodel_ambient[] = {1.0,1.0,1.0,0.15};
//static float Imodel_twoside[] = {GL_TRUE};
////glLightModelfv(GL_LIGHT_MODEL_AMBIENT,Imodel_ambient);
////glLightModelfv(GL_LIGHT_MODEL_TWO_SIDE,Imodel_twoside);

// //   glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
//    glMaterialfv(GL_FRONT_LEFT, GL_DIFFUSE, dl);
//    glMaterialfv(GL_FRONT_RIGHT, GL_DIFFUSE, dr);

//    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
//    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.039f);
//    glEnable( GL_LIGHTING );
////glShadeModel(GL_SMOOTH);

    ((Viewer *) aPointer)->paintOpenGL();
    return 0;
}


#include <BRepTools_ShapeSet.hxx>
#include <BRep_Builder.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_Shape.hxx>
#include <Standard_Stream.hxx>
#include <BRepTools.hxx>

#include <time.h>
#include <iostream>


//DWORD WINAPI Fun1Proc(LPVOID lpParameter);//thread data
//HANDLE hThread1;
bool Viewer::loadAllModule()
{

////    gp_Trsf transform;

////    for (int iii=0;iii<1;iii++){
////        TopoDS_Shape shapeT;
////        Handle_AIS_Shape aisShape = new AIS_Shape(shapeT);
////        aisShape->SetContext(myContext);
////        ModuleLoadThread *loadThread = new ModuleLoadThread("e:\\test.brep", aisShape, transform, true);
////        connect(loadThread, SIGNAL(moduleLoadFinish(QString)), this, SLOT(moduleLoadFinish(QString)));
////    loadThread->start();
////    }
////    return true;

//    double start = clock();

////            QSqlDatabase db = QSqlDatabase::database();
////            db.transaction();
//    QSqlQuery query;
//    QString id;

//    query.exec("SELECT * FROM Axis WHERE isUse=1 ORDER BY displayOrder");
//    while(query.next()) {
//        QSqlRecord record = query.record();
//        id = record.value("id").toString();
//        Axis *axis = axisMap[id];
//        if (!axis){
//            axis = new Axis;
//            axisMap.insert(std::pair<QString, Axis*>(id,axis));
//        }
//        axis->id = id;
//        axis->sign = record.value("sign").toString();
//        axis->name = record.value("name").toString();
//        axis->parentId = record.value("parentId").toString();
//        axis->type = record.value("type").toInt();
//        axis->start[X] = record.value("startX").toDouble();
//        axis->start[Y] = record.value("startY").toDouble();
//        axis->start[Z] = record.value("startZ").toDouble();
//        axis->end[X] = record.value("endX").toDouble();
//        axis->end[Y] = record.value("endY").toDouble();
//        axis->end[Z] = record.value("endZ").toDouble();
//        axis->symbol = record.value("symbol").toInt();
//        axis->minValue = record.value("minValue").toDouble();
//        axis->maxValue = record.value("maxValue").toDouble();

//        double length = axis->symbol * sqrt((axis->end[X]-axis->start[X])*(axis->end[X]-axis->start[X]) + (axis->end[Y]-axis->start[Y])*(axis->end[Y]-axis->start[Y]) + (axis->end[Z]-axis->start[Z])*(axis->end[Z]-axis->start[Z]));
//        axis->stepValue[X] = (axis->end[X]-axis->start[X]) / length;
//        axis->stepValue[Y] = (axis->end[Y]-axis->start[Y]) / length;
//        axis->stepValue[Z] = (axis->end[Z]-axis->start[Z]) / length;

//        axis->originOffset = record.value("originOffset").toDouble();

//        if (!axis->parentId.isEmpty()) {
//            Axis *parentAxis = axisMap[axis->parentId];
//            if (!parentAxis) {
//                parentAxis = new Axis;
//                axisMap[axis->parentId] = parentAxis;
//            }
//            parentAxis->subAxisList.push_back(axis->id);
//        }


//        if (axis->type == 1 && !axis->sign.isEmpty()) {
//            axis->value = 0.0;
//            axisSignMap[axis->sign] = axis->id;
//         //   emit updateAxisValue(axis->sign, axis->value);
//		} else {
//	        axis->value = axis->originOffset;

//		}

//		axisMap[id] = axis;

////        myContext->Display(new AIS_Line(new Geom_CartesianPoint(axis->start[X],axis->start[Y],axis->start[Z]), new Geom_CartesianPoint(axis->end[X],axis->end[Y],axis->end[Z])), Standard_False);
//    }

//    //BRep_Builder builder = BRep_Builder();
//    //BRepTools_ShapeSet SS(builder);
//    query.exec("SELECT * FROM Module WHERE isUse=1");
//    while(query.next()) {
//        QSqlRecord record = query.record();
//        Module *module = new Module;
//        module->id = record.value("id").toString();
//        module->code = record.value("code").toString();
//        module->name = record.value("name").toString();
//        module->type = record.value("type").toInt();
//        module->fileId = record.value("fileId").toString();
//        module->bindAxis = record.value("bindAxis").toString();
//        module->pos[X] = record.value("x").toDouble();
//        module->pos[Y] = record.value("y").toDouble();
//        module->pos[Z] = record.value("z").toDouble();

//		module->transparency = record.value("parentId").toDouble();

////        module->rotatePoint[X] = 0.0;
////        module->rotatePoint[Y] = 0.0;
////        module->rotatePoint[Z] = 0.0;
//        module->rotateDirection[X] = record.value("rotateX").toDouble();
//        module->rotateDirection[Y] = record.value("rotateY").toDouble();
//        module->rotateDirection[Z] = record.value("rotateZ").toDouble();
//        module->rotateAngle = record.value("rotateAngle").toDouble();
//        module->material = record.value("material").toInt();
//        module->color = record.value("color").toInt();
//		module->isShow = (record.value("isLock").toInt() != 1);

//        if (!module->bindAxis.isEmpty()) axisMap[module->bindAxis]->moduleList.push_back(module->id);
///* gl
//		//QList<MeshTranslate::STL_FORMAT> stlList;
//		QByteArray data = SysUtils::readFile(SysUtils::getSysParam("modelPath").toString() + module->code + ".stl");
		
//		qint32 count;// = (qint32)(*(data.constData() + 80));
//		memcpy(&count, data.constData() + 80, 4);
//	//file.seek(80);
//	//file.read((char *)(&count), 4);
//	//MeshTranslate::STL_FORMAT *stlList = (MeshTranslate::STL_FORMAT *)malloc(50 * count);// = (MeshTranslate::STL_FORMAT*)(data.constData() + 84);
//	//memcpy(stlList, data.constData() + 84, 50 * count);
////MeshTranslate::STL_FORMAT *stlList = (MeshTranslate::STL_FORMAT*)(data.constData() + 84);
//		int drawId;
//		//MeshTranslate::STL_FORMAT *stlList = MeshTranslate::loadSTLBin(SysUtils::getSysParam("modelPath").toString() + module->code + ".stl") ;
//		drawId = glGenLists(1);
//		GLfloat tf[4]={0.9,0.0,0.0,1};
//		tf[0] = ((module->color>>16) & 0xff) * 0.003;
//		tf[1] = ((module->color>>8) & 0xff) * 0.003;
//		tf[2] = ((module->color) & 0xff) * 0.003;
//		glNewList(drawId, GL_COMPILE);
//			if (module->code == "env") {
//				glEnable(GL_BLEND);
//				glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
//				tf[3] = 0.5;
//			}
//				glPushMatrix();
//			if (module->code == "a4") {

//				//Axis *ax1 = axisMap["2001"];
//				Axis *ax2 = axisMap["2004"];
//				//gp_Trsf trsf1, trsf;
//				//gp_Trsf trsf2;
//				//trsf1.SetRotation(gp_Ax1(gp_Pnt(ax1->start[X], ax1->start[Y], ax1->start[Z]), gp_Dir(ax1->end[X]-ax1->start[X], ax1->end[Y]-ax1->start[Y], ax1->end[Z]-ax1->start[Z])), 1);//ax1->value);
//				//trsf2.SetRotation(gp_Ax1(gp_Pnt(ax2->start[X], ax2->start[Y], ax2->start[Z]), gp_Dir(ax2->end[X]-ax2->start[X], ax2->end[Y]-ax2->start[Y], ax2->end[Z]-ax2->start[Z])), 1);//ax2->value);
//				//float mm[16]={
//				//	1., 0., 0., 0.,
//				//	0., 1., 0., 0.,
//				//	0., 0., 1., 0.,
//				//	0., 0., 0., 1.0
//				//};
//				//float mm1[16]={
//				//	1., 0., 0., 0.,
//				//	0., cosf(1.5707963), sinf(1.5707963), 0.,
//				//	0., -sinf(1.5707963), cosf(1.5707963), 0.,
//				//	ax2->start[X], ax2->start[Y], ax2->start[Z], 1.0
//				//};
//				//glMultMatrixf(mm1);
//				//float mm2[16]={
//				//	cosf(1.5707963), 0., -sinf(1.5707963), 0.,
//				//	0., 1., 0., 0.,
//				//	sinf(1.5707963), 0., cosf(1.5707963), 0.,
//				//	ax2->start[X], ax2->start[Y], ax2->start[Z], 1.0
//				//};
//				//glMultMatrixf(mm2);
//				//glTranslatef(-ax2->start[X], -ax2->start[Y], -ax2->start[Z]);

//				SysUtils::glRotate(X, ax2->start, 1.5707963);

////glRotate(Y, ax2->start[X], ax2->start[Y], ax2->start[Z], 1.5707963);
//				//trsf = trsf1;
//				//mm[0] = trsf.Value(1, 1);
//				//mm[4] = trsf.Value(1, 2);
//				//mm[8] = trsf.Value(1, 3);
//				////mm[3] = trsf.Value(1, 4);
//				//mm[1] = trsf.Value(2, 1);
//				//mm[5] = trsf.Value(2, 2);
//				//mm[9] = trsf.Value(2, 3);
//				////mm[7] = trsf.Value(2, 4);
//				//mm[2] = trsf.Value(3, 1);
//				//mm[6] = trsf.Value(3, 2);
//				//mm[10] = trsf.Value(3, 3);
//				////mm[11] = trsf.Value(3, 4);
//				//mm[3] = trsf.TranslationPart().X();
//				//mm[7] = trsf.TranslationPart().Y();
//				//mm[11] = trsf.TranslationPart().Z();
//				//glMultMatrixf(mm);

//				//trsf = trsf2;
//				//mm[0] = trsf.Value(1, 1);
//				//mm[4] = trsf.Value(1, 2);
//				//mm[8] = trsf.Value(1, 3);
//				////mm[3] = trsf.Value(1, 4);
//				//mm[1] = trsf.Value(2, 1);
//				//mm[5] = trsf.Value(2, 2);
//				//mm[9] = trsf.Value(2, 3);
//				////mm[7] = trsf.Value(2, 4);
//				//mm[2] = trsf.Value(3, 1);
//				//mm[6] = trsf.Value(3, 2);
//				//mm[10] = trsf.Value(3, 3);
//				////mm[11] = trsf.Value(3, 4);
//				//mm[3] = trsf.TranslationPart().X();
//				//mm[7] = trsf.TranslationPart().Y();
//				//mm[11] = trsf.TranslationPart().Z();
//				//glMultMatrixf(mm);

////				glTranslatef(-175, 100, -495);
////glPushMatrix();
//				//glTranslatef(-495, 100, -175);
//			//	glTranslatef(175, -100, 495);
//				//glRotatef(40, 0, 1, 0);
//				//glTranslatef(0, 0, 495);
//				//glTranslatef(0, 175, 0);
//				//glPushMatrix();
//				//glTranslatef(0, 0, 200);
//				//glTranslatef(175, -100, 495);
//				//glTranslatef(-175, 100, -495);

//			}
//			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, tf);
//			for(int i=0;i<count;i++) {
//				MeshTranslate::STL_FORMAT face;
//				memcpy(&face, data.constData() + 84 + i*50, 50);
//				DrawTriChip(face);
//				//DrawTriChip(stlList[i]);
//			}
//			if (module->code == "env") {
//				glDisable(GL_BLEND);
//				tf[3] = 1.0;
//			}
//			//if (module->code == "a1") {
//				//glPopMatrix();
//				glPopMatrix();
//			//}
//		glEndList();
//		glDrawList.append(drawId);

//*/
//			//QByteArray file = SysUtils::getFile(module->fileId);


//  //      std::stringbuf sbuf;
//  //      sbuf.sputn(file.constData(), file.size());
//		//std::iostream inn(&sbuf);

//  //      SS.Read(inn);
//  //      SS.Read(shape, inn);

////		QString filename = SysUtils::getSysParam("modelPath").toString() + module->code + ".brep";
////		QString filename = robotPath + "\\" + module->code + ".brep";
//		QString filename = robotPath + "\\" + module->code;

//		TopoDS_Shape shape;
////		BRep_Builder aBuilder;
////		BRepTools::Read(  shape, filename.toAscii().data(), aBuilder );
////
////
////	Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
////	sps->Append( shape );
////	MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, filename + ".dzm", sps);
////continue;

////Handle_TopTools_HSequenceOfShape sps = MeshTranslate::importModel( MeshTranslate::FormatCSFDB, filename + ".dzm" );
////TopoDS_Shape shape = sps->Value( 1 );

//		if (module->rotateAngle!=0.0) {
//            gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
//            module->transform.SetRotation(ax, module->rotateAngle);
//        }
//		if (module->type == 2) {
//		    gp_Trsf transformEnv;
//            gp_Ax1 az = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(0, 0, 1));
//            transformEnv.SetRotation(az, -0.9948);//2.147);
//			module->transform = transformEnv * module->transform;
//		}
//        module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));

//        if (module->type == 5) {  // 毛坯
////            machineCutting.setTransform(module->transform);
////
//////            machineCutting.setRough(shape);
////
////            //＊圆柱
////            gp_Pnt neckLocation(0, 0, 10);
////            gp_Dir neckNormal = gp_Dir(0, 0, -1);
////            gp_Ax2 neckAx2(neckLocation , neckNormal);
////            Standard_Real myNeckRadius = 30;
////            Standard_Real myNeckHeight = 120;
////            TopoDS_Shape topoMaoPei = BRepPrimAPI_MakeCylinder(neckAx2 , myNeckRadius , myNeckHeight).Shape();
////            machineCutting.setRough(topoMaoPei);

//        } else {
//            shape.Location(TopLoc_Location(module->transform));
//            module->aisShape = new AIS_Shape(shape);
//			if (module->material >= 0){
//	            module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)module->material );
//			} else {
//	            module->aisShape->SetMaterial( Graphic3d_NOM_DEFAULT );//(Graphic3d_NameOfMaterial)module->material );
//	            module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
//			}
//			if (module->transparency > 0.0) {
//				module->aisShape->SetTransparency(module->transparency);
//			}
////            gp_Trsf trsfMove;
////                    trsfMove.SetRotation(gp_Ax1(gp_Pnt(axis->start[X], axis->start[Y], axis->start[Z]), gp_Dir(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), spindleRotate);
////trsfMove.SetTranslationPart(gp_Vec(100, 100, 100));
////trsfMove.SetTranslationPart(gp_Vec(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z]));

////module->aisShape->SetLocation(TopLoc_Location(trsfMove));

//			//if (module->id != "rhaS5iUkS6m1L4JoJMALBQ==" && module->id != "rhaS6UkS6m1L4JoJMALBQ==") {
///*
//			if (module->isShow) {
//	            myContext->Display(module->aisShape, Standard_False);
//				//myContext->Select();
//				//myContext->HilightWithColor(module->aisShape, Quantity_NOC_GRAY25, Standard_False);
//			}
//*/
//	//Handle_TopTools_HSequenceOfShape sps = new TopTools_HSequenceOfShape();
//	//sps->Append( shape );
//	//MeshTranslate::exportModel(MeshTranslate::FormatCSFDB, filename + ".dzm", sps);

//			module->aisShape->SetContext(myContext);
//			ModuleLoadThread *loadThread = new ModuleLoadThread(filename, module->aisShape, module->transform, module->isShow);
//			connect(loadThread, SIGNAL(moduleLoadFinish(QString)), this, SLOT(moduleLoadFinish(QString)));
//			loadThread->start();


//	//BRep_Builder aBuilder;
//	//TopoDS_Shape shape = module->aisShape->Shape();
//	//BRepTools::Read( shape, filename.toAscii().data(), aBuilder );
//	//shape.Location(TopLoc_Location(module->transform));
//	//module->aisShape->Set( shape );
//	//module->aisShape->Redisplay();
//	//if (module->isShow) {
//	//	 moduleLoadFinish( module->aisShape );
//	//}

//        }

//        moduleMap[module->id] = module;
//    }
////    myView->Redraw();
////db.commit();

//    printf("\n显示模块用时为 %f 毫秒！\n", (clock()-start)/1000.0);

	return true;
}

void Viewer::moduleLoadFinish( QString id )
{
	//Module *module = moduleMap[id];
	//if (module->isShow) {
//		myContext->Display(module->aisShape, Standard_True);
//	}
//	myView->SetComputedMode(true);
//	myView->Redraw();

    try{
        if (++moduleLoadedCount < moduleLoadCount)
            return;
    } catch(...) {}
//    ModuleObject *module = moduleMap["2NMFvqD7TAO74EQj1jED"];

//     if (!module->aisShape->GetContext()->IsDisplayed(module->aisShape)) {
//                 module->aisShape->GetContext()->Display(module->aisShape, false);
//                    }
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

//    foreach(ModuleObject *module, moduleMap) {
//        if (!module->isHidden) {
//    //        try {
//               module->aisShape->GetContext()->Display(module->aisShape, Standard_False);
//    //        } catch (...) { }
//        }
//    }

    loadingText->Erase();
    myView->FitAll();
//    myView->Update();

    emit moduleAllLoaded(moduleLoadedCount);

    qApp->restoreOverrideCursor();
}



void Viewer::paintOpenGL()
{

//    GLfloat dd[] = {0.35, 0.45, 0.35, 0.1};
////    glMaterialfv(GL_FRONT, GL_DIFFUSE, dd);

//    glMaterialfv(GL_BACK, GL_AMBIENT, dd);
//    glTranslatef(0,0.0f,0.0f);

//    glBegin(GL_QUADS);
//        glNormal3f(0.0, 1.0, 0.0);
//        glVertex3f(-1500.0f, 1500.0f, 0.0f);					// 左上
//        glVertex3f( 1500.0f, 1500.0f, 0.0f);					// 右上
//        glVertex3f( 1500.0f,-1500.0f, 0.0f);					// 左下
//        glVertex3f(-1500.0f,-1500.0f, 0.0f);					// 右下
//    glEnd();


//    glLoadIdentity();
//    GLuint list = glGenLists(1);
//    HDC hDC = wglGetCurrentDC();
//    wglUseFontBitmaps(hDC, "A", 1, list);
//    glCallList(list);
//    releaseDC(hDC);

//	for (int j=0;j<glDrawList.count();j++) {
//		glCallList(glDrawList[j]);
//		/*for(int i=0;i<glDrawList[j].count();i++) {
//			DrawTriChip(glDrawList[j].at(i));
//		}*/
//	}
//////    glMatrixMode(GL_MODELVIEW);
//////        glLoadIdentity();
////     glBegin(GL_LINES);
//////     glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseCheTou);
////     glVertex3f(0.0f,0.0f,0.0f);
////     glVertex3f(25.5f,0.0f,0.0f);
////     glVertex3f(25.5f,0.0f,0.0f);
////     glVertex3f(24.0f,1.0f,0.0f);
////     glVertex3f(25.5f,0.0f,0.0f);
////     glVertex3f(24.0f,-1.0f,0.0f);
//// //  outputfont(26.5,0.0,"X");
////
//////     glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaoPei);
////     glVertex3f(0.0f,0.0f,0.0f);
////     glVertex3f(0.0f,25.5f,0.0f);
////     glVertex3f(0.0f,25.5f,0.0f);
////     glVertex3f(1.0f,24.0f,0.0f);
////     glVertex3f(0.0f,25.5f,0.0f);
////     glVertex3f(-1.0f,24.0f,0.0f);
////
//////     glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseJiaTou);
////     glVertex3f(0.0f,0.0f,0.0f);
////     glVertex3f(0.0f,0.0f,25.5f);
////     glVertex3f(0.0f,0.0f,25.5f);
////     glVertex3f(0.0f,1.0f,24.0f);
////     glVertex3f(0.0f,0.0f,25.5f);
////     glVertex3f(0.0f,-1.0f,24.0f);
////     glEnd();
}

void Viewer::paintEvent(QPaintEvent * e)
{
    if (!myView.IsNull())
        myView->Redraw();
}


/*!
  Get paint engine for the OpenGL viewer. [ virtual public ]
*/
QPaintEngine* Viewer::paintEngine() const
{
  return 0;
}

void Viewer::resizeEvent(QResizeEvent * e)
{
    if (!myView.IsNull())
    {
        myView->MustBeResized();
    }
}

int lastTouchCount = 0;
bool Viewer::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::TouchBegin:
    case QEvent::TouchUpdate:
    case QEvent::TouchEnd:
    {
        QTouchEvent *touchEvent = static_cast<QTouchEvent *>(event);
        QList<QTouchEvent::TouchPoint> touchPoints = touchEvent->touchPoints();
        if (touchPoints.count() == 2) {
            if (lastTouchCount != touchPoints.count()) {
                lastTouchCount = touchPoints.count();
                break;
            }
            //switch (touchPoints.state()) {
            //case Qt::TouchPointStationary:
            //    continue;
            //default:
            //    {
			    mouseLeftPressed = false;
				mouseRightPressed = false;
				mouseMiddleBtn = false;

					QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();
					QTouchEvent::TouchPoint &touchPoint1 = touchPoints.last();

                    qreal currentScale = myView->Scale();
                    qreal currentScaleFactor = QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
                            / QLineF(touchPoint0.lastPos(), touchPoint1.lastPos()).length();
                    if (currentScaleFactor > 1)
                    {
                        if (currentScale < 500.0)
                            myView->SetScale( currentScale * currentScaleFactor );
                    }
                    else
                    {
                        if (currentScale > 0.05)
                            myView->SetScale( currentScale * currentScaleFactor );
                    }
                    lastTouchCount = touchPoints.count();
                    return true;

					//if (touchPoint0.state() == Qt::TouchPointStationary) {
					//	//&& touchPoint1.state() == Qt::TouchPointStationary) {
					//	QPointF p1 = touchPoint1.pos() - touchPoint1.startPos();
					//	myView->Pan( p1.x(), p1.y() );
					//	return true;
					//}
					//if (touchPoint1.state() == Qt::TouchPointStationary) {
					//	//&& touchPoint1.state() == Qt::TouchPointStationary) {
					//	QPointF p2 = touchPoint0.pos() - touchPoint0.startPos();
					//	myView->Pan( p2.x(), p2.y() );
					//	return true;
					//}
                    /*qreal*/ currentScaleFactor =
							QLineF(touchPoint0.pos(), touchPoint1.pos()).length()
							/ QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();


					//qreal l1 = QLineF(touchPoint0.pos(), touchPoint1.pos()).length();
					//qreal l2 = QLineF(touchPoint0.startPos(), touchPoint1.startPos()).length();
					//if (l1 > l2)
					//{
					//	myView->SetScale( myView->Scale() * 1.1 );
					//}
					//else
					//{
					//	myView->SetScale( myView->Scale() / 1.1 );
					//}

				
					if (touchEvent->touchPointStates() & Qt::TouchPointReleased) {
						if (totalScaleFactor * currentScaleFactor > 0.05 && totalScaleFactor * currentScaleFactor < 100.0) {
							totalScaleFactor *= currentScaleFactor;
						}
						currentScaleFactor = 1;
					}

					//if (currentScaleFactor > 0.9 && currentScaleFactor < 1.1) {
					//	QPointF p1 = (touchPoint1.pos() - touchPoint1.startPos())* 0.3;
					//	myView->Pan( p1.x(), -p1.y() );
					//} else {

					if (totalScaleFactor * currentScaleFactor > 0.05 && totalScaleFactor * currentScaleFactor < 100.0) {
						myView->SetScale( totalScaleFactor * currentScaleFactor );
					}
					//}
            //    }
            //    break;
            //}
        } else if (touchPoints.count()>2){
            if (lastTouchCount != touchPoints.count()) {
                lastTouchCount = touchPoints.count();
                break;
            }
				mouseLeftPressed = false;
				mouseRightPressed = false;
				mouseMiddleBtn = false;

				QTouchEvent::TouchPoint &touchPoint0 = touchPoints.first();

            myView->Pan( touchPoint0.pos().x() - touchPoint0.lastPos().x(), touchPoint0.lastPos().y() - touchPoint0.pos().y() );
		}
        lastTouchCount = touchPoints.count();
        break;
    }
    default:
        return QWidget::event(event);
    }
    return true;
}

void Viewer::mouseDoubleClickEvent ( QMouseEvent* e )
{
    if (e->buttons() != Qt::LeftButton)
        return;
    myContext->MoveTo(e->x(), e->y(), myView);
//    if (e->modifiers().testFlag( Qt::ShiftModifier )) {
//        myContext->ShiftSelect( true );
//    } else {
        myContext->Select( true );
//    }
//        if ( myContext->HasSelectedShape())
//            qDebug("true");
//        else
//            qDebug("false");
//        if ( myContext->SelectedInteractive())
//            qDebug("true");
//        else
//            qDebug("false");
//        TopoDS_Edge edge = TopoDS::Edge( myContext->SelectedShape() );
//        Standard_Real first,last;
//        Handle_Geom_Curve m_curve = BRep_Tool::Curve(edge,first,last);
//        Handle_Geom_Conic hgc =  Handle_Geom_Conic::DownCast(m_curve);
//        gp_Pnt centerPnt = hgc->Location();
//        TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(centerPnt);
//        Handle_AIS_Shape pntVertex1 = new AIS_Shape(pnt1);
//        myContext->Display(pntVertex1 , Standard_True);

//        for(myContext->InitSelected();myContext->MoreSelected();myContext->NextSelected())
//            {
//                // select Edge
//                if(myContext->HasSelectedShape() && myContext->SelectedShape().ShapeType()==TopAbs_EDGE)
//                {
//                    TopoDS_Shape selectObj = myContext->SelectedShape();
//                    TopoDS_Edge edge =TopoDS::Edge(selectObj);
//                    Standard_Real first,last;
//                    Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);


//                    Handle(Geom_Conic) hgc =  Handle_Geom_Conic::DownCast(m_curve);
//                    //bool isCircle = hgc.IsNull();
//                    if(!hgc.IsNull()){
//                        gp_Pnt centerPnt = hgc->Location();

//                        TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(centerPnt);
//                        Handle_AIS_Shape pntVertex1 = new AIS_Shape(pnt1);

//                        myContext->Display(pntVertex1);
//                    }
//                }
//            }

    QStringList currentModuleIdList;
    foreach (ModuleObject *module, moduleMap.values()) {
        if (module && myContext->IsCurrent(module->aisShape)){
            currentModuleIdList.append(module->id);
            break;
        }
    }
    emit shapeSelected(currentModuleIdList, false);
}

void Viewer::mousePressEvent ( QMouseEvent* e )
{
        startPos = e->pos();
        if(e->button() == Qt::LeftButton)
        {
                mouseLeftPressed = true;
                if(toolSelectMode)                //浮动工件模式，在view类处理模型选择
                {
                    myContext->InitDetected();

                    if(myContext->HasDetected()){
                        //获取选中shape
                        TopoDS_Shape selectShape = myContext->DetectedShape();
                        //Handle(AIS_Shape) aio;
                        //aio = new AIS_Shape(selectShape);
                        //myContext->Hilight(aio);
                        //判断模式
                        int selectMode = -1;
                        m_pFloatTool->getGeometricElementSelectMode(selectMode);

                        //m_pFloatTool->clearShapes();
                        if(selectMode == FaceSelectType)      // 判断是面
                        {
                           m_pFloatTool->selectSurfaceFromShape(selectShape);
                           m_pFloatTool->setCurrentTopoDSShape(selectShape);
                           //面选中信号
                        }else if(selectMode == LinSelectType)
                        {
                            //还需判断线
                            m_pFloatTool->selectLineFromShape(selectShape);
                        }else
                        {                            //发送设置点信息
                            m_pFloatTool->selectPointFromShape(selectShape, e->x(), e->y());
                        }
                        emit setPntInfoSignal();
                    }
                }else
                {
                    emit mousePressSignal(e);       //在其他工具类处理（例如MeasureTool类）
                }
        }
        if(e->button() == Qt::RightButton) {
                mouseRightPressed = true;
        }
        if(e->button() == Qt::MidButton) {
               mouseMiddleBtn = true;
        }
}
void Viewer::createActions()
{
    //创建菜单、菜单项
    //pop_menu = new QMenu();
    //QAction *transformAction = new QAction(this);
    //transformAction->setText(tr("add images"));
    //QAction *add_folder_action = new QAction(this);
    //QAction *remove_selected_action = new QAction(this);
    //QAction *remove_all_action = new QAction(this);

    //连接信号与槽
    //connect(transformAction, &QAction::triggered, this, SLOT(showToolWidget(int type)));
//    connect(add_folder_action, &QAction::triggered, this, &ImageTree::selectFolder);
//    connect(remove_selected_action, &QAction::triggered, this, &ImageTree::removeSelectedImages);
//    connect(remove_all_action, &QAction::triggered, this, &ImageTree::removeAllImages);
}

void Viewer::contextMenuEvent(QContextMenuEvent* e)
{
    //创建菜单、菜单项
    pop_menu = new QMenu();
    QAction *transformAction = new QAction(this);
    transformAction->setText(tr("位置设置"));
    pop_menu->addAction(transformAction);
    //QAction *add_folder_action = new QAction(this);
    //QAction *remove_selected_action = new QAction(this);
    //QAction *remove_all_action = new QAction(this);

    //连接信号与槽
    connect(transformAction, SIGNAL(triggered(bool)), this, SLOT(openTransformTool()));
    //connect(transformAction, &QAction::triggered, this, SLOT(showToolWidget(int type)));
    //清除原有菜单
//    pop_menu->clear();

//    pop_menu->addAction(add_folder_action);
//    pop_menu->addAction(remove_selected_action);
//    pop_menu->addAction(remove_all_action);

    //菜单出现的位置为当前鼠标的位置
    pop_menu->exec(QCursor::pos());
    //event->accept();
}
void Viewer::mouseReleaseEvent( QMouseEvent* e )
{
	//Q_UNUSED(e);
	//if(e->button() == Qt::LeftButton) {
	//	if (e->modifiers() == Qt::ControlModifier) {
	//		myContext->Select( e->x(), e->y(), startPos.x(), startPos.y(), myView, true );
	//	} else if (e->modifiers() == Qt::ShiftModifier) {
	//		myContext->ShiftSelect( e->x(), e->y(), startPos.x(), startPos.y(), myView,true );
	//	}
	//}
    //当释放鼠标左键再发送选择信号
    QStringList currentModuleIdList;
    foreach (ModuleObject *module, moduleMap.values()) {
        if (module && myContext->IsCurrent(module->aisShape)){
            currentModuleIdList.append(module->id);
            //break;
        }
    }
    emit shapeSelected(currentModuleIdList, true);
    mouseLeftPressed = false;
    mouseRightPressed = false;
    mouseMiddleBtn = false;
}

#include "AIS_LocalContext.hxx"
#include <SelectMgr_EntityOwner.hxx>
#include <PrsMgr_PresentationManager3d.hxx>
void Viewer::mouseMoveEvent(QMouseEvent* e)
{
    if( !myView.IsNull() )
    {

        if (toolSelectMode || e->modifiers() == Qt::ControlModifier || e->modifiers() == Qt::ShiftModifier) {

            myContext->MoveTo(e->x(), e->y(), myView);

            static Handle(AIS_Shape) circleCenterPoint = NULL;
        }

        if(mouseLeftPressed){

            int selectMode = -1;
            if(m_pFloatTool != NULL)
            {
                m_pFloatTool->getGeometricElementSelectMode(selectMode);
            }
            if(/*!toolSelectMode && */selectMode != ComponentSelectType)
            {
                rotX += (- startPos.x() + e->x())*0.00314159;//*0.5;
                rotY += ( startPos.y() - e->y())*0.00314159;//*0.5;

                if (rotX>-0.000002 && rotX<0.000002) rotX = 0.0;
                if (rotY>-0.000002 && rotY<0.000002) rotY = 0.0;

                myView->Rotate(rotX, rotY, 0, startPos.x(), - startPos.y(),0 /* 989, 250, 418.5*/, Standard_False);    //before 6.7

                startPos = e->pos();
                //qDebug()<<"按下左键移动";
            }else
            {
                //发送设置点信息
                //m_pFloatTool->selectComponentByViewRegion(startPos.x(), startPos.y(), e->x(), e->y());
                myContext->Select(startPos.x(), startPos.y(), e->x(), e->y(), myView, Standard_True);


            }


        }
        if(mouseMiddleBtn || mouseRightPressed){
            myView->Pan( e->x() - startPos.x(), startPos.y() - e->y() );
            startPos = e->pos();

        }
    }
}

void Viewer::wheelEvent( QWheelEvent* e )
{
    if( !myView.IsNull() )
    {
        Standard_Real currentScale = myView->Scale();
        if (e->delta() > 0)
        {
            if (currentScale < 500.0)
				currentScale *= 1.2; // +5%
        }
        else
        {
			if (currentScale > 0.05)
				currentScale /= 1.2; // -5%
        }
        myView->SetScale( currentScale );
    }
}

void Viewer::keyPressEvent( QKeyEvent * e )
{
	QWidget::keyPressEvent(e);
//	if (e->modifiers().testFlag( Qt::ControlModifier )) {
//		viewMouseMode = 2;
//	}
}

void Viewer::keyReleaseEvent( QKeyEvent * e )
{
	QWidget::keyReleaseEvent(e);
//    if( !e->modifiers().testFlag( Qt::ControlModifier ) && viewMouseMode == 2 )
//    {
//		viewMouseMode = 1;

//		myContext->MoveTo(10000,10000, myView);

//		//myView->Update();
//		//myContext->ClearSelected(true);
//		//myContext->UnhilightCurrents(true);
//    }
}


void Viewer::axisMove(QString axisId, double value)
{
    AxisObject *axis = axisMap[axisId];
	if (!axis) return;
    if (axis->type == 1) {   // 移动轴
        //gp_Trsf trsfMove;
        //trsfMove.SetTranslation((knifeBladeMap[currentKnifeBlade]->originOffsetMap[axis->sign] + value) * gp_Vec(axis->stepValue[X], axis->stepValue[Y], axis->stepValue[Z]));
        //for(vector<QString>::size_type j = 0; j < axis->moduleList.size(); ++j) {
        //    Module *module = moduleMap[axis->moduleList[j]];
        //    Handle_AIS_Shape aisShape = module->aisShape;

        //    TopoDS_Shape shape;
        //    shape = aisShape->Shape();
        //    shape.Location(TopLoc_Location(trsfMove));// * shape.Location().Transformation()));
        //    aisShape->Set(shape);
        //    aisShape->Redisplay(false);
        //}

        //for(vector<QString>::size_type k = 0; k < axis->subAxisList.size(); ++k) {
        //    Axis *subAxis = axisMap[axis->subAxisList[k]];

        //    for(vector<QString>::size_type j = 0; j < subAxis->moduleList.size(); ++j) {
        //        Module *module = moduleMap[subAxis->moduleList[j]];
        //        Handle_AIS_Shape aisShape = module->aisShape;

        //        TopoDS_Shape shape;
        //        shape = aisShape->Shape();
        //        shape.Location(TopLoc_Location(trsfMove * module->transform * shape.Location().Transformation()));
        //        aisShape->Set(shape);
        //        aisShape->Redisplay(false);
        //    }
        //}
        //axis->value = value;
    }

	if (axis->type == 2) {	 // 旋转轴
//		if (axis->value < axis->minValue )
//			axis->symbol *= -1;
//		if (axis->value > axis->maxValue )
//			axis->symbol *= -1;
//
//		axis->value += axis->symbol * value;
////spindleRotate += axis->symbol * 0.2;
axis->value = value;

		gp_Trsf trsfSpindle;
		trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(axis->start[X], axis->start[Y], axis->start[Z]), gp_Dir(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), axis->value);

        int j;

		for(j = 0; j < axis->moduleList.size(); ++j) {
			Handle_AIS_Shape aisShape = moduleMap[axis->moduleList[j]]->aisShape;


			myContext->SetLocation(aisShape, TopLoc_Location(trsfSpindle));
//			machineCutting.setRotation(trsfSpindle);
	//                myContext->Redisplay(aisShape, true);
		}

		while (axis->subAxisList.size()) {
			for(int k = 0; k < axis->subAxisList.size(); ++k) {
                AxisObject *subAxis = axisMap[axis->subAxisList[k]];

				for(int j = 0; j < subAxis->moduleList.size(); ++j) {
                    ModuleObject *module = moduleMap[subAxis->moduleList[j]];
					Handle_AIS_Shape aisShape = module->aisShape;

					TopoDS_Shape shape;
					shape = aisShape->Shape();
					//shape.Location(TopLoc_Location(/*trsfMove **/ module->transform * shape.Location().Transformation()));
					//gp_Trsf trsfSpindle;
					//trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(axis->start[X], axis->start[Y], axis->start[Z]), gp_Dir(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), spindleRotate);
 					myContext->SetLocation(aisShape, TopLoc_Location(trsfSpindle/* * myContext->Location(aisShape).Transformation()*/));

					//   aisShape->Set(shape);
					//    aisShape->Redisplay(false);
					//	myContext->Redisplay(aisShape, true);
				}
			}
			axis = axisMap[axis->subAxisList[0]];
		}
	}
	myView->Redraw();



}

//#include <Geom_Transformation.hxx>
void Viewer::timerEvent( QTimerEvent* e)
{


//	axisMove("1002", 0.2);

//	axisMove("1001", 100);

//    int timerId = e->timerId();
//    if (timerId == redrawTimerId) {
////        myView->Redraw();
//    }
//    if (timerId == rotateTimerId) {
//        killTimer(timerId);
////return;
////        spindleRotate += spindleStep;
//        spindleRotate = fmod(spindleRotate + spindleStep, 2*M_PI);
//        if (EQUAL(spindleStep, 0.2) && (spindleRotate > -0.11)) {
//            spindleStep = 0.0;
//            spindleRotate = 0.0;
//
////            machineCutting.acceptBladeRadius(0.1);
//
//        }
////        printf("%f\n",spindleRotate);
//        for(map<QString, Axis*>::iterator i = axisMap.begin(); i != axisMap.end(); ++i) {
//            Axis *axis = i->second;
//
//            if (axis->type == 1) {   // 移动轴
//                emit updateAxisValue(axis->sign, axis->value);
//            }
//
//            if (axis->type != 2) continue;  // 旋转轴
//            for(vector<QString>::size_type j = 0; j < axis->moduleList.size(); ++j) {
//                Handle_AIS_Shape aisShape = moduleMap[axis->moduleList[j]]->aisShape;
//
//                gp_Trsf trsfSpindle;
//                trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(axis->start[X], axis->start[Y], axis->start[Z]), gp_Dir(axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z])), spindleRotate);
//
//                myContext->SetLocation(aisShape, TopLoc_Location(trsfSpindle));
//                machineCutting.setRotation(trsfSpindle);
////                myContext->Redisplay(aisShape, true);
//            }
//            QApplication::processEvents();
//        }
//        myView->Redraw();
//        rotateTimerId = startTimer(rotateInterval);
//
//    }


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
////aisCheTou->SetLocation(TopLoc_Location(trsfSpindle));axis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z]
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
////	trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(418.5, 250, 989), gp_Dir(1, 0, 0)), spiaxis->end[X]-axis->start[X], axis->end[Y]-axis->start[Y], axis->end[Z]-axis->start[Z]ndleRotate);
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
////value
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

//#include <sys/times.h>
//extern Interpreter interp;
//int lastCodeLine = 0;
//void Viewer::processCommand(CommandStruct &commandStruct){
//
//    commandStruct.line = interp.line();//   line;
//    commandStruct.ncLine = interp.nNumber();//    ncNumber();
//
//    if (lastCodeLine != commandStruct.line) {
//        mainWindow->setCodeRow( commandStruct.line );
//        lastCodeLine = commandStruct.line;
//    }
//
//    switch (commandStruct.command) {
//        case CMD_STRAIGHT_TRAVERSE:
//            {
////            printf("CMD_STRAIGHT_TRAVERSE %f %f %f\n", commandStruct.params[X], commandStruct.params[Y], commandStruct.params[Z]);
//
//                axisMove(axisSignMap["X"], commandStruct.params[X]);
//                axisMove(axisSignMap["Z"], commandStruct.params[Z]);
//                QApplication::processEvents();
//                machineCutting.moveTo(gp_Pnt2d(commandStruct.params[Z], commandStruct.params[X]));
//                QApplication::processEvents();
//
//                break;
//            }
//        case CMD_STRAIGHT_FEED:
////            printf("CMD_STRAIGHT_FEED %f %f %f\n", commandStruct.params[0], commandStruct.params[1], commandStruct.params[2]);
//
//            axisMove(axisSignMap["X"], commandStruct.params[X]);
//            axisMove(axisSignMap["Z"], commandStruct.params[Z]);
//            QApplication::processEvents();
//            machineCutting.cutTo(gp_Pnt2d(commandStruct.params[Z], commandStruct.params[X]));
//            QApplication::processEvents();
//
//            break;
//        case CMD_ARC_FEED:
//            printf("CMD_ARC_FEED %f %f %f\n", commandStruct.params[0], commandStruct.params[1], commandStruct.params[2]);
//
//            axisMove(axisSignMap["X"], commandStruct.params[X]);
//            axisMove(axisSignMap["Z"], commandStruct.params[Z]);
//            QApplication::processEvents();
//            machineCutting.cutTo(gp_Pnt2d(commandStruct.params[Z], commandStruct.params[X]));
//            QApplication::processEvents();
//
//            break;
//        case CMD_SET_SPINDLE_SPEED:
//            machineCutting.resetCut();
////            commandStruct.params[0] = 10;
//            if (commandStruct.params[0] > 100) {
//                spindleStep = 4.757767; // 4.577998; 4.757767; 4.581489; 4.584979;
//                //spindleStep = 1.394518; //6 * M_PI/180.0;    //rpm/60.0/25.0*360 * M_PI/180.0;
//                //spindleStep: 1.357866
//                //spindleStep: 1.396263
//                //spindleStep: 1.364847
//                //spindleStep: 1.380555
//                //spindleStep: 1.387537
//                //spindleStep: 1.389282
//                //spindleStep: 1.394518
//                //spindleStep: 1.396263
//            } else {
//                spindleStep = commandStruct.params[0]/60.0/25.0*360 * M_PI/180.0;
//            }
//            QApplication::processEvents();
//            break;
//        case CMD_STOP_SPINDLE_TURNING:
//            spindleStep = 0.2;
//            mainWindow->setCodeRow( -1 );
////            spindleRotate = 0;
//            break;
//        case CMD_DWELL:
//            long start = clock();
//            long end = commandStruct.params[0] * 1000000l;
//            while(clock()-start < end) {
//                QApplication::processEvents();
//            }
////            printf("%d %d\n", clock(), start);
//            break;
//    }
////    myView->Redraw();
////    QApplication::processEvents();
//}
//
//void Viewer::updateBladePolygon(std::vector<gp_Pnt2d> bladePolygon){
//    machineCutting.setBlade(bladePolygon);
//}

//TopLoc_Location lastBindLocation;
//gp_Trsf currentTrsf, lastBindTrsf;
//double currentAxis[6], lastBindAxis[6];
//void Viewer::syncPosition(double axis1, double axis2, double axis3, double axis4, double axis5, double axis6 )
void Viewer::syncPosition( QStringList posList, int index )
{
    AxisObject *ax1;
    AxisObject *ax2;
    AxisObject *ax3;
    AxisObject *ax4;
    AxisObject *ax5;
    AxisObject *ax6;

    QSqlQuery query;
    QString axisId;
    AxisObject *axBase = NULL;
    //update zhugz

     query.exec(QString("select rootAxis from StationMotion, RobotLibrary where libraryId=RobotLibrary.id and stationId='%1' and displayOrder=%2 order by displayOrder ASC limit 1").arg(currentStationId).arg(index));

    if(query.next()) {

        axisId = query.value(0).toString();
        axBase = axisMap[axisId];
        if (!axBase || axBase->subAxisList.count()==0)
            return;
        ax1 = axisMap[axBase->subAxisList.at(0)];
        ax1->value = posList[0].toDouble();
        if (!ax1 || ax1->subAxisList.count()==0)
            return;
        ax2 = axisMap[ax1->subAxisList.at(0)];
        ax2->value = posList[1].toDouble();
        if (!ax2 || ax2->subAxisList.count()==0)
            return;
        ax3 = axisMap[ax2->subAxisList.at(0)];
        ax3->value = posList[2].toDouble();
        if (!ax3 || ax3->subAxisList.count()==0)
            return;
        ax4 = axisMap[ax3->subAxisList.at(0)];
        ax4->value = posList[3].toDouble();
        if (!ax4 || ax4->subAxisList.count()==0)
            return;
        ax5 = axisMap[ax4->subAxisList.at(0)];
        ax5->value = posList[4].toDouble();
        if (!ax5 || ax5->subAxisList.count()==0)
            return;
        ax6 = axisMap[ax5->subAxisList.at(0)];
        ax6->value = posList[5].toDouble();
    } else {
        return;
    }


    while (!axBase->parentId.isEmpty()) {
        axBase = axisMap[axBase->parentId];
    }
    TopLoc_Location locBase;


    while (true) {
        gp_Trsf trsfBase;
        if (axBase->type == 1) {
            trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
            locBase = locBase * TopLoc_Location(trsfBase);

        } else if(axBase->type == 2) {
            //qDebug()<<axBase->start[X]<<moduleMap[axBase->moduleList.last()]->pos[X]<<axBase->start[Y]<<moduleMap[axBase->moduleList.last()]->pos[Y]<<axBase->start[Z]<<moduleMap[axBase->moduleList.last()]->pos[Z];
            trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
            locBase = locBase * TopLoc_Location(trsfBase);

        }


        for(int j = 0; j < axBase->moduleList.size(); ++j) {

            //ronglk
            //参考https://www.opencascade.com/content/simple-object-transformations

            Handle_AIS_Shape anIS = moduleMap[axBase->moduleList[j]]->aisShape;
            myContext->SetLocation(anIS, locBase);

            //moduleMap[axBase->moduleList[j]]->currentLoc = locBase;
            moduleMap[axBase->moduleList[j]]->transform = locBase;


        }
        if (axBase->subAxisList.isEmpty()) {
            // 临时记录TCP运动轨迹点
//            static TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(gp_Pnt(axBase->end[X],0,axBase->end[Z]));
//            Handle(AIS_Shape) shape = new AIS_Shape(pnt1);
//            myContext->SetLocation(shape, locBase);
//            myContext->Display(shape, false);

            break;
        }
        axBase = axisMap[axBase->subAxisList.at(0)];
    }
   myView->Redraw();
}


void Viewer::updateUnitData( QByteArray data )
{
	gp_Trsf transNull;

}

void Viewer::syncDisplayState( int displayState, int pickCount )
{

 try {
	QString pickId;
	if (robotType == 1) {
		/*  yangji */
		pickId.sprintf("4GLGx92bRweP%.3do57cTWg==", pickCount + 1);
		switch(displayState) {
			case 1:
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2j
				myContext->Remove(moduleMap["4G1sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j1s
				myContext->Remove(moduleMap["4G2sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j2s
				myContext->Remove(moduleMap["4G3sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j3s
				for(int i=1;i<=50;i++) {
					pickId.sprintf("4GLGx92bRweP%.3do57cTWg==", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}
				//myContext->Display(moduleMap["4GLGx92bRweP001o57cTWg=="]->aisShape, Standard_False); // pp1
				//myContext->Display(moduleMap["4GLGx92bRweP002o57cTWg=="]->aisShape, Standard_False); // pp2
				//myContext->Display(moduleMap["4GLGx92bRweP003o57cTWg=="]->aisShape, Standard_False); // pp3
				myContext->Display(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_False); // p3
				myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False); // p4
				myContext->Display(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_False); // p5
				myContext->Display(moduleMap["4G1Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False); // j1j
				myContext->Display(moduleMap["4G2Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False); // j2j
				myContext->Display(moduleMap["4G3Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False); // j3j
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_True);  // z2s
				break;
			case 2:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap[pickId]->aisShape, Standard_False); // pp1
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 3:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p3
				myContext->Remove(moduleMap["4G1Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j1j
				myContext->Display(moduleMap["4G1sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j1s
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
				break;
			case 4:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Remove(moduleMap["4G1sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j1s
				myContext->Display(moduleMap["4G1Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j1j
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p3
				break;
			case 5:
				myContext->Remove(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2j
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Display(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2s
				myContext->Display(moduleMap[pickId]->aisShape, Standard_True);  // pp1
				break;
				
			case 12:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap[pickId]->aisShape, Standard_False); // pp2
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 13:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p4
				myContext->Remove(moduleMap["4G2Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j2j
				myContext->Display(moduleMap["4G2sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j2s
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
				break;
			case 14:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Remove(moduleMap["4G2sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j2s
				myContext->Display(moduleMap["4G2Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j2j
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p4
				break;
			case 15:
				myContext->Remove(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2j
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Display(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2s
				myContext->Display(moduleMap[pickId]->aisShape, Standard_True);  // pp2
				break;

			case 22:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap[pickId]->aisShape, Standard_False); // pp3
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 23:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p5
				myContext->Remove(moduleMap["4G3Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j3j
				myContext->Display(moduleMap["4G3sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j3s
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
				break;
			case 24:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Remove(moduleMap["4G3sx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j3s
				myContext->Display(moduleMap["4G3Gx92eRwePDq/o57cTWg=="]->aisShape, Standard_False);  // j3j
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p5
				break;
			case 25:
				myContext->Remove(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2j
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Display(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2s
				myContext->Display(moduleMap[pickId]->aisShape, Standard_True);  // pp3
				break;
		}
	} else if(robotType == 2) {
/*  wuxiang */
	switch(displayState) {
		case 1:
			myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
			myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
			myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
			myContext->Remove(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2j

			myContext->Display(moduleMap["4GLGx92bRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp1
			myContext->Display(moduleMap["4GLGx92cRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp2
			myContext->Display(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_False); // p3
			myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False); // p4
			myContext->Display(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_False); // p5
			myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
			myContext->Display(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_True);  // z2s
			break;
		case 2:
			myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
			myContext->Remove(moduleMap["4GLGx92bRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp1
			myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
			myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
			break;
		case 3:
			myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
			myContext->Remove(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p4
			myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
			myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
			break;
		case 4:
			myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
			myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
			myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
			myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p4
			break;
		case 5:
			myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
			myContext->Remove(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p3
			myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
			myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
			break;
		case 6:
			myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
			myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
			myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
			myContext->Display(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p3
			break;
		case 7:
			myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
			myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
			myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
			myContext->Display(moduleMap["4GLGx92bRwePDq/o57cTWg=="]->aisShape, Standard_True); // pp1
			break;

		case 8:
			myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
			myContext->Remove(moduleMap["4GLGx92cRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp2
			myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
			myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
			break;
		case 9:
			myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
			myContext->Remove(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p4
			myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
			myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
			break;
		case 10:
			myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
			myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
			myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
			myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p4
			break;
		case 11:
			myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
			myContext->Remove(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p5
			myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
			myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
			break;
		case 12:
			myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
			myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
			myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
			myContext->Display(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p5
			break;
		case 14:
			myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
			myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
			myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
			myContext->Display(moduleMap["4GLGx92cRwePDq/o57cTWg=="]->aisShape, Standard_True); // pp2
			break;
	}


//return;
	} else if(robotType == 3) {

	/* boli */
	switch(displayState) {
		case 1:
			myContext->Display(moduleMap["rhaS0iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS6UkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 2:
			myContext->Display(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS1iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 3:
			myContext->Display(moduleMap["rhaS6UkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			myContext->Remove(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 4:
			myContext->Display(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS0iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 5:
			myContext->Display(moduleMap["rhaS1iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);

			break;
		case 6:
			myContext->Display(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS3iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);

			myContext->Display(moduleMap["rhaS0iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS6UkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 7:
			myContext->Display(moduleMap["rhaS6UkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			myContext->Remove(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 8:
			myContext->Display(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS0iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
		case 9:
			myContext->Display(moduleMap["rhaS3iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_False);
			myContext->Remove(moduleMap["rhaS5iUkS6m1L4JoJMALBQ=="]->aisShape, Standard_True);
			break;
	}

	} else if(robotType == 4) {
		/*  zhengming */
		switch(displayState) {
			case 1:
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2j

				myContext->Display(moduleMap["4GLGx92bRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp1
				myContext->Display(moduleMap["4GLGx92cRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp2
				myContext->Display(moduleMap["4GLGx92dRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp3
				myContext->Display(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_False); // p3
				myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False); // p4
				myContext->Display(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_False); // p5
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_True);  // z2s
				break;
			case 2:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["4GLGx92bRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp1
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 3:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p4
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
				break;
			case 4:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p4
				break;
			case 5:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p3
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 6:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx923RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p3
				break;
			case 7:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx92bRwePDq/o57cTWg=="]->aisShape, Standard_True); // pp1
				break;

			case 8:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["4GLGx92cRwePDq/o57cTWg=="]->aisShape, Standard_False); // pp2
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 9:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p4
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p2
				break;
			case 10:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx924RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p4
				break;
			case 11:
				myContext->Remove(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p5
				myContext->Display(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p1
				break;
			case 12:
				myContext->Remove(moduleMap["4GLGx928RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z2s
				myContext->Remove(moduleMap["4GLGx922RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p2
				myContext->Display(moduleMap["4GLGx929RwePDq/o57cTWg=="]->aisShape, Standard_False); // z2j
				myContext->Display(moduleMap["4GLGx925RwePDq/o57cTWg=="]->aisShape, Standard_True);  // p5
				break;
			case 14:
				myContext->Remove(moduleMap["4GLGx927RwePDq/o57cTWg=="]->aisShape, Standard_False);  // z1j
				myContext->Remove(moduleMap["4GLGx921RwePDq/o57cTWg=="]->aisShape, Standard_False);  // p1
				myContext->Display(moduleMap["4GLGx926RwePDq/o57cTWg=="]->aisShape, Standard_False); // z1s
				myContext->Display(moduleMap["4GLGx92cRwePDq/o57cTWg=="]->aisShape, Standard_True);  // pp2
				break;
		}
	} else if (robotType == 5) {
		/*  上海爱乐 */
		switch(displayState) {
			case 1:
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["jgz1"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["m1o"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["m2o"]->aisShape, Standard_False);

				for(int i=1;i<=8;i++) {
					pickId.sprintf("ZA%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("ZB%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("ZC%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);

					pickId.sprintf("DESTA%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("DESTB%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}
				//for(int i=1;i<=64;i++) {
				for(int i=1;i<=8;i++) {
					pickId.sprintf("SRCA%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("SRCB%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}
				myContext->Display(moduleMap["jgz2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_True);
				break;
			case 2:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 3:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 4:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 5:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 6:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 7:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 8:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA01"]->aisShape, Standard_True);
				break;
			case 9:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA02"]->aisShape, Standard_True);
				break;
			case 10:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 11:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 12:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 13:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 14:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 15:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 16:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA03"]->aisShape, Standard_True);
				break;
			case 17:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA04"]->aisShape, Standard_True);
				break;
			case 18:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 19:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 20:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 21:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 22:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 23:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 24:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA05"]->aisShape, Standard_True);
				break;
			case 25:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA06"]->aisShape, Standard_True);
				break;
			case 26:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 27:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCA08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 28:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 29:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 30:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 31:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 32:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA07"]->aisShape, Standard_True);
				break;
			case 33:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA08"]->aisShape, Standard_True);
				break;

			case 101:
				myContext->Remove(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m1o"]->aisShape, Standard_False);
				break;
			case 102:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 103:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 104:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA01"]->aisShape, Standard_True);
				break;
			case 105:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA02"]->aisShape, Standard_True);
				break;
			case 106:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 107:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 108:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA03"]->aisShape, Standard_True);
				break;
			case 109:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA04"]->aisShape, Standard_True);
				break;
			case 110:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 111:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 112:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA05"]->aisShape, Standard_True);
				break;
			case 113:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA06"]->aisShape, Standard_True);
				break;
			case 114:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 115:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 116:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA07"]->aisShape, Standard_True);
				break;
			case 117:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA08"]->aisShape, Standard_True);
				break;

			case 401:
				myContext->Remove(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["jgz2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m1o"]->aisShape, Standard_False);
				myContext->Display(moduleMap["jgz1"]->aisShape, Standard_False);
				break;
			case 402:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 403:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 404:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB01"]->aisShape, Standard_True);
				break;
			case 405:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB02"]->aisShape, Standard_True);
				break;
			case 406:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 407:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 408:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB03"]->aisShape, Standard_True);
				break;
			case 409:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB04"]->aisShape, Standard_True);
				break;
			case 410:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 411:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 412:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB05"]->aisShape, Standard_True);
				break;
			case 413:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB06"]->aisShape, Standard_True);
				break;
			case 414:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 415:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 416:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB07"]->aisShape, Standard_True);
				break;
			case 417:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZB08"]->aisShape, Standard_True);
				break;

			case 501:
				myContext->Remove(moduleMap["m1o"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["jgz1"]->aisShape, Standard_False);
				for(int i=1;i<=8;i++) {
					pickId.sprintf("ZB%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("ZC%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}
				myContext->Display(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["jgz2"]->aisShape, Standard_True);
				break;
			case 502:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 503:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 506:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 507:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 508:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 509:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
				break;
			case 510:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA01"]->aisShape, Standard_True);
				break;
			case 511:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA02"]->aisShape, Standard_True);
				break;
			case 512:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 513:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 516:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 517:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 518:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 519:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
				break;
			case 520:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA03"]->aisShape, Standard_True);
				break;
			case 521:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA04"]->aisShape, Standard_True);
				break;
			case 522:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 523:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 526:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 527:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 528:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 529:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
				break;
			case 530:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA05"]->aisShape, Standard_True);
				break;
			case 531:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA06"]->aisShape, Standard_True);
				break;
			case 532:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 533:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 536:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 537:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 538:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 539:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
				break;
			case 540:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA07"]->aisShape, Standard_True);
				break;
			case 541:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA08"]->aisShape, Standard_True);
				break;

			// 机床二
			case 201:
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["jgz1"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["m1o"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["m2o"]->aisShape, Standard_False);

				for(int i=1;i<=8;i++) {
					pickId.sprintf("ZA%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("ZB%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("ZC%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);

					pickId.sprintf("SRCA%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("SRCB%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("DESTA%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("DESTB%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}
				myContext->Display(moduleMap["jgz2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_True);
				break;
			case 202:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 203:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 204:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 205:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 206:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 207:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 208:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA01"]->aisShape, Standard_True);
				break;
			case 209:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA02"]->aisShape, Standard_True);
				break;
			case 210:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 211:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 212:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 213:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 214:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 215:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 216:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA03"]->aisShape, Standard_True);
				break;
			case 217:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA04"]->aisShape, Standard_True);
				break;
			case 218:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 219:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 220:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 221:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 222:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 223:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 224:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA05"]->aisShape, Standard_True);
				break;
			case 225:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA06"]->aisShape, Standard_True);
				break;
			case 226:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 227:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["SRCB08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 228:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 229:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 230:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
				break;
			case 231:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 232:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA07"]->aisShape, Standard_True);
				break;
			case 233:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZA08"]->aisShape, Standard_True);
				break;

			case 301:
				myContext->Remove(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2o"]->aisShape, Standard_False);
				break;
			case 302:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 303:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 304:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB01"]->aisShape, Standard_True);
				break;
			case 305:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB02"]->aisShape, Standard_True);
				break;
			case 306:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 307:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 308:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB03"]->aisShape, Standard_True);
				break;
			case 309:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB04"]->aisShape, Standard_True);
				break;
			case 310:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 311:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 312:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB05"]->aisShape, Standard_True);
				break;
			case 313:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB06"]->aisShape, Standard_True);
				break;
			case 314:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 315:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZA08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 316:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB07"]->aisShape, Standard_True);
				break;
			case 317:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB08"]->aisShape, Standard_True);
				break;

			case 601:
				myContext->Remove(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["jgz1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2o"]->aisShape, Standard_False);
				myContext->Display(moduleMap["jgz2"]->aisShape, Standard_False);
				break;
			case 602:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 603:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 604:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC01"]->aisShape, Standard_True);
				break;
			case 605:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC02"]->aisShape, Standard_True);
				break;
			case 606:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 607:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 608:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC03"]->aisShape, Standard_True);
				break;
			case 609:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC04"]->aisShape, Standard_True);
				break;
			case 610:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 611:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 612:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC05"]->aisShape, Standard_True);
				break;
			case 613:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC06"]->aisShape, Standard_True);
				break;
			case 614:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 615:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 616:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC07"]->aisShape, Standard_True);
				break;
			case 617:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["ZC08"]->aisShape, Standard_True);
				break;

			case 701:
				myContext->Remove(moduleMap["m2o"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["jgz1"]->aisShape, Standard_False);
				for(int i=1;i<=8;i++) {
					pickId.sprintf("ZB%.2d", i);
					myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("ZC%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}
				myContext->Display(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["jgz2"]->aisShape, Standard_True);
				break;
			case 702:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 703:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 706:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 707:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 708:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 709:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
				break;
			case 710:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB01"]->aisShape, Standard_True);
				break;
			case 711:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB02"]->aisShape, Standard_True);
				break;
			case 712:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 713:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC04"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 716:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 717:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 718:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 719:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
				break;
			case 720:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB03"]->aisShape, Standard_True);
				break;
			case 721:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB04"]->aisShape, Standard_True);
				break;
			case 722:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC05"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 723:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC06"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 726:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 727:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 728:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 729:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 730:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB05"]->aisShape, Standard_True);
				break;
			case 731:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB06"]->aisShape, Standard_True);
				break;
			case 732:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC07"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);
				break;
			case 733:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["ZC08"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 736:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 737:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p3"]->aisShape, Standard_True);
				break;
			case 738:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p5"]->aisShape, Standard_True);
				break;
			case 739:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p5"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p4"]->aisShape, Standard_True);
				break;
			case 740:
				myContext->Remove(moduleMap["j1m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p3"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB07"]->aisShape, Standard_True);
				break;
			case 741:
				myContext->Remove(moduleMap["j2m"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p4"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCB08"]->aisShape, Standard_True);
				break;
		}
	} else if (robotType == 6) {
		/*  程泰 */
		pickId.sprintf("%.2d", pickCount + 1);
		switch(displayState) {
			case 1:
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["m1o"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["m2o"]->aisShape, Standard_False);

				//for(int i=1;i<=8;i++) {
				//	pickId.sprintf("ZA%.2d", i);
				//	myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
				//	pickId.sprintf("ZB%.2d", i);
				//	myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);
				//	pickId.sprintf("ZC%.2d", i);
				//	myContext->Remove(moduleMap[pickId]->aisShape, Standard_False);

				//	pickId.sprintf("DESTA%.2d", i);
				//	myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				//	pickId.sprintf("DESTB%.2d", i);
				//	myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				//}

				for(int i=1;i<=7;i++) {
					pickId.sprintf("SRCA%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
					pickId.sprintf("SRCB%.2d", i);
					myContext->Display(moduleMap[pickId]->aisShape, Standard_False);
				}

				myContext->Display(moduleMap["DESTA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC02"]->aisShape, Standard_False);

				myContext->Display(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_True);
				break;

			case 101:
				myContext->Remove(moduleMap["m1c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m1o"]->aisShape, Standard_True);
				break;
			case 102:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["SRCA"+pickId]->aisShape, Standard_False); // pp1
//				myContext->Remove(moduleMap["DESTA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);  // p1
//				myContext->Display(moduleMap["DESTA02"]->aisShape, Standard_False);
				break;
			case 103:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 104:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA01"]->aisShape, Standard_True);
				break;
			case 105:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA"+pickId]->aisShape, Standard_True);
				break;
			case 106:
				myContext->Remove(moduleMap["m1o"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m1c"]->aisShape, Standard_True);
				break;
			case 107:
				myContext->Remove(moduleMap["DESTA02"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA02J"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["WorkbenchA01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA03J"]->aisShape, Standard_False);
				myContext->Display(moduleMap["WorkbenchA02"]->aisShape, Standard_True);
				break;
			case 108:
				myContext->Remove(moduleMap["DESTA01"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA03"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTA03J"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["WorkbenchA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTA02J"]->aisShape, Standard_False);
				myContext->Display(moduleMap["WorkbenchA01"]->aisShape, Standard_True);
				break;

			case 201:
				myContext->Remove(moduleMap["m2c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2o"]->aisShape, Standard_True);
				break;
			case 202:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["SRCA"+pickId]->aisShape, Standard_False); // pp1
				//myContext->Remove(moduleMap["DESTB01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);  // p1
				//myContext->Display(moduleMap["DESTB02"]->aisShape, Standard_True);
				break;
			case 203:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 204:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB01"]->aisShape, Standard_True);
				break;
			case 205:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA"+pickId]->aisShape, Standard_True);
				break;
			case 206:
				myContext->Remove(moduleMap["m2o"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m2c"]->aisShape, Standard_True);
				break;
			case 207:
				myContext->Remove(moduleMap["DESTB02"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB02J"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["WorkbenchB01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB03J"]->aisShape, Standard_False);
				myContext->Display(moduleMap["WorkbenchB02"]->aisShape, Standard_True);
				break;
			case 208:
				myContext->Remove(moduleMap["DESTB01"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB03"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTB03J"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["WorkbenchB02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTB02J"]->aisShape, Standard_False);
				myContext->Display(moduleMap["WorkbenchB01"]->aisShape, Standard_True);
				break;

			case 301:
				myContext->Remove(moduleMap["m3c"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m3o"]->aisShape, Standard_True);
				break;
			case 302:
				myContext->Remove(moduleMap["j1s"]->aisShape, Standard_False);  // z1s
				myContext->Remove(moduleMap["SRCA"+pickId]->aisShape, Standard_False); // pp1
				//myContext->Remove(moduleMap["DESTC01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1j"]->aisShape, Standard_False); // z1j
				myContext->Display(moduleMap["p1"]->aisShape, Standard_True);  // p1
				//myContext->Display(moduleMap["DESTC02"]->aisShape, Standard_True);
				break;
			case 303:
				myContext->Remove(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTC02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Display(moduleMap["p2"]->aisShape, Standard_True);
				break;
			case 304:
				myContext->Remove(moduleMap["j1j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p1"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j1s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC01"]->aisShape, Standard_True);
				break;
			case 305:
				myContext->Remove(moduleMap["j2j"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["p2"]->aisShape, Standard_False);
				myContext->Display(moduleMap["j2s"]->aisShape, Standard_False);
				myContext->Display(moduleMap["SRCA"+pickId]->aisShape, Standard_True);
				break;
			case 306:
				myContext->Remove(moduleMap["m3o"]->aisShape, Standard_False);
				myContext->Display(moduleMap["m3c"]->aisShape, Standard_True);
				break;
			case 307:
				myContext->Remove(moduleMap["DESTC02"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTC02J"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["WorkbenchC01"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC03"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC03J"]->aisShape, Standard_False);
				myContext->Display(moduleMap["WorkbenchC02"]->aisShape, Standard_True);
				break;
			case 308:
				myContext->Remove(moduleMap["DESTC01"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTC03"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["DESTC03J"]->aisShape, Standard_False);
				myContext->Remove(moduleMap["WorkbenchC02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC02"]->aisShape, Standard_False);
				myContext->Display(moduleMap["DESTC02J"]->aisShape, Standard_False);
				myContext->Display(moduleMap["WorkbenchC01"]->aisShape, Standard_True);
				break;
		}
	}

 } catch (...) {
 }
}

void Viewer::showAll() 
{
    myView->FitAll();
//    myView->ZFitAll();
	totalScaleFactor = myView->Scale();
}

//#include <QThreadPool>

void Viewer::importModuleFile(QString filename, QString id, QString name)
{
    qDebug(filename.toLocal8Bit().constData());

    Handle_AIS_Shape aisShape;
    ModuleObject *module = moduleMap[id];
    if (module) {
        if (module->aisShape!=NULL) {
            aisShape = module->aisShape;
            myContext->Remove(aisShape, Standard_False);
        }
        if (module->material >= 0){
            if (module->material > 18) {
                Quantity_Color color;
                Quantity_Color::Argb2color(module->color, color);
                module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
            }
            module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)module->material );
        } else {
            module->aisShape->UnsetColor();
            module->aisShape->SetMaterial( Graphic3d_NOM_CHROME );//(Graphic3d_NameOfMaterial)module->material );
//                module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
        }
        if (module->transparency > 0) {
            module->aisShape->SetTransparency(module->transparency);
//            qDebug(QString::number(module->transparency).toLocal8Bit());
        } else {
            module->aisShape->SetTransparency(0.0);
        }
    } else {
        TopoDS_Shape shape;
        aisShape = new AIS_Shape(shape);
        aisShape->SetContext(myContext);

        module = new ModuleObject();
        module->id = id;
        module->name = name;
        module->aisShape = aisShape;
        moduleMap[id] = module;
        module->filename = "Module\\" + id;
        module->material = Graphic3d_NOM_CHROME;    // 14
        module->color = 0;
        module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)module->material );
    }

//    ModuleImportThread *importThread = new ModuleImportThread(filename, id, aisShape, module->transform, module->filename);
    ModuleImportThread *importThread = new ModuleImportThread(filename, module);
    connect(importThread, SIGNAL(importFinish(QString)), this, SLOT(moduleImportFinish(QString)));
    importThread->start();

}

void Viewer::moduleImportFinish( QString id )
{
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);
//    ModuleObject *module = moduleMap[id];
//    module->material = module->aisShape->Material();
//    Quantity_Color color;
//    module->aisShape->Color(color);
//    Quantity_Color::Color2argb(color, module->color);

//    QSqlQuery query;
//    qDebug(QString(QString("UPDATE StationModule SET material=%1").arg(module->material)+ QString(",color=%1 WHERE id='").arg(module->color) + id +"'").toLocal8Bit().constData());
//    if ( query.exec(QString(QString("UPDATE StationModule SET material=%1").arg(module->material)+ QString(",color=%1 WHERE id='").arg(module->color) + id +"'")) ) {
//        qDebug("修改成功！");
//    } else {
//        qDebug("修改失败！", true);
////        return;
//    }

//    if (module->isShow) {
//		myContext->Display(module->aisShape, Standard_True);
//	}
//	myView->SetComputedMode(true);
//	myView->Redraw();
myView->Update();
    qApp->restoreOverrideCursor();
}

void Viewer::stationChanged(QString stationId)
{
//    qDebug(stationId.toLocal8Bit().constData());

    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

//    myView->Reset();
    myView->ResetViewMapping();
    myView->ResetViewOrientation();
    rotX= SysUtils::getSysParam("viewRotateX").toDouble();//0.0;
    rotY= SysUtils::getSysParam("viewRotateY").toDouble() - 0.5;//0.0;//-1.570796327;
    myView->Rotate(rotX, rotY, 0, 0,0,0, Standard_True);
    myView->Pan(0, -3.0, 1.0);
    myView->SetScale(1.0);	//2.0
    myView->SetZSize( 100000.0 );   //20000.0

    loadingText->Display();
    myView->FitAll(false);

    myContext->RemoveAll(true);


//    axisMap.clear();
//    moduleMap.clear();


    currentStationId = stationId;
    moduleLoadedCount = 0;
//    moduleLoadCount = moduleMap.count();
//    if (moduleLoadCount == 0) {
//        emit moduleAllLoaded(moduleLoadedCount);
//        loadingText->Erase();
//        myView->Update();
//    }

    QMap<QString, QList<ModuleObject*> > moduleLoadMap;

    foreach (ModuleObject *module, moduleMap) {
        if (!module) {
            continue;
        }

        TopoDS_Shape shape;

        //        if (module->rotateAngle!=0.0) {
        //            gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
        //            module->transform.SetRotation(ax, module->rotateAngle);
        //        }
        //        module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));

        //            shape.Location(TopLoc_Location(module->transform));
        //            module->aisShape = new AIS_Shape(shape);


//        if (module->rotateAngle!=0.0) {
//              gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
//            module->transform.SetRotation(ax, module->rotateAngle);
//        }
        module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));
        shape.Location(TopLoc_Location(module->transform));
        module->aisShape = new AIS_Shape(shape);



        if (module->material >= 0){
            if (module->material > 18) {
                Quantity_Color color;
                Quantity_Color::Argb2color(module->color, color);
//                    module->aisShape->SetColor( color );
                module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
            }
            module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)module->material );
        } else {
            module->aisShape->UnsetColor();
            module->aisShape->SetMaterial( Graphic3d_NOM_CHROME );//(Graphic3d_NameOfMaterial)module->material );
//                module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
        }
        if (module->transparency > 0) {
            module->aisShape->SetTransparency(module->transparency);
//                qDebug(QString::number(module->transparency).toLocal8Bit());
        } else {
            module->aisShape->SetTransparency(0.0);
        }

        module->aisShape->SetContext(myContext);
//            module->aisShape->Set(shape);
//            module->aisShape->Redisplay();
//            module->aisShape->GetContext()->Update(module->aisShape, true);

        moduleLoadMap[module->filename].append(module);
//            ModuleLoadThread *loadThread = new ModuleLoadThread( module->filename, module->aisShape, module->transform, module->isHidden);
//            connect(loadThread, SIGNAL(moduleLoadFinish(QString)), this, SLOT(moduleLoadFinish(QString)));
//            loadThread->start();


    }
//    moduleAllLoaded(0);

    moduleLoadCount = moduleLoadMap.count();
    if (moduleLoadCount == 0) {
        emit moduleAllLoaded(moduleLoadedCount);
        loadingText->Erase();
        myView->Update();
    }

    foreach (QList<ModuleObject *> moduleList, moduleLoadMap) {
//        qDebug(moduleList.at(0)->filename.toLocal8Bit() + " %d", moduleList.count());

        ModuleLoadThread *loadThread = new ModuleLoadThread( moduleList.at(0)->filename, moduleList );
        connect(loadThread, SIGNAL(moduleLoadFinish(QString)), this, SLOT(moduleLoadFinish(QString)));
        loadThread->start();
    }

//    QSqlQuery query;
//    QString axisId;

//    query.exec(QString("SELECT * FROM StationAxis WHERE stationId='%1' AND isUse=1 ORDER BY displayOrder").arg(stationId));
//    while(query.next()) {
//        QSqlRecord record = query.record();
//        axisId = record.value("id").toString();
//        Axis *axis = axisMap[axisId];
//        if (!axis){
//            axis = new Axis;
//            axisMap.insert(std::pair<QString, Axis*>(axisId,axis));
//        }
//        axis->id = axisId;
//        axis->sign = record.value("sign").toString();
//        axis->name = record.value("name").toString();
//        axis->parentId = record.value("parentId").toString();
//        axis->type = record.value("type").toInt();
//        axis->start[X] = record.value("startX").toDouble();
//        axis->start[Y] = record.value("startY").toDouble();
//        axis->start[Z] = record.value("startZ").toDouble();
//        axis->end[X] = record.value("endX").toDouble();
//        axis->end[Y] = record.value("endY").toDouble();
//        axis->end[Z] = record.value("endZ").toDouble();
//        axis->symbol = record.value("symbol").toInt();
//        axis->minValue = record.value("minValue").toDouble();
//        axis->maxValue = record.value("maxValue").toDouble();

//        double length = axis->symbol * sqrt((axis->end[X]-axis->start[X])*(axis->end[X]-axis->start[X]) + (axis->end[Y]-axis->start[Y])*(axis->end[Y]-axis->start[Y]) + (axis->end[Z]-axis->start[Z])*(axis->end[Z]-axis->start[Z]));
//        axis->stepValue[X] = (axis->end[X]-axis->start[X]) / length;
//        axis->stepValue[Y] = (axis->end[Y]-axis->start[Y]) / length;
//        axis->stepValue[Z] = (axis->end[Z]-axis->start[Z]) / length;

//        axis->originOffset = record.value("originOffset").toDouble();

//        if (!axis->parentId.isEmpty()) {
//            Axis *parentAxis = axisMap[axis->parentId];
//            if (!parentAxis) {
//                parentAxis = new Axis;
//                axisMap[axis->parentId] = parentAxis;
//            }
//            parentAxis->subAxisList.push_back(axis->id);
//        }


//        if (axis->type == 1 && !axis->sign.isEmpty()) {
//            axis->value = 0.0;
//            axisSignMap[axis->sign] = axis->id;
//         //   emit updateAxisValue(axis->sign, axis->value);
//        } else {
//            axis->value = axis->originOffset;

//        }

//        axisMap[axisId] = axis;
//    }

//    query.exec(QString("SELECT * FROM StationModule WHERE stationId='%1' AND isUse=1").arg(stationId));
//    while(query.next()) {
//        QSqlRecord record = query.record();
//        Module *module = new Module;
//        module->id = record.value("id").toString();
////        module->code = record.value("code").toString();
//        module->name = record.value("name").toString();
//        module->type = record.value("type").toInt();
////        module->fileId = record.value("fileId").toString();
//        module->bindAxis = record.value("bindAxis").toString();
//        module->pos[X] = record.value("x").toDouble();
//        module->pos[Y] = record.value("y").toDouble();
//        module->pos[Z] = record.value("z").toDouble();

//        module->transparency = record.value("transparent").toDouble();

////        module->rotatePoint[X] = 0.0;
////        module->rotatePoint[Y] = 0.0;
////        module->rotatePoint[Z] = 0.0;
//        module->rotateDirection[X] = record.value("rotateX").toDouble();
//        module->rotateDirection[Y] = record.value("rotateY").toDouble();
//        module->rotateDirection[Z] = record.value("rotateZ").toDouble();
//        module->rotateAngle = record.value("rotateAngle").toDouble();
//        module->material = record.value("material").toInt();
//        module->color = record.value("color").toInt();
//        module->isShow = (record.value("isShow").toInt() != 1);

//        if (!module->bindAxis.isEmpty()) axisMap[module->bindAxis]->moduleList.push_back(module->id);

//        QString filename = "D:\\DzicsRobot\\Module\\" + module->id;

//        TopoDS_Shape shape;

//        if (module->rotateAngle!=0.0) {
//            gp_Ax1 ax = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(module->rotateDirection[X], module->rotateDirection[Y], module->rotateDirection[Z]));
//            module->transform.SetRotation(ax, module->rotateAngle);
//        }
//        if (module->type == 2) {
//            gp_Trsf transformEnv;
//            gp_Ax1 az = gp_Ax1(gp_Pnt(module->rotatePoint[X], module->rotatePoint[Y], module->rotatePoint[Z]), gp_Dir(0, 0, 1));
//            transformEnv.SetRotation(az, -0.9948);//2.147);
//            module->transform = transformEnv * module->transform;
//        }
//        module->transform.SetTranslationPart(gp_Vec(module->pos[X], module->pos[Y], module->pos[Z]));

//        if (module->type == 5) {  // 毛坯

//        } else {
//            shape.Location(TopLoc_Location(module->transform));
//            module->aisShape = new AIS_Shape(shape);

//            Quantity_Color color;
//            Quantity_Color::Argb2color(module->color, color);
//            module->aisShape->SetColor( color );
//            if (module->material >= 0){
//                module->aisShape->SetMaterial( (Graphic3d_NameOfMaterial)module->material );
////                module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
//            } else {
//                module->aisShape->SetMaterial( Graphic3d_NOM_CHROME );//(Graphic3d_NameOfMaterial)module->material );
//                module->aisShape->UnsetColor();
////                module->aisShape->SetColor( Quantity_Color(((module->color & 0xFF0000) >> 16) / 255.0, ((module->color & 0xFF00) >> 8) / 255.0, (module->color & 0xFF) / 255.0, Quantity_TOC_RGB) ); // Quantity_NOC_WHITE );
//            }
//            if (module->transparency > 0) {
//                module->aisShape->SetTransparency(module->transparency);
//            }

//            module->aisShape->SetContext(myContext);
//            ModuleLoadThread *loadThread = new ModuleLoadThread(filename, module->aisShape, module->transform, module->isShow);
//            connect(loadThread, SIGNAL(moduleLoadFinish(QString)), this, SLOT(moduleLoadFinish(QString)));
//            loadThread->start();

//        }

//        moduleMap[module->id] = module;
//    }
}

void Viewer::moduleChanged(QString id, bool isRemoved)
{
    if (isRemoved) {
        if (moduleMap[id]!=NULL) {
            if (moduleMap[id]->aisShape!=NULL) {
                myContext->Remove(moduleMap[id]->aisShape, Standard_True);
            }
        }
        delete moduleMap[id];
        moduleMap[id] = NULL;
    }
}

#include "PageStation.h"
extern PageStation *pageStation;

#include <MoveTool.h>
#include <MeasureTool.h>
#include <SelectTool.h>
MoveTool *moveTool = NULL;
MeasureTool *measureTool = NULL;
SelectTool *selectTool = NULL;

//独立出模型变化的类
void Viewer::openTransformTool()
{
    if(moveTool == NULL){
        moveTool = new MoveTool(this->parentWidget(), m_pFloatTool);
        connect(this , SIGNAL(mousePressSignal(QMouseEvent*)) , moveTool , SLOT(mouseLeftButtonClick(QMouseEvent*)));
        connect(moveTool, SIGNAL(moveEnd(double,double,double)), pageStation, SLOT(moveEnd(double,double,double)));
        connect(moveTool, SIGNAL(rotateEnd(double,double,double,double,double,double,double)), pageStation, SLOT(rotateEnd(double,double,double,double,double,double,double)));
        connect(moveTool , SIGNAL(toolClose()) , this , SLOT(toolClose()));
    }
    disconnect(this , SIGNAL(setPntInfoSignal()) , measureTool , SLOT(setPntInfo()));
    connect(this , SIGNAL(setPntInfoSignal()) , moveTool , SLOT(setPntInfo()));

    moveTool->show();
}
void Viewer::showToolWidget(int type)
{
    if (type == 1) {
        if(moveTool == NULL){
            moveTool = new MoveTool(this->parentWidget(), m_pFloatTool);
            connect(this , SIGNAL(mousePressSignal(QMouseEvent*)) , moveTool , SLOT(mouseLeftButtonClick(QMouseEvent*)));
            connect(moveTool, SIGNAL(moveEnd(double,double,double)), pageStation, SLOT(moveEnd(double,double,double)));
            connect(moveTool, SIGNAL(rotateEnd(double,double,double,double,double,double,double)), pageStation, SLOT(rotateEnd(double,double,double,double,double,double,double)));
            connect(moveTool , SIGNAL(toolClose()) , this , SLOT(toolClose()));
        }
        if(moveTool != NULL)
        {
            disconnect(this , SIGNAL(setPntInfoSignal()) , measureTool , SLOT(setPntInfo()));
            connect(this , SIGNAL(setPntInfoSignal()) , moveTool , SLOT(setPntInfo()));
            moveTool->show();
        }

    } else if (type == 2) {
        if(measureTool == NULL){
            measureTool = new MeasureTool(this->parentWidget(), m_pFloatTool);
            connect(this , SIGNAL(mousePressSignal(QMouseEvent*)) , measureTool , SLOT(mouseLeftButtonClick(QMouseEvent*)));
            connect(measureTool , SIGNAL(toolClose()) , this , SLOT(toolClose()));
            //connect(this , SIGNAL(setPntInfoSignal()) , measureTool , SLOT(setPntInfo()));
        }
        if(measureTool != NULL)
        {
            disconnect(this , SIGNAL(setPntInfoSignal()) , moveTool , SLOT(setPntInfo()));
            connect(this , SIGNAL(setPntInfoSignal()) , measureTool , SLOT(setPntInfo()));
            measureTool->show();
        }

    } else if (type == 3) {
        if(selectTool == NULL){

            selectTool = new SelectTool(this->parentWidget(), myContext, myView, myDevice);
            connect(this , SIGNAL(mousePressSignal(QMouseEvent*)) , selectTool , SLOT(mouseLeftButtonClick(QMouseEvent*)));
            connect(selectTool, SIGNAL(selectEdges(QList<TopoDS_Edge>)), this, SLOT(nurbsGenerate(QList<TopoDS_Edge>)));

            connect(selectTool, SIGNAL(selectEdges2(QList<TopoDS_Edge>)), this, SLOT(nurbsGenerate2(QList<TopoDS_Edge>)));
            connect(selectTool , SIGNAL(toolClose()) , this , SLOT(toolClose()));
            connect(selectTool,SIGNAL(setLock(bool)),mainWindow,SLOT(setLock_slot(bool)));

            connect(selectTool,SIGNAL(selectPoint(gp_Pnt)), pageStation, SLOT(updateCalibrationPoint(gp_Pnt)));
        }
        if(measureTool != NULL)
        {
            selectTool->show();
        }

    }
    else if (type == 4) {
           if(m_pFloatTool == NULL)
           {
               m_pFloatTool = new FloatTool(myContext, myView);
           }
       }
}

void Viewer::toolClose()
{
    toolSelectMode = false;
}

#include <TopExp_Explorer.hxx>
#include <GeomConvert.hxx>
#include "DzicsMotion.h"

QString getEdgeType(TopoDS_Edge edge){
    Standard_Real first,last;
    Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);

    Handle(Standard_Type) type = m_curve->DynamicType();
    return QString(type->Name());
}

bsplines convertToNurbs(Handle_Geom_BSplineCurve aBSpline){
    bsplines line;
        Standard_Integer poles_len = aBSpline->NbPoles() ,
                         knots_len = aBSpline->NbKnots();
        // 顶点
        line.poles = (double **)malloc(sizeof(double*)*poles_len);
        for(int i = 0 ;  i < poles_len ; i++){
            line.poles[i] = (double*)malloc(sizeof(double) * 3);
        }
        for(int i = 0 ; i < poles_len ; i++){
            gp_Pnt p = aBSpline->Pole(i+1);
            line.poles[i][0] = p.X();
            line.poles[i][1] = p.Y();
            line.poles[i][2] = p.Z();
        }
        // 节点
        line.knots = (double*)malloc(sizeof(double) *knots_len);
        for(int i = 0 ; i < knots_len ; i++){
            line.knots[i] = aBSpline->Knot(i+1);
        }
        line.multiplicities = (int*)malloc(sizeof(int)*knots_len);
        for(int i = 0; i < knots_len ; i++){
            line.multiplicities[i] = aBSpline->Multiplicity(i+1);
        }

        line.weights = (double*)malloc(sizeof(double)*poles_len);
        for(int i = 0; i < poles_len ; i++){
            line.weights[i] = aBSpline->Weight(i+1);
        }

        line.weightLen = poles_len;
        line.polesLen = poles_len;
        line.knotsLen = knots_len;
        line.multipLen = knots_len;
        line.degree = aBSpline->Degree();
        line.periodic = aBSpline->IsPeriodic();
        line.rational = aBSpline->IsRational();
        return line;
}

void Viewer::nurbsGenerate(QList<TopoDS_Edge> edges)
{
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

    QList<bsplines> nurbsList;
    foreach (TopoDS_Edge edge, edges) {
        if (edge.IsNull()) {
            continue;
        }
        if (getEdgeType(edge) == "Geom_Line") {
            Standard_Real first,last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge,first,last);

            bsplines line;
            line.polesLen = line.knotsLen = line.weightLen = line.multipLen = 2; // 直线标识
            // 顶点
            line.poles = (double **)malloc(sizeof(double*)*2);
            for(int i = 0 ;  i < line.polesLen ; i++){
                line.poles[i] = (double*)malloc(sizeof(double) * 3);
            }
            gp_Pnt p = curve->Value(first);
            line.poles[0][0] = p.X();
            line.poles[0][1] = p.Y();
            line.poles[0][2] = p.Z();
            p = curve->Value(last);
            line.poles[1][0] = p.X();
            line.poles[1][1] = p.Y();
            line.poles[1][2] = p.Z();

            // 节点
            line.knots = (double*)malloc(sizeof(double)*line.knotsLen);
            for(int i = 0 ; i < line.knotsLen ; i++) {
                line.knots[i] = i;
            }
            line.multiplicities = (int*)malloc(sizeof(int)*line.multipLen);
            for(int i = 0; i < line.multipLen ; i++) {
                line.multiplicities[i] = 2;
            }
            line.weights = (double*)malloc(sizeof(double)*line.weightLen);
            for(int i = 0; i < line.weightLen ; i++) {
                line.weights[i] = 1;
            }

            line.degree = 1;
            line.periodic = 0;
            line.rational = 0;

            nurbsList.append(line);
            continue;
        }
//        Standard_Real first,last;
        BRepAdaptor_Curve acurve(edge);
        Handle(Geom_Curve) m_curve = GeomAdaptor::MakeCurve(acurve);
//        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
        if(m_curve != NULL){
//            qDebug("CurveToBSplineCurve");
            Handle(Geom_BSplineCurve) bs =  GeomConvert::CurveToBSplineCurve(m_curve);
            if(bs != NULL){
//                qDebug("append bspline");
                nurbsList.append(convertToNurbs(bs));
            }
        }
    }

    QString p = "Sub Main\n  Takearm Keep = 0\n";
//double TiSpe[],double u0u1[],double zixyz[],double ** XYZ, double timesP
    double TiSpe[] = {100, 10, 10};
    double u0u1[] = { 0.1, 0.01 };
    double zixyz[] = { -172, 28, -173 };    //{180,0,180};
double **resultList = NULL;
//    double **resultList = (double **)malloc(sizeof(double *)*100000);
//    for (int i=0; i<100000; i++)
//        resultList[i] = (double *)malloc(sizeof(double)*6);
    foreach (bsplines nurbs, nurbsList) {
        int len=0;
        try{
            qDebug("divideNbs");
            resultList=divideNbs(nurbs, TiSpe, u0u1, zixyz, &len, 0.1, 0);
        } catch (...) {

        }

        for(int i=0;i<len;i++) {
            p+= "  MOVE P,P(" + QString::number(resultList[i][0])+", "+QString::number(resultList[i][1])+", "+QString::number(resultList[i][2])+", "+QString::number(resultList[i][3])+", "+QString::number(resultList[i][4])+", "+QString::number(resultList[i][5])+", 1)\n";
        }
    }

    p += "End Sub\n";

//    qDebug(p.toLocal8Bit());

    emit updateProgram(p);

    qApp->restoreOverrideCursor();
}

extern int myRobot;
extern QString robotName;
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
void Viewer::nurbsGenerate2(QList<TopoDS_Edge> edges)
{
    if(robotName==""||robotName=="请选择")
        return;
    qApp->restoreOverrideCursor();
    qApp->setOverrideCursor(Qt::WaitCursor);

    QList<bsplines> nurbsList;
    foreach (TopoDS_Edge edge, edges) {
        if (edge.IsNull()) {
            continue;
        }
        if (getEdgeType(edge) == "Geom_Line") {
            Standard_Real first,last;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge,first,last);

            bsplines line;
            line.polesLen = line.knotsLen = line.weightLen = line.multipLen = 2; // 直线标识
            // 顶点
            line.poles = (double **)malloc(sizeof(double*)*2);
            for(int i = 0 ;  i < line.polesLen ; i++){
                line.poles[i] = (double*)malloc(sizeof(double) * 3);
            }
            gp_Pnt p = curve->Value(first);
            line.poles[0][0] = p.X();
            line.poles[0][1] = p.Y();
            line.poles[0][2] = p.Z();
            p = curve->Value(last);
            line.poles[1][0] = p.X();
            line.poles[1][1] = p.Y();
            line.poles[1][2] = p.Z();

            // 节点
            line.knots = (double*)malloc(sizeof(double)*line.knotsLen);
            for(int i = 0 ; i < line.knotsLen ; i++) {
                line.knots[i] = i;
            }
            line.multiplicities = (int*)malloc(sizeof(int)*line.multipLen);
            for(int i = 0; i < line.multipLen ; i++) {
                line.multiplicities[i] = 2;
            }
            line.weights = (double*)malloc(sizeof(double)*line.weightLen);
            for(int i = 0; i < line.weightLen ; i++) {
                line.weights[i] = 1;
            }

            line.degree = 1;
            line.periodic = 0;
            line.rational = 0;

            nurbsList.append(line);
            continue;
        }
//        Standard_Real first,last;
        BRepAdaptor_Curve acurve(edge);
        Handle(Geom_Curve) m_curve = GeomAdaptor::MakeCurve(acurve);
//        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
        if(m_curve != NULL){
//            qDebug("CurveToBSplineCurve");
            Handle(Geom_BSplineCurve) bs =  GeomConvert::CurveToBSplineCurve(m_curve);
            if(bs != NULL){
//                qDebug("append bspline");
                nurbsList.append(convertToNurbs(bs));
            }
        }
    }

    QString p = "Sub Main\n  Takearm Keep = 0\n";
//double TiSpe[],double u0u1[],double zixyz[],double ** XYZ, double timesP
    double TiSpe[] = {100, 10, 10};


double **resultList = NULL;
//    double **resultList = (double **)malloc(sizeof(double *)*100000);
//    for (int i=0; i<100000; i++)
//        resultList[i] = (double *)malloc(sizeof(double)*6);

//update by zhugz

double timesP;
int flagNbsReverse[4];
double toolANDzuan[14];
int flagZAngle[2];
double ZincluAngle;
double u0u1[2];
double zixyz[3];    //{180,0,180};

QSqlQuery query;
int ret=query.exec(QString("select * from RobotSetting where id='%1'").arg(robotName));
if(query.next())
{
    timesP=query.value(12).toDouble();
    flagNbsReverse[0]=myRobot;
    flagNbsReverse[1]=query.value(18).toInt();
    flagNbsReverse[2]=query.value(1).toInt();
    flagNbsReverse[3]=query.value(5).toInt();
    toolANDzuan[0]=query.value(15).toDouble();
    toolANDzuan[1]=query.value(16).toDouble();
    toolANDzuan[2]=query.value(17).toDouble();
    toolANDzuan[3]=query.value(9).toDouble();
    toolANDzuan[4]=query.value(6).toDouble();
    toolANDzuan[5]=query.value(7).toDouble();
    toolANDzuan[6]=query.value(8).toDouble();
    toolANDzuan[7]=query.value(2).toDouble();
    toolANDzuan[8]=query.value(3).toDouble();
    toolANDzuan[9]=query.value(4).toDouble();
    toolANDzuan[10]=query.value(10).toDouble();
    toolANDzuan[11]=query.value(24).toDouble();
    toolANDzuan[12]=query.value(25).toDouble();
    toolANDzuan[13]=query.value(26).toDouble();
    flagZAngle[0]=query.value(22).toInt();
    flagZAngle[1]=query.value(23).toInt();
    ZincluAngle=query.value(11).toDouble();
    u0u1[0]=query.value(13).toDouble();
    u0u1[1]=query.value(14).toDouble();
    zixyz[0]=query.value(19).toDouble();
    zixyz[1]=query.value(20).toDouble();
    zixyz[2]=query.value(21).toDouble();

}
/*
NdivideNbs(bsplines nbsData, double TiSpe[], double u0u1[],
double zixyz[], int * XYZLen, double timesP, int flagNbsReverse[4],
 double toolANDzuan[14], int flagZAngle[2], double ZincluAngle)
*/

    foreach (bsplines nurbs, nurbsList) {
        int len=0;
        try{
            qDebug("divideNbs");
            //resultList=divideNbs(nurbs, TiSpe, u0u1, zixyz, &len, 0.1, 0);
            resultList=NdivideNbs(nurbs,TiSpe,u0u1,zixyz,&len,timesP,flagNbsReverse,toolANDzuan,flagZAngle,ZincluAngle);
        } catch (...) {

        }

        for(int i=0;i<len;i++) {
            p+= "  MOVE P,P(" + QString::number(resultList[i][0])+", "+QString::number(resultList[i][1])+", "+QString::number(resultList[i][2])+", "+QString::number(resultList[i][3])+", "+QString::number(resultList[i][4])+", "+QString::number(resultList[i][5])+", 1)\n";
        }
    }

    p += "End Sub\n";

//    qDebug(p.toLocal8Bit());

    emit updateProgram(p);

    qApp->restoreOverrideCursor();
}

void Viewer::showText(char * str)
{
    gp_Pnt aPnt1(100/2.,2,11);

    qDebug()<<"x"<<aPnt1.X()<<aPnt1.Y()<<aPnt1.Z();
    Handle_Prs3d_TextAspect aTextAspect= new Prs3d_TextAspect();
    aTextAspect->SetFont("Monospace");//Graphic3d_NOF_ASCII_COMPLEX);
//    aTextAspect->SetFont("simhei");// "Monospace");//Graphic3d_NOF_ASCII_COMPLEX);
    aTextAspect->SetHeight(30);
    aTextAspect->SetColor(Quantity_NOC_YELLOW);

    gp_Pnt p;
    Prs3d_Text::Draw(text1, aTextAspect, str, p);
//    Draw_Text3D t(p, "Loading...", Draw_orange);
    text1->Display();
}


void Viewer::showArea(QString curArea)
{
    //myContext->Display(myRect);
    //BRepPrimAPI_MakeBox r1(BRepPrimAPI_MakeBox(gp_Pnt(60, 60, 0), 600.,550.,600.));

    if(myRect[curArea].IsNull())
        return;

    myContext->Display(myRect[curArea]);
}

void Viewer::removeArea(QString curArea)
{

    if(curArea==QString.null)
        return;
    if(myRect[curArea].IsNull())
        return;

    myContext->Remove(myRect[curArea]);
    qDebug()<<"before"<<myRect[curArea].IsNull()<<myRect[curArea];
    //myRect->Delete();
    myRect[curArea]->ClearOwner();
    delete myRect[curArea];
    myRect[curArea]=NULL;

    qDebug()<<"del"<<myRect[curArea].IsNull()<<myRect[curArea];

}
void Viewer::moveRect(QString uuid,int x,int y,int z)
{

    if(myRect[uuid].IsNull())
        return;

    myContext->SetSelected(myRect[uuid]);

    gp_Trsf theTransformation;
    gp_Vec theVectorOfTranslation(x,y,z);
    theTransformation.SetTranslation(theVectorOfTranslation);


    myRect[uuid]->SetLocation(theTransformation);
    myRect[uuid]->Redisplay();
    myRect[uuid]->GetContext()->Update(myRect[uuid],true);

}

void Viewer::createArea(QString uuid,int len1, int len2, int len3,int x,int y,int z)
{
    //updated by zhugz
    //    Handle_AIS_Shape aCone = new AIS_Shape(BRepPrimAPI_MakeCone(50, 30, 80));
    //    Handle_AIS_Shape aSphere = new AIS_Shape(BRepPrimAPI_MakeSphere(60));

      // myContext->Display(aCone);
      //myContext->Display(aSphere);

    if(!len1||!len2||!len3)
        return;

        BRepPrimAPI_MakeBox r1(BRepPrimAPI_MakeBox(gp_Pnt(x, y, z), len1,len2,len3));
        //SetInteriorColorIndex(0);

        myRect[uuid]= new AIS_Shape(r1 );

        if(myRect[uuid].IsNull())
            return;

        myRect[uuid]->SetColor(Quantity_NOC_RED);
        myRect[uuid]->SetTransparency(0.7);
        myContext->Display(myRect[uuid]);

        //旋转
    //    gp_Trsf trsfSpindle;
    //    trsfSpindle.SetRotation(gp_Ax1(gp_Pnt(60, 60,0), gp_Dir(800, 800, 800)), 0);
        //平移
    //    gp_Trsf theTransformation;
    //    gp_Vec theVectorOfTranslation(-300,-300,0);
    //    theTransformation.SetTranslation(theVectorOfTranslation);
    //    myRect->SetLocation(theTransformation);

}

int Viewer::drawText(const TCollection_AsciiString& aText,
                     const Standard_Real            anX,
                     const Standard_Real            anY,
                     const Standard_Real            aZ,
                     const Aspect_TypeOfText        aType,
                     const Quantity_PlaneAngle      anAngle,
                     const Standard_Real            aSlant,
                     const Standard_Integer         aColorIndex,
                     const Standard_Integer         aFontIndex,
                     const Quantity_Factor          aScale)
{
  if( myContext.IsNull() )
  {

    return -1;
  }
  Handle(ISession_Text) aGraphicText = new ISession_Text(aText,anX,anY,aZ,Aspect_TOT_SOLID,0.0, 0.0, 1, 1, 0.1);
  myContext->Display(aGraphicText, 1);
  return 0;
}

void Viewer::setViewUpdateState(bool state)
{
    this->viewUpdateState = state;
}

void Viewer::setGeometricElementSelectMode(int selectMode)
{

    if(selectMode != 9)
    {
        toolSelectMode = true;
    }else
    {
        toolSelectMode = false;     //鼠标框选模型，不需要切换occ选择模式
    }
    m_pFloatTool->UpdateActualLocation();
    m_pFloatTool->setGeometricElementSelectMode(selectMode);
}
void Viewer::cancelGeometricElementSelectMode()
{
     toolSelectMode = false;
     myContext->CloseLocalContext();
     m_pFloatTool->ReturnOriginalLocation();
     m_pFloatTool->clearOldActivatedStandard();
}
void Viewer::cancelButtonDownSlot()
{
    emit cancelButtonDownSignal();
}
//// 鼠标点击选取点
//void Viewer::mouseLeftButtonClick(QMouseEvent *e)
//{
//        if ( e->button() == Qt::LeftButton ){

//                    bool hasAppend = true;
//                    for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//                        if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//                            hasAppend = false;
//                        }
//                    }
//                    if(hasAppend){
//                        if(aShapes.count() == 2){
//                            clearShapes();
//                        }
//                        myContext->Hilight(aio);
//                        aShapes.append(aio);
//                    }

//                    setPntToPntInfo();
//                }

//                else if(myMoveType == LinLengthType)    // 直线测距
//                {
//                    TopoDS_Edge edge =TopoDS::Edge(selectShape);
//                    Standard_Real first,last;
//                    Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
//                    Handle(Geom_Line) hgc =  Handle_Geom_Line ::DownCast(m_curve);
//                    // 判断是否是直线
//                    if(!hgc.IsNull()){
//                        bool hasAppend = true;
////                        TopoDS_Edge pnt1 = BRepBuilderAPI_MakeEdge(m_curve);
//                        Handle(AIS_Shape) aio = new AIS_Shape(edge);
//                        for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//                            if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//                                hasAppend = false;
//                            }
//                        }
//                        if(hasAppend){
//                            if(aShapes.count() == 1){
//                                clearShapes();
//                            }
//                            myContext->Hilight(aio);
//                            aShapes.append(aio);
//                            setLinInfo();
//                        }
//                    }
//                }
//                // 三点测角度
//                else if(myMoveType == ThreePntAngleType)
//                {
//                    Handle(AIS_Shape) aio = selectPointFromShape(selectShape, e->x(), e->y());
//                    // 判断之前是否存在
//                    bool hasAppend = true;
//                    for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//                        if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//                            hasAppend = false;
//                        }
//                    }
//                    if(hasAppend){
//                        if(aShapes.count() >= 3){
//                            clearShapes();
//                        }
//                        myContext->Hilight(aio);
//                        aShapes.append(aio);
//                        setThreePntInfo();
//                    }
//                }
//                // 两条直线角度
//                else if(myMoveType == TwoLinAngleType){
//                    if(selectShape.ShapeType() == TopAbs_EDGE){
//                        TopoDS_Edge edge =TopoDS::Edge(selectShape);
//                        Standard_Real first,last;
//                        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
//                        Handle(Geom_Line) hgc =  Handle_Geom_Line ::DownCast(m_curve);
//                        // 判断是否是直线
//                        if(!hgc.IsNull()){
//                            bool hasAppend = true;
//                            Handle(AIS_Shape) aio = new AIS_Shape(edge);
//                            for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//                                if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//                                    hasAppend = false;
//                                }
//                            }
//                            if(hasAppend){
//                                if(aShapes.count() >= 2){
//                                    clearShapes();
//                                }
//                                myContext->Hilight(aio);
//                                aShapes.append(aio);
//                            }
//                        }
//                    }
//                }
//                // 两个平面角度
//                else if(myMoveType == TwoPlaneAngleType){
//                    // 判断是面
//                    if(selectShape.ShapeType() == TopAbs_FACE){
//                        TopoDS_Face face = TopoDS::Face(selectShape);
//                        bool hasAppend = true;
//                        Handle(AIS_Shape) aio = new AIS_Shape(face);
//                        for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//                            if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//                                hasAppend = false;
//                            }
//                        }
//                        if(hasAppend){
//                            if(aShapes.count() >= 2){
//                                clearShapes();
//                            }
//                            //myContext->Hilight(aio);
//                            aShapes.append(aio);
//                            setTwoPlaneInfo();
//                        }

//                    }
//                }
//                // 点到曲面最短距离
//                else if(myMoveType == PointToSurfaceType){

//                    if(mySelectType == FaceSelectType)      // 判断是面
//                    {
//                        selectSurfaceFromShape(selectShape);
//                        setPointToSurfaceInfo();
//                    }else
//                    {
//                        Handle(AIS_Shape) aio = selectPointFromShape(selectShape, e->x(), e->y());
//                        // 判断之前是否存在
//                        bool hasAppend = true;
//                        for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//                            if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//                                hasAppend = false;
//                            }
//                        }
//                        if(hasAppend){
//                            if(aShapes.count() == 2){
//                                clearShapes();
//                            }
//                            myContext->Hilight(aio);
//                            aShapes.append(aio);
//                        }

//                        setPointToSurfaceInfo();
//                    }
//                }
//            }

//        }
//}

