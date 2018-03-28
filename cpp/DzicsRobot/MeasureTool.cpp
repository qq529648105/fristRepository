#include "MeasureTool.h"
#include "ui_MeasureTool.h"
#include "math.h"
#include "AxisObject.h"
#include <QMouseEvent>

#include <TopoDS_Vertex.hxx>
#include <Geom_Point.hxx>
#include <Geom_CartesianPoint.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS.hxx>
#include <BRep_PointOnCurve.hxx>
#include <TopExp_Explorer.hxx>
#include <TopoDS_Shape.hxx>
#include <AIS_LocalContext.hxx>
#include <TColStd_ListIteratorOfListOfInteger.hxx>
#include <Geom_Conic.hxx>
#include <TopoDS_Edge.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRepBuilderAPI_MakeShape.hxx>
#include <Geom_Line.hxx>
#include <gp_Lin.hxx>
#include <Geom_Plane.hxx>
#include <gp_Pln.hxx>
#include <AIS_LengthDimension.hxx>
#include <AIS_AngleDimension.hxx>
#include <BRepBuilderAPI_MakeFace.hxx>
#include <gp_Pln.hxx>
#include <TopExp.hxx>
#include <V3d_Viewer.hxx>
#include <TopTools_LocationSet.hxx>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>
#include <BRepPrimAPI_MakeSphere.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
// 记录原始点击模式
//QList<Standard_Integer> myOldActivatedStandardModes;

MeasureTool::MeasureTool(QWidget *parent, FloatTool *floatTool):
    QMainWindow(parent),
    ui(new Ui::MeasureTool)
{
    m_pFloatTool = floatTool;

 //   myMoveState = DefaultState;
    // 自动摧毁控件
    //    this->setAttribute(Qt::WA_DeleteOnClose, true);
    // 设置窗体的按钮
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);

    ui->setupUi(this);

    // 定位
    QRect widgetRect=parent->rect();
    QPoint panelPos=QPoint(widgetRect.left() + widgetRect.width() - this->width() - 10, widgetRect.top());
    panelPos=parent->mapToGlobal(panelPos);
    move(panelPos);

    //UpdateShapes();

    // 默认选择页面
    ui->stackedWidget->setCurrentWidget(ui->page);

    //connect(this, SIGNAL(syncPosition(QStringList,int)), pageViewer, SLOT(syncPosition(QStringList,int)), Qt::DirectConnection);

    m_surfaceIndex = 0;

    ui->comboBoxPointType->addItem("第一个点",0);
    ui->comboBoxPointType->addItem("第二个点",1);

    ui->comboBoxLineIndex->addItem("第一条直线",0);
    ui->comboBoxLineIndex->addItem("第二条直线",1);

    ui->comboBoxPointIndex->addItem("第一个点",0);
    ui->comboBoxPointIndex->addItem("第二个点",1);
    ui->comboBoxPointIndex->addItem("第三个点",2);

    ui->comboBoxSurfaceIndex->addItem("第一个面",0);
    ui->comboBoxSurfaceIndex->addItem("第二个面",1);

    connect(ui->surfaceComboBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateDispalySurfaceShape()));
}

MeasureTool::~MeasureTool()
{
    //清楚显示
    m_pFloatTool->cleanDisplayInfo();

    emit toolClose();
    delete ui;
}


// 窗口影藏的时候触发
void MeasureTool::hideEvent(QHideEvent *)
{

    ui->stackedWidget->setCurrentWidget(ui->page);
    //m_pFloatTool->UpdateActualLocation();
    m_pFloatTool->cleanDisplayInfo();
    m_pFloatTool->clearSaveAISShape();
    //m_pFloatTool->clearSaveAISShape();

//    hasOpenContext = false;
//    myContext->CloseLocalContext();

    emit toolClose();
}

void MeasureTool::showEvent(QShowEvent *){
//    openContextStandardMode();
//    UpdateShapes();
}

// 打开选取模式
void MeasureTool::openContextStandardMode(){

}

// 记录原始选取类型
void MeasureTool::clearOldActivatedStandard(bool isSave)
{
}



void MeasureTool::UpdateShapes()
{

}

