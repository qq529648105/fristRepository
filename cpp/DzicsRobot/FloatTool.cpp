#include "FloatTool.h"
#include "Global.h"



FloatTool::FloatTool()
{
}
FloatTool::FloatTool(Handle_AIS_InteractiveContext theContext , Handle_V3d_View theView)
{
    myContext = theContext;
    myView = theView;
    hasOpenContext = false;
    //myMoveType = DefaultMoveType;
    //myMoveState = DefaultState;
}


void FloatTool::setGeometricElementSelectMode(int selectMode)
{
    if(9 != selectMode)
    {
        openContextStandardMode();
        clearOldActivatedStandard();
    }

    // 设置选点模式
    switch(selectMode)
    {
        case 0: //顶点
            myContext->ActivateStandardMode(TopAbs_VERTEX);
            mySelectType = PntSelectType;
            break;
        case 1: //任意点
            myContext->ActivateStandardMode(TopAbs_FACE);
            mySelectType = AnyPntSelectType;
            break;
        case 2: //端点
            myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = EndPntSelectType;
            break;
        case 3: //中点
            myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = MiddlePntSelectType;
            break;
        case 4: //圆心点
            myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = CircleCenterPntSelectType;
            break;
        case 5: //选边界上的点
            myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = EdgePntSelectType;
            break;
        case 6: //选面
            myContext->ActivateStandardMode(TopAbs_FACE);
            mySelectType = FaceSelectType;
            break;
        case 7: //选边
            myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = LinSelectType;
            break;
        case 8: //选曲线
            myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = NurbsSelectType;
            break;
        case 9: //选组件
            //myContext->ActivateStandardMode(TopAbs_EDGE);
            mySelectType = ComponentSelectType;
        break;
        default:
            break;
    }
}

void FloatTool::getGeometricElementSelectMode(int& selectMode)
{
    selectMode = mySelectType;
}
// 打开选取模式
void FloatTool::openContextStandardMode(){
    if(!myContext->HasOpenedContext()){
        myContext->OpenLocalContext();
        //myContext->ActivateStandardMode(TopAbs_SHAPE);
    }
}

// 记录原始选取类型
void FloatTool::clearOldActivatedStandard(bool isSave)
{
    mySelectType = DefaultSelectType;
    myContext->DeactivateStandardMode(TopAbs_VERTEX);
    myContext->DeactivateStandardMode(TopAbs_EDGE);
    myContext->DeactivateStandardMode(TopAbs_WIRE);
    myContext->DeactivateStandardMode(TopAbs_FACE);
    myContext->DeactivateStandardMode(TopAbs_SHELL);
    myContext->DeactivateStandardMode(TopAbs_SOLID);
    myContext->DeactivateStandardMode(TopAbs_COMPOUND);
    myContext->DeactivateStandardMode(TopAbs_SHAPE);
}

