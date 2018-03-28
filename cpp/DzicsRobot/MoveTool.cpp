#include "movetool.h"
#include "ui_movetool.h"

#include <QRegExpValidator>
#include <QDebug>

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
#include <TColStd_ListOfInteger.hxx>
// 记录原始点击模式
//QList<Standard_Integer> myOldActivatedStandardModes;

MoveTool::MoveTool(QWidget *parent, FloatTool *floatTool) :
    QMainWindow(parent),
    ui(new Ui::MoveTool)
{
    m_pFloatTool = floatTool;
    myMoveType = DefaultMoveType;
    myMoveState = DefaultState;
    // 自动摧毁控件
//    this->setAttribute(Qt::WA_DeleteOnClose, true);
    // 设置窗体的按钮
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    ui->setupUi(this);

    // 数字的正则验证
    QRegExp rx("^(-?\\d+)(\\.\\d+)?$");
    QRegExpValidator *pRevalidotor = new QRegExpValidator(rx, this);
    // 给当行文本加入数字验证
    ui->XOffsetTxt->setValidator(pRevalidotor);
    ui->YOffsetTxt->setValidator(pRevalidotor);
    ui->ZOffsetTxt->setValidator(pRevalidotor);

    // 默认选择模式
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    QRect widgetRect=parent->rect();
    QPoint panelPos=QPoint(widgetRect.left() + widgetRect.width() - this->width() - 10, widgetRect.top());
    panelPos=parent->mapToGlobal(panelPos);
    move(panelPos);


    ui->comboBoxPointType->addItem("到达点",0);
    ui->comboBoxPointType->addItem("源点",1);

    ui->comboBoxRotationPoint->addItem("第一个点",0);
    ui->comboBoxRotationPoint->addItem("第二个点",1);

    ui->comboBoxSurfaceIndex->addItem("目标面",0);
    ui->comboBoxSurfaceIndex->addItem("源面",1);

    ui->AxisXRadioButton->setChecked(true);
    connect(ui->AxisXRadioButton,SIGNAL(clicked(bool)),this,SLOT(setRotateAxis()));
    connect(ui->AxisYRadioButton,SIGNAL(clicked(bool)),this,SLOT(setRotateAxis()));
    connect(ui->AxisZRadioButton,SIGNAL(clicked(bool)),this,SLOT(setRotateAxis()));
}

MoveTool::~MoveTool()
{
    // 还原选取模式
    if(!myOldActivatedStandardModes.isEmpty()){
        clearOldActivatedStandard();
        Standard_Integer SelectMode;
        // 重新将原来的选择模式赋值上去
        for (int i = 0 ;  i < myOldActivatedStandardModes.size() ; i++) {
            SelectMode = myOldActivatedStandardModes.value(i);
            switch (SelectMode)
            {
            case 1:
                myContext->ActivateStandardMode(TopAbs_VERTEX);
                break;
            case 2:
                myContext->ActivateStandardMode(TopAbs_EDGE);
                break;
            case 3:
                myContext->ActivateStandardMode(TopAbs_WIRE);
                break;
            case 4:
                myContext->ActivateStandardMode(TopAbs_FACE);
                break;
            case 5:
                myContext->ActivateStandardMode(TopAbs_SHELL);
                break;
            case 6:
                myContext->ActivateStandardMode(TopAbs_SOLID);
                break;
            case 7:
                myContext->ActivateStandardMode(TopAbs_COMPOUND);
                break;
            default:
                myContext->ActivateStandardMode(TopAbs_SHAPE);
            }
        }
        myOldActivatedStandardModes.clear();
    }

    //aShapes.clear();

    emit toolClose();

    delete ui;
}


