#include "SelectTool.h"
#include "ui_SelectTool.h"
#include "math.h"
#include <TopoDS_Vertex.hxx>
#include <TopoDS.hxx>
#include <BRep_Tool.hxx>
#include <TopExp_Explorer.hxx>
#include <Geom_Conic.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <QMessageBox>
#include <GeomLib_Tool.hxx>
#include <BRepBuilderAPI_MakeWire.hxx>
#include <QDebug>
#include <BRepClass3d_SolidClassifier.hxx>
#include <GeomConvert.hxx>
#include <TColStd_HArray2OfReal.hxx>
#include <BRepPrimAPI_MakeSphere.hxx>
#include "Global.h"

SelectTool::SelectTool(QWidget *parent, Handle_AIS_InteractiveContext theContext, Handle_V3d_View theView, Handle(Graphic3d_GraphicDriver) theDevice) :
    QMainWindow(parent),
    ui(new Ui::SelectTool)
{

    mySelectType = DefaultSelectType;
    // 设置窗体样式
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

//    this->setWindowTitle(tr("请选择"));

    ui->setupUi(this);


    // 定位
    QRect widgetRect=parent->rect();
    QPoint panelPos=QPoint(widgetRect.left() + widgetRect.width() - this->width() - 10, widgetRect.top());
    panelPos=parent->mapToGlobal(panelPos);
    move(panelPos);

    myContext = theContext;
    myView = theView;
    myDevice = theDevice;

    ui->stackedWidget->setCurrentWidget(ui->page_1);

    //    QMapIterator<QString, RobotObject*> i(robotMap);
    //     while (i.hasNext()) {
    //         i.next();
    //         ui->comboBox->addItem(i.key());
    //     }

    //手动添加combobox
    QStringList robots;
    robots<<"请选择"<<"DENSO"<<"ABB";
    ui->comboBox->addItems(robots);

    connect(ui->comboBox,SIGNAL(currentIndexChanged(QString)),this,SLOT(initParam(QString)));
}

SelectTool::~SelectTool()
{
    delete ui;
    clearOldActivatedStandard();
    myContext->CloseAllContexts();
    clearMoreEdgesSelect();
    emit toolClose();
}


void SelectTool::showEvent(QShowEvent *){
    qDebug()<<"showEvent";
    myContext->OpenLocalContext();
}

// 隐藏工具
void SelectTool::hideEvent(QHideEvent *){
    clearOldActivatedStandard();
    myContext->CloseAllContexts();
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
    clearMoreEdgesSelect();
    emit toolClose();
}

// 清除数据
void SelectTool::clearValue(){
    ui->EdgeEndX->clear();
    ui->EdgeEndY->clear();
    ui->EdgeEndZ->clear();
    ui->EdgeStartX->clear();
    ui->EdgeStartY->clear();
    ui->EdgeStartZ->clear();
    ui->PntX->clear();
    ui->PntY->clear();
    ui->PntZ->clear();
    ui->WireCutMiddleZ->clear();
    ui->WireCutEndX->clear();
    ui->WireCutEndY->clear();
    ui->WireCutEndZ->clear();
    ui->WireCutMiddleX->clear();
    ui->WireCutMiddleY->clear();
    ui->WireCutStartX->clear();
    ui->WireCutStartY->clear();
    ui->WireCutStartZ->clear();
    ui->WireEndX->clear();
    ui->WireEndY->clear();
    ui->WireEndZ->clear();
    ui->WireStartX->clear();
    ui->WireStartY->clear();
    ui->WireStartZ->clear();
}