// 设置点的数据
void MeasureTool::setPntInfo()
{
    //qDebug()<<aShapes.size()<<"aShapes.size()";
    Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(m_pFloatTool->aShapes.size()-1);

     TopoDS_Shape selectObj = aio->Shape();

     int SelectType = -1;
     if(!selectObj.IsNull()){

         if(myMeasureType == PntToPntLengthType)
         {

             if(selectObj.ShapeType() == TopAbs_VERTEX)
             {
                 int index = ui->comboBoxPointType->currentIndex();
                 m_pFloatTool->savePointShapes(index);
                 TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                 gp_Pnt pnt = BRep_Tool::Pnt(vertex);
                 if (index == 0)
                  {
                     ui->OneXCoordinate->setText(QString("%1").arg(pnt.X()));
                     ui->OneYCoordinate->setText(QString("%1").arg(pnt.Y()));
                     ui->OneZCoordinate->setText(QString("%1").arg(pnt.Z()));
                  }else
                 {
                    ui->TwoXCoordinate->setText(QString("%1").arg(pnt.X()));
                    ui->TwoYCoordinate->setText(QString("%1").arg(pnt.Y()));
                    ui->TwoZCoordinate->setText(QString("%1").arg(pnt.Z()));
                 }
             }
         }else if(myMeasureType == PointToSurfaceType)
         {
             if(selectObj.ShapeType() == TopAbs_VERTEX)
             {
                 //m_pFloatTool->savePointShapes(index);
                 TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                 gp_Pnt pnt = BRep_Tool::Pnt(vertex);
                 ui->OneXCoordinate_2->setText(QString("%1").arg(pnt.X()));
                 ui->OneYCoordinate_2->setText(QString("%1").arg(pnt.Y()));
                 ui->OneZCoordinate_2->setText(QString("%1").arg(pnt.Z()));
             }
             else if(selectObj.ShapeType() == TopAbs_FACE){
                 TopoDS_Face face = TopoDS::Face(selectObj);
                 //aSurface = BRep_Tool::Surface(face);
             }
         }else if(myMeasureType == LinLengthType)
         {
             if(selectObj.ShapeType() == TopAbs_EDGE)
             {
                 int index = ui->comboBoxLineIndex->currentIndex();
                 m_pFloatTool->saveLineShapes(index);
                 TopoDS_Edge edge  = TopoDS::Edge(selectObj);

                 if(!edge.IsNull()){
                     Standard_Real start , end;
                     Handle(Geom_Curve) curve = BRep_Tool::Curve(edge , start , end);
                     Handle(Geom_Line) line = Handle_Geom_Line::DownCast(curve);
                     if(!line.IsNull())
                     {
                         TopoDS_Vertex aFirst, aSecond;
                         TopExp::Vertices (edge, aFirst, aSecond);
                         gp_Pnt pnt1 = BRep_Tool::Pnt(aFirst);
                         gp_Pnt pnt2 = BRep_Tool::Pnt(aSecond);
                         ui->LinOneXCoordinate->setText(QString("%1").arg(pnt1.X()));
                         ui->LinOneYCoordinate->setText(QString("%1").arg(pnt1.Y()));
                         ui->LinOneZCoordinate->setText(QString("%1").arg(pnt1.Z()));
                         ui->LinTwoXCoordinate->setText(QString("%1").arg(pnt2.X()));
                         ui->LinTwoYCoordinate->setText(QString("%1").arg(pnt2.Y()));
                         ui->LinTwoZCoordinate->setText(QString("%1").arg(pnt2.Z()));
                         ui->LinXOffset->setText(QString("%1").arg(pnt1.X()- pnt2.X()));
                         ui->LinYOffset->setText(QString("%1").arg(pnt1.Y()- pnt2.Y()));
                         ui->LinZOffset->setText(QString("%1").arg(pnt1.Z()- pnt2.Z()));
                         this->setWindowTitle(tr("选线完成"));
                         this->setFocus();
                     }else
                     {
                         this->setWindowTitle(tr("请选择直线"));
                     }
                 }


             }
         }else if(myMeasureType == TwoPlaneAngleType)
         {
             if(selectObj.ShapeType() == TopAbs_FACE)
             {
                 int index = ui->comboBoxSurfaceIndex->currentIndex();
                 m_pFloatTool->saveSurfaceShapes(index);
                 TopoDS_Face face = TopoDS::Face(selectObj);

                 if(index == 0){
                     if(!face.IsNull()){
                         Handle(Geom_Surface) curve = BRep_Tool::Surface(face);
                         Handle(Geom_Plane) line = Handle_Geom_Plane::DownCast(curve);
                         if(!line.IsNull()){
                             gp_Pnt location = line->Pln().Position().Location();
                             gp_Dir dir = line->Pln().Position().Direction();
                             ui->TwoPlaneOneXCoordinate->setText(QString("%1").arg(location.X()));
                             ui->TwoPlaneOneYCoordinate->setText(QString("%1").arg(location.Y()));
                             ui->TwoPlaneOneZCoordinate->setText(QString("%1").arg(location.Z()));
                             ui->TwoPlaneTwoXCoordinate->setText(QString("%1").arg(dir.X()));
                             ui->TwoPlaneTwoYCoordinate->setText(QString("%1").arg(dir.Y()));
                             ui->TwoPlaneTwoZCoordinate->setText(QString("%1").arg(dir.Z()));
                         }
                     }
                 }
                 // 第二条
                 else{
                     if(!face.IsNull()){
                         Handle(Geom_Surface) curve = BRep_Tool::Surface(face);
                         Handle(Geom_Plane) line = Handle_Geom_Plane::DownCast(curve);
                         if(!line.IsNull()){
                             gp_Pnt location = line->Pln().Position().Location();
                             gp_Dir dir = line->Pln().Position().Direction();
                             ui->TwoPlaneOneXCoordinate_2->setText(QString("%1").arg(location.X()));
                             ui->TwoPlaneOneYCoordinate_2->setText(QString("%1").arg(location.Y()));
                             ui->TwoPlaneOneZCoordinate_2->setText(QString("%1").arg(location.Z()));
                             ui->TwoPlaneTwoXCoordinate_2->setText(QString("%1").arg(dir.X()));
                             ui->TwoPlaneTwoYCoordinate_2->setText(QString("%1").arg(dir.Y()));
                             ui->TwoPlaneTwoZCoordinate_2->setText(QString("%1").arg(dir.Z()));
                         }
                     }
                 }


             }
         }else if(myMeasureType == ThreePntAngleType)
         {
             if(selectObj.ShapeType() == TopAbs_VERTEX)
             {
                 int index = ui->comboBoxPointIndex->currentIndex();
                 m_pFloatTool->savePointShapes(index);
                 TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                 gp_Pnt pnt = BRep_Tool::Pnt(vertex);
                 if (index == 0)
                  {
                     ui->ThreeOneXCoordinate->setText(QString("%1").arg(pnt.X()));
                     ui->ThreeOneYCoordinate->setText(QString("%1").arg(pnt.Y()));
                     ui->ThreeOneZCoordinate->setText(QString("%1").arg(pnt.Z()));
                  }else if (index == 1)
                 {
                    ui->ThreeTwoXCoordinate->setText(QString("%1").arg(pnt.X()));
                    ui->ThreeTwoYCoordinate->setText(QString("%1").arg(pnt.Y()));
                    ui->ThreeTwoZCoordinate->setText(QString("%1").arg(pnt.Z()));
                 }
                 else
                 {
                    ui->ThreeThreeXCoordinate->setText(QString("%1").arg(pnt.X()));
                    ui->ThreeThreeYCoordinate->setText(QString("%1").arg(pnt.Y()));
                    ui->ThreeThreeZCoordinate->setText(QString("%1").arg(pnt.Z()));
                 }
             }
         }else if(myMeasureType == TwoLinAngleType)
         {
             if(selectObj.ShapeType() == TopAbs_EDGE)
             {
                 int index = ui->comboBoxLineIndex->currentIndex();
                 m_pFloatTool->saveLineShapes(index);
                 TopoDS_Edge edge = TopoDS::Edge(selectObj);
                 // 第一条直线
                 if(index == 0)
                 {
                     if(!edge.IsNull()){
                         Standard_Real start , end;
                         Handle(Geom_Curve) curve = BRep_Tool::Curve(edge , start , end);
                         Handle(Geom_Line) line = Handle_Geom_Line::DownCast(curve);

                         if(!line.IsNull()){

                             TopExp_Explorer anExp(edge, TopAbs_VERTEX);
                             gp_Pnt endPnt1;
                             gp_Pnt endPnt2;
                             Standard_Real minDistance = 9999999;
                             int i = 0;
                             for( ; anExp.More(); anExp.Next()){
                                 const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
                                 if(i == 0)
                                 {
                                     endPnt1 = BRep_Tool::Pnt(vertex);
                                     qDebug()<<"X"<<endPnt1.X()<<"Y"<<endPnt1.Y()<<"Z"<<endPnt1.Z();
                                 }else
                                 {
                                     endPnt2 = BRep_Tool::Pnt(vertex);
                                     qDebug()<<"X"<<endPnt2.X()<<"Y"<<endPnt2.Y()<<"Z"<<endPnt2.Z();
                                 }
                                 i++;
                             }

                             gp_Pnt location = line->Lin().Position().Location();
                             gp_Dir dir = line->Lin().Position().Direction();
//                             ui->TwoLinOneXCoordinate->setText(QString("%1").arg(location.X()));
//                             ui->TwoLinOneYCoordinate->setText(QString("%1").arg(location.Y()));
//                             ui->TwoLinOneZCoordinate->setText(QString("%1").arg(location.Z()));
//                             ui->TwoLinTwoXCoordinate->setText(QString("%1").arg(dir.X()));
//                             ui->TwoLinTwoYCoordinate->setText(QString("%1").arg(dir.Y()));
//                             ui->TwoLinTwoZCoordinate->setText(QString("%1").arg(dir.Z()));
                             ui->TwoLinOneXCoordinate->setText(QString("%1").arg(endPnt1.X()));
                             ui->TwoLinOneYCoordinate->setText(QString("%1").arg(endPnt1.Y()));
                             ui->TwoLinOneZCoordinate->setText(QString("%1").arg(endPnt1.Z()));
                             ui->TwoLinTwoXCoordinate->setText(QString("%1").arg(endPnt2.X()));
                             ui->TwoLinTwoYCoordinate->setText(QString("%1").arg(endPnt2.Y()));
                             ui->TwoLinTwoZCoordinate->setText(QString("%1").arg(endPnt2.Z()));
                         }else
                         {
                             ui->TwoLinOneXCoordinate->setText("");
                             ui->TwoLinOneYCoordinate->setText("");
                             ui->TwoLinOneZCoordinate->setText("");
                             ui->TwoLinTwoXCoordinate->setText("");
                             ui->TwoLinTwoYCoordinate->setText("");
                             ui->TwoLinTwoZCoordinate->setText("");
                         }
                     }
                 }else {
                     if(!edge.IsNull()){
                         Standard_Real start , end;
                         Handle(Geom_Curve) curve = BRep_Tool::Curve(edge , start , end);
                         Handle(Geom_Line) line = Handle_Geom_Line::DownCast(curve);


                         if(!line.IsNull()){

                             TopExp_Explorer anExp(edge, TopAbs_VERTEX);
                             gp_Pnt endPnt1;
                             gp_Pnt endPnt2;
                             Standard_Real minDistance = 9999999;
                             int i = 0;
                             for( ; anExp.More(); anExp.Next()){
                                 const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
                                 if(i == 0)
                                 {
                                     endPnt1 = BRep_Tool::Pnt(vertex);
                                     qDebug()<<"X"<<endPnt1.X()<<"Y"<<endPnt1.Y()<<"Z"<<endPnt1.Z();
                                 }else
                                 {
                                     endPnt2 = BRep_Tool::Pnt(vertex);
                                     qDebug()<<"X"<<endPnt2.X()<<"Y"<<endPnt2.Y()<<"Z"<<endPnt2.Z();
                                 }
                                 i++;
                             }


                             gp_Pnt location = line->Lin().Position().Location();
                             gp_Dir dir = line->Lin().Position().Direction();
//                             ui->TwoLinOneXCoordinate_2->setText(QString("%1").arg(location.X()));
//                             ui->TwoLinOneYCoordinate_2->setText(QString("%1").arg(location.Y()));
//                             ui->TwoLinOneZCoordinate_2->setText(QString("%1").arg(location.Z()));
//                             ui->TwoLinTwoXCoordinate_2->setText(QString("%1").arg(dir.X()));
//                             ui->TwoLinTwoYCoordinate_2->setText(QString("%1").arg(dir.Y()));
//                             ui->TwoLinTwoZCoordinate_2->setText(QString("%1").arg(dir.Z()));
                               ui->TwoLinOneXCoordinate_2->setText(QString("%1").arg(endPnt1.X()));
                               ui->TwoLinOneYCoordinate_2->setText(QString("%1").arg(endPnt1.Y()));
                               ui->TwoLinOneZCoordinate_2->setText(QString("%1").arg(endPnt1.Z()));
                               ui->TwoLinTwoXCoordinate_2->setText(QString("%1").arg(endPnt2.X()));
                               ui->TwoLinTwoYCoordinate_2->setText(QString("%1").arg(endPnt2.Y()));
                               ui->TwoLinTwoZCoordinate_2->setText(QString("%1").arg(endPnt2.Z()));
                         }else
                         {
                             gp_Pnt location = line->Lin().Position().Location();
                             gp_Dir dir = line->Lin().Position().Direction();
                             ui->TwoLinOneXCoordinate_2->setText("");
                             ui->TwoLinOneYCoordinate_2->setText("");
                             ui->TwoLinOneZCoordinate_2->setText("");
                             ui->TwoLinTwoXCoordinate_2->setText("");
                             ui->TwoLinTwoYCoordinate_2->setText("");
                             ui->TwoLinTwoZCoordinate_2->setText("");
                         }
                     }
                 }
             }
         }
     }
}


