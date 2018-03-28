/****************************************************************************
**
**  Created By:  Daiyl  at  2013-12-30
**  Copyright (c) 2013 Dzics.com, All rights reserved.
**
****************************************************************************/

#ifndef MODULEIMPORTTHREAD_H
#define MODULEIMPORTTHREAD_H

#include <QThread>
//#include <QString>
#include <QMutex>
//#include <QObject>

#include <Handle_AIS_Shape.hxx>
#include "ModuleObject.h"

//struct ShapeStruct
//{
//    QString id;
//    QString code;
//};

class ModuleImportThread : public QThread
{
    Q_OBJECT
public:
    ModuleImportThread(QString filename, ModuleObject *module);
    ~ModuleImportThread(void);

    virtual void run();

signals:
    void importFinish( QString id );

private:
    QString filename;
    ModuleObject *module;
//    QString id;
//    Handle_AIS_Shape aisShape;
//    gp_Trsf transform;
//    QString moduleFileName;


};

#endif // MODULEIMPORTTHREAD_H
