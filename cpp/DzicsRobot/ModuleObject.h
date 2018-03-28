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
    double yaw;           //������ת
    double pitch;         //������ת
    double roll;          //������ת


    TopLoc_Location currentLoc;//���ڵ�λ��
    gp_Trsf originalTransform;//��ʼ��λ��
    Handle_AIS_Shape aisShape;
    bool isShapeLoaded;  // ����Ƿ��Ѽ���

signals:
    
public slots:
    
};

#endif // MODULEOBJECT_H