// 鼠标点击选取点
void SelectTool::mouseLeftButtonClick(QMouseEvent *e)
{
    if(mySelectType != DefaultSelectType){
        if ( e->button() == Qt::LeftButton ){
            // 判断是否有选中的
            myContext->InitDetected();
            if(myContext->HasDetected()){
                // 获取选中值
                TopoDS_Shape selectShape = myContext->DetectedShape();
                // 判断模式
                // 取点
                if(mySelectType == PntSelectType){
                    // 判断选择的是否是点
                    if(selectShape.ShapeType() == TopAbs_VERTEX){
                        Handle(AIS_Shape) aio = new AIS_Shape(selectShape);
                        // 判断之前是否存在
                        setPntInfo(aio);
                        currentShape = selectShape;
                    }
                    // 判断是否是边
                    if(selectShape.ShapeType() == TopAbs_EDGE){
                        TopoDS_Edge edge =TopoDS::Edge(selectShape);
                        Standard_Real first,last;
                        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
                        Handle(Geom_Conic) hgc =  Handle_Geom_Conic::DownCast(m_curve);
                        // 判断是否是圆
                        if(!hgc.IsNull()){
                            gp_Pnt centerPnt = hgc->Location();
                            TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(centerPnt);
                            Handle(AIS_Shape) aio = new AIS_Shape(pnt1);
                            setPntInfo(aio);
                            currentShape = selectShape;
                        }
                    }
                }
                // 取边
                else if(mySelectType == LinSelectType){
                    TopoDS_Edge edge =TopoDS::Edge(selectShape);
                    if(!edge.IsNull()){
                        if(getEdgeType(edge) == "Geom_Line"){
                            setEdgeInfo(edge);
                            currentShape = selectShape;
                        }
                    }
                }
                // 取曲线
                else if(mySelectType == NurbsSelectType){
                    //BRep_Tool 提供了从Topo模型 转 Geom模型 的方法
                    if(ui->stackedWidget->currentWidget() == ui->page_4){
                        TopoDS_Wire wire = TopoDS::Wire(selectShape);
                        if(!wire.IsNull()){
                            setWireInfo(wire);
                            currentShape = selectShape;
                        }
                    }else if(ui->stackedWidget->currentWidget() == ui->page_5){
                        TopoDS_Vertex vertex = TopoDS::Vertex(selectShape);
                        if(!vertex.IsNull()){
                            startVertex = vertex;
                            setStartCutInfo();
                        }
                    }else if(ui->stackedWidget->currentWidget() == ui->page_6){
                        TopoDS_Vertex vertex = TopoDS::Vertex(selectShape);
                        if(!vertex.IsNull()){
                            middleVertex = vertex;
                            setMiddleCutInfo();
                        }
                    }else if(ui->stackedWidget->currentWidget() == ui->page_7){
                        TopoDS_Vertex vertex = TopoDS::Vertex(selectShape);
                        if(!vertex.IsNull()){
                            endVertex = vertex;
                            setEndCutInfo();
                        }
                    }

                }
                // 取多边
                else if(mySelectType == MoreEdgeSelectType){
                    TopoDS_Edge edge =TopoDS::Edge(selectShape);
                    if(!edge.IsNull()){
                        currentShape = edge;
                        setMoreEdgeInfo(edge);
                        addMoreEdgesInfo();
                    }
                }
                // 取中点
                else if(mySelectType == MiddlePntSelectType){
                    // 判断是否是边
                    if(selectShape.ShapeType() == TopAbs_EDGE){
                        TopoDS_Edge edge =TopoDS::Edge(selectShape);

                        TopExp_Explorer anExp(edge, TopAbs_VERTEX);
                        int point_count = 0;
                        Standard_Real x_sum=0;
                        Standard_Real y_sum=0;
                        Standard_Real z_sum=0;
                        gp_Pnt Pnt;
                        for( ; anExp.More(); anExp.Next()){
                            const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
                            Pnt = BRep_Tool::Pnt(vertex);
                            x_sum += Pnt.X();
                            y_sum += Pnt.Y();
                            z_sum += Pnt.Z();
                            point_count++;
                        }
                        Pnt.SetX(x_sum/point_count);
                        Pnt.SetY(y_sum/point_count);
                        Pnt.SetZ(z_sum/point_count);

                        TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(Pnt);
                        Handle(AIS_Shape) aio = new AIS_Shape(pnt1);
                        myContext->Hilight(aio);
                        setPntInfo(aio);
                    }
                }
                // 取端点
                else if(mySelectType == EndPntSelectType){
                    // 判断是否是边
                    if(selectShape.ShapeType() == TopAbs_EDGE){
                        Handle(AIS_Shape) aio;
                        TopoDS_Edge edge =TopoDS::Edge(selectShape);

                        TopExp_Explorer anExp(edge, TopAbs_VERTEX);
                        gp_Pnt Pnt;
                        Standard_Real minDistance = 9999999;
                        for( ; anExp.More(); anExp.Next()){
                            const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
                            Pnt = BRep_Tool::Pnt(vertex);

                            GLint viewport[4];
                            GLdouble modleview[16], projection[16];


                            Handle(Visual3d_View) FromView = myView->View() ;
                            Visual3d_ViewOrientation viewOrientation = myView->ViewOrientation();
                            GLdouble aOrientationMatrix[16];

                            glPushMatrix();

                            glGetIntegerv(GL_VIEWPORT, viewport);
                            glGetDoublev(GL_MODELVIEW_MATRIX, modleview);
                            glGetDoublev(GL_PROJECTION_MATRIX, projection);
                            glPopMatrix();

                            GLdouble winX = e->x();
                            GLdouble winY = viewport[3] - e->y();
                            GLdouble winZ1 = 0;
                            GLdouble winZ2 = 10;
                            double x1, y1, z1, x2, y2, z2;

                            gluUnProject(winX, winY, winZ1, modleview, projection, viewport, &x1, &y1, &z1);
                            gluUnProject(winX, winY, winZ2, modleview, projection, viewport, &x2, &y2, &z2);
                            Handle(Geom_Line) aLine = new Geom_Line(gp_Pnt(x1, y1, z1), gp_Dir(x2 - x1, y2 - y1, z2 - z1));



                            GeomAPI_ProjectPointOnCurve Proj(Pnt, aLine);
                            Standard_Integer NumSolutions = Proj.NbPoints();
                            gp_Pnt pnt2 = Proj.NearestPoint();
                            Standard_Real D = Proj.LowerDistance();
                            if(minDistance > D)
                            {
                                minDistance = D;
                                aio = new AIS_Shape(vertex);
                            }
                        }
                        myContext->Hilight(aio);
                        setPntInfo(aio);
                    }
                }
                // 取任意点
                else if(mySelectType == AnyPntSelectType){
                    GLint viewport[4];
                    GLdouble modleview[16], projection[16];


                    Handle(Visual3d_View) FromView = myView->View() ;
                    Visual3d_ViewOrientation viewOrientation = myView->ViewOrientation();
                    //Handle_TColStd_HArray2OfReal viewMatrix;
                    //const Handle(TColStd_HArray2OfReal)& Mat
                    //viewOrientation.ViewReferencePlane();
                    //viewOrientation.ViewReferencePoint();
                    //viewOrientation.ViewReferenceUp();

                    //viewOrientation.SetCustomModelViewMatrix(viewMatrix);
                    //const TColStd_Array2OfReal & theOrientation = viewMatrix->Array2();

                    //Graphic3d_CView ACView;
                    GLdouble aOrientationMatrix[16];
                    //GLdouble aViewMappingMatrix[16];
                    //TColStd_Array2OfReal theOrientation (0, 3, 0, 3);
                    //TColStd_Array2OfReal theViewMapping (0, 3, 0, 3);
//                    for (int j = 0; j < 4; ++j)
//                      for (int i = 0; i < 4; ++i)
//                      {
//                        aOrientationMatrix [4 * j + i] = theOrientation (i, j);
//                        //aViewMappingMatrix [4 * j + i] = theViewMapping (i, j);
//                      }

                    //myDevice->InquireMat(ACView,theOrientation,theViewMapping);

                    //TColStd_Array2OfReal aOrientation (0, 3, 0, 3);
                    //viewOrientation.SetCustomModelViewMatrix (aOrientation);

                       glPushMatrix();
                       //glLoadIdentity();

                       //glLoadMatrixd(aOrientationMatrix);
                       glGetIntegerv(GL_VIEWPORT, viewport);
                       glGetDoublev(GL_MODELVIEW_MATRIX, modleview);
                       glGetDoublev(GL_PROJECTION_MATRIX, projection);
                       glPopMatrix();

                       GLdouble winX = e->x();
                       GLdouble winY = viewport[3] - e->y();
                       GLdouble winZ1 = 0;
                       GLdouble winZ2 = 10;
                       double x1, y1, z1, x2, y2, z2;

                       gluUnProject(winX, winY, winZ1, modleview, projection, viewport, &x1, &y1, &z1);
                       gluUnProject(winX, winY, winZ2, modleview, projection, viewport, &x2, &y2, &z2);

                       Handle(Geom_Line) aLine = new Geom_Line(gp_Pnt(x1, y1, z1), gp_Dir(x2 - x1, y2 - y1, z2 - z1));

                       TopoDS_Face face = TopoDS::Face(selectShape);
                       currentShape = selectShape;

                       Handle_Geom_Surface  currentSur;
//                       TopExp_Explorer Ex;
//                       Ex.Init(currentShape,TopAbs_FACE);
//                       TopoDS_Face currentVt = TopoDS::Face(Ex.Current());
                       currentSur = BRep_Tool::Surface(face);

                       GeomAPI_IntCS intCS(aLine, currentSur);
                       gp_Pnt aPnt;
                       TopoDS_Vertex pnt1;
                       if(intCS.IsDone())
                       {
                         qDebug()<<intCS.NbPoints();
                         if(intCS.NbPoints()==1)
                         {
                             aPnt = intCS.Point(1);
                             pnt1 = BRepBuilderAPI_MakeVertex(aPnt);
                             Handle(AIS_Shape) aio = new AIS_Shape(pnt1);
                             myContext->Hilight(aio);
                             setPntInfo(aio);
                             qDebug()<<"找到交点";

                         }else if(intCS.NbPoints()==2)
                         {
                             aPnt = intCS.Point(2);
                             Standard_Real distance2 = minDistanceAPointandSurfaceVertex(selectShape, aPnt);

                             aPnt = intCS.Point(1);
                             Standard_Real distance1 = minDistanceAPointandSurfaceVertex(selectShape, aPnt);
                             if(distance1>distance2)
                             {
                                 aPnt = intCS.Point(2);
                                 pnt1 = BRepBuilderAPI_MakeVertex(aPnt);
                                 Handle(AIS_Shape) aio = new AIS_Shape(pnt1);
                                 myContext->Hilight(aio);
                                 setPntInfo(aio);

                             }else
                             {
                                 aPnt = intCS.Point(1);
                                 pnt1 = BRepBuilderAPI_MakeVertex(aPnt);
                                 Handle(AIS_Shape) aio = new AIS_Shape(pnt1);
                                 myContext->Hilight(aio);
                                 setPntInfo(aio);
                             }


                         }else
                         {
                             qDebug()<<"未找到交点";
                             return;
                         }

                         currentShape = pnt1;
                         qDebug()<<"找到交点";


                       }else
                       {
                           qDebug()<<"未找到交点";
                       }

                }
                // 取边界点
                else if(mySelectType == EdgePntSelectType){
                    // 判断是否是边
                    if(selectShape.ShapeType() == TopAbs_EDGE){
                        TopoDS_Edge edge =TopoDS::Edge(selectShape);

                        TopExp_Explorer anExp(edge, TopAbs_VERTEX);
                        int point_count = 0;
                        Standard_Real x_sum=0;
                        Standard_Real y_sum=0;
                        Standard_Real z_sum=0;
                        gp_Pnt Pnt;
                        for( ; anExp.More(); anExp.Next()){
                            const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
                            Pnt = BRep_Tool::Pnt(vertex);
                            x_sum += Pnt.X();
                            y_sum += Pnt.Y();
                            z_sum += Pnt.Z();
                            point_count++;
                        }
                        Pnt.SetX(x_sum/point_count);
                        Pnt.SetY(y_sum/point_count);
                        Pnt.SetZ(z_sum/point_count);
                        //myContext->Hilight(aio);
                    }
                }
                // 取圆心
                else if(mySelectType == CircleCenterPntSelectType){
                    // 判断是否是边
                    if(selectShape.ShapeType() == TopAbs_EDGE)
                    {

                        TopoDS_Edge edge =TopoDS::Edge(selectShape);
                        TopLoc_Location L;
                        Standard_Real First;
                        Standard_Real Last;
                        Handle_Geom_Curve aGeomcurve = BRep_Tool::Curve(edge, L, First, Last);
                        Handle (Geom_Circle) aCircle = Handle (Geom_Circle)::DownCast(aGeomcurve);

                        gp_Pnt Pnt;
                        if (!aCircle.IsNull()) {
                            Pnt = aCircle->Location();
                            TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(Pnt);
                            Handle(AIS_Shape) aio = new AIS_Shape(pnt1);
                            myContext->Hilight(aio);
                            setPntInfo(aio);
                        }
                    }
                }
            }

        }
    }
}