// 窗口影藏的时候触发
void MoveTool::hideEvent(QHideEvent *){

    //ui->stackedWidget->setCurrentWidget(ui->page);
    myMoveType = DefaultMoveType;
//    myMoveState = DefaultState;

    //m_pFloatTool->UpdateActualLocation();
    m_pFloatTool->cleanDisplayInfo();
    m_pFloatTool->clearSaveAISShape();
    //m_pFloatTool->clearSaveAISShape();

//    hasOpenContext = false;
//    myContext->CloseLocalContext();

    emit toolClose();
}

void MoveTool::showEvent(QShowEvent *){
    //myContext->OpenLocalContext();
}

// 鼠标点击选取点
void MoveTool::mouseLeftButtonClick(QMouseEvent *e)
{

}

// 当执行选点的时候，根据选择的偏移模式来选择所执行的方法
void MoveTool::setPntInfo()
{
    Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(m_pFloatTool->aShapes.size()-1);

    TopoDS_Shape selectObj = aio->Shape();

    if(!selectObj.IsNull()){

        if(myMoveType == PntToPntMoveType)
        {
            //DefaultMoveType , PntToPntMoveType  , DeltaMoveType , PntRotateType  , TwoPntRotateType ,  LineRotateType
            if(selectObj.ShapeType() == TopAbs_VERTEX)
            {
                int index = ui->comboBoxPointType->currentIndex();
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
        }else if(myMoveType == PntRotateType)
        {
            if(selectObj.ShapeType() == TopAbs_VERTEX)
            {
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull()){
                    gp_Pnt vertex1 = BRep_Tool::Pnt(vertex);
                    ui->OneRotateXCoordinate->setText(QString("%1").arg(vertex1.X()));
                    ui->OneRotateYCoordinate->setText(QString("%1").arg(vertex1.Y()));
                    ui->OneRotateZCoordinate->setText(QString("%1").arg(vertex1.Z()));

                    myXOffset = vertex1.X() ;
                    myYOffset = vertex1.Y() ;
                    myZOffset = vertex1.Z();
                    if(ui->AxisXRadioButton->isChecked() )
                    {
                        myXRotation = 10;
                        myYRotation = 0;
                        myZRotation = 0;
                    }else if(ui->AxisYRadioButton->isChecked() )
                    {
                        myXRotation = 0;
                        myYRotation = 10;
                        myZRotation = 0;
                    }else if(ui->AxisZRadioButton->isChecked() )
                    {
                        myXRotation = 0;
                        myYRotation = 0;
                        myZRotation = 10;
                    }

//                    myXRotation = vertex1.X();
//                    myYRotation = vertex1.Y();
//                    myZRotation = vertex1.Z();
//                    gp_Vec vec = gp_Vec(vertex1 , gp_Pnt(0 , 0 , 0));
//                    myXOffset = vec.X() ;
//                    myYOffset = vec.Y() ;
//                    myZOffset = vec.Z();
                    myMoveState = CompleteState;
                }
            }

        }else if(myMoveType == TwoPntRotateType)
        {
            int index = ui->comboBoxRotationPoint->currentIndex();
            if(index == 0){
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull()){
                    gp_Pnt vertex1 = BRep_Tool::Pnt(vertex);
                    ui->TwoOneRotateXCoordinate->setText(QString("%1").arg(vertex1.X()));
                    ui->TwoOneRotateYCoordinate->setText(QString("%1").arg(vertex1.Y()));
                    ui->TwoOneRotateZCoordinate->setText(QString("%1").arg(vertex1.Z()));
                }
            }else{
                TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                if(!vertex.IsNull()){
                    gp_Pnt vertex2 = BRep_Tool::Pnt(vertex);
                    ui->TwoTwoRotateXCoordinate->setText(QString("%1").arg(vertex2.X()));
                    ui->TwoTwoRotateYCoordinate->setText(QString("%1").arg(vertex2.Y()));
                    ui->TwoTwoRotateZCoordinate->setText(QString("%1").arg(vertex2.Z()));
                }
            }

            if(!ui->TwoOneRotateXCoordinate->text().isEmpty() && !ui->TwoTwoRotateXCoordinate->text().isEmpty() )
            {
                myXOffset = ui->TwoOneRotateXCoordinate->text().toDouble() - ui->TwoTwoRotateXCoordinate->text().toDouble();
                myYOffset = ui->TwoOneRotateYCoordinate->text().toDouble() - ui->TwoTwoRotateYCoordinate->text().toDouble();
                myZOffset = ui->TwoOneRotateZCoordinate->text().toDouble() - ui->TwoTwoRotateZCoordinate->text().toDouble();
                gp_Pnt pnt1 = gp_Pnt(ui->TwoOneRotateXCoordinate->text().toDouble() , ui->TwoOneRotateYCoordinate->text().toDouble() , ui->TwoOneRotateZCoordinate->text().toDouble());
                gp_Pnt pnt2 = gp_Pnt(ui->TwoTwoRotateXCoordinate->text().toDouble() , ui->TwoTwoRotateYCoordinate->text().toDouble() , ui->TwoTwoRotateZCoordinate->text().toDouble());

                gp_Vec vec = gp_Vec(pnt1 , pnt2);
                //myXRotation = vec.X();
                //myYRotation = vec.Y();
                //myZRotation = vec.Z();

                myXRotation = pnt1.X() - pnt2.X();
                myYRotation = pnt1.Y() - pnt2.Y();
                myZRotation = pnt1.Z() - pnt2.Z();

                myXOffset = pnt1.X() ;
                myYOffset = pnt1.Y() ;
                myZOffset = pnt1.Z();

                myMoveState = CompleteState;
                this->setWindowTitle(QObject::tr("选点完成"));
                this->setFocus();
            }else
            {
                myMoveState = StartState;
                this->setWindowTitle(QObject::tr("请选取目标点"));
            }
        }else if(myMoveType == MoveToSurfaceType)
        {
            // 清空原始值
            if(!selectObj.IsNull())
            {
                if(selectObj.ShapeType() == TopAbs_FACE)
                {
                    TopoDS_Face aFace = TopoDS::Face(selectObj);
                    Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);

                    //计算面的法向量
                    //StdPrs_ToolShadedShape SST;
                    //for( NumFace=0,ExpFace.Init(myShape,TopAbs_FACE); ExpFace.More(); ExpFace.Next(),NumFace++ )
                    //{
                        //TopoDS_Face myFace = TopoDS::Face(ExpFace.Current());
                    TopLoc_Location aLocation = aFace.Location();


                    Standard_Real umin, umax, vmin, vmax;
                    //SST.Normal(aFace, pc, myNormal);
                    BRepTools::UVBounds(aFace,umin, umax, vmin, vmax);

                    //dUmax = (Umax - Umin);
                    //dVmax = (Vmax - Vmin);

                    GeomLProp_SLProps props(aSurface, umin, vmin,1, 0.01);
                    gp_Dir normal = props.Normal();
                    Standard_Real x = normal.X();
                    Standard_Real y = normal.Y();
                    Standard_Real z = normal.Z();

                    gp_Pnt pnt = props.Value();

                    qDebug()<<"Direction Validation"<<x<<y<<z;

                    //求面的中心
//                    TopExp_Explorer anExp(aFace, TopAbs_EDGE);
//                    gp_Pnt Pnt;
//                    for( ; anExp.More(); anExp.Next()){
//                        const TopoDS_Edge& aEdge = TopoDS::Edge(anExp.Current());

//                    }
                    //aLocation.myItems.

                    int index = ui->comboBoxSurfaceIndex->currentIndex();
                    if(index==0)
                    {
                        ui->targetNormalX->setText(QString("%1").arg(normal.X()));
                        ui->targetNormalY->setText(QString("%1").arg(normal.Y()));
                        ui->targetNormalZ->setText(QString("%1").arg(normal.Z()));

                        ui->targetPointX->setText(QString("%1").arg(pnt.X()));
                        ui->targetPointY->setText(QString("%1").arg(pnt.Y()));
                        ui->targetPointZ->setText(QString("%1").arg(pnt.Z()));
                    }else
                    {
                        ui->sourceNormalX->setText(QString("%1").arg(normal.X()));
                        ui->sourceNormalY->setText(QString("%1").arg(normal.Y()));
                        ui->sourceNormalZ->setText(QString("%1").arg(normal.Z()));

                        ui->sourcePointX->setText(QString("%1").arg(pnt.X()));
                        ui->sourcePointY->setText(QString("%1").arg(pnt.Y()));
                        ui->sourcePointZ->setText(QString("%1").arg(pnt.Z()));
                    }

                    if(!ui->targetNormalX->text().isEmpty() && !ui->sourceNormalX->text().isEmpty() )
                    {
                        //计算直线旋转矩阵

                        myMoveState = CompleteState;
                        this->setWindowTitle(QObject::tr("选面完成"));
                        this->setFocus();
                    }else
                    {
                        myMoveState = StartState;
                        this->setWindowTitle(QObject::tr("请选面"));
                    }

                }else if(selectObj.ShapeType() == TopAbs_VERTEX)
                 {
                    TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
                    gp_Pnt pnt = BRep_Tool::Pnt(vertex);

                    int index = ui->comboBoxSurfaceIndex->currentIndex();
                    if(index==0)
                    {
                        ui->targetPointX->setText(QString("%1").arg(pnt.X()));
                        ui->targetPointY->setText(QString("%1").arg(pnt.Y()));
                        ui->targetPointZ->setText(QString("%1").arg(pnt.Z()));

                        ui->targetPointX->setText(QString("%1").arg(pnt.X()));
                        ui->targetPointY->setText(QString("%1").arg(pnt.Y()));
                        ui->targetPointZ->setText(QString("%1").arg(pnt.Z()));
                    }else
                    {
                        ui->sourcePointX->setText(QString("%1").arg(pnt.X()));
                        ui->sourcePointY->setText(QString("%1").arg(pnt.Y()));
                        ui->sourcePointZ->setText(QString("%1").arg(pnt.Z()));

                        ui->sourcePointX->setText(QString("%1").arg(pnt.X()));
                        ui->sourcePointY->setText(QString("%1").arg(pnt.Y()));
                        ui->sourcePointZ->setText(QString("%1").arg(pnt.Z()));
                    }
                }
            }else
            {
                 QMessageBox::warning(NULL,"系统提示","请选择面");
            }

            }else if(myMoveType == LineRotateType)
            {
                // 清空原始值
                if(!selectObj.IsNull()){
                    if(selectObj.ShapeType() == TopAbs_EDGE)
                    {
                        TopoDS_Edge edge  = TopoDS::Edge(selectObj);
                        if(!edge.IsNull()){
                            Standard_Real First,  Last;
                            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge , First , Last);
                            Handle(Geom_Line) hgc =  Handle_Geom_Line::DownCast(curve);
                            // 判断是否是直线
                            if(!hgc.IsNull()){
                                gp_Pnt orginalPnt = hgc->Lin().Location();
                                gp_Dir dir = hgc->Lin().Direction();
                                ui->LineOneRotateXCoordinate->setText(QString("%1").arg(orginalPnt.X()));
                                ui->LineOneRotateYCoordinate->setText(QString("%1").arg(orginalPnt.Y()));
                                ui->LineOneRotateZCoordinate->setText(QString("%1").arg(orginalPnt.Z()));

                                ui->LineTwoRotateXCoordinate->setText(QString("%1").arg(dir.X()));
                                ui->LineTwoRotateYCoordinate->setText(QString("%1").arg(dir.Y()));
                                ui->LineTwoRotateZCoordinate->setText(QString("%1").arg(dir.Z()));
                                myXRotation = dir.X();
                                myYRotation = dir.Y();
                                myZRotation = dir.Z();

                                myXOffset = orginalPnt.X() ;
                                myYOffset = orginalPnt.Y() ;
                                myZOffset = orginalPnt.Z();

                                myMoveState = CompleteState;
                            }
                        }
                    }else
                    {
                        QMessageBox::warning(NULL,"系统提示","请选择直线");
                    }

                }

            //this->setWindowTitle(QObject::tr("选线完成"));
            //this->setFocus();
        }
    }
}

