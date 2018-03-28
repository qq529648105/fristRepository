/****************************************************************************
**
**  Created By:  Daiyl  at  2014-2-19
**  Copyright (c) 2014 Dzics.com, All rights reserved.
**
****************************************************************************/

#ifndef MODULEOBJECT_H
#define MODULEOBJECT_H

#include <QObject>
#include <QDebug>
#include <gp_Trsf.hxx>
#include <AIS_Shape.hxx>
#include <gp_Quaternion.hxx>
#include <AIS_InteractiveContext.hxx>
#include <AIS_InteractiveContext.hxx>
class ModuleObject : public QObject
{
    Q_OBJECT
public:
    explicit ModuleObject(QObject *parent = 0);
    ~ModuleObject();

    int updateModouleMatrix();
    int returnModouleOriginalLocation();
    QString id;
    QString code;
    QString name;
    int type;
    QString filename;
    QString bindAxis;
    double pos[3];
    double rotatePoint[3];
    double rotateDirection[3];
    double rotateAngle;
    double transparency;
    int material;
    int color;
    bool isHidden;
    QString parentId;
    gp_Trsf transform;

    gp_Quaternion quaternion;
    double yaw;           //最终旋转
    double pitch;         //最终旋转
    double roll;          //最终旋转


    TopLoc_Location currentLoc;//现在的位置
    gp_Trsf originalTransform;//初始的位置
    Handle_AIS_Shape aisShape;
    bool isShapeLoaded;  // 组件是否已加载

signals:
    
public slots:
    
};

#endif // MODULEOBJECT_H