//TopoDS_Shape SelectTool::TrimNurbsToSTL( const TopoDS_Face& aTrimTopFace,Standard_Real precision )
//{
//    TopoDS_Face TopoFace = BRepBuilderAPI_MakeFace(aTrimTopFace).Face();
//    Handle_StlMesh_Mesh aSTLMesh = new StlMesh_Mesh();
//    StlTransfer::BuildIncrementalMesh(TopoFace, precision, Standard_False, aSTLMesh);

//    Standard_Integer NumberDomains = aSTLMesh->NbDomains();
//    Standard_Integer iND;
//    gp_XYZ p1, p2, p3;
//    TopoDS_Vertex Vertex1, Vertex2, Vertex3;
//    TopoDS_Face AktFace;
//    TopoDS_Wire AktWire;
//    BRepBuilderAPI_Sewing aSewingTool;
//    Standard_Real x1, y1, z1;
//    Standard_Real x2, y2, z2;
//    Standard_Real x3, y3, z3;

//    aSewingTool.Init(CAD_NURBS_STL_TOLERANCE,Standard_True);

//    TopoDS_Compound aComp;
//    BRep_Builder BuildTool;
//    BuildTool.MakeCompound( aComp );

//    StlMesh_MeshExplorer aMExp (aSTLMesh);

//    for (iND=1;iND<=NumberDomains;iND++)
//    {
//        for (aMExp.InitTriangle (iND); aMExp.MoreTriangle (); aMExp.NextTriangle ())
//        {
//            aMExp.TriangleVertices (x1,y1,z1,x2,y2,z2,x3,y3,z3);
//            p1.SetCoord(x1,y1,z1);
//            p2.SetCoord(x2,y2,z2);
//            p3.SetCoord(x3,y3,z3);

//            if ((!(p1.IsEqual(p2,0.0))) && (!(p1.IsEqual(p3,0.0))))
//            {
//                Vertex1 = BRepBuilderAPI_MakeVertex(p1);
//                Vertex2 = BRepBuilderAPI_MakeVertex(p2);
//                Vertex3 = BRepBuilderAPI_MakeVertex(p3);

//                AktWire = BRepBuilderAPI_MakePolygon( Vertex1, Vertex2, Vertex3, Standard_True);

//                if( !AktWire.IsNull())
//                {
//                    AktFace = BRepBuilderAPI_MakeFace( AktWire);
//                    if(!AktFace.IsNull())
//                        BuildTool.Add( aComp, AktFace );
//                }
//            }
//        }
//    }

//    aSTLMesh->Clear();

//    aSewingTool.Load( aComp );
//    aSewingTool.Perform();
//    TopoDS_Shape aShape = aSewingTool.SewedShape();
//    if ( aShape.IsNull() )
//        aShape = aComp;