void MoveTool::setRotateAxis()
{
    if(ui->AxisXRadioButton->isChecked() )
    {
        myXRotation = 10;
        myYRotation = 0;
        myZRotation = 0;
    }else if(ui->AxisYRadioButton->isChecked() )
    {
        myXRotation = 0;
        myYRotation = 10;
        myZRotation = 0;
    }else if(ui->AxisZRadioButton->isChecked() )
    {
        myXRotation = 0;
        myYRotation = 0;
        myZRotation = 10;
    }
}
// 当执行选点的时候，根据选择的偏移模式来选择所执行的方法
void MoveTool::setPntToPntInfo()
{

}


// 设置绕点旋转信息
void MoveTool::setPntRotateInfo(){

//    Handle(AIS_Shape) aio = m_pFloatTool->aShapes.value(aShapes.size());

//    TopoDS_Shape selectObj = aio->Shape();
//    if(!selectObj.IsNull()){

//        if(selectObj.ShapeType() == TopAbs_VERTEX)
//        {
//            TopoDS_Vertex vertex  = TopoDS::Vertex(selectObj);
//            if(!vertex.IsNull()){
//                gp_Pnt vertex1 = BRep_Tool::Pnt(vertex);
//                ui->OneRotateXCoordinate->setText(QString("%1").arg(vertex1.X()));
//                ui->OneRotateYCoordinate->setText(QString("%1").arg(vertex1.Y()));
//                ui->OneRotateZCoordinate->setText(QString("%1").arg(vertex1.Z()));
//                myXRotation = vertex1.X();
//                myYRotation = vertex1.Y();
//                myZRotation = vertex1.Z();
//                gp_Vec vec = gp_Vec(vertex1 , gp_Pnt(0 , 0 , 0));
//                myXOffset = vec.X() ;
//                myYOffset = vec.Y() ;
//                myZOffset = vec.Z();
//            }
//        }
//    }
//    // 清空原始值
//    clearShowValue();
//    Handle(AIS_Shape) aio = aShapes.value(0);
//    TopoDS_Shape selectObj = aio->Shape();
//    if(!selectObj.IsNull()){

//    }
//    aShapes.clear();
//    myMoveState = CompleteState;
//    this->setWindowTitle(QObject::tr("选点完成"));
//    this->setFocus();
}

