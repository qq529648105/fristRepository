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
    // ִ��״̬
    enum MoveState{
        DefaultState , CompleteState , StartState
    };
    // ִ������
    //enum MoveToolType {DefaultMoveType , PntToPntLengthType  , LinLengthType , ThreePntAngleType  , TwoLinAngleType ,  TwoPlaneAngleType ,  PointToSurfaceType};

public:
    FloatTool();
    FloatTool(Handle_AIS_InteractiveContext theContext , Handle_V3d_View theView);

    void setGeometricElementSelectMode(int selectMode);
    void getGeometricElementSelectMode(int& selectMode);
    // ��ѡȡģʽ
    void openContextStandardMode();

    int selectSurfaceFromShape(TopoDS_Shape& selectShape);
    int selectLineFromShape(TopoDS_Shape& selectShape);
    double displayLineDistance(gp_Pnt& pnt1, gp_Pnt& pnt2);
    // ��¼ԭʼѡȡ����
    void clearOldActivatedStandard(bool isSave = false);

    int selectPointFromShape(TopoDS_Shape& selectShape, int mouseStartX, int mouseStartY);
    int selectComponentByViewRegion(int mouseStartX, int mouseStartY, int mouseEndX, int mouseEndY);
    //�������ѡ��Ŀؼ���ģ��
    void clearShapes();
    //���map�б���ĵ�
    void clearSaveAISShape();
    void updateSelectDisplayShape(int index);
    //���浽����
    void savePointShapes(int index);
    void saveLineShapes(int index);
    void saveSurfaceShapes(int index);
    //�ڻ��洴��
    int createSurfaceTopoDSShape(int shapeIndex);
    //���浽Ӳ��
    void saveSurfaceTopoDSShape();

    // �޸�����
    void modifySurfaceTopoDSShape(int shapeIndex);

    double displayPtnToPtnDistance(gp_Pnt& pnt1, gp_Pnt& pnt2);
    double displayThreePtnToAngle(gp_Pnt& pnt1, gp_Pnt& pnt2, gp_Pnt& pnt3);

    double displayPtnToSurfaceDistance(gp_Pnt& pnt1);
    double displayLineToLineAngle(gp_Lin& lin1, gp_Lin& lin2,const Standard_Real  theFlyout);
    //ƽ����ƽ��ĽǶ�
    double displaySurfaceToSurfaceAngle(TopoDS_Face face1, TopoDS_Face face2);
    //����ʾ�ĳ߶ȱ��
    void cleanDisplayInfo();
    //����ģ��
    void UpdateActualLocation();
    void ReturnOriginalLocation();
    void setCurrentTopoDSShape(TopoDS_Shape& selectShape);

private slots:
    //void mouseLeftButtonClick(QMouseEvent*e);
private:
    // ������潻��
    Handle(AIS_InteractiveContext) myContext;
    Handle(V3d_View) myView;
    // ��¼ִ�н��
    MoveState myMoveState;// ִ��״̬
    //MoveToolType myMoveType;// ִ��ģʽ
    SelectToolType mySelectType;
    bool hasOpenContext;
    QList<Handle(AIS_Dimension)> measureResult; // ������Ŀؼ�
public:
    QList<Handle(AIS_Shape)> aShapes; // ����ѡ��Ŀؼ�
    QMap<int, Handle(AIS_Shape)> m_mapPointAISShape;  //��������
    QMap<int, Handle(AIS_Shape)> m_mapLineAISShape;   //��������
    QMap<int, Handle(AIS_Shape)> m_mapSurfaceAISShape;   //��������
    QMap<int, TopoDS_Shape> m_mapSurfaceTopoDSShape;  //��������
    TopoDS_Shape m_currentTopoDSShape;  //��ǰѡ��shape
    int m_pointShapeIndex;
};

#endif // FLOATTOOL_H
