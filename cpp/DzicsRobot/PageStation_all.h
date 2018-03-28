#ifndef PAGESTATION_ALL_H
#define PAGESTATION_ALL_H
#include "PageStation.h"
#include "SysUtils.h"

#include <QSqlQuery>
#include <QSqlRecord>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QDebug>
#include <QSqlError>
//rong lk
#include <QTime>
//end
#include "AxisObject.h"
#include "ModuleObject.h"
#include "ActionObject.h"
#include "RobotObject.h"
#include "RobotExport.h"
//rong lk
#include "CalibrationObject.h"
#include "TrajectoryObject.h"
#include "selectTool.h"
//end
#include <QtSliderFactory>
#include <QCustomEvent>
#include <QInputDialog>

#include <gp_Ax1.hxx>
#include <Handle_AIS_Shape.hxx>
//rong lk
#include <Geom_Axis1Placement.hxx>
#include <AIS_Axis.hxx>
#include <TopTools_LocationSet.hxx>
//end
#include "Global.h"



extern QMap<QString, AxisObject*> axisMap;
extern QMap<QString, ModuleObject*> moduleMap;
extern QMap<QString, RobotObject*> robotMap;

extern QMap<QString, ActionObject*> actionMap;
extern QMap<QString, ActionDetail*> actionDetailMap;

extern QMap<QString,int> txtSequence;

extern QStringList modulNameList;
extern QStringList axisNameList;
extern QStringList knifeList;
extern QStringList moduleIdList;
extern QStringList axisIdList;
extern QStringList knifeIdList;

extern QStringList operList;
extern QStringList actionList;
extern QStringList exterList;
extern QStringList axisTypeList;
/**************update5 by cxs 2014 5.6**********/
Q_DECLARE_METATYPE(QCmmF)
Q_DECLARE_METATYPE(QPoint4F)

/*******************end**************************/

#endif // PAGESTATION_ALL_H
