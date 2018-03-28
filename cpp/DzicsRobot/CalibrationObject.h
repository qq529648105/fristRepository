/****************************************************************************
**
**  Created By:  Ronglk  at  2017-6-27
**  Copyright (c) 2017 Dzics.com, All rights reserved.
**
****************************************************************************/
#ifndef CALIBRATIONOBJECT_H
#define CALIBRATIONOBJECT_H

#include <QObject>

class CalibrationObject : public QObject
{
    Q_OBJECT
public:
    explicit CalibrationObject(QObject *parent = 0);
    ~CalibrationObject();


    QString id;
    QString stationId;
    int calibrationNum;
    QString name;


signals:

public slots:

};


class CalibrationDetail : public QObject
{
    Q_OBJECT
public:
    CalibrationDetail(QObject *parent = 0);
    ~CalibrationDetail();
    QString id;
    QString calibrationId;
    QString name;
    QString point[9];
    int pnt_index;  //模型上选点时，对应的选取点位号
//    QString point2;
//    QString point3;
//    QString point4;
//    QString point5;
//    QString point6;
//    QString point7;
//    QString point8;
//    QString point9;

    double pointX[9];
    double pointY[9];
    double pointZ[9];

    double falaX;
    double falaY;
    double falaZ;
    double falaQ1;
    double falaQ2;
    double falaQ3;
    double falaQ4;

    double toolPosX;
    double toolPosY;
    double toolPosZ;
    double toolQ1;
    double toolQ2;
    double toolQ3;
    double toolQ4;

//    double toolVx1;
//    double toolVx2;
//    double toolVx3;
//    double toolVy1;
//    double toolVy2;
//    double toolVy3;
//    double toolVz1;
//    double toolVz2;
//    double toolVz3;
    double toolKg;
    double toolMassX;
    double toolMassY;
    double toolMassZ;

    double sensorPosX;
    double sensorPosY;
    double sensorPosZ;
    double sensorDirectionX;
    double sensorDirectionY;
    double sensorDirectionZ;
    double sensorReads;
    double sensorDistance;
};

#endif // CALIBRATIONOBJECT_H
