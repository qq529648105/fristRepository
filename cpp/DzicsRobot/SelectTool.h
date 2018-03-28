#ifndef SELECTTOOL_H
#define SELECTTOOL_H

#include <QMainWindow>
#include <AIS_InteractiveContext.hxx>
#include <AIS_Shape.hxx>
#include <AIS_Dimension.hxx>
#include <QMouseEvent>
#include <TopoDS_Edge.hxx>
#include <TopoDS_Wire.hxx>
#include <TopoDS_Vertex.hxx>
#include <Geom_BSplineCurve.hxx>

namespace Ui {
class SelectTool;
}

struct wirecurve{
    TopoDS_Edge edge;// 边
    gp_Pnt startPnt;// 起始点
    gp_Pnt endPnt;// 结束点
    bool operator == (const wirecurve& other) const{
        if(edge.IsEqual(other.edge)){
            return true;
        }
        return false;
    }
} ;

enum SelectToolType {DefaultSelectType , PntSelectType , LinSelectType , NurbsSelectType , MoreEdgeSelectType, MiddlePntSelectType, EndPntSelectType, AnyPntSelectType, EdgePntSelectType, CircleCenterPntSelectType, FaceSelectType, ComponentSelectType};
Standard_Real minDistanceAPointandSurfaceVertex(const TopoDS_Shape& S, gp_Pnt& aPnt);
class SelectTool : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit SelectTool(QWidget *parent , Handle_AIS_InteractiveContext theContext , Handle_V3d_View theView, Handle(Graphic3d_GraphicDriver) theDevice);
    ~SelectTool();
    void hideEvent(QHideEvent *);
    void showEvent(QShowEvent *);
    

private:
    // 选取类型
    //enum SelectToolType {DefaultSelectType , PntSelectType , LinSelectType , NurbsSelectType , MoreEdgeSelectType, MiddlePntSelectType, EndPntSelectType, AnyPntSelectType, EdgePntSelectType, CircleCenterPntSelectType};

signals:
    void selectPoint(gp_Pnt p);
    void selectEdge(TopoDS_Edge edge);
    void selectWire(TopoDS_Wire s);
    void selectEdges(QList<TopoDS_Edge> edges);
    void selectEdges2(QList<TopoDS_Edge> edges);
    void toolClose();
    void setLock(bool x);

private slots:
    void mouseLeftButtonClick(QMouseEvent*e);

    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_9_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_11_clicked();

    void on_pushButton_10_clicked();

    void on_pushButton_13_clicked();

    void on_pushButton_12_clicked();

    void on_pushButton_14_clicked();

    void on_pushButton_15_clicked();

    void on_pushButton_16_clicked();

    void on_pushButton_18_clicked();

    void on_pushButton_19_clicked();

    void on_pushButton_20_clicked();

    void on_pushButton_17_clicked();

    void on_pushButton_22_clicked();

    void on_pushButton_21_clicked();

    void on_pushButton_23_clicked();
    void initParam(QString str);

    void on_pushButton_24_clicked();
    void on_pushButton_25_clicked();
    void on_pushButton_26_clicked();
    void on_pushButton_27_clicked();
    void on_pushButton_28_clicked();

private:
    void clearOldActivatedStandard(bool isSave = false);
    void setPntInfo(Handle(AIS_Shape));
    void setEdgeInfo(TopoDS_Edge edge);
    void setWireInfo(TopoDS_Wire wire, TopoDS_Vertex s, TopoDS_Vertex m, TopoDS_Vertex e);
    void clearValue();
    QString getEdgeType(TopoDS_Edge edge);
    void getNurbsData(Handle_Geom_BSplineCurve aBSpline);
    bool pntOnEdge(gp_Pnt p1 , gp_Pnt p2);
    void setWireInfo(TopoDS_Wire wire);
    void setMoreEdgeInfo(TopoDS_Edge edge);
    void setStartCutInfo();
    void setMiddleCutInfo();
    void clearMoreEdgesSelect();
    void setEndCutInfo();
    void addMoreEdgesInfo();
    void refreshMoreSelectList();
//    QList<bsplines> wireToNurbs(TopoDS_Wire wire);
//    bsplines convertToNurbs(Handle_Geom_BSplineCurve aBSpline);
    QList<TopoDS_Edge> getWireEdge(gp_Pnt currentPnt , gp_Pnt m , gp_Pnt e , QList<wirecurve> edgeList , QList<wirecurve> allList, bool isMiddle , bool isEnd , QList<TopoDS_Edge> returnList);
    bool findAPointInTheSurface(const TopoDS_Shape& S, gp_Pnt& P);
    //Standard_Real minDistanceAPointandSurfaceVertex(const TopoDS_Shape& S, gp_Pnt& aPnt);
    void showSelectPoint(gp_Pnt& aPnt);
private:
    Ui::SelectTool *ui;
    Handle(AIS_InteractiveContext) myContext;
    Handle(V3d_View) myView;
    Handle(Graphic3d_GraphicDriver) myDevice;
    SelectToolType mySelectType;
    TopoDS_Shape currentShape;
    TopoDS_Vertex startVertex;
    TopoDS_Vertex middleVertex;
    TopoDS_Vertex endVertex;
    bool hasOpenContext;
    QList<Handle(AIS_Shape)> moreEdgeSelect;
};

#endif // SELECTTOOL_H