//    return aShape;
//}
Standard_Real minDistanceAPointandSurfaceVertex(const TopoDS_Shape& S, gp_Pnt& aPnt)
{
    //根据距离取距离顶点最近的点
    TopExp_Explorer Ex;
    Standard_Real MinDistance = 1e+10;
    Standard_Real CurDistance = 0.0;
    gp_Pnt current,ResPnt;

    for (Ex.Init(S,TopAbs_VERTEX); Ex.More(); Ex.Next())
    {
        TopoDS_Vertex currentVt = TopoDS::Vertex(Ex.Current());
        current = BRep_Tool::Pnt(currentVt);
        CurDistance = current.Distance(aPnt);
        if(CurDistance<MinDistance)
        {
            MinDistance = CurDistance;
            ResPnt = current;
        }
    }
    return MinDistance;
}
bool SelectTool::findAPointInTheSurface(const TopoDS_Shape& S, gp_Pnt& P)
{
    //点是否在面上
    //TopoDS_Face face = TopoDS::Face(S);
//    gp_Pnt aPnt1(100/2.,2,11);
//    Standard_Boolean aState1 = BRepClass3d_SolidExplorer::FindAPointInTheFace(face, P);
//    if(!aState1)
//    {
//        qDebug()<<"FindAPointInTheFace交点不在面上";
//    }

//                             BRepClass3d_SolidClassifier aSC(selectShape);
//                             aSC.Perform(aPnt1, 1.0E-6);
//                             TopAbs_State aState = aSC.State();
//                             if(aState == TopAbs_OUT || aState == TopAbs_UNKNOWN)
//                             {
//                                 qDebug()<<"Perform交点不在面上";
//                             }

    //TopoDS_Face TopoFace = BRepBuilderAPI_MakeFace(face).Face();
    //gp_Pnt aPnt1(100/2.,2,11);
//    Standard_Boolean aState1 = BRepClass3d_SolidExplorer::FindAPointInTheFace(face, P);
//    if(!aState1)
//    {
//        qDebug()<<"FindAPointInTheFace交点不在面上";
//    }


    BRepClass3d_SolidClassifier aSC(S);
    aSC.Perform(P, 1000);
    TopAbs_State aState = aSC.State();
    if(aState == TopAbs_OUT || aState == TopAbs_UNKNOWN)
    {
        qDebug()<<"Perform交点不在面上";
        return 0;
    }else
    {
        return 1;
    }
}
// 设置点的数据
void SelectTool::setPntInfo(Handle(AIS_Shape) aio){
    TopoDS_Shape selectObj = aio->Shape();
    if(!selectObj.IsNull()){
        TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
        if(!vertex.IsNull()){
            gp_Pnt pnt = BRep_Tool::Pnt(vertex);
            ui->PntX->setText(QString("%1").arg(pnt.X()));
            ui->PntY->setText(QString("%1").arg(pnt.Y()));
            ui->PntZ->setText(QString("%1").arg(pnt.Z()));
            emit selectPoint(pnt);
        }
    }

}

// 设置边的数据
void SelectTool::setEdgeInfo(TopoDS_Edge edge){
    TopExp_Explorer anExp(edge, TopAbs_VERTEX);
    int i = 0;
    for(; anExp.More() ; anExp.Next()){
        if(i == 0){
            const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
            gp_Pnt pnt =BRep_Tool::Pnt(vertex);
            ui->EdgeStartX->setText(QString("%1").arg(pnt.X()));
            ui->EdgeStartY->setText(QString("%1").arg(pnt.Y()));
            ui->EdgeStartZ->setText(QString("%1").arg(pnt.Z()));
            i++;
        }else{
            const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
            gp_Pnt pnt =BRep_Tool::Pnt(vertex);
            ui->EdgeEndX->setText(QString("%1").arg(pnt.X()));
            ui->EdgeEndY->setText(QString("%1").arg(pnt.Y()));
            ui->EdgeEndZ->setText(QString("%1").arg(pnt.Z()));
        }
    }
}


bool SelectTool::pntOnEdge(gp_Pnt p1 , gp_Pnt p2){
    Standard_Real rtl = 1.0E-6;
    if(abs(p1.X()-p2.X()) < rtl && abs(p1.Y() - p2.Y()) < rtl && abs(p1.Z() - p2.Z()) < rtl){
        return true;
    }
    return false;
}

// 曲线设置数据
void SelectTool::setWireInfo(TopoDS_Wire wire){
    TopExp_Explorer anExp(wire, TopAbs_VERTEX);
    gp_Pnt p;
    bool isStart = true;
    for(; anExp.More() ; anExp.Next()){
        if(isStart){
            const TopoDS_Vertex vertex = TopoDS::Vertex(anExp.Current());
            p = BRep_Tool::Pnt(vertex);
            ui->WireStartX->setText(QString("%1").arg(p.X()));
            ui->WireStartY->setText(QString("%1").arg(p.Y()));
            ui->WireStartZ->setText(QString("%1").arg(p.Z()));
            isStart = false;
        }else{
            const TopoDS_Vertex vertex = TopoDS::Vertex(anExp.Current());
            p = BRep_Tool::Pnt(vertex);
            ui->WireEndX->setText(QString("%1").arg(p.X()));
            ui->WireEndY->setText(QString("%1").arg(p.Y()));
            ui->WireEndZ->setText(QString("%1").arg(p.Z()));
        }

    }
}

// 曲线测试方法
void SelectTool::setWireInfo(TopoDS_Wire wire , TopoDS_Vertex s , TopoDS_Vertex m , TopoDS_Vertex e){
    TopExp_Explorer anExp(wire, TopAbs_EDGE);
    QList<TopoDS_Edge> leftEdges;
    QList<wirecurve> edgeList;
    wirecurve startCurve;
    gp_Pnt startPnt;
    gp_Pnt p1 = BRep_Tool::Pnt(s) ,
           p2 = BRep_Tool::Pnt(m),
           p3 = BRep_Tool::Pnt(e);
    for(; anExp.More() ; anExp.Next()){
        const TopoDS_Edge edge = TopoDS::Edge(anExp.Current());
        Standard_Real first,last;
        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
        gp_Pnt fristPnt = m_curve->Value(first);
        gp_Pnt lastPnt = m_curve->Value(last);
        wirecurve wire;
        wire.edge = edge;
        wire.endPnt = lastPnt;
        wire.startPnt = fristPnt;
        // 如果这条边上有起始点， 那么记录一下这条边
        if(pntOnEdge(fristPnt , p1)){
            startCurve = wire;
            startPnt = lastPnt;
        }
        if(pntOnEdge(lastPnt , p1)){
            startCurve = wire;
            startPnt = fristPnt;
        }
        edgeList.append(wire);
    }
    QList<wirecurve> copyEdgeList;
    copyEdgeList.append(edgeList);
    // 把起始点去掉，然后
    leftEdges.append(startCurve.edge);
    copyEdgeList.removeOne(startCurve);
    leftEdges = getWireEdge(startPnt , p2 , p3 , copyEdgeList ,edgeList, false , false , leftEdges);

    BRepBuilderAPI_MakeWire newWireMake;
    for(int i = 0 ; i < leftEdges.size() ; i++){
        newWireMake.Add(leftEdges.at(i));
    }
    newWireMake.Build();
    TopoDS_Wire newWire = newWireMake.Wire();
    selectWire(newWire);
    //Handle(AIS_Shape) shape = new AIS_Shape(newWire);
    //myContext->Display(shape);
}


// 曲线切点开始点数据
void SelectTool::setStartCutInfo(){
    if(!startVertex.IsNull()){
        gp_Pnt p = BRep_Tool::Pnt(startVertex);
        ui->WireCutStartX->setText(QString("%1").arg(p.X()));
        ui->WireCutStartY->setText(QString("%1").arg(p.Y()));
        ui->WireCutStartZ->setText(QString("%1").arg(p.Z()));
    }
}