// 鼠标点击选取曲面
void MeasureTool::selectSurfaceFromShape(TopoDS_Shape& selectShape)
{
}
// 鼠标点击选取点
void MeasureTool::mouseLeftButtonClick(QMouseEvent *e)
{
}


// 两点测距模式数据
void MeasureTool::setPntToPntInfo()
{

}


// 直线测距模式数据
void MeasureTool::setLinInfo(){
    // 清空原始值
    clearShowValue();
    Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(0);
    TopoDS_Shape selectObj = aio->Shape();
    if(!selectObj.IsNull()){
        TopoDS_Edge edge = TopoDS::Edge(selectObj);
        if(!edge.IsNull()){
            Standard_Real start , end;
            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge , start , end);
            Handle(Geom_Line) line = Handle_Geom_Line::DownCast(curve);
            if(!line.IsNull()){
//                gp_Pnt location = line->Lin().Position().Location();
//                gp_Dir dir = line->Lin().Position().Direction();
                TopoDS_Vertex aFirst, aSecond;
                TopExp::Vertices (edge, aFirst, aSecond);
                gp_Pnt pnt1 = BRep_Tool::Pnt(aFirst);
                gp_Pnt pnt2 = BRep_Tool::Pnt(aSecond);
                ui->LinOneXCoordinate->setText(QString("%1").arg(pnt1.X()));
                ui->LinOneYCoordinate->setText(QString("%1").arg(pnt1.Y()));
                ui->LinOneZCoordinate->setText(QString("%1").arg(pnt1.Z()));
                ui->LinTwoXCoordinate->setText(QString("%1").arg(pnt2.X()));
                ui->LinTwoYCoordinate->setText(QString("%1").arg(pnt2.Y()));
                ui->LinTwoZCoordinate->setText(QString("%1").arg(pnt2.Z()));
                ui->LinXOffset->setText(QString("%1").arg(pnt1.X()- pnt2.X()));
                ui->LinYOffset->setText(QString("%1").arg(pnt1.Y()- pnt2.Y()));
                ui->LinZOffset->setText(QString("%1").arg(pnt1.Z()- pnt2.Z()));
            }
        }
    }
    //aShapes.clear();
    this->setWindowTitle(tr("选线完成"));
    this->setFocus();
}