// 鼠标点击选取点
int FloatTool::selectPointFromShape(TopoDS_Shape& selectShape, int mouseStartX, int mouseStartY)
{
    Handle(AIS_Shape) aio;
    gp_Pnt gp_pnt1;
    if(mySelectType == PntSelectType){
        // 判断选择的是否是点
        if(selectShape.ShapeType() == TopAbs_VERTEX){
            aio = new AIS_Shape(selectShape);
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
            gp_Pnt middlePnt;
            for( ; anExp.More(); anExp.Next()){
                const TopoDS_Vertex& vertex = TopoDS::Vertex(anExp.Current());
                middlePnt = BRep_Tool::Pnt(vertex);
                x_sum += middlePnt.X();
                y_sum += middlePnt.Y();
                z_sum += middlePnt.Z();
                point_count++;
            }
            middlePnt.SetX(x_sum/point_count);
            middlePnt.SetY(y_sum/point_count);
            middlePnt.SetZ(z_sum/point_count);
            TopoDS_Vertex pnt1 = BRepBuilderAPI_MakeVertex(middlePnt);
            aio = new AIS_Shape(pnt1);
        }
    }
    // 取边界点
    else if(mySelectType == EdgePntSelectType){
        // 判断是否是边
        if(selectShape.ShapeType() == TopAbs_EDGE)
        {

            TopoDS_Edge edge =TopoDS::Edge(selectShape);
            Standard_Real first,last;
            Handle(Geom_Curve) m_curve = BRep_Tool::Curve (edge,first,last);
            Handle(Geom_Line) hgc =  Handle_Geom_Line ::DownCast(m_curve);

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

            GLdouble winX = mouseStartX;
            GLdouble winY = viewport[3] - mouseStartY;
            GLdouble winZ1 = 0;
            GLdouble winZ2 = 10;
            double x1, y1, z1, x2, y2, z2;
            gp_Pnt commonPerpendicularPnt1;
            gp_Pnt commonPerpendicularPnt2;

            gluUnProject(winX, winY, winZ1, modleview, projection, viewport, &x1, &y1, &z1);
            gluUnProject(winX, winY, winZ2, modleview, projection, viewport, &x2, &y2, &z2);
            Handle(Geom_Line) aLine = new Geom_Line(gp_Pnt(x1, y1, z1), gp_Dir(x2 - x1, y2 - y1, z2 - z1));

            GeomAPI_ExtremaCurveCurve extremaCurveCurve(aLine, m_curve);
            extremaCurveCurve.NearestPoints(commonPerpendicularPnt1, commonPerpendicularPnt2);

            TopoDS_Vertex nearestVertex = BRepBuilderAPI_MakeVertex(commonPerpendicularPnt2);
            aio = new AIS_Shape(nearestVertex);

        }
    }
    // 取任意点
    else if(mySelectType == AnyPntSelectType){
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

           GLdouble winX = mouseStartX;
           GLdouble winY = viewport[3] - mouseStartY;
           GLdouble winZ1 = 0;
           GLdouble winZ2 = 10;
           double x1, y1, z1, x2, y2, z2;

           gluUnProject(winX, winY, winZ1, modleview, projection, viewport, &x1, &y1, &z1);
           gluUnProject(winX, winY, winZ2, modleview, projection, viewport, &x2, &y2, &z2);

           Handle(Geom_Line) aLine = new Geom_Line(gp_Pnt(x1, y1, z1), gp_Dir(x2 - x1, y2 - y1, z2 - z1));

           TopoDS_Face face = TopoDS::Face(selectShape);

           Handle_Geom_Surface  currentSur;

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
                 aio = new AIS_Shape(pnt1);
                 gp_pnt1 = BRep_Tool::Pnt(pnt1);
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
                     aio = new AIS_Shape(pnt1);
                     gp_pnt1 = BRep_Tool::Pnt(pnt1);
                 }else
                 {

                     aPnt = intCS.Point(1);
                     pnt1 = BRepBuilderAPI_MakeVertex(aPnt);
                     aio = new AIS_Shape(pnt1);
                     gp_pnt1 = BRep_Tool::Pnt(pnt1);

                 }


             }else
             {
                 QMessageBox::warning(NULL,"选点失败","未找到交点,请重新选取",QMessageBox::tr("确认"));
                 qDebug()<<"未找到交点";
                 return NULL;
             }

             //currentShape = pnt1;
             qDebug()<<"找到交点";


           }else
           {
               qDebug()<<"未找到交点";
           }

    }
    // 取端点
    else if(mySelectType == EndPntSelectType){
        // 判断是否是边
        if(selectShape.ShapeType() == TopAbs_EDGE){
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

                GLdouble winX = mouseStartX;
                GLdouble winY = viewport[3] - mouseStartY;
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
                aio = new AIS_Shape(pnt1);
            }else
            {
                QMessageBox::warning(NULL,"选点失败","请选圆",QMessageBox::tr("确认"));
                return NULL;
            }
        }
    }

    // 判断之前是否存在