// 曲线切点中间点数据
void SelectTool::setMiddleCutInfo(){
    if(!middleVertex.IsNull()){
        gp_Pnt p = BRep_Tool::Pnt(middleVertex);
        ui->WireCutMiddleX->setText(QString("%1").arg(p.X()));
        ui->WireCutMiddleY->setText(QString("%1").arg(p.Y()));
        ui->WireCutMiddleZ->setText(QString("%1").arg(p.Z()));
    }
}

// 曲线切点结束点数据
void SelectTool::setEndCutInfo(){
    if(!endVertex.IsNull()){
        gp_Pnt p = BRep_Tool::Pnt(endVertex);
        ui->WireCutEndX->setText(QString("%1").arg(p.X()));
        ui->WireCutEndY->setText(QString("%1").arg(p.Y()));
        ui->WireCutEndZ->setText(QString("%1").arg(p.Z()));
    }
}

// 设置多边信息
void SelectTool::setMoreEdgeInfo(TopoDS_Edge edge){
    if(!edge.IsNull()){
        Standard_Real first,last;
        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
        gp_Pnt fristPnt = m_curve->Value(first);
        gp_Pnt lastPnt = m_curve->Value(last);
        ui->EdgesStartXYZ->setText(QString("X:%1 , Y:%2 , Z:%3").arg(fristPnt.X()).arg(fristPnt.Y()).arg(fristPnt.Z()));
        ui->EdgesEndXYZ->setText(QString("X:%1 , Y:%2 , Z:%3").arg(lastPnt.X()).arg(lastPnt.Y()).arg(lastPnt.Z()));


    }
}




QList<TopoDS_Edge> SelectTool::getWireEdge(gp_Pnt currentPnt , gp_Pnt m , gp_Pnt e , QList<wirecurve> edgeList , QList<wirecurve> allList , bool isMiddle , bool isEnd , QList<TopoDS_Edge> returnList){
    for(int i = 0 ; i < edgeList.count() ; i++){
        wirecurve currentWire = edgeList.at(i);
        // 开始点在这条线上
        if(pntOnEdge(currentWire.startPnt , currentPnt)){
            gp_Pnt nextPnt = currentWire.endPnt;
            //判断是否有结束点 或者是否有中间点
            if(pntOnEdge(currentWire.endPnt , e) || pntOnEdge(currentWire.startPnt,e)){
                isEnd = true;
            }
            if(pntOnEdge(currentWire.endPnt , m) || pntOnEdge(currentWire.startPnt,m
                                                            )){
                isEnd = true;
            }
            returnList.append(currentWire.edge);
            edgeList.removeOne(currentWire);
            // 先找到了结束点 没有找到中间点
            // 那么找到的全不要,其他的就是需要的
            if(!isMiddle && isEnd){
                returnList.clear();
                for(int j = 0 ; j < edgeList.size() ; j++){
                    returnList.append(edgeList.at(j).edge);
                }
                return returnList;
            }
            // 如果扫到了 M 点 又扫到了E点
            if(isMiddle && isEnd){
                return returnList;
            }
            return getWireEdge(nextPnt , m , e , edgeList , allList, isMiddle , isEnd , returnList);
        }
        // 开始点在这条线上
        if(pntOnEdge(currentWire.endPnt , currentPnt)){
            gp_Pnt nextPnt = currentWire.startPnt;
            if(pntOnEdge(currentWire.endPnt , e) || pntOnEdge(currentWire.startPnt,e)){
                isEnd = true;
            }
            if(pntOnEdge(currentWire.endPnt , m) || pntOnEdge(currentWire.startPnt,m
                                                            )){
                isEnd = true;
            }
            returnList.append(currentWire.edge);
            edgeList.removeOne(currentWire);
            // 先找到了结束点 没有找到中间点
            // 那么找到的全不要,其他的就是需要的
            if(!isMiddle && isEnd){
                returnList.clear();
                for(int j = 0 ; j < edgeList.size() ; j++){
                    returnList.append(edgeList.at(j).edge);
                }
                return returnList;
            }
            // 如果扫到了 M 点 又扫到了E点
            if(isMiddle && isEnd){
                return returnList;
            }
            return getWireEdge(nextPnt , m , e , edgeList ,allList , isMiddle , isEnd , returnList);
        }
    }
    return returnList;
}


// 记录原始选取类型
void SelectTool::clearOldActivatedStandard(bool isSave)
{
    myContext->DeactivateStandardMode(TopAbs_VERTEX);
    myContext->DeactivateStandardMode(TopAbs_EDGE);
    myContext->DeactivateStandardMode(TopAbs_WIRE);
    myContext->DeactivateStandardMode(TopAbs_FACE);
    myContext->DeactivateStandardMode(TopAbs_SHELL);
    myContext->DeactivateStandardMode(TopAbs_SOLID);
    myContext->DeactivateStandardMode(TopAbs_COMPOUND);
    myContext->DeactivateStandardMode(TopAbs_SHAPE);
}

// 选点模式
void SelectTool::on_pushButton_3_clicked()
{
    this->setWindowTitle("选取点");
    currentShape.Nullify();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    mySelectType = PntSelectType;
    clearOldActivatedStandard();
    // 设置选点模式
    myContext->ActivateStandardMode(TopAbs_VERTEX);
    clearValue();
//    this->setWindowTitle(tr("选取点"));

}
// 选中点模式
void SelectTool::on_pushButton_25_clicked()
{
    this->setWindowTitle("选取中点");
    currentShape.Nullify();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    mySelectType = MiddlePntSelectType;
    clearOldActivatedStandard();
    // 设置选点模式
    myContext->ActivateStandardMode(TopAbs_EDGE);
    clearValue();
//    this->setWindowTitle(tr("选取点"));

}
// 选端点模式
void SelectTool::on_pushButton_26_clicked()
{
    this->setWindowTitle("选取端点");
    currentShape.Nullify();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    mySelectType = EndPntSelectType;
    clearOldActivatedStandard();
    // 设置选点模式
    myContext->ActivateStandardMode(TopAbs_EDGE);
    clearValue();
//    this->setWindowTitle(tr("选取点"));

}
// 选任意点模式
void SelectTool::on_pushButton_27_clicked()
{
    this->setWindowTitle("选取任意点");
    currentShape.Nullify();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    mySelectType = AnyPntSelectType;
    clearOldActivatedStandard();
    // 设置选点模式
    myContext->ActivateStandardMode(TopAbs_FACE);
    clearValue();
//    this->setWindowTitle(tr("选取点"));

}