// 设置两点旋转信息
void MoveTool::setTwoPntRotateInfo(){

}


// 设置直线旋转信息
void MoveTool::setLineRotateInfo(){

}



// 点击点到点平移模式按钮
void MoveTool::on_pushButton_clicked()
{
    // 状态操作还原
    myMoveType = PntToPntMoveType;

    // 清空界面原始值
    clearShowValue();

    ui->comboBoxPointType->setCurrentIndex(0);
    this->setWindowTitle(tr("请选择第一个点"));
    ui->stackedWidget->setCurrentWidget(ui->page_2);
//    this->setWindowTitle(tr("请选取第一个点"));
//    myMoveType = PntToPntMoveType;
//    myMoveState = DefaultState;
//    // 清空界面原始值
//    clearShowValue();

//    // 记录原始点击模式
//    if(myOldActivatedStandardModes.isEmpty()){
//        clearOldActivatedStandard(true);
//    }else{
//        clearOldActivatedStandard();
//    }
//    // 切换选择模式
//    myContext->ActivateStandardMode(TopAbs_VERTEX);
//    myContext->ActivateStandardMode(TopAbs_EDGE);
//    // 切换到信息显示界面
//    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

// 点击增量模式按钮
void MoveTool::on_pushButton_2_clicked()
{
    myMoveType = DeltaMoveType;
    this->setWindowTitle(tr("增量平移"));
    // 清空界面原始值
    clearShowValue();
    // 切换界面显示
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

// 点到点模式确定按钮点击
void MoveTool::on_pushButton_3_clicked()
{
    //if(myMoveState == CompleteState){
    if(!ui->OneXCoordinate->text().isEmpty() && !ui->TwoXCoordinate->text().isEmpty() )
    {
            myXOffset = ui->OneXCoordinate->text().toDouble() - ui->TwoXCoordinate->text().toDouble();
            myYOffset = ui->OneYCoordinate->text().toDouble() - ui->TwoYCoordinate->text().toDouble();
            myZOffset = ui->OneZCoordinate->text().toDouble() - ui->TwoZCoordinate->text().toDouble();
            emit moveEnd(myXOffset , myYOffset , myZOffset);
            m_pFloatTool->clearShapes();
    }else
    {
        //myMoveState = StartState;
        this->setWindowTitle(QObject::tr("请选取目标点"));
    }


    //}
}

// 点到点模式取消按钮点击
void MoveTool::on_pushButton_4_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("请选择操作"));
    m_pFloatTool->clearShapes();
}

