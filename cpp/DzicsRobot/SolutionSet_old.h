#ifndef SOLUTIONSET_H
#define SOLUTIONSET_H
#include <stdio.h>
#include <QFile>
#include <QThread>
#include <QStringList>

enum TASKTYPE{

    TASKEXPORT =0,      //��������
    TASKIMPORT =1,      //��������
    ROBOTEXPORT =2,     //�����˵���
    ROBOTIMPORT =3,      //�����˵���
    ALARMCODEEXPORT =4,  //�澯���뵼��
    ALARMCODEIMPORT =5,  //�澯���뵼��
    KNIFEEXPORT =6,    //���߿⵼��
    KNIFEIMPORT = 7    //���߿⵼��
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
//�ӿ�
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
