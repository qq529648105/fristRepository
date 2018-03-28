#ifndef SOLUTIONSET_H
#define SOLUTIONSET_H
#include <stdio.h>
#include <QFile>
#include <QThread>
#include <QStringList>

enum TASKTYPE{

    TASKEXPORT =0,      //方案导出
    TASKIMPORT =1,      //方案导入
    ROBOTEXPORT =2,     //机器人导出
    ROBOTIMPORT =3,      //机器人导入
    ALARMCODEEXPORT =4,  //告警代码导出
    ALARMCODEIMPORT =5,  //告警代码导入
    KNIFEEXPORT =6,    //刀具库导出
    KNIFEIMPORT = 7    //刀具库导入
};
class QString;
class SolutionSet : public QThread
{

    Q_OBJECT
private:

public:
    TASKTYPE taskType;
    SolutionSet();
    bool   isTaskSuccess;
    QString backDb;
    QString compressFile;
    QString curStationId;
    QStringList exportFiles;
    QString newStationId;
    QString newStationName;
    bool tranFlag;
    bool compressFiles(QString comPressedFile, QStringList &srcFile, TASKTYPE type);
    bool importProgramDb(QString solDb);
    bool exportProgramDb(QString currentStationId);
/***********update8 byc xs************/
  //  bool PraseFileName(QString fileName);
   QString praseDirPath(QString fileName);
    bool exportRobotDb();
    bool importRobotDb(const QString &dbName);
    bool exportErrorCode();
    bool importErrorCode(const QString &dbName);
    bool exportKnife();
    bool importKnife(const QString &dbName);

/************end************/
//接口
     void setTaskExport(QString currentStationId, QString &path, QStringList& fileList);
     void setTaskImport(QString compressedfile);
     bool exportSolution(QString srcFile,TASKTYPE type);
     bool importSolution(QString compressedfile, TASKTYPE type);
 /************update8 by cxs****************/
     void setTaskRobotExport(QString &path);
     void setTaskRobotImport(const QString &path);
//     bool exportRobot();
//     bool importRobot(QString &path);
 /*********************end*******************/
 protected:
     void run();
};

#endif // SOLUTIONSET_H