// 增量平移模式确定按钮点击
void MoveTool::on_pushButton_6_clicked()
{
    myXOffset = ui->XOffsetTxt->text().toDouble();
    myYOffset = ui->YOffsetTxt->text().toDouble();
    myZOffset = ui->ZOffsetTxt->text().toDouble();
    emit moveEnd(myXOffset , myYOffset , myZOffset);
    m_pFloatTool->clearShapes();
}

// 增量平移模式取消按钮点击
void MoveTool::on_pushButton_5_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("请选择操作"));
    m_pFloatTool->clearShapes();
}


// 记录原始选取类型
void MoveTool::clearOldActivatedStandard(bool isSave)
{
    myContext->DeactivateStandardMode(TopAbs_VERTEX);
    myContext->DeactivateStandardMode(TopAbs_EDGE);
    myContext->DeactivateStandardMode(TopAbs_WIRE);
    myContext->DeactivateStandardMode(TopAbs_FACE);
    myContext->DeactivateStandardMode(TopAbs_SHELL);
    myContext->DeactivateStandardMode(TopAbs_SOLID);
    myContext->DeactivateStandardMode(TopAbs_COMPOUND);
    myContext->DeactivateStandardMode(TopAbs_SHAPE);
//    // 获取当前选取模式
//    const TColStd_ListOfInteger& nowModes = myContext->ActivatedStandardModes();

//    // 清除选取模式，并保存选取模式
//    Standard_Integer SelectMode;
//    // 清除选择模式
//    for (TColStd_ListIteratorOfListOfInteger anIt (nowModes);
//         anIt.More();
//         anIt.Next())
//    {
//        SelectMode = anIt.Value();
//        switch (SelectMode)
//        {
//        case 1:
//            myContext->DeactivateStandardMode(TopAbs_VERTEX);
//            break;
//        case 2:
//            myContext->DeactivateStandardMode(TopAbs_EDGE);
//            break;
//        case 3:
//            myContext->DeactivateStandardMode(TopAbs_WIRE);
//            break;
//        case 4:
//            myContext->DeactivateStandardMode(TopAbs_FACE);
//            break;
//        case 5:
//            myContext->DeactivateStandardMode(TopAbs_SHELL);
//            break;
//        case 6:
//            myContext->DeactivateStandardMode(TopAbs_SOLID);
//            break;
//        case 7:
//            myContext->DeactivateStandardMode(TopAbs_COMPOUND);
//            break;
//        default:
//            myContext->DeactivateStandardMode(TopAbs_SHAPE);
//        }
//        if(isSave){
//            myOldActivatedStandardModes.append(anIt.Value());
//        }
//    }
}

