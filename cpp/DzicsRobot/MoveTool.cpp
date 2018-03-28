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
// ��¼ԭʼ���ģʽ
//QList<Standard_Integer> myOldActivatedStandardModes;

MoveTool::MoveTool(QWidget *parent, FloatTool *floatTool) :
    QMainWindow(parent),
    ui(new Ui::MoveTool)
{
    m_pFloatTool = floatTool;
    myMoveType = DefaultMoveType;
    myMoveState = DefaultState;
    // �Զ��ݻٿؼ�
//    this->setAttribute(Qt::WA_DeleteOnClose, true);
    // ���ô���İ�ť
    this->setWindowFlags(windowFlags() & ~Qt::WindowMinMaxButtonsHint);
    ui->setupUi(this);

    // ���ֵ�������֤
    QRegExp rx("^(-?\\d+)(\\.\\d+)?$");
    QRegExpValidator *pRevalidotor = new QRegExpValidator(rx, this);
    // �������ı�����������֤
    ui->XOffsetTxt->setValidator(pRevalidotor);
    ui->YOffsetTxt->setValidator(pRevalidotor);
    ui->ZOffsetTxt->setValidator(pRevalidotor);

    // Ĭ��ѡ��ģʽ
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    QRect widgetRect=parent->rect();
    QPoint panelPos=QPoint(widgetRect.left() + widgetRect.width() - this->width() - 10, widgetRect.top());
    panelPos=parent->mapToGlobal(panelPos);
    move(panelPos);


    ui->comboBoxPointType->addItem("�����",0);
    ui->comboBoxPointType->addItem("Դ��",1);

    ui->comboBoxRotationPoint->addItem("��һ����",0);
    ui->comboBoxRotationPoint->addItem("�ڶ�����",1);

    ui->comboBoxSurfaceIndex->addItem("Ŀ����",0);
    ui->comboBoxSurfaceIndex->addItem("Դ��",1);

    ui->AxisXRadioButton->setChecked(true);
    connect(ui->AxisXRadioButton,SIGNAL(clicked(bool)),this,SLOT(setRotateAxis()));
    connect(ui->AxisYRadioButton,SIGNAL(clicked(bool)),this,SLOT(setRotateAxis()));
    connect(ui->AxisZRadioButton,SIGNAL(clicked(bool)),this,SLOT(setRotateAxis()));
}

