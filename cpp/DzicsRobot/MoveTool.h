#ifndef MOVETOOL_H
#define MOVETOOL_H

#include <QMainWindow>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <TColgp_Array1OfDir.hxx>
#include <Poly_Connect.hxx>
#include <GProp_PGProps.hxx>
#include <StdPrs_ToolShadedShape.hxx>
#include <Poly_Array1OfTriangle.hxx>
#include <Poly_Triangulation.hxx>
#include <BRepTools.hxx>
#include <GeomLProp_SLProps.hxx>
#include <QMouseEvent>
#include "FloatTool.h"
namespace Ui {
class MoveTool;
}

class MoveTool : public QMainWindow
{
    Q_OBJECT
private:
    // 执行状态
    enum MoveState{
        DefaultState , CompleteState , StartState
    };
    // 执行类型
    enum MoveToolType {DefaultMoveType , PntToPntMoveType  , DeltaMoveType , PntRotateType  , TwoPntRotateType ,  LineRotateType , MoveToSurfaceType};
    
public:
    explicit MoveTool(QWidget *parent, FloatTool *floatTool);
    ~MoveTool();
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);

signals:
    void moveEnd(double X , double Y , double Z);
    void rotateEnd(double X , double Y , double Z, double XR , double YR ,double ZR , double R);
    void toolClose();


private slots:
    void setPntInfo();
    void setRotateAxis();
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_5_clicked();

    void mouseLeftButtonClick(QMouseEvent*e);


    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();
    void on_pushButton_16_clicked();
    void on_pushButton_17_clicked();
    void on_pushButton_18_clicked();

private:
    void setPntToPntInfo();
    void clearOldActivatedStandard(bool isSave = false);
    void clearShowValue();
    void setPntRotateInfo();
    void setTwoPntRotateInfo();
    void setLineRotateInfo();
private:
    Ui::MoveTool *ui;
    FloatTool *m_pFloatTool;
    Handle(AIS_InteractiveContext) myContext;
    // 记录执行结果
    Standard_Real myXOffset , myYOffset , myZOffset , myXRotation , myYRotation , myZRotation , myRotation;
    MoveState myMoveState;// 执行状态
    MoveToolType myMoveType;// 执行模式
    //QList<Handle(AIS_Shape)> aShapes;


    QList<Standard_Integer> myOldActivatedStandardModes;

};

#endif // MOVETOOL_H