// 将原始值清空
void MoveTool::clearShowValue(){
    ui->OneXCoordinate->setText("");
    ui->OneYCoordinate->setText("");
    ui->OneZCoordinate->setText("");
    ui->TwoXCoordinate->setText("");
    ui->TwoYCoordinate->setText("");
    ui->TwoZCoordinate->setText("");

    ui->XOffset->setText("");
    ui->YOffset->setText("");
    ui->ZOffset->setText("");

    ui->OneRotateXCoordinate->setText("");
    ui->OneRotateYCoordinate->setText("");
    ui->OneRotateZCoordinate->setText("");
    ui->OneRotateAngle->setText("");

    ui->TwoOneRotateXCoordinate->setText("");
    ui->TwoOneRotateYCoordinate->setText("");
    ui->TwoOneRotateZCoordinate->setText("");
    ui->TwoTwoRotateXCoordinate->setText("");
    ui->TwoTwoRotateYCoordinate->setText("");
    ui->TwoTwoRotateZCoordinate->setText("");
    ui->TwoRotateTxt->setText("");

    ui->LineOneRotateXCoordinate->setText("");
    ui->LineOneRotateYCoordinate->setText("");
    ui->LineOneRotateZCoordinate->setText("");
    ui->LineTwoRotateXCoordinate->setText("");
    ui->LineTwoRotateYCoordinate->setText("");
    ui->LineTwoRotateZCoordinate->setText("");

    ui->LineRotateTxt->setText("");

    ui->targetNormalX->setText("");
    ui->targetNormalY->setText("");
    ui->targetNormalZ->setText("");
    ui->targetPointX->setText("");
    ui->targetPointY->setText("");
    ui->targetPointZ->setText("");

    ui->sourcePointX->setText("");
    ui->sourcePointY->setText("");
    ui->sourcePointZ->setText("");
    ui->sourceNormalX->setText("");
    ui->sourceNormalY->setText("");
    ui->sourceNormalZ->setText("");

    ui->moveInX->setText("");
    ui->moveInY->setText("");
    ui->moveInZ->setText("");

    ui->rotateInX->setText("");
    ui->rotateInY->setText("");
    ui->rotateInZ->setText("");
}