MoveTool::~MoveTool()
{
    // ��ԭѡȡģʽ
    if(!myOldActivatedStandardModes.isEmpty()){
        clearOldActivatedStandard();
        Standard_Integer SelectMode;
        // ���½�ԭ����ѡ��ģʽ��ֵ��ȥ
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


// ����Ӱ�ص�ʱ�򴥷�
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

// �����ѡȡ��
void MoveTool::mouseLeftButtonClick(QMouseEvent *e)
{

}

// ��ִ��ѡ���ʱ�򣬸���ѡ���ƫ��ģʽ��ѡ����ִ�еķ���
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
                this->setWindowTitle(QObject::tr("ѡ�����"));
                this->setFocus();
            }else
            {
                myMoveState = StartState;
                this->setWindowTitle(QObject::tr("��ѡȡĿ���"));
            }
        }else if(myMoveType == MoveToSurfaceType)
        {
            // ���ԭʼֵ
            if(!selectObj.IsNull())
            {
                if(selectObj.ShapeType() == TopAbs_FACE)
                {
                    TopoDS_Face aFace = TopoDS::Face(selectObj);
                    Handle(Geom_Surface) aSurface = BRep_Tool::Surface(aFace);

                    //������ķ�����
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

                    //���������
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
                        //����ֱ����ת����

                        myMoveState = CompleteState;
                        this->setWindowTitle(QObject::tr("ѡ�����"));
                        this->setFocus();
                    }else
                    {
                        myMoveState = StartState;
                        this->setWindowTitle(QObject::tr("��ѡ��"));
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
                 QMessageBox::warning(NULL,"ϵͳ��ʾ","��ѡ����");
            }

            }else if(myMoveType == LineRotateType)
            {
                // ���ԭʼֵ
                if(!selectObj.IsNull()){
                    if(selectObj.ShapeType() == TopAbs_EDGE)
                    {
                        TopoDS_Edge edge  = TopoDS::Edge(selectObj);
                        if(!edge.IsNull()){
                            Standard_Real First,  Last;
                            Handle(Geom_Curve) curve = BRep_Tool::Curve(edge , First , Last);
                            Handle(Geom_Line) hgc =  Handle_Geom_Line::DownCast(curve);
                            // �ж��Ƿ���ֱ��
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
                        QMessageBox::warning(NULL,"ϵͳ��ʾ","��ѡ��ֱ��");
                    }

                }

            //this->setWindowTitle(QObject::tr("ѡ�����"));
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
// ��ִ��ѡ���ʱ�򣬸���ѡ���ƫ��ģʽ��ѡ����ִ�еķ���
void MoveTool::setPntToPntInfo()
{

}


// �����Ƶ���ת��Ϣ
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
//    // ���ԭʼֵ
//    clearShowValue();
//    Handle(AIS_Shape) aio = aShapes.value(0);
//    TopoDS_Shape selectObj = aio->Shape();
//    if(!selectObj.IsNull()){

//    }
//    aShapes.clear();
//    myMoveState = CompleteState;
//    this->setWindowTitle(QObject::tr("ѡ�����"));
//    this->setFocus();
}

// ����������ת��Ϣ
void MoveTool::setTwoPntRotateInfo(){

}


// ����ֱ����ת��Ϣ
void MoveTool::setLineRotateInfo(){

}



// ����㵽��ƽ��ģʽ��ť
void MoveTool::on_pushButton_clicked()
{
    // ״̬������ԭ
    myMoveType = PntToPntMoveType;

    // ��ս���ԭʼֵ
    clearShowValue();

    ui->comboBoxPointType->setCurrentIndex(0);
    this->setWindowTitle(tr("��ѡ���һ����"));
    ui->stackedWidget->setCurrentWidget(ui->page_2);
//    this->setWindowTitle(tr("��ѡȡ��һ����"));
//    myMoveType = PntToPntMoveType;
//    myMoveState = DefaultState;
//    // ��ս���ԭʼֵ
//    clearShowValue();

//    // ��¼ԭʼ���ģʽ
//    if(myOldActivatedStandardModes.isEmpty()){
//        clearOldActivatedStandard(true);
//    }else{
//        clearOldActivatedStandard();
//    }
//    // �л�ѡ��ģʽ
//    myContext->ActivateStandardMode(TopAbs_VERTEX);
//    myContext->ActivateStandardMode(TopAbs_EDGE);
//    // �л�����Ϣ��ʾ����
//    ui->stackedWidget->setCurrentWidget(ui->page_2);
}

// �������ģʽ��ť
void MoveTool::on_pushButton_2_clicked()
{
    myMoveType = DeltaMoveType;
    this->setWindowTitle(tr("����ƽ��"));
    // ��ս���ԭʼֵ
    clearShowValue();
    // �л�������ʾ
    ui->stackedWidget->setCurrentWidget(ui->page_3);
}

// �㵽��ģʽȷ����ť���
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
        this->setWindowTitle(QObject::tr("��ѡȡĿ���"));
    }


    //}
}

// �㵽��ģʽȡ����ť���
void MoveTool::on_pushButton_4_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("��ѡ�����"));
    m_pFloatTool->clearShapes();
}

// ����ƽ��ģʽȷ����ť���
void MoveTool::on_pushButton_6_clicked()
{
    myXOffset = ui->XOffsetTxt->text().toDouble();
    myYOffset = ui->YOffsetTxt->text().toDouble();
    myZOffset = ui->ZOffsetTxt->text().toDouble();
    emit moveEnd(myXOffset , myYOffset , myZOffset);
    m_pFloatTool->clearShapes();
}

// ����ƽ��ģʽȡ����ť���
void MoveTool::on_pushButton_5_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("��ѡ�����"));
    m_pFloatTool->clearShapes();
}