//    bool hasAppend = true;
//    for(Standard_Integer anIt = 0 ; anIt < aShapes.size() ; anIt ++){
//        if(aShapes.value(anIt)->Shape().IsPartner(aio->Shape())){
//            hasAppend = false;
//        }
//    }
//    if(hasAppend){
//        myContext->Hilight(aio);
//        aShapes.append(aio);
//    }

    clearShapes();
    myContext->Hilight(aio);
    aShapes.append(aio);
    return 1;
}
int FloatTool::selectComponentByViewRegion(int mouseStartX, int mouseStartY, int mouseEndX, int mouseEndY)
{

    return 1;
}
// 鼠标点击选取曲面
int FloatTool::selectSurfaceFromShape(TopoDS_Shape& selectShape)
{
    Handle(AIS_Shape) aio;
    if(selectShape.ShapeType() == TopAbs_FACE)                    // 判断是面
    {
        //clearShapes();
        TopoDS_Face face = TopoDS::Face(selectShape);
        aio = new AIS_Shape(face);
        aShapes.append(aio);
        myContext->Hilight(aio);
        return 1;
    }else
    {
        return -1;
    }
}
// 鼠标点击选取线
int FloatTool::selectLineFromShape(TopoDS_Shape& selectShape)
{
    Handle(AIS_Shape) aio;
    if(selectShape.ShapeType() == TopAbs_EDGE)                    // 判断是面
    {
        clearShapes();
        TopoDS_Edge edge = TopoDS::Edge(selectShape);
        aio = new AIS_Shape(edge);
        aShapes.append(aio);
        myContext->Hilight(aio);
        return 1;
    }else
    {
        return -1;
    }


}
// 清除选取点、线、面
void FloatTool::clearShapes(){
    for(int i = 0 ; i < aShapes.size() ; i++){
        myContext->Unhilight(aShapes.value(i));
    }
    aShapes.clear();
}
void FloatTool::clearSaveAISShape()
{


    if(!m_mapPointAISShape.isEmpty())
    {
        QMap<int, Handle(AIS_Shape)>::iterator it; //遍历map
        for ( it = m_mapPointAISShape.begin(); it != m_mapPointAISShape.end(); ++it )
        {
            myContext->Unhilight(it.value());
        }
        m_mapPointAISShape.clear();
    }

    if(!m_mapSurfaceTopoDSShape.isEmpty())
    {
//        QMap<int, Handle(AIS_Shape)>::iterator it; //遍历map
//        for ( it = m_mapSurfaceTopoDSShape.begin(); it != m_mapSurfaceTopoDSShape.end(); ++it )
//        {
//            myContext->Unhilight(it.value());
//        }
        m_mapSurfaceTopoDSShape.clear();
    }

    if(!m_mapLineAISShape.isEmpty())
    {
        QMap<int, Handle(AIS_Shape)>::iterator it; //遍历map
        for ( it = m_mapLineAISShape.begin(); it != m_mapLineAISShape.end(); ++it )
        {
            myContext->Unhilight(it.value());
        }
        m_mapLineAISShape.clear();
    }

}

void FloatTool::updateSelectDisplayShape(int index)
{
    clearShapes();
    Handle(AIS_Shape) aio = new AIS_Shape(m_mapSurfaceTopoDSShape[index]);     //编号从1开始
    m_currentTopoDSShape = m_mapSurfaceTopoDSShape[index];
    aShapes.append(aio);
    myContext->Hilight(aio);
}


void FloatTool::savePointShapes(int index)
{
    if(!aShapes.isEmpty())
    {
        if(m_mapPointAISShape.contains(index))
        {
            myContext->Unhilight(m_mapPointAISShape[index]);
            m_mapPointAISShape.remove(index);

        }
        m_mapPointAISShape.insert(index, aShapes[aShapes.size()-1]);
    }
}

void FloatTool::saveLineShapes(int index)
{
    if(!aShapes.isEmpty())
    {
        if(m_mapLineAISShape.contains(index))
        {
            myContext->Unhilight(m_mapLineAISShape[index]);
            m_mapLineAISShape.remove(index);

        }
        m_mapLineAISShape.insert(index, aShapes[aShapes.size()-1]);
    }
}
void FloatTool::saveSurfaceShapes(int index)
{
    if(!aShapes.isEmpty())
    {
        if(m_mapSurfaceAISShape.contains(index))
        {
            myContext->Unhilight(m_mapSurfaceAISShape[index]);
            m_mapSurfaceAISShape.remove(index);

        }
        m_mapSurfaceAISShape.insert(index, aShapes[aShapes.size()-1]);
    }
}

