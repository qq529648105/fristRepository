#ifndef FLOATTOOL_H
#define FLOATTOOL_H
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Dimension.hxx>
#include <AIS_LengthDimension.hxx>
#include <AIS_AngleDimension.hxx>
#include <V3d_View.hxx>
#include <Geom_Surface.hxx>
#include <Geom_Plane.hxx>
#include <Geom_Line.hxx>
#include <GeomAPI_ExtremaCurveCurve.hxx>
#include <TopoDS.hxx>
#include <TopExp_Explorer.hxx>
#include <BRep_Tool.hxx>
#include <TopoDS_Vertex.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>

#include <gp_Lin.hxx>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include "SelectTool.h"
#include "SysUtils.h"
class FloatTool
{
private:
    // 执行状态
    enum MoveState{
        DefaultState , CompleteState , StartState
    };
    // 执行类型
    //enum MoveToolType {DefaultMoveType , PntToPntLengthType  , LinLengthType , ThreePntAngleType  , TwoLinAngleType ,  TwoPlaneAngleType ,  PointToSurfaceType};

public:
    FloatTool();
    FloatTool(Handle_AIS_InteractiveContext theContext , Handle_V3d_View theView);

    void setGeometricElementSelectMode(int selectMode);
    void getGeometricElementSelectMode(int& selectMode);
    // 打开选取模式
    void openContextStandardMode();

    int selectSurfaceFromShape(TopoDS_Shape& selectShape);
    int selectLineFromShape(TopoDS_Shape& selectShape);
    double displayLineDistance(gp_Pnt& pnt1, gp_Pnt& pnt2);
    // 记录原始选取类型
    void clearOldActivatedStandard(bool isSave = false);

    int selectPointFromShape(TopoDS_Shape& selectShape, int mouseStartX, int mouseStartY);
    int selectComponentByViewRegion(int mouseStartX, int mouseStartY, int mouseEndX, int mouseEndY);
    //清除缓存选择的控件的模型
    void clearShapes();
    //清除map中保存的点
    void clearSaveAISShape();
    void updateSelectDisplayShape(int index);
    //保存到缓存
    void savePointShapes(int index);
    void saveLineShapes(int index);
    void saveSurfaceShapes(int index);
    //在缓存创建
    int createSurfaceTopoDSShape(int shapeIndex);
    //保存到硬盘
    void saveSurfaceTopoDSShape();

    // 修改曲面
    void modifySurfaceTopoDSShape(int shapeIndex);

    double displayPtnToPtnDistance(gp_Pnt& pnt1, gp_Pnt& pnt2);
    double displayThreePtnToAngle(gp_Pnt& pnt1, gp_Pnt& pnt2, gp_Pnt& pnt3);

    double displayPtnToSurfaceDistance(gp_Pnt& pnt1);
    double displayLineToLineAngle(gp_Lin& lin1, gp_Lin& lin2,const Standard_Real  theFlyout);
    //平面与平面的角度
    double displaySurfaceToSurfaceAngle(TopoDS_Face face1, TopoDS_Face face2);
    //清显示的尺度标记
    void cleanDisplayInfo();
    //重置模型
    void UpdateActualLocation();
    void ReturnOriginalLocation();
    void setCurrentTopoDSShape(TopoDS_Shape& selectShape);

private slots:
    //void mouseLeftButtonClick(QMouseEvent*e);
private:
    // 缓存界面交互
    Handle(AIS_InteractiveContext) myContext;
    Handle(V3d_View) myView;
    // 记录执行结果
    MoveState myMoveState;// 执行状态
    //MoveToolType myMoveType;// 执行模式
    SelectToolType mySelectType;
    bool hasOpenContext;
    QList<Handle(AIS_Dimension)> measureResult; // 缓存测距的控件
public:
    QList<Handle(AIS_Shape)> aShapes; // 缓存选择的控件
    QMap<int, Handle(AIS_Shape)> m_mapPointAISShape;  //保存多个点
    QMap<int, Handle(AIS_Shape)> m_mapLineAISShape;   //保存多个线
    QMap<int, Handle(AIS_Shape)> m_mapSurfaceAISShape;   //保存多个线
    QMap<int, TopoDS_Shape> m_mapSurfaceTopoDSShape;  //保存多个面
    TopoDS_Shape m_currentTopoDSShape;  //当前选中shape
    int m_pointShapeIndex;
};

#endif // FLOATTOOL_H