// 三点测角度模式数据
void MeasureTool::setThreePntInfo(){
    // 清空原始值
    clearShowValue();
    gp_Pnt vertex1 , vertex2 , vertex3;
    for(int i = 0 ; i < m_pFloatTool->aShapes.size() ; i++){
        Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(i);
        TopoDS_Shape selectObj = aio->Shape();
        if(!selectObj.IsNull()){
            if(i == 0){
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull()){
                    vertex1 = BRep_Tool::Pnt(vertex);
                    ui->ThreeOneXCoordinate->setText(QString("%1").arg(vertex1.X()));
                    ui->ThreeOneYCoordinate->setText(QString("%1").arg(vertex1.Y()));
                    ui->ThreeOneZCoordinate->setText(QString("%1").arg(vertex1.Z()));
                }
            }else if(i == 1){
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull()){
                    vertex2 = BRep_Tool::Pnt(vertex);
                    ui->ThreeTwoXCoordinate->setText(QString("%1").arg(vertex2.X()));
                    ui->ThreeTwoYCoordinate->setText(QString("%1").arg(vertex2.Y()));
                    ui->ThreeTwoZCoordinate->setText(QString("%1").arg(vertex2.Z()));
                }
            }else{
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull()){
                    vertex3 = BRep_Tool::Pnt(vertex);
                    ui->ThreeThreeXCoordinate->setText(QString("%1").arg(vertex3.X()));
                    ui->ThreeThreeYCoordinate->setText(QString("%1").arg(vertex3.Y()));
                    ui->ThreeThreeZCoordinate->setText(QString("%1").arg(vertex3.Z()));
                }
            }
        }
    }
//    if(aShapes.count() == 3){
//        myMoveState = CompleteState;
//        this->setWindowTitle(QObject::tr("选点完成"));
//        this->setFocus();
//    }else if(aShapes.count() == 2){
//        this->setWindowTitle(QObject::tr("请选取第三个点"));
//    }else{
//        myMoveState = StartState;
//        this->setWindowTitle(QObject::tr("请选取中心点"));
//    }
}