int FloatTool::createSurfaceTopoDSShape(int shapeIndex)
{
    if(!m_currentTopoDSShape.IsNull())
    {
        m_mapSurfaceTopoDSShape.insert(shapeIndex, TopoDS::Face(m_currentTopoDSShape));
        return 1;
    }else
    {
        return 0;
    }
}

void FloatTool::saveSurfaceTopoDSShape()
{
    QMap<int, TopoDS_Shape>::Iterator  it;
    int tmpIndex = 0;

    foreach (TopoDS_Shape aTopoDSShape, m_mapSurfaceTopoDSShape)
    {
        char pathName[256];
        sprintf (pathName, "%s%d%s","D:\\DzicsRobot\\temp\\", tmpIndex, ".brep");

        ofstream dumpFile1(pathName);


        //pText->append("key="+it.key()+"value="+it.value());   //在文本中显示键值
        BRepTools::Write(aTopoDSShape, dumpFile1);
        BRepTools::Dump(aTopoDSShape, dumpFile1);
    }
    //m_mapTopoDSShape.clear();
    //m_surfaceIndex = 0;
    //m_mapTopoDSShape.insert(m_surfaceIndex++, TopoDS::Face(selectShape));
    //BRepTools::Write(m_aTopoDSShape, dumpFile1);
    //BRepTools::Dump(m_aTopoDSShape, dumpFile1);

//    GeomTools::Write(aHandleGeomSurface, dumpFile2);
//    GeomTools::Dump(aHandleGeomSurface, dumpFile2);
}

void FloatTool::modifySurfaceTopoDSShape(int shapeIndex)
{
    if(!m_currentTopoDSShape.IsNull())
    {
        m_mapSurfaceTopoDSShape.remove(shapeIndex);
        m_mapSurfaceTopoDSShape.insert(shapeIndex, TopoDS::Face(m_currentTopoDSShape));
    }
}