// ��¼ԭʼѡȡ����
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
//    // ��ȡ��ǰѡȡģʽ
//    const TColStd_ListOfInteger& nowModes = myContext->ActivatedStandardModes();

//    // ���ѡȡģʽ��������ѡȡģʽ
//    Standard_Integer SelectMode;
//    // ���ѡ��ģʽ
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

// ��ԭʼֵ���
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



// �Ƶ���ת
void MoveTool::on_pushButton_7_clicked()
{
    // �л����Ƶ���תģʽ
    myMoveType = PntRotateType;
    // ��ս���ԭʼֵ
    clearShowValue();

    this->setWindowTitle(tr("��ѡ���һ����"));
    ui->stackedWidget->setCurrentWidget(ui->page_4);





}


// �Ƶ���תȷ����ť���
void MoveTool::on_pushButton_8_clicked()
{
    if(myMoveState == CompleteState){
        myRotation = ui->OneRotateAngle->text().toDouble();
        emit rotateEnd(myXOffset , myYOffset , myZOffset ,myXRotation , myYRotation , myZRotation , myRotation);
        m_pFloatTool->clearShapes();
    }
}

// �Ƶ���תȡ����ť���
void MoveTool::on_pushButton_9_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("��ѡ�����"));
    m_pFloatTool->clearShapes();
}


// ������ת
void MoveTool::on_pushButton_11_clicked()
{

    // ״̬������ԭ
    myMoveType = TwoPntRotateType;

    // ��ս���ԭʼֵ
    clearShowValue();

    ui->comboBoxRotationPoint->setCurrentIndex(0);
    this->setWindowTitle(tr("��ѡ���һ����"));
    // �л�������ʾ
    ui->stackedWidget->setCurrentWidget(ui->page_5);
}

//ronglk
// ģ��ƽ�Ƶ���
void MoveTool::on_pushButton_16_clicked()
{

    // ״̬������ԭ
    myMoveType = MoveToSurfaceType;

    // ��ս���ԭʼֵ
    clearShowValue();
    this->setWindowTitle(tr("��ѡ��Ŀ����"));
    // �л�������ʾ
    ui->stackedWidget->setCurrentWidget(ui->page_7);
}
//end

// ������תȷ��
void MoveTool::on_pushButton_12_clicked()
{
    if(myMoveState == CompleteState){
        myRotation = ui->TwoRotateTxt->text().toDouble();
        emit rotateEnd(myXOffset , myYOffset , myZOffset ,myXRotation , myYRotation , myZRotation , myRotation);
        m_pFloatTool->clearShapes();
    }
}

// ������תȡ��
void MoveTool::on_pushButton_13_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("��ѡ�����"));
    m_pFloatTool->clearShapes();
}

// ֱ����ת
void MoveTool::on_pushButton_10_clicked()
{
    // �л����Ƶ���תģʽ
    myMoveType = LineRotateType;
    // ��ս���ԭʼֵ
    clearShowValue();

    this->setWindowTitle(tr("��ѡ���һ��ֱ��"));
    // �л�������ʾ
    ui->stackedWidget->setCurrentWidget(ui->page_6);

}

// ֱ����תȷ��
void MoveTool::on_pushButton_14_clicked()
{
    if(myMoveState == CompleteState){
        myRotation = ui->LineRotateTxt->text().toDouble();
        emit rotateEnd(myXOffset , myYOffset , myZOffset, myXRotation , myYRotation , myZRotation , myRotation);
        m_pFloatTool->clearShapes();
    }
}

// ֱ����תȡ��
void MoveTool::on_pushButton_15_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("��ѡ�����"));
    m_pFloatTool->clearShapes();
}

// ģ��ƽ�Ƶ���ȷ��
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

// ģ��ƽ�Ƶ���ȡ��
void MoveTool::on_pushButton_18_clicked()
{
    myMoveType = DefaultMoveType;
    ui->stackedWidget->setCurrentWidget(ui->page_1);
    this->setWindowTitle(tr("��ѡ�����"));
    m_pFloatTool->clearShapes();
}