// 设置两平面测角度数据
void MeasureTool::setTwoPlaneInfo(){
    // 清空原始值
    clearShowValue();

    for(int i = 0 ; i < m_pFloatTool->aShapes.size() ; i++){
        Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(i);
        TopoDS_Shape selectObj = aio->Shape();
        if(!selectObj.IsNull()){
            // 第一个平面
            if(i == 0){
                TopoDS_Face face = TopoDS::Face(selectObj);
                if(!face.IsNull()){
                    Handle(Geom_Surface) curve = BRep_Tool::Surface(face);
                    Handle(Geom_Plane) line = Handle_Geom_Plane::DownCast(curve);
                    if(!line.IsNull()){
                        gp_Pnt location = line->Pln().Position().Location();
                        gp_Dir dir = line->Pln().Position().Direction();
                        ui->TwoPlaneOneXCoordinate->setText(QString("%1").arg(location.X()));
                        ui->TwoPlaneOneYCoordinate->setText(QString("%1").arg(location.Y()));
                        ui->TwoPlaneOneZCoordinate->setText(QString("%1").arg(location.Z()));
                        ui->TwoPlaneTwoXCoordinate->setText(QString("%1").arg(dir.X()));
                        ui->TwoPlaneTwoYCoordinate->setText(QString("%1").arg(dir.Y()));
                        ui->TwoPlaneTwoZCoordinate->setText(QString("%1").arg(dir.Z()));
                    }else{
                        m_pFloatTool->aShapes.removeAt(i);
                    }
                }
            }
            // 第二条
            else{
                TopoDS_Face face = TopoDS::Face(selectObj);
                if(!face.IsNull()){
                    Handle(Geom_Surface) curve = BRep_Tool::Surface(face);
                    Handle(Geom_Plane) line = Handle_Geom_Plane::DownCast(curve);
                    if(!line.IsNull()){
                        gp_Pnt location = line->Pln().Position().Location();
                        gp_Dir dir = line->Pln().Position().Direction();
                        ui->TwoPlaneOneXCoordinate_2->setText(QString("%1").arg(location.X()));
                        ui->TwoPlaneOneYCoordinate_2->setText(QString("%1").arg(location.Y()));
                        ui->TwoPlaneOneZCoordinate_2->setText(QString("%1").arg(location.Z()));
                        ui->TwoPlaneTwoXCoordinate_2->setText(QString("%1").arg(dir.X()));
                        ui->TwoPlaneTwoYCoordinate_2->setText(QString("%1").arg(dir.Y()));
                        ui->TwoPlaneTwoZCoordinate_2->setText(QString("%1").arg(dir.Z()));
                    }
                }
            }
        }
    }
    // 选完了一个平面
//    if(aShapes.count() == 1){
//        this->setWindowTitle(tr("请选择第二个平面"));
//    }else if(aShapes.count() == 2){
//        this->setWindowTitle(tr("选面完成"));
//        myMoveState = CompleteState;
//        this->setFocus();
//    }else{
//        this->setWindowTitle(tr("请选择第一个平面"));
//    }
}


// 设置点到曲面最短距离数据
void MeasureTool::setPointToSurfaceInfo(){
    // 清空原始值
    clearShowValue();
    gp_Pnt vertex1;
    for(int i = 0 ; i < m_pFloatTool->aShapes.size() ; i++){
        Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(i);
        TopoDS_Shape selectObj = aio->Shape();
        if(!selectObj.IsNull()){
            // 第一个平面
            if(selectObj.ShapeType() == TopAbs_VERTEX)
            {
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull())
                {
                    vertex1 = BRep_Tool::Pnt(vertex);
                    ui->OneXCoordinate_2->setText(QString("%1").arg(vertex1.X()));
                    ui->OneYCoordinate_2->setText(QString("%1").arg(vertex1.Y()));
                    ui->OneZCoordinate_2->setText(QString("%1").arg(vertex1.Z()));
                }else
                {
                    //aShapes.removeAt(i);
                }
            }
            // 第二条
            else if(selectObj.ShapeType() == TopAbs_FACE){
                TopoDS_Face face = TopoDS::Face(selectObj);
                if(!face.IsNull()){
                    //aSurface = BRep_Tool::Surface(face);
                }else{
                    //aShapes.removeAt(i);
                }
            }
        }
    }

    // 选完了一个平面
//    if(aShapes.count() == 1){
//        if(aShapes.value(0)->Shape().ShapeType() == TopAbs_VERTEX)
//        {
//            this->setWindowTitle(tr("请选择曲面"));
//        }else
//        {
//            this->setWindowTitle(tr("请选择点"));
//        }

//    }else if(aShapes.count() == 2){
//        this->setWindowTitle(tr("选点和曲面完成"));
//        myMoveState = CompleteState;
//        this->setFocus();
//    }else{
//        this->setWindowTitle(tr("请选择点和曲面"));
//    }
}
// 清除显示数据
void MeasureTool::clearShowValue(){
    ui->LinOneXCoordinate->setText("");
    ui->LinOneYCoordinate->setText("");
    ui->LinOneZCoordinate->setText("");
    ui->LinTwoXCoordinate->setText("");
    ui->LinTwoYCoordinate->setText("");
    ui->LinTwoZCoordinate->setText("");
    ui->LinXOffset->setText("");
    ui->LinYOffset->setText("");
    ui->LinZOffset->setText("");
    ui->OneXCoordinate->setText("");
    ui->OneYCoordinate->setText("");
    ui->OneZCoordinate->setText("");
    ui->ThreeOneXCoordinate->setText("");
    ui->ThreeOneYCoordinate->setText("");
    ui->ThreeOneZCoordinate->setText("");
    ui->ThreeThreeXCoordinate->setText("");
    ui->ThreeThreeYCoordinate->setText("");
    ui->ThreeThreeZCoordinate->setText("");
    ui->ThreeTwoXCoordinate->setText("");
    ui->ThreeTwoYCoordinate->setText("");
    ui->ThreeTwoZCoordinate->setText("");
    ui->TwoLinOneXCoordinate->setText("");
    ui->TwoLinOneYCoordinate->setText("");
    ui->TwoLinOneZCoordinate->setText("");
    ui->TwoLinOneXCoordinate_2->setText("");
    ui->TwoLinOneYCoordinate_2->setText("");
    ui->TwoLinOneZCoordinate_2->setText("");
    ui->TwoLinTwoXCoordinate->setText("");
    ui->TwoLinTwoYCoordinate->setText("");
    ui->TwoLinTwoZCoordinate->setText("");
    ui->TwoLinTwoXCoordinate_2->setText("");
    ui->TwoLinTwoYCoordinate_2->setText("");
    ui->TwoLinTwoZCoordinate_2->setText("");
    ui->TwoPlaneOneXCoordinate->setText("");
    ui->TwoPlaneOneYCoordinate->setText("");
    ui->TwoPlaneOneZCoordinate->setText("");
    ui->TwoPlaneOneXCoordinate_2->setText("");
    ui->TwoPlaneOneYCoordinate_2->setText("");
    ui->TwoPlaneOneZCoordinate_2->setText("");
    ui->TwoPlaneTwoXCoordinate->setText("");
    ui->TwoPlaneTwoYCoordinate->setText("");
    ui->TwoPlaneTwoZCoordinate->setText("");
    ui->TwoPlaneTwoXCoordinate_2->setText("");
    ui->TwoPlaneTwoYCoordinate_2->setText("");
    ui->TwoPlaneTwoZCoordinate_2->setText("");
    ui->TwoXCoordinate->setText("");
    ui->TwoYCoordinate->setText("");
    ui->TwoZCoordinate->setText("");
    ui->XOffset->setText("");
    ui->YOffset->setText("");
    ui->ZOffset->setText("");
    ui->OneLength->setText("");
    ui->TwoLength->setText("");
    ui->ThreeLength->setText("");
    ui->fourLength->setText("");
    ui->fiveLength->setText("");
    ui->OneXCoordinate_2->setText("");
    ui->OneYCoordinate_2->setText("");
    ui->OneZCoordinate_2->setText("");

    ui->MinDistanceOfPointToSurface->setText("");
    ui->ProjectionCoordinateX->setText("");
    ui->ProjectionCoordinateY->setText("");
    ui->ProjectionCoordinateZ->setText("");
}