// 选取圆心
void SelectTool::on_pushButton_28_clicked()
{
    this->setWindowTitle("选取圆心");
    currentShape.Nullify();
    ui->stackedWidget->setCurrentWidget(ui->page_2);
    mySelectType = CircleCenterPntSelectType;
    clearOldActivatedStandard();
    // 设置选点模式
    myContext->ActivateStandardMode(TopAbs_EDGE);
    clearValue();
//    this->setWindowTitle(tr("选取点"));

}
// 选边模式
void SelectTool::on_pushButton_4_clicked()
{
    clearValue();
    currentShape.Nullify();
    ui->stackedWidget->setCurrentWidget(ui->page_3);
    mySelectType = LinSelectType;
    clearOldActivatedStandard();

    // 设置选边模式
    myContext->ActivateStandardMode(TopAbs_EDGE);
    this->setWindowTitle(tr("选取边"));
}

// 选曲线模式
void SelectTool::on_pushButton_5_clicked()
{
    clearValue();
    currentShape.Nullify();
    startVertex.Nullify();
    middleVertex.Nullify();
    endVertex.Nullify();
    this->setWindowTitle(tr("选取曲线"));
    ui->stackedWidget->setCurrentWidget(ui->page_4);
    mySelectType = NurbsSelectType;
    clearOldActivatedStandard();

    // 设置选边模式
    myContext->ActivateStandardMode(TopAbs_WIRE);
}


// 多选边
void SelectTool::on_pushButton_16_clicked()
{

    clearValue();
    clearMoreEdgesSelect();
    this->setWindowTitle(tr("选取边"));
    ui->stackedWidget->setCurrentWidget(ui->page_8);
    mySelectType = MoreEdgeSelectType;
    clearOldActivatedStandard();

    // 设置选边模式
    myContext->ActivateStandardMode(TopAbs_EDGE);
}

// 选点确认
void SelectTool::on_pushButton_clicked()
{
    if(!currentShape.IsNull()){
        TopoDS_Vertex vertex  = TopoDS::Vertex(currentShape);
        if(!vertex.IsNull()){
            gp_Pnt pnt = BRep_Tool::Pnt(vertex);
            emit selectPoint(pnt);
            this->setWindowTitle(tr("请选择"));
            ui->stackedWidget->setCurrentWidget(ui->page_1);
        }
    }
}

// 选点取消
void SelectTool::on_pushButton_2_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}

// 选边确认
void SelectTool::on_pushButton_6_clicked()
{
    if(!currentShape.IsNull()){
        TopoDS_Edge edge =TopoDS::Edge(currentShape);
        if(!edge.IsNull()){
            emit selectEdge(edge);
            this->setWindowTitle(tr("请选择"));
            ui->stackedWidget->setCurrentWidget(ui->page_1);
//            getEdgeType(edge);
        }
    }
}

// 选边取消
void SelectTool::on_pushButton_7_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}

// 选曲线取消
void SelectTool::on_pushButton_9_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}

// 选曲线下一步
void SelectTool::on_pushButton_8_clicked()
{
    qDebug() << currentShape.IsNull();
    if(!currentShape.IsNull()){
        // 切换到选点的模式
        this->setWindowTitle(tr("请选择起始点"));
        ui->stackedWidget->setCurrentWidget(ui->page_5);
        clearOldActivatedStandard();
        // 设置选边模式
        myContext->ActivateStandardMode(TopAbs_VERTEX);
    }else{
        QMessageBox::information(this , tr("错误") , "请选择曲线");
    }
}


// 选曲线中的切点 第一个取消
void SelectTool::on_pushButton_11_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}

// 选曲线切点 第一个
void SelectTool::on_pushButton_10_clicked()
{
    if(startVertex.IsNull()){
        QMessageBox::information(this , tr("错误") , "请选择起始点");
        return;
    }
    this->setWindowTitle(tr("请选择中间点"));
    ui->stackedWidget->setCurrentWidget(ui->page_6);
}

// 选曲线中的切点 第二个 取消
void SelectTool::on_pushButton_13_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}

// 选曲线的切点 第二个
void SelectTool::on_pushButton_12_clicked()
{
    if(middleVertex.IsNull()){
        QMessageBox::information(this , tr("错误") , "请选择中间点");
        return;
    }
    this->setWindowTitle(tr("请选择结束点"));
    ui->stackedWidget->setCurrentWidget(ui->page_7);
}


// 选曲线中的切点 第三个
void SelectTool::on_pushButton_14_clicked()
{
    if(endVertex.IsNull()){
        QMessageBox::information(this , tr("错误") , "请选择结束点");
        return;
    }
    //TopoDS_Wire wire , TopoDS_Vertex s , TopoDS_Vertex m , TopoDS_Vertex e
    TopoDS_Wire wire = TopoDS::Wire(currentShape);
    setWireInfo(wire , startVertex , middleVertex , endVertex);
    ui->stackedWidget->setCurrentWidget(ui->page_1);
}

// 选曲线中的切点 第三个取消
void SelectTool::on_pushButton_15_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}


// 添加一条边
void SelectTool::addMoreEdgesInfo(){
    if(currentShape.IsNull()){
        QMessageBox::information(this , tr("提示") , tr("请选择边"));
        return;
    }
    TopoDS_Edge edge = TopoDS::Edge(currentShape);
    for(int i = 0 ; i < moreEdgeSelect.count() ;i++){
        if(moreEdgeSelect.at(i)->Shape().IsEqual(edge)){
            QMessageBox::information(this , tr("提示") , tr("这条边已存在"));
            return;
        }
    }
    Handle(AIS_Shape) ashape = new AIS_Shape(edge);
    moreEdgeSelect.append(ashape);
    refreshMoreSelectList();
}

// 清空数据
void SelectTool::clearMoreEdgesSelect(){
    for(int i = 0 ; i < moreEdgeSelect.count() ; i++){
        myContext->Unhilight(moreEdgeSelect.at(i));
    }
    moreEdgeSelect.clear();
}

// 选多边删除操作
void SelectTool::on_pushButton_20_clicked()
{
    if(ui->listWidget->SelectColumns){
        for(int i = 0 ;  i < moreEdgeSelect.length() ; i++){
            myContext->Unhilight(moreEdgeSelect.at(i));
        }
        QList<QListWidgetItem*> list =  ui->listWidget->selectedItems();
        for(int i = 0 ; i < list.count() ; i++){
//            Handle(AIS_Shape) aio = new AIS_Shape(moreEdgeSelect.at(ui->listWidget->row(list.at(i))));
//            myContext->Unhilight(aio , (i == list.count() -1));
            moreEdgeSelect.removeAt(ui->listWidget->row(list.at(i)));
        }
        refreshMoreSelectList();
    }
}

// 选多边确定
void SelectTool::on_pushButton_18_clicked()
{
    if(moreEdgeSelect.count()){
        QList<TopoDS_Edge> edges ;
        for(int i = 0  ; i < moreEdgeSelect.count() ;i++){
            TopoDS_Edge edge = TopoDS::Edge(moreEdgeSelect.at(i)->Shape());
            edges.append(edge);
        }
        emit selectEdges(edges);
        this->setWindowTitle(tr("请选择"));
        ui->stackedWidget->setCurrentWidget(ui->page_1);
        clearMoreEdgesSelect();
    }
}