double FloatTool::displayPtnToPtnDistance(gp_Pnt& pnt1, gp_Pnt& pnt2)
{
    //clearOldActivatedStandard();
    TopoDS_Edge lineEdge = BRepBuilderAPI_MakeEdge(pnt1 , pnt2);

    // 测试自带测距
    Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
    anAspect->MakeText3d (Standard_True);
//        anAspect->SetCommonColor(Quantity_NOC_GREEN);
    //anAspect->TextAspect()->SetHeight (Draw::Atoi ("15"));

    Handle(AIS_Dimension) aDim;
    gp_Pln aWorkingPlane;

//        aWorkingPlane = gp_Pln(pnt1 ,gp_Dir(gp_Vec(pnt1 , pnt2)));
    Standard_Real x_ori, y_ori, z_ori;
    myView->Proj (x_ori, y_ori, z_ori);
    gp_Dir proj_orientation (x_ori, y_ori, z_ori);
    aWorkingPlane = gp_Pln (gp_Pnt (0, 0, 0), proj_orientation);
//      aWorkingPlane = gp_Pln (gp_Ax3 (gp::YOZ()));
    aWorkingPlane.SetLocation (pnt1);

    aDim = new AIS_LengthDimension (lineEdge, aWorkingPlane);
    aDim->SetDimensionAspect (anAspect);
    aDim->SetFlyout(15);


    // 切换到初始状态
    cleanDisplayInfo();

    measureResult.append(aDim);
    myContext->Display(aDim);
    QMap<int, Handle(AIS_Shape)>::iterator it; //遍历map
    for ( it = m_mapPointAISShape.begin(); it != m_mapPointAISShape.end(); ++it )
    {
//        Handle(AIS_Shape) aio = new AIS_Shape(it.value());     //编号从1开始
//        aShapes.append(aio);
        myContext->Hilight(it.value());
    }

    return aDim->GetValue();
}
double FloatTool::displayThreePtnToAngle(gp_Pnt& pnt1, gp_Pnt& pnt2, gp_Pnt& pnt3)
{
    //clearOldActivatedStandard();

    // 创建文字属性
    Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
    anAspect->MakeText3d (Standard_True);
//        anAspect->SetCommonColor(Quantity_NOC_GREEN);
    // 创建测量组件
    Handle(AIS_Dimension) aDim;
    aDim = new AIS_AngleDimension(pnt1 , pnt2 , pnt3);
    aDim->SetDimensionAspect (anAspect);

    Standard_Real edgeDisance1 = pnt2.Distance(pnt1);
    Standard_Real edgeDisance2 = pnt2.Distance(pnt3);

    Standard_Real theFlyout = edgeDisance2;
    if(edgeDisance1 <= edgeDisance2)
    {
        theFlyout = edgeDisance1;
        qDebug()<<"edgeDisance1"<<edgeDisance1;
    }

    aDim->SetFlyout(theFlyout);

    // 切换到初始状态
    cleanDisplayInfo();

    measureResult.append(aDim);
    myContext->Display(aDim);
    QMap<int, Handle(AIS_Shape)>::iterator it; //遍历map
    for ( it = m_mapPointAISShape.begin(); it != m_mapPointAISShape.end(); ++it )
    {
//        Handle(AIS_Shape) aio = new AIS_Shape(it.value());     //编号从1开始
//        aShapes.append(aio);
        myContext->Hilight(it.value());
    }

    return aDim->GetValue();
}
double FloatTool::displayLineToLineAngle(gp_Lin& lin1, gp_Lin& lin2,const Standard_Real  theFlyout)
{
    // 判断状态

    //clearOldActivatedStandard();
    TopoDS_Edge edge1 = BRepBuilderAPI_MakeEdge(lin1);
    TopoDS_Edge edge2 = BRepBuilderAPI_MakeEdge(lin2);
    // 创建文字属性
    Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
    anAspect->MakeText3d (Standard_True);
    // 创建测量组件
    Handle(AIS_Dimension) aDim;
    aDim = new AIS_AngleDimension(edge1 , edge2);
    aDim->SetDimensionAspect (anAspect);

    aDim->SetFlyout(theFlyout);

    // 切换到初始状态
    cleanDisplayInfo();
    // 显示
    measureResult.append(aDim);
    myContext->Display(aDim);

    QMap<int, Handle(AIS_Shape)>::iterator it; //遍历map
    for ( it = m_mapLineAISShape.begin(); it != m_mapLineAISShape.end(); ++it )
    {
//        Handle(AIS_Shape) aio = new AIS_Shape(it.value());     //编号从1开始
//        aShapes.append(aio);
        myContext->Hilight(it.value());
    }
    return aDim->GetValue();
}
double FloatTool::displayPtnToSurfaceDistance(gp_Pnt& pnt1)
{
    //clearOldActivatedStandard();
    //        myContext->ActivateStandardMode(TopAbs_SHAPE);

    TopoDS_Face face = TopoDS::Face(m_currentTopoDSShape);
    Handle(Geom_Surface) aSurface = BRep_Tool::Surface(face);
    GeomAPI_ProjectPointOnSurf Proj (pnt1, aSurface);

    Standard_Integer NumSolutions = Proj.NbPoints();
    //gp_Pnt Pn = Proj.Point(Index);
    if(NumSolutions<1)
    {
        QMessageBox::warning(NULL,"系统提示","无法找到最短距离，请重新选择曲面");
        return -1;
    }
    gp_Pnt pnt2 = Proj.NearestPoint();
    Standard_Real D = Proj.LowerDistance();


    TopoDS_Edge lineEdge = BRepBuilderAPI_MakeEdge(pnt1 , pnt2);
    // 测试自带测距
    Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
    anAspect->MakeText3d (Standard_True);
    //        anAspect->SetCommonColor(Quantity_NOC_GREEN);
    //anAspect->TextAspect()->SetHeight (Draw::Atoi ("15"));

    Handle(AIS_Dimension) aDim;
    gp_Pln aWorkingPlane;

    //        aWorkingPlane = gp_Pln(pnt1 ,gp_Dir(gp_Vec(pnt1 , pnt2)));
    Standard_Real x_ori, y_ori, z_ori;
    myView->Proj (x_ori, y_ori, z_ori);
    gp_Dir proj_orientation (x_ori, y_ori, z_ori);
    aWorkingPlane = gp_Pln (gp_Pnt (0, 0, 0), proj_orientation);
    //        aWorkingPlane = gp_Pln (gp_Ax3 (gp::YOZ()));
    aWorkingPlane.SetLocation (pnt1);

    aDim = new AIS_LengthDimension (lineEdge, aWorkingPlane);
    aDim->SetDimensionAspect (anAspect);
    aDim->SetFlyout(15);


    // 切换到初始状态
    cleanDisplayInfo();

    measureResult.append(aDim);
    myContext->Display(aDim);

//    for ( it = m_mapPointAISShape.begin(); it != m_mapPointAISShape.end(); ++it )
//    {
////        Handle(AIS_Shape) aio = new AIS_Shape(it.value());     //编号从1开始
////        aShapes.append(aio);
//        myContext->Hilight(it.value());
//    }
    return aDim->GetValue();
}