#include <QMessageBox>
// 两点测距
void MeasureTool::on_pushButton_clicked()
{
    myMeasureType = PntToPntLengthType;
    // 清空界面原始值
    clearShowValue();

    ui->comboBoxPointType->setCurrentIndex(0);
    this->setWindowTitle(tr("请选择第一个点"));
    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

// 两点测距确定
void MeasureTool::on_pushButton_6_clicked()
{
    gp_Pnt pnt1 = gp_Pnt(ui->OneXCoordinate->text().toDouble() , ui->OneYCoordinate->text().toDouble() , ui->OneZCoordinate->text().toDouble());
    gp_Pnt pnt2 = gp_Pnt(ui->TwoXCoordinate->text().toDouble() , ui->TwoYCoordinate->text().toDouble() , ui->TwoZCoordinate->text().toDouble());

    //清除ashapes
    double ptnToPtnDistance = m_pFloatTool->displayPtnToPtnDistance(pnt1, pnt2);
    ui->OneLength->setText(QString("%1").arg(ptnToPtnDistance));

    // 切换到初始状态
    //myMoveType = DefaultMoveType;
    //myMoveState = DefaultState;
}

// 两点测距取消
void MeasureTool::on_pushButton_7_clicked()
{
    m_pFloatTool->cleanDisplayInfo();
    m_pFloatTool->clearSaveAISShape();
    ui->stackedWidget->setCurrentWidget(ui->page);
}


// 直线测距
void MeasureTool::on_pushButton_2_clicked()
{
    myMeasureType = LinLengthType;
    // 清空界面原始值
    clearShowValue();

//    // 记录原始选择模式
//    if(myOldActivatedStandardModes.isEmpty()){
//        clearOldActivatedStandard(true);
//    }else{
//        clearOldActivatedStandard();
//    }

//    // 切换选择模式
//    myContext->ActivateStandardMode(TopAbs_EDGE);

    this->setWindowTitle(tr("请选择一条直线"));
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}


// 直线测距确定
void MeasureTool::on_pushButton_8_clicked()
{
    // 如果选取已经完成
    gp_Pnt pnt1 = gp_Pnt(ui->LinOneXCoordinate->text().toDouble() , ui->LinOneYCoordinate->text().toDouble() , ui->LinOneZCoordinate->text().toDouble());
    gp_Pnt pnt2 = gp_Pnt(ui->LinTwoXCoordinate->text().toDouble() , ui->LinTwoYCoordinate->text().toDouble() , ui->LinTwoZCoordinate->text().toDouble());

    //清除ashapes
    double lineDistance = m_pFloatTool->displayLineDistance(pnt1, pnt2);
    ui->TwoLength->setText(QString("%1").arg(lineDistance));

    // 切换到初始页面
    //myMoveType = DefaultMoveType;
    //myMoveState = DefaultState;
    //aShapes.clear();

}

// 直线测距取消
void MeasureTool::on_pushButton_9_clicked()
{
    m_pFloatTool->cleanDisplayInfo();
    m_pFloatTool->clearSaveAISShape();
    ui->stackedWidget->setCurrentWidget(ui->page);

    ui->stackedWidget->setCurrentWidget(ui->page);
}

// 三点测角度
void MeasureTool::on_pushButton_3_clicked()
{
    myMeasureType = ThreePntAngleType;
    // 清空界面原始值
    clearShowValue();

    ui->comboBoxPointIndex->setCurrentIndex(0);

//    this->setWindowTitle(tr("请选择第一个点"));

    ui->stackedWidget->setCurrentWidget(ui->page_4);
}


// 三点测角度确定
void MeasureTool::on_pushButton_10_clicked()
{
    gp_Pnt pnt1 = gp_Pnt(ui->ThreeOneXCoordinate->text().toDouble() , ui->ThreeOneYCoordinate->text().toDouble() , ui->ThreeOneZCoordinate->text().toDouble());
    gp_Pnt pnt2 = gp_Pnt(ui->ThreeTwoXCoordinate->text().toDouble() , ui->ThreeTwoYCoordinate->text().toDouble() , ui->ThreeTwoZCoordinate->text().toDouble());
    gp_Pnt pnt3 = gp_Pnt(ui->ThreeThreeXCoordinate->text().toDouble() , ui->ThreeThreeYCoordinate->text().toDouble() , ui->ThreeThreeZCoordinate->text().toDouble());

    //清除ashapes
    double ThreePtnAngle = m_pFloatTool->displayThreePtnToAngle(pnt1, pnt2, pnt3);
    ui->ThreeLength->setText(QString("%1").arg(ThreePtnAngle/M_PI *180));
}

// 三点测角度取消
void MeasureTool::on_pushButton_11_clicked()
{
    m_pFloatTool->cleanDisplayInfo();
    m_pFloatTool->clearSaveAISShape();

    ui->stackedWidget->setCurrentWidget(ui->page);
}


// 两直线测角度
void MeasureTool::on_pushButton_4_clicked()
{
    myMeasureType = TwoLinAngleType;
    // 清空界面原始值
    clearShowValue();

    this->setWindowTitle(tr("请选择第一条直线"));

    ui->stackedWidget->setCurrentWidget(ui->page_5);
}

// 两直线测角度确定
void MeasureTool::on_pushButton_12_clicked()
{
    if( ui->TwoLinOneXCoordinate->text().isEmpty() ||ui->TwoLinOneYCoordinate->text().isEmpty() ||ui->TwoLinOneZCoordinate->text().isEmpty() )
    {
        return;
    }
    if( ui->TwoLinOneXCoordinate_2->text().isEmpty() ||ui->TwoLinOneYCoordinate_2->text().isEmpty() ||ui->TwoLinOneZCoordinate_2->text().isEmpty() )
    {
        return;
    }
//    gp_Pnt pnt1 = gp_Pnt(ui->TwoLinOneXCoordinate->text().toDouble() , ui->TwoLinOneYCoordinate->text().toDouble() , ui->TwoLinOneZCoordinate->text().toDouble());
//    gp_Dir dir1 = gp_Dir(ui->TwoLinTwoXCoordinate->text().toDouble() , ui->TwoLinTwoYCoordinate->text().toDouble() , ui->TwoLinTwoZCoordinate->text().toDouble());

//    gp_Pnt pnt2 = gp_Pnt(ui->TwoLinOneXCoordinate_2->text().toDouble() , ui->TwoLinOneYCoordinate_2->text().toDouble() , ui->TwoLinOneZCoordinate_2->text().toDouble());
//    gp_Dir dir2 = gp_Dir(ui->TwoLinTwoXCoordinate_2->text().toDouble() , ui->TwoLinTwoYCoordinate_2->text().toDouble() , ui->TwoLinTwoZCoordinate_2->text().toDouble());

    gp_Pnt firstEndPntOfLine1 = gp_Pnt(ui->TwoLinOneXCoordinate->text().toDouble() , ui->TwoLinOneYCoordinate->text().toDouble() , ui->TwoLinOneZCoordinate->text().toDouble());
    gp_Pnt secondEndPntOfLine1 = gp_Pnt(ui->TwoLinTwoXCoordinate->text().toDouble() , ui->TwoLinTwoYCoordinate->text().toDouble() , ui->TwoLinTwoZCoordinate->text().toDouble());

    gp_Pnt firstEndPntOfLine2 = gp_Pnt(ui->TwoLinOneXCoordinate_2->text().toDouble() , ui->TwoLinOneYCoordinate_2->text().toDouble() , ui->TwoLinOneZCoordinate_2->text().toDouble());
    gp_Pnt secondEndPntOfLine2 = gp_Pnt(ui->TwoLinTwoXCoordinate_2->text().toDouble() , ui->TwoLinTwoYCoordinate_2->text().toDouble() , ui->TwoLinTwoZCoordinate_2->text().toDouble());

//    // 根据数据生成两直线
//    gp_Lin lin1 = gp_Lin(pnt1 , dir1);
//    gp_Lin lin2 = gp_Lin(pnt2 , dir2);

    // 根据数据生成两直线
    gp_Vec vec1 = gp_Vec(firstEndPntOfLine1, secondEndPntOfLine1);
    gp_Vec vec2 = gp_Vec(firstEndPntOfLine2, secondEndPntOfLine2);
    gp_Dir dir1 = gp_Dir(vec1);
    gp_Dir dir2 = gp_Dir(vec2);

    gp_Lin lin1 = gp_Lin(firstEndPntOfLine1 , dir1);
    gp_Lin lin2 = gp_Lin(firstEndPntOfLine2 , dir2);

    Standard_Real edgeDisance1 = firstEndPntOfLine1.Distance(secondEndPntOfLine1);
    Standard_Real edgeDisance2 = firstEndPntOfLine2.Distance(secondEndPntOfLine2);
    Standard_Real theFlyout = edgeDisance2;
    if(edgeDisance1 <= edgeDisance2)
    {
        theFlyout = edgeDisance1;
        qDebug()<<"edgeDisance1"<<edgeDisance1;
    }

    double lineToLineAngle = m_pFloatTool->displayLineToLineAngle(lin1, lin2, theFlyout);
    ui->fourLength->setText(QString("%1").arg(lineToLineAngle/M_PI *180));
}


// 两直线测角度取消
void MeasureTool::on_pushButton_13_clicked()
{
    m_pFloatTool->clearShapes();

    ui->stackedWidget->setCurrentWidget(ui->page);
}


// 两平面测角度
void MeasureTool::on_pushButton_5_clicked()
{
    myMeasureType = TwoPlaneAngleType;
    clearShowValue();

//    // 记录原始选择模式
//    if(myOldActivatedStandardModes.isEmpty()){
//        clearOldActivatedStandard(true);
//    }else{
//        clearOldActivatedStandard();
//    }

//    // 切换选择模式
//    myContext->ActivateStandardMode(TopAbs_FACE);

//    this->setWindowTitle(tr("请选择第一个平面"));

    ui->stackedWidget->setCurrentWidget(ui->page_6);
}

// 两平面测角度确定
void MeasureTool::on_pushButton_14_clicked()
{
    //clearOldActivatedStandard();
    //        myContext->ActivateStandardMode(TopAbs_SHAPE);
    if( ui->TwoPlaneOneXCoordinate->text().isEmpty() ||ui->TwoPlaneOneYCoordinate->text().isEmpty() ||ui->TwoPlaneOneZCoordinate->text().isEmpty() )
    {
        return;
    }
    if( ui->TwoPlaneTwoXCoordinate->text().isEmpty() ||ui->TwoPlaneTwoYCoordinate->text().isEmpty() ||ui->TwoPlaneTwoZCoordinate->text().isEmpty() )
    {
        return;
    }
    if( ui->TwoPlaneOneXCoordinate_2->text().isEmpty() ||ui->TwoPlaneOneYCoordinate_2->text().isEmpty() ||ui->TwoPlaneOneZCoordinate_2->text().isEmpty() )
    {
        return;
    }
    if( ui->TwoPlaneTwoXCoordinate_2->text().isEmpty() ||ui->TwoPlaneTwoYCoordinate_2->text().isEmpty() ||ui->TwoPlaneTwoZCoordinate_2->text().isEmpty() )
    {
        return;
    }
    gp_Pnt pnt1 = gp_Pnt(ui->TwoPlaneOneXCoordinate->text().toDouble() , ui->TwoPlaneOneYCoordinate->text().toDouble() , ui->TwoPlaneOneZCoordinate->text().toDouble());
    gp_Dir dir1 = gp_Dir(ui->TwoPlaneTwoXCoordinate->text().toDouble() , ui->TwoPlaneTwoYCoordinate->text().toDouble() , ui->TwoPlaneTwoZCoordinate->text().toDouble());
    gp_Pnt pnt2 = gp_Pnt(ui->TwoPlaneOneXCoordinate_2->text().toDouble() , ui->TwoPlaneOneYCoordinate_2->text().toDouble() , ui->TwoPlaneOneZCoordinate_2->text().toDouble());
    gp_Dir dir2 = gp_Dir(ui->TwoPlaneTwoXCoordinate_2->text().toDouble() , ui->TwoPlaneTwoYCoordinate_2->text().toDouble() , ui->TwoPlaneTwoZCoordinate_2->text().toDouble());
    // 根据数据生成两直线
    gp_Pln pln1 = gp_Pln(pnt1 , dir1);
    gp_Pln pln2 = gp_Pln(pnt2 , dir2);
    TopoDS_Face face1 = BRepBuilderAPI_MakeFace(pln1);
    TopoDS_Face face2 = BRepBuilderAPI_MakeFace(pln2);

     double planeToPlaneAngle = m_pFloatTool->displaySurfaceToSurfaceAngle(face1, face2);

    ui->fiveLength->setText(QString("%1").arg(planeToPlaneAngle/M_PI *180));
}

// 两平面测角度取消
void MeasureTool::on_pushButton_15_clicked()
{
    m_pFloatTool->clearShapes();
    ui->stackedWidget->setCurrentWidget(ui->page);
}
// 点到曲面的最短距离
void MeasureTool::on_pushButton_16_clicked()
{    
    myMeasureType = PointToSurfaceType;
    // 清空界面原始值
    clearShowValue();

    // 切换选择模式
    //myContext->ActivateStandardMode(TopAbs_VERTEX);

    //this->setWindowTitle(tr("请选择第一个平面"));

    ui->stackedWidget->setCurrentWidget(ui->page_7);
}
// 点到曲面的最短距离确定
void MeasureTool::on_pushButton_17_clicked()
{
    if(m_pFloatTool != NULL)
    {
        gp_Pnt pnt1 = gp_Pnt(ui->OneXCoordinate_2->text().toDouble() , ui->OneYCoordinate_2->text().toDouble() , ui->OneZCoordinate_2->text().toDouble());
        double ptnToSurfaceDistance = m_pFloatTool->displayPtnToSurfaceDistance(pnt1);
        if(ptnToSurfaceDistance >= 0)
        {
            ui->MinDistanceOfPointToSurface->setText(QString("%1").arg(ptnToSurfaceDistance));
        }

//        ui->ProjectionCoordinateX->setText(QString("%1").arg(pnt2.X()));
//        ui->ProjectionCoordinateY->setText(QString("%1").arg(pnt2.Y()));
//        ui->ProjectionCoordinateZ->setText(QString("%1").arg(pnt2.Z()));
    }

}
// 点到曲面的最短距离取消
void MeasureTool::on_pushButton_18_clicked()
{
//    aShapes.clear();
    ui->stackedWidget->setCurrentWidget(ui->page);
}

// 保存曲面
void MeasureTool::on_pushButton_19_clicked()
{
    if(m_pFloatTool != NULL)
    {
        m_pFloatTool->saveSurfaceTopoDSShape();
    }
}


// 创建曲面
void MeasureTool::on_createSurfacePushButton_clicked()
{
    if(m_pFloatTool != NULL)
    {
        if(1 == m_pFloatTool->createSurfaceTopoDSShape(m_surfaceIndex))
        {
            QString strSurfaceIndex;
            strSurfaceIndex = QString("曲面%1").arg(++m_surfaceIndex);
            ui->surfaceComboBox->addItem(strSurfaceIndex,0);
            ui->surfaceComboBox->setCurrentIndex(m_surfaceIndex-1);
        }
    }
}

void MeasureTool::updateDispalySurfaceShape()
{
    int index = ui->surfaceComboBox->currentIndex();

    if(m_pFloatTool != NULL)
    {
        m_pFloatTool->updateSelectDisplayShape(index);
    }
}

// 修改曲面
void MeasureTool::on_modifySurfacePushButton_clicked()
{
    int index = ui->surfaceComboBox->currentIndex();

    if(m_pFloatTool != NULL)
    {
        m_pFloatTool->modifySurfaceTopoDSShape(index);
    }
}