// 选多边取消
void SelectTool::on_pushButton_19_clicked()
{
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}

// 选多边刷新操作
void SelectTool::refreshMoreSelectList(){
    ui->listWidget->clear();
    for(int i = 0 ; i < moreEdgeSelect.count() ; i++){
        TopoDS_Edge edge = TopoDS::Edge(moreEdgeSelect.at(i)->Shape());

        Standard_Real first,last;
        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
        gp_Pnt fristPnt = m_curve->Value(first);
        gp_Pnt lastPnt = m_curve->Value(last);
        ui->listWidget->addItem(QString("X1:%1,Y1:%2,Z1:%3  X2:%4,Y2:%5,Z2:%6").arg(fristPnt.X()).arg(fristPnt.Y()).arg(fristPnt.Z()).arg(lastPnt.X()).arg(lastPnt.Y()).arg(lastPnt.Z()));

        // 将之前选过的高亮
//        Handle(AIS_Shape) aio = new AIS_Shape(moreEdgeSelect.at(i));

        myContext->Hilight(moreEdgeSelect.at(i) , (i == moreEdgeSelect.count() - 1));
    }
    if(moreEdgeSelect.count()){
        ui->listWidget->item(moreEdgeSelect.count() - 1)->setSelected(true);
    }
}


QString SelectTool::getEdgeType(TopoDS_Edge edge){
    Standard_Real first,last;
    Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);

    Handle(Standard_Type) type = m_curve->DynamicType();
    return QString(type->Name());
}

//QList<bsplines> SelectTool::wireToNurbs(TopoDS_Wire wire){
//    TopExp_Explorer anExp(wire, TopAbs_EDGE);
//    QList<bsplines> edges;
//    for(; anExp.More(); anExp.Next()){
//        const TopoDS_Edge edge = TopoDS::Edge(anExp.Current());
//        if(edge.IsNull()){
//            continue;
//        }
//        if(getEdgeType(edge) == "Geom_Line"){
//            continue;
//        }
//        Standard_Real first,last;
//        Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
//        if(m_curve != NULL){
//            qDebug() << "CurveToBSplineCurve";
//            Handle(Geom_BSplineCurve) bs =  GeomConvert::CurveToBSplineCurve(m_curve);
//            if(bs != NULL){
//                qDebug() << "append bspline";
//                edges.append(convertToNurbs(bs));
//            }
//        }
//    }
//    return edges;
//}

//bsplines SelectTool::convertToNurbs(Handle_Geom_BSplineCurve aBSpline){
//    bsplines line;
//    Standard_Integer poles_len = aBSpline->NbPoles() ,
//                     knots_len = aBSpline->NbKnots();
//    line.polesLen = poles_len;
//    line.knotsLen = knots_len;
//    // 顶点
//    line.poles = (double **)malloc(sizeof(double*)*poles_len);
//    for(int i = 0 ;  i < poles_len ; i++){
//        line.poles[i] = (double*)malloc(sizeof(double) * 3);
//    }
//    for(int i = 0 ; i < poles_len ; i++){
//        gp_Pnt p = aBSpline->Pole(i+1);
//        line.poles[i][0] = p.X();
//        line.poles[i][1] = p.Y();
//        line.poles[i][2] = p.Z();
//    }
//    // 节点
//    line.knots = (double*)malloc(sizeof(double) *knots_len);
//    for(int i = 0 ; i < knots_len ; i++){
//        line.knots[i] = aBSpline->Weight(i+1);
//    }
//    line.multiplicities = (int*)malloc(sizeof(int)*knots_len);
//    for(int i = 0; i < knots_len ; i++){
//        line.multiplicities[i] = aBSpline->Multiplicity(i+1);
//    }
//    line.multipLen = knots_len;
//    line.degree = aBSpline->Degree();
//    line.periodic = aBSpline->IsPeriodic();
//    line.rational = aBSpline->IsRational();
//    return line;
//}

//void SelectTool::getNurbsData(Handle_Geom_BSplineCurve aBSpline){
//    QString s;
//    Standard_Integer poles_len = aBSpline->NbPoles() ,
//                     knots_len = aBSpline->NbKnots();


//    s.append("Poles:").append("\n");
//    for(int i = 0 ; i < poles_len ; i++){
//        gp_Pnt p = aBSpline->Pole(i+1);
//        s.append(QString("{%1,%2,%3}").arg( p.X()).arg(p.Y()).arg(p.Z())).append("\n");
//    }
//    s.append("Poles End").append("\n");

//    s.append("Weights:").append("\n");
//    for(int i = 0 ; i < poles_len ; i++){
//        Standard_Real k = aBSpline->Weight(i+1);
//        s.append(QString("%1").arg(k)).append("\n");
//    }
//    s.append("Weights End").append("\n");


//    s.append("Knots:").append("\n");
//    for(int i = 0 ; i < knots_len ; i++){
//        Standard_Real k = aBSpline->Knot(i+1);
//        if(i > 0){
//            s.append(",").append("\n");
//        }
//        s.append(QString("%1").arg(k)).append("\n");
//    }
//    s.append("Knots End").append("\n");



//    s.append("Multiplicities:").append("\n");
//    for(int i = 0 ; i < knots_len ; i++){
//        Standard_Integer k = aBSpline->Multiplicity(i+1);
//        s.append(QString("%1").arg(k)).append("\n");
//    }
//    s.append("Multiplicities End").append("\n");

//    s.append("Degree:").append("\n");
//    s.append(QString("%1").arg(aBSpline->Degree())).append("\n");
//    s.append("Degree End").append("\n");

//    s.append("Periodic:").append("\n");
//    s.append(QString("%1").arg(aBSpline->IsPeriodic())).append("\n");
//    s.append("Periodic End").append("\n");


//    s.append("Rational").append("\n");
//    s.append(QString("%1").arg(aBSpline->IsRational())).append("\n");
//    s.append("Rational End").append("\n");

//    QMessageBox::information(this ,"11111" , s);
//}



void SelectTool::on_pushButton_17_clicked()
{
    this->resize(800,600);
    ui->page->resize(800,600);

    ui->stackedWidget->setCurrentWidget(ui->page);
}