double FloatTool::displayLineDistance(gp_Pnt& pnt1, gp_Pnt& pnt2)
{
    TopoDS_Edge lineEdge = BRepBuilderAPI_MakeEdge(pnt1 , pnt2);
    // 测试自带测距
    Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
    anAspect->MakeText3d (Standard_True);

    Handle(AIS_Dimension) aDim;
    gp_Pln aWorkingPlane;


    Standard_Real x_ori, y_ori, z_ori;
    myView->Proj (x_ori, y_ori, z_ori);
    gp_Dir proj_orientation (x_ori, y_ori, z_ori);
    aWorkingPlane = gp_Pln (gp_Pnt (0, 0, 0), proj_orientation);
    aWorkingPlane.SetLocation (pnt1);

    aDim = new AIS_LengthDimension (lineEdge, aWorkingPlane);
    aDim->SetDimensionAspect (anAspect);
    aDim->SetFlyout(15);

    // 切换到初始状态
    cleanDisplayInfo();

    measureResult.append(aDim);
    myContext->Display(aDim);
    return aDim->GetValue();
}
double FloatTool::displaySurfaceToSurfaceAngle(TopoDS_Face face1, TopoDS_Face face2)
{
    // 创建文字属性
    Handle(Prs3d_DimensionAspect) anAspect = new Prs3d_DimensionAspect();
    anAspect->MakeText3d (Standard_True);
//        anAspect->SetCommonColor(Quantity_NOC_GREEN);
    // 创建测量组件
    Handle(AIS_Dimension) aDim;
    aDim = new AIS_AngleDimension(face1 , face2);
    aDim->SetDimensionAspect (anAspect);
    aDim->SetFlyout(15);

    // 切换到初始状态
    cleanDisplayInfo();

    measureResult.append(aDim);
    myContext->Display(aDim);
    return aDim->GetValue();
}
void FloatTool::cleanDisplayInfo()
{
    //aShapes.clear();
    clearShapes();

    if(!measureResult.isEmpty()){
        Handle(AIS_Dimension) shape;
        for(int i = 0 ; i < measureResult.count() ; i++){
            shape = measureResult.value(i);
            if(i == measureResult.count() -1){
    //                 myContext->Erase(shape , true);
                 myContext->Clear(shape , true);
    //                 myContext->LocalContext()->ClearOutdatedSelection(shape , true);
            }else{
    //                 myContext->Erase(shape , false);
                 myContext->Clear(shape , false);
    //                 myContext->LocalContext()->ClearOutdatedSelection(shape , false);
            }
    //            myContext->Erase(shape , false);
    //            myContext->LocalContext()->ClearOutdatedSelection(shape);
        }
        measureResult.clear();
    }
    QMap<int, Handle(AIS_Shape)>::iterator it;
    for ( it = m_mapPointAISShape.begin(); it != m_mapPointAISShape.end(); ++it )
    {
        myContext->Unhilight(it.value());
    }
}