// 绕点旋转
void MoveTool::on_pushButton_7_clicked()
{
    // 切换到绕点旋转模式
    myMoveType = PntRotateType;
    // 清空界面原始值
    clearShowValue();

    this->setWindowTitle(tr("请选择第一个点"));
    ui->stackedWidget->setCurrentWidget(ui->page_4);





}


// 绕点旋转确定按钮点击
void MoveTool::on_pushButton_8_clicked()
{
    if(myMoveState == CompleteState){
        myRotation = ui->OneRotateAngle->text().toDouble();
        emit rotateEnd(myXOffset , myYOffset , myZOffset ,myXRotation , myYRotation , myZRotation , myRotation);
        m_pFloatTool->clearShapes();
    }
}

// 绕点旋转取消按钮点击
void MoveTool::on_pushButton_9_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("请选择操作"));
    m_pFloatTool->clearShapes();
}


// 两点旋转
void MoveTool::on_pushButton_11_clicked()
{

    // 状态操作还原
    myMoveType = TwoPntRotateType;

    // 清空界面原始值
    clearShowValue();

    ui->comboBoxRotationPoint->setCurrentIndex(0);
    this->setWindowTitle(tr("请选择第一个点"));
    // 切换界面显示
    ui->stackedWidget->setCurrentWidget(ui->page_5);
}

//ronglk
// 模型平移到面
void MoveTool::on_pushButton_16_clicked()
{

    // 状态操作还原
    myMoveType = MoveToSurfaceType;

    // 清空界面原始值
    clearShowValue();
    this->setWindowTitle(tr("请选择目标面"));
    // 切换界面显示
    ui->stackedWidget->setCurrentWidget(ui->page_7);
}
//end

