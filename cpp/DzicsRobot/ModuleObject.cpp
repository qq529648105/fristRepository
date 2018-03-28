/****************************************************************************
**
**  Created By:  Daiyl  at  2014-2-19
**  Copyright (c) 2014 Dzics.com, All rights reserved.
**
****************************************************************************/

#include "ModuleObject.h"

ModuleObject::ModuleObject(QObject *parent) :
    QObject(parent)
{
    type=0;
    pos[0]=pos[1]=pos[2]=0;
    rotatePoint[0]=rotatePoint[1]=rotatePoint[2]=0;
    rotateDirection[0]=rotateDirection[1]=0; rotateDirection[2]=1;
    rotateAngle=0;
    transparency=0;
    material=0;
    color=0;
    isHidden=false;
    isShapeLoaded=false;
}

ModuleObject::~ModuleObject()
{
    qDebug("del");
    if (!aisShape.IsNull()) {
        aisShape->Shape().Free();
        delete aisShape;
    }
}

int ModuleObject::updateModouleMatrix()
{
    this->quaternion.SetEulerAngles(gp_YawPitchRoll, this->yaw, this->pitch, this->roll) ;
    gp_Mat rotateMatrix = this->quaternion.GetMatrix();
    double a11 = rotateMatrix.Value(1, 1);
    double a12 = rotateMatrix.Value(1, 2);
    double a13 = rotateMatrix.Value(1, 3);

    double a21 = rotateMatrix.Value(2, 1);
    double a22 = rotateMatrix.Value(2, 2);
    double a23 = rotateMatrix.Value(2, 3);

    double a31 = rotateMatrix.Value(3, 1);
    double a32 = rotateMatrix.Value(3, 2);
    double a33 = rotateMatrix.Value(3, 3);

    this->transform.SetValues(a11, a12, a13, this->pos[0], a21, a22, a23, this->pos[1], a31, a32, a33, this->pos[2],0.01,0.001);
    return 1;
}

int ModuleObject::returnModouleOriginalLocation()
{
    TopoDS_Shape shape = this->aisShape->Shape();
    shape.Location(TopLoc_Location(this->originalTransform));
    this->aisShape->Set( shape );
    this->aisShape->Redisplay();
    this->aisShape->GetContext()->Update(this->aisShape, true);


    return 1;
}