void FloatTool::UpdateActualLocation(){


    QSqlQuery query;
    QString axisId;
    AxisObject *axBase = NULL;

    query.exec(QString("select rootAxis from StationMotion where stationId='%1'").arg(pageStation->currentStationId));
    if(query.next()) {

       axisId = query.value(0).toString();
       axBase = axisMap[axisId];
       if (!axBase || axBase->subAxisList.count()==0)
           return;


       while (!axBase->parentId.isEmpty()) {
           axBase = axisMap[axBase->parentId];
       }
       TopLoc_Location locBase;

       while (true) {
//           gp_Trsf trsfBase;
//           if (axBase->type == 1) {
//               trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
//               locBase = locBase * TopLoc_Location(trsfBase);
//           } else if(axBase->type == 2) {
//               trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
//               locBase = locBase * TopLoc_Location(trsfBase);
//           }

           for(int j = 0; j < axBase->moduleList.size(); ++j) {
               //ronglk 模型位置更新
               Handle_AIS_Shape anIS = moduleMap[axBase->moduleList[j]]->aisShape;

               myContext->ResetLocation(anIS);

               TopoDS_Shape shape = anIS->Shape();
               shape.Location(TopLoc_Location(moduleMap[axBase->moduleList[j]]->transform));
               anIS->Set(shape);

               qDebug()<<"回到实际位置"<<axBase->moduleList[j];

               moduleMap[axBase->moduleList[j]]->aisShape->Redisplay(true);
               myContext->Activate(moduleMap[axBase->moduleList[j]]->aisShape);

              }
           if (axBase->subAxisList.isEmpty()) {
               break;
           }
           axBase = axisMap[axBase->subAxisList.at(0)];
       }
   } else {
       return;
   }
    pageViewer->setViewUpdateState(true);
    myView->Redraw();
}


void FloatTool::ReturnOriginalLocation(){


    QSqlQuery query;
    QString axisId;
    AxisObject *axBase = NULL;

    query.exec(QString("select rootAxis from StationMotion where stationId='%1'").arg(pageStation->currentStationId));
    if(query.next()) {

       axisId = query.value(0).toString();
       axBase = axisMap[axisId];
       if (!axBase || axBase->subAxisList.count()==0)
           return;


       while (!axBase->parentId.isEmpty()) {
           axBase = axisMap[axBase->parentId];
       }
       TopLoc_Location locBase;

       while (true) {
//           gp_Trsf trsfBase;
//           if (axBase->type == 1) {
//               trsfBase.SetTranslation((axBase->value - axBase->originOffset) * axBase->symbol * gp_Vec(axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z]));
//               locBase = locBase * TopLoc_Location(trsfBase);
//           } else if(axBase->type == 2) {
//               trsfBase.SetRotation(gp_Ax1(gp_Pnt(axBase->start[X] + moduleMap[axBase->moduleList.last()]->pos[X], axBase->start[Y] + moduleMap[axBase->moduleList.last()]->pos[Y], axBase->start[Z] + moduleMap[axBase->moduleList.last()]->pos[Z]), gp_Dir( axBase->end[X]-axBase->start[X], axBase->end[Y]-axBase->start[Y], axBase->end[Z]-axBase->start[Z])), (axBase->value - axBase->originOffset) * axBase->symbol * M_PI / 180);
//               locBase = locBase * TopLoc_Location(trsfBase);
//           }

           for(int j = 0; j < axBase->moduleList.size(); ++j) {
               //ronglk 模型位置更新
               Handle_AIS_Shape anIS = moduleMap[axBase->moduleList[j]]->aisShape;

               TopoDS_Shape shape = anIS->Shape();
               shape.Location(TopLoc_Location(moduleMap[axBase->moduleList[j]]->originalTransform));
               anIS->Set(shape);

               qDebug()<<"重置初始位置"<<axBase->moduleList[j];

               moduleMap[axBase->moduleList[j]]->aisShape->Redisplay(true);
               myContext->Activate(moduleMap[axBase->moduleList[j]]->aisShape);

               myContext->SetLocation(moduleMap[axBase->moduleList[j]]->aisShape, moduleMap[axBase->moduleList[j]]->transform);

              }
           if (axBase->subAxisList.isEmpty()) {
               break;
           }
           axBase = axisMap[axBase->subAxisList.at(0)];
       }
   } else {
       return;
   }
    pageViewer->setViewUpdateState(true);
    myView->Redraw();
}
void FloatTool::setCurrentTopoDSShape(TopoDS_Shape& selectShape)
{
    qDebug()<<"FloatTool setCurrentTopoDSShape sucess!";
    m_currentTopoDSShape = selectShape;
}
