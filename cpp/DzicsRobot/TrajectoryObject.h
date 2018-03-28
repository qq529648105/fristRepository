#ifndef TRAJECTORYOBJECT_H
#define TRAJECTORYOBJECT_H

#include <QObject>
#include "DzicsMotion.h"
class TrajectoryObject : public QObject
{
    Q_OBJECT
public:
    explicit TrajectoryObject(QObject *parent = 0);
    ~TrajectoryObject();
    void serialize(bsplines nurbs, char fileName[256]);
    void deserialize(bsplines &nurbs, char fileName[256]);
public:
    QString id;
    QString stationId;
    int trajectoryNum;   				    //轨迹类别号
    QString name;   				    //轨迹名称

signals:
    
public slots:
    
};


class TrajectoryDetail : public QObject
{
    Q_OBJECT
public:
    TrajectoryDetail(QObject *parent = 0);
    ~TrajectoryDetail();
    QString id;
    QString trajectoryId;
    QString name;                                 //路径名称
    //int trajectoryNum;                            //轨迹号
    int pathType;                                 //路径类型

    QString pathFile;                           //路径文件位置(比如nurbs对象的序列化后的文件位置)
    int knifeType;
    double pointIndensity;                            //点密度

    double cutRadius;                             //切割半径 tool[3]
    double cutDepth;        			         //切割深度 tool[10]
    double cutAngle;        			      //切割角度ZincluAngle
    int in1;                                 //切割方向(1表示内切，0表示外切)
    int out1;                                 //切割方向(1表示内切，0表示外切)
    double precision1;                            //精度1（u0）
    double precision2;                            //精度2（u1）

    double knifeX;                                //刀具X坐标 tool[4]
    double knifeY;                                //刀具Y坐标 tool[5]
    double knifeZ;                                //刀具Z坐标 tool[6]
    double knifeDirectionX;                            //刀具方向X tool[17]
    double knifeDirectionY;                            //刀具方向Y tool[18]
    double knifeDirectionZ;                            //刀具方向Z tool[19]
    double longBase;                              //刀具数据长边 tool[7]
    double height;                                //刀具数据高 tool[8]
    double shortBase;                             //刀具数据短边 tool[9]

    double FalaX;                                 //法兰盘X坐标 tool[0]
    double FalaY;                                 //法兰盘Y坐标 tool[1]
    double FalaZ;                                 //法兰盘Z坐标 tool[2]
    double FalaRX;                                //法兰盘RX姿态角 tool[14]
    double FalaRY;                                //法兰盘RY姿态角
    double FalaRZ;                                //法兰盘RZ姿态角

    double others1;                                 //备用1 tool[11]
    double others2;                                 //备用2 tool[12]
    double others3;                                 //备用3 tool[13]
};

#endif // TRAJECTORYOBJECT_H