// 两点旋转确定
void MoveTool::on_pushButton_12_clicked()
{
    if(myMoveState == CompleteState){
        myRotation = ui->TwoRotateTxt->text().toDouble();
        emit rotateEnd(myXOffset , myYOffset , myZOffset ,myXRotation , myYRotation , myZRotation , myRotation);
        m_pFloatTool->clearShapes();
    }
}

// 两点旋转取消
void MoveTool::on_pushButton_13_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("请选择操作"));
    m_pFloatTool->clearShapes();
}

// 直线旋转
void MoveTool::on_pushButton_10_clicked()
{
    // 切换到绕点旋转模式
    myMoveType = LineRotateType;
    // 清空界面原始值
    clearShowValue();

    this->setWindowTitle(tr("请选择第一条直线"));
    // 切换界面显示
    ui->stackedWidget->setCurrentWidget(ui->page_6);

}

// 直线旋转确定
void MoveTool::on_pushButton_14_clicked()
{
    if(myMoveState == CompleteState){
        myRotation = ui->LineRotateTxt->text().toDouble();
        emit rotateEnd(myXOffset , myYOffset , myZOffset, myXRotation , myYRotation , myZRotation , myRotation);
        m_pFloatTool->clearShapes();
    }
}

// 直线旋转取消
void MoveTool::on_pushButton_15_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("请选择操作"));
    m_pFloatTool->clearShapes();
}

// 模型平移到面确定
void MoveTool::on_pushButton_17_clicked()
{
    if(myMoveState == CompleteState){
        //Geom_Line(const gp_Lin& L);
        Standard_Real targetXp = ui->targetPointX->text().toDouble();
        Standard_Real targetYp = ui->targetPointY->text().toDouble();
        Standard_Real targetZp = ui->targetPointZ->text().toDouble();
        gp_Pnt targetP(targetXp, targetYp, targetZp);
        Standard_Real targetXv = ui->targetNormalX->text().toDouble();
        Standard_Real targetYv = ui->targetNormalY->text().toDouble();
        Standard_Real targetZv = ui->targetNormalZ->text().toDouble();
        gp_Dir targetV(targetXv, targetYv, targetZv);
        gp_Lin targetL(targetP, targetV);

        Standard_Real sourceXp = ui->sourcePointX->text().toDouble();
        Standard_Real sourceYp = ui->sourcePointY->text().toDouble();
        Standard_Real sourceZp = ui->sourcePointZ->text().toDouble();
        gp_Pnt sourceP(sourceXp, sourceYp, sourceZp);
        Standard_Real sourceXv = ui->sourceNormalX->text().toDouble();
        Standard_Real sourceYv = ui->sourceNormalY->text().toDouble();
        Standard_Real sourceZv = ui->sourceNormalZ->text().toDouble();
        gp_Dir sourceV(sourceXv, sourceYv, sourceZv);
        gp_Lin sourceL(sourceP, sourceV);

        gp_Dir normalV = sourceV.Crossed(targetV);

        gp_Ax1 A1(sourceP, normalV);
        Standard_Real Ang = sourceV.Angle(targetV);
        sourceV.Rotate(A1, Ang);

        myXRotation = normalV.X();
        myYRotation = normalV.Y();
        myZRotation = normalV.Z();
        myXOffset = sourceXp;
        myYOffset = sourceYp;
        myZOffset = sourceZp;
        myRotation = Ang * 180 / M_PI;

        emit rotateEnd(myXOffset , myYOffset , myZOffset ,myXRotation , myYRotation , myZRotation , myRotation);

//        myXOffset = targetXp - sourceXp;
//        myYOffset = targetYp - sourceYp;
//        myZOffset = targetZp - sourceZp;
//        emit moveEnd(myXOffset , myYOffset , myZOffset);

        m_pFloatTool->clearShapes();
    }
}

// 模型平移到面取消
void MoveTool::on_pushButton_18_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("请选择操作"));
    m_pFloatTool->clearShapes();
}
