#pragma once

#include <QThread>
#include <QMutex>
#include <Handle_AIS_Shape.hxx>
#include <gp_Trsf.hxx>\

#include "ModuleObject.h"

class ModuleLoadThread :
	public QThread
{
    Q_OBJECT
public:
//    ModuleLoadThread(QString filename, Handle_AIS_Shape aisShape, gp_Trsf transform, bool isHidden);
    ModuleLoadThread(QString filename, QList<ModuleObject *> moduleList);
    ~ModuleLoadThread(void);

	virtual void run();

signals:
	void moduleLoadFinish( QString id );

private:
	QString filename;
//	Handle_AIS_Shape aisShape;
//	gp_Trsf transform;
//    bool isHidden;
    QList<ModuleObject *> moduleList;

};