void SelectTool::on_pushButton_22_clicked()
{
    this->resize(197,458);
    ui->page->resize(197,600);
    this->setWindowTitle(tr("请选择"));
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    mySelectType = DefaultSelectType;
}
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QDebug>
int myRobot=0;
QString robotName="请选择";
void SelectTool::on_pushButton_21_clicked()
{

    bool tranFlag = true;
    QSqlQuery query;




    myRobot=ui->comboBox->currentIndex();
    robotName=ui->comboBox->currentText();
    if(robotName=="请选择")
    {
        QMessageBox::about(this,"提示",robotName);
        return;
    }


    query.exec(QString("select id from RobotSetting where id='%1' and solutionId='%2'").arg(robotName).arg(pageStation->currentStationId));
    qDebug()<<"aaa"<<QString("select id from RobotSetting where id='%1' and solutionId='%2'").arg(robotName).arg(pageStation->currentStationId);

    if(query.next())
    {
        qDebug()<<"what ?:"<<query.value(0).toString();
        QString s1="update robotsetting set ";
        s1+="knifeType="+ui->lineEdit->text();
        s1+=",knifeData1="+ui->lineEdit_2->text();
        s1+=",knifeData2="+ui->lineEdit_3->text();
        s1+=",knifeData3="+ui->lineEdit_4->text();
        s1+=",pos1="+ui->lineEdit_5->text();
        s1+=",pos2="+ui->lineEdit_6->text();
        s1+=",pos3="+ui->lineEdit_7->text();
        s1+=",pos4="+ui->lineEdit_8->text();
        s1+=",r="+ui->lineEdit_9->text();
        s1+=",deep="+ui->lineEdit_10->text();
        s1+=",angle ="+ui->lineEdit_11->text();
        s1+=",density="+ui->lineEdit_12->text();
        s1+=",precision1="+ui->lineEdit_13->text();
        s1+=",precision2="+ui->lineEdit_27->text();
        s1+=",flange1="+ui->lineEdit_28->text();
        s1+=",flange2="+ui->lineEdit_29->text();
        s1+=",flange3="+ui->lineEdit_30->text();
        s1+=",attitude1="+ui->lineEdit_31->text();
        s1+=",attitude2="+ui->lineEdit_32->text();
        s1+=",attitude3="+ui->lineEdit_33->text();
        s1+=",attitude4="+ui->lineEdit_34->text();
        s1+=",in1="+ui->lineEdit_35->text();
        s1+=",out1="+ui->lineEdit_36->text();
        s1+=",other1="+ui->lineEdit_37->text();
        s1+=",other2="+ui->lineEdit_38->text();
        s1+=",other3="+ui->lineEdit_39->text();
        s1+=" where id='"+robotName+"' and solutionId='"+pageStation->currentStationId+"'";


        tranFlag= query.exec(s1);

        if(!tranFlag)
           qDebug()<<"update failed";

    }
    else
    {
        QString s2="INSERT INTO RobotSetting VALUES ('";
        s2+=robotName+"'";
        s2+=","+ui->lineEdit->text();
        s2+=","+ui->lineEdit_2->text();
        s2+=","+ui->lineEdit_3->text();
        s2+=","+ui->lineEdit_4->text();
        s2+=","+ui->lineEdit_5->text();
        s2+=","+ui->lineEdit_6->text();
        s2+=","+ui->lineEdit_7->text();
        s2+=","+ui->lineEdit_8->text();
        s2+=","+ui->lineEdit_9->text();
        s2+=","+ui->lineEdit_10->text();
        s2+=","+ui->lineEdit_11->text();
        s2+=","+ui->lineEdit_12->text();
        s2+=","+ui->lineEdit_13->text();
        s2+=","+ui->lineEdit_27->text();
        s2+=","+ui->lineEdit_28->text();
        s2+=","+ui->lineEdit_29->text();
        s2+=","+ui->lineEdit_30->text();
        s2+=","+ui->lineEdit_31->text();
        s2+=","+ui->lineEdit_32->text();
        s2+=","+ui->lineEdit_33->text();
        s2+=","+ui->lineEdit_34->text();
        s2+=","+ui->lineEdit_35->text();
        s2+=","+ui->lineEdit_36->text();
        s2+=","+ui->lineEdit_37->text();
        s2+=","+ui->lineEdit_38->text();
        s2+=","+ui->lineEdit_39->text();
        s2+=",'"+pageStation->currentStationId+"'";
        s2+=")";


       tranFlag= query.exec(s2);

       if(!tranFlag)
       {
          qDebug()<<"add failed";
          qDebug()<<s2;
       }

    }


    on_pushButton_22_clicked();
}

void SelectTool::on_pushButton_23_clicked()
{
    if(moreEdgeSelect.count()){
        QList<TopoDS_Edge> edges ;
        for(int i = 0  ; i < moreEdgeSelect.count() ;i++){
            TopoDS_Edge edge = TopoDS::Edge(moreEdgeSelect.at(i)->Shape());
            edges.append(edge);
        }
        emit selectEdges2(edges);
        this->setWindowTitle(tr("请选择"));
        ui->stackedWidget->setCurrentWidget(ui->page_1);
        clearMoreEdgesSelect();
    }
}

void SelectTool::initParam(QString str)
{
    myRobot=ui->comboBox->currentIndex();
    if(ui->comboBox->currentText()=="请选择")
    {

        return;

    }

    QSqlQuery query;
    int ret=query.exec(QString("select * from RobotSetting where id='%1' and solutionId='%2'").arg(ui->comboBox->currentText()).arg(pageStation->currentStationId));
    if(query.next())
    {
        ui->lineEdit->setText(query.value(1).toString());
        ui->lineEdit_2->setText(query.value(2).toString());
        ui->lineEdit_3->setText(query.value(3).toString());
        ui->lineEdit_4->setText(query.value(4).toString());
        ui->lineEdit_5->setText(query.value(5).toString());
        ui->lineEdit_6->setText(query.value(6).toString());
        ui->lineEdit_7->setText(query.value(7).toString());
        ui->lineEdit_8->setText(query.value(8).toString());
        ui->lineEdit_9->setText(query.value(9).toString());
        ui->lineEdit_10->setText(query.value(10).toString());
        ui->lineEdit_11->setText(query.value(11).toString());
        ui->lineEdit_12->setText(query.value(12).toString());
        ui->lineEdit_13->setText(query.value(13).toString());
        ui->lineEdit_27->setText(query.value(14).toString());
        ui->lineEdit_28->setText(query.value(15).toString());
        ui->lineEdit_29->setText(query.value(16).toString());
        ui->lineEdit_30->setText(query.value(17).toString());
        ui->lineEdit_31->setText(query.value(18).toString());
        ui->lineEdit_32->setText(query.value(19).toString());
        ui->lineEdit_33->setText(query.value(20).toString());
        ui->lineEdit_34->setText(query.value(21).toString());
        ui->lineEdit_35->setText(query.value(22).toString());
        ui->lineEdit_36->setText(query.value(23).toString());
        ui->lineEdit_37->setText(query.value(24).toString());
        ui->lineEdit_38->setText(query.value(25).toString());
        ui->lineEdit_39->setText(query.value(26).toString());

    }
    if(!ret)
        qDebug()<<"query failed";

}

void SelectTool::on_pushButton_24_clicked()
{
    emit setLock(false);
    this->close();
}
