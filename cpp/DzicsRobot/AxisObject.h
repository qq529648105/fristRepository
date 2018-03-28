/****************************************************************************
**
**  Created By:  Daiyl  at  2014-2-19
**  Copyright (c) 2014 Dzics.com, All rights reserved.
**
****************************************************************************/

#ifndef AXISOBJECT_H
#define AXISOBJECT_H

#include <QObject>

class AxisObject : public QObject
{
    Q_OBJECT
public:
    explicit AxisObject(QObject *parent = 0);
    ~AxisObject();

    QString id;
    QString sign;
    QString name;
    QString parentId;
    int type;
    double start[3];
    double end[3];
    double stepValue[3];
    double move[3];
    double originOffset;    // Áã×ø±êÆ«ÒÆÖµ
    double value;
    int symbol;
    double minValue;
    double maxValue;
    int cur_robotindex;
    QList<QString> subAxisList;
    QList<QString> moduleList;

signals:
    
public slots:
    
};

#endif // AXISOBJECT_H
