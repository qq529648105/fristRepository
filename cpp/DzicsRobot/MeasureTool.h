#ifndef MEASURETOOL_H
#define MEASURETOOL_H

#include <QMainWindow>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Dimension.hxx>
#include <V3d_View.hxx>
#include <Geom_Surface.hxx>
#include <QMap>
#include <QDockWidget>

#include "Global.h"
#include "SysUtils.h"
//#include "TopoDS_Shape.hxx"
//#include "Handle_AIS_Shape.hxx"
//#include "Handle_AIS_InteractiveContext.hxx"
//#include "Handle_TopTools_HSequenceOfShape.hxx"

#include "mesh/MeshTranslate.h"
#include "SelectTool.h"
#include "FloatTool.h"
namespace Ui {
class MeasureTool;
}

class MeasureTool : public QMainWindow
{
    Q_OBJECT
    
private:
    // ִ��״̬
    enum MoveState{
        DefaultState , CompleteState , StartState
    };
    // ִ������
    enum MeasureToolType {DefaultMeasureToolType , PntToPntLengthType  , LinLengthType , ThreePntAngleType  , TwoLinAngleType ,  TwoPlaneAngleType ,  PointToSurfaceType};

public:
    explicit MeasureTool(QWidget *parent, FloatTool *floatTool);
    ~MeasureTool();
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);

private:
    // ���ԭʼѡ��״̬
    void clearOldActivatedStandard(bool isSave = false);
    // ������������Ϣ
    void setPntToPntInfo();
    // ����ֱ�߲����Ϣ
    void setLinInfo();
    // ���������Ƕ���Ϣ
    void setThreePntInfo();
    // ������ƽ���Ƕ���Ϣ
    void setTwoPlaneInfo();
    // ���õ㵽��Ƕ���Ϣ
    void setPointToSurfaceInfo();
    // �����ʾ
    void clearShowValue();
    // ��ѡȡģʽ
    void openContextStandardMode();
    //����ģ��
    void UpdateShapes();

    //Handle(AIS_Shape) selectPointFromShape(TopoDS_Shape& selectShape, int winX, int winY);
    void selectSurfaceFromShape(TopoDS_Shape& selectShape);

private slots:
    void mouseLeftButtonClick(QMouseEvent*e);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_createSurfacePushButton_clicked();

    void updateDispalySurfaceShape();

    void on_modifySurfacePushButton_clicked();
    void setPntInfo();
signals:
    void toolClose();

private:
    Ui::MeasureTool *ui;
    FloatTool *m_pFloatTool;
    // ������潻��
    Handle(AIS_InteractiveContext) myContext;
    Handle(V3d_View) myView;
    // ��¼ִ�н��
    MoveState myMoveState;// ִ��״̬
    MeasureToolType myMeasureType;// ִ��ģʽ
    //QList<Handle(AIS_Shape)> aShapes; // ����ѡ��Ŀؼ�
    QList<Handle(AIS_Dimension)> measureResult; // ������Ŀؼ�

    Handle(Geom_Surface) aSurface; // ����

    TColStd_ListOfInteger myTestList;

    QList<Standard_Integer> myOldActivatedStandardModes;
    bool hasOpenContext;
    QMap<int, TopoDS_Shape> m_mapTopoDSShape;  //����������
    TopoDS_Shape m_currentTopoDSShape;  //����������
    //Handle(Geom_Surface) m_aHandleGeomSurface;
    //Handle(AIS_Shape) m_aHandleAISShape;
    int m_surfaceIndex;
    SelectToolType mySelectType;
//    QDockWidget *toolDock;
//    QPushButton *btn_vertexPointSelect;
//    QPushButton *btn_anyPointSelect;
//    QPushButton *btn_endPointSelect;
//    QPushButton *btn_middlePointSelect;
//    QPushButton *btn_centrePointSelect;
//    QPushButton *btn_surfaceSelect;
//    QPushButton *btn_edgePointSelect;
//    QPushButton *btn_edgeSelect;
};

#endif // MEASURETOOL_H
