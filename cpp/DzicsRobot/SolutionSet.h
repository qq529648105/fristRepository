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
    ALARMEXPORT =4,  //�澯���뵼��
    ALARMIMPORT =5,  //�澯���뵼��
    KNIFEEXPORT =6,    //���߿⵼��
    KNIFEIMPORT = 7,    //���߿⵼��
    CONFIGURATIONEXPORT = 8,    //�����������
    CONFIGURATIONIMPORT = 9    //�����������
};
class QString;
class SolutionSet : public QThread
{

    Q_OBJECT
public:
    SolutionSet(){};
    SolutionSet(TASKTYPE taskType, QString filename, QString id = "");
    bool isTaskSuccess;
    QString newStationId;
    QString newStationName;
    //chenyj update
    bool importCsvToAbrasion(const QString &csvFile);
    bool exportAbrasionToCsv(const QString& csvFile);
    /*******************update9 by ************************/
    bool exportErrorCodeToCsv(const QString& csvFile);
    bool importCsvToErrorCode(const QString &csvFile);
    bool exportKnifeToCsv(const QString& csvFile);
    bool importCsvToKnife(const QString &csvFile);

    bool exportErrorCode();
    bool importErrorCode(const QString &dbName);
    bool exportKnife();
    bool importKnife(const QString &dbName);

    /***************************/

    int safeAreaImport(QString path);
    int safeAreaExport(QString path);
    bool exportConfiguration();
private:

    TASKTYPE taskType;
    QString backDb;
    QString compressFile;
    QString id;
//    QString curStationId;
//    QString exportRobotId;
    QStringList exportFiles;
    bool tranFlag;
    bool compressFiles(QString comPressedFile, QStringList &srcFile, TASKTYPE type);
    bool importProgramDb(QString solDb);
    bool exportProgramDb(QString currentStationId);
/***********update8 byc xs************/
  //  bool PraseFileName(QString fileName);
   QString praseDirPath(QString fileName);
    bool exportRobotDb();
    bool importRobotDb(const QString &dbName);

/************end************/
//�ӿ�
//     void setTaskExport(QString currentStationId, QString &path, QStringList& fileList);
     void setTaskExport(QString currentStationId, QString &path);
     void setTaskImport(QString compressedfile);
     bool exportSolution(QString srcFile,TASKTYPE type);
     bool importSolution(QString compressedfile, TASKTYPE type);

/************update8 by cxs****************/
     void setTaskRobotExport(QString &robotId,QString &path);
     void setTaskRobotImport(const QString &path);
//     bool exportRobot();
//     bool importRobot(QString &path);
 /*********************end*******************/
 protected:
     void run();
};

#endif // SOLUTIONSET_H
