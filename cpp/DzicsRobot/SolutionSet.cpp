/******************************************
 *update8  2014.6.3
 *导入导出方案中添加与stationId有关内容
 *修改导出方案时主ID不变 导入方案时更换主ID
 *
 *
 *
 *
 *
 ********************************************/



#include "SolutionSet.h"
#include <QSqlQuery>
#include <qsqldatabase.h>
#include "SysUtils.h"
#include <stdio.h>
#include "string.h"
#include <QCoreApplication>
#include <QStringList>
#include <QSqlDriver>
#include <QSqlError>
#include <QDir>
#include <QDebug>
#include "Global.h"



/***********压缩文件头2个字节代表压缩文件的版本*********
 *
 * "11" 方案压缩格式
 * "21"  机器人方案压缩格式
 * "31" 刀具文件压缩格式
 *、"41" 告警代码文件压缩格式
 *其他字符 代表无效文件
 ***************************************************/
extern QString robotDat;

#define READMAX 1024UL*1024*100


SolutionSet::SolutionSet(TASKTYPE taskType, QString filename, QString id)
{
    isTaskSuccess = false;
    this->taskType = taskType;
    this->compressFile = filename;
    this->id = id;
}

//void SolutionSet::setTaskExport(QString currentStationId, QString &path, QStringList &fileList)
void SolutionSet::setTaskExport(QString currentStationId, QString &path)
{
     compressFile = path;
     id = currentStationId;
//     exportFiles = fileList;
     taskType = TASKEXPORT;

}


void SolutionSet::setTaskImport(QString compressedfile)
{
    compressFile = compressedfile;
    taskType =TASKIMPORT;
}


/**************update8 **************************/
void SolutionSet::setTaskRobotExport(QString &robotId,QString &path)
{
    id = robotId;
    compressFile = path;
    taskType = ROBOTEXPORT;
}


void SolutionSet::setTaskRobotImport(const QString &path)
{
    compressFile = path;
    taskType = ROBOTIMPORT;
}
/*******************************end**************/

void SolutionSet::run()
{
    if(taskType == TASKEXPORT)
    {
        if(exportSolution(compressFile,TASKEXPORT))
            isTaskSuccess = true;
        else
            isTaskSuccess = false;
    }
    else if(taskType == TASKIMPORT)
    {
        if(importSolution(compressFile,TASKIMPORT))
            isTaskSuccess = true;
        else
            isTaskSuccess = false;
    }
/**********update8 **********************/
    else if(taskType == ROBOTEXPORT){
        if(exportSolution(compressFile ,ROBOTEXPORT))
            isTaskSuccess = true;
        else
            isTaskSuccess = false;
    }
    else if(taskType == ROBOTIMPORT){
        if(importSolution(compressFile,ROBOTIMPORT))
            isTaskSuccess = true;
        else
            isTaskSuccess = false;
    }
/*********************end***************/

}
//接口
/*******************************************
 *函数 导出方案接口
 *
 *参数
 *type  TASKEXPORT 方案导出参数
 *      ROBOTEXPORT 机器人导出参数
 *      ALARMEXPORT  告警代码导出参数
 *      KNIFEEXPORT      刀具库导出
 *
 *srcFile  压缩后文件
 ********************************************/
bool SolutionSet::exportSolution(QString srcFile, TASKTYPE type)
{

    QSqlQuery query;
    if(type ==  TASKEXPORT){
        if(!exportProgramDb( id ))
        {
            qDebug()<<"导出失败";
            return false;
        }
        query.exec(QString("SELECT DISTINCT filename FROM StationModule WHERE stationId='%1'" ).arg(id));
        while(query.next())
        {
            exportFiles.append(query.value(0).toString() + ".dzm");
        }
        exportFiles.append( "Program/" + id + "/MainModule.mod" );
    }
    else if(type == ROBOTEXPORT){
        if(!exportRobotDb())
            return false;
        query.exec(QString("SELECT filename FROM RobotModule WHERE robotId ='%1'").arg(id));
        while(query.next()){
            exportFiles.append(query.value(0).toString() + ".dzm");
        }
        query.exec(QString("SELECT preview FROM RobotLibrary WHERE id ='%1'").arg(id));
        while(query.next()){
            exportFiles.append(query.value(0).toString());
        }
    }else if(type == ALARMEXPORT){
        if(!exportErrorCode())
            return false;

    }else if(type == KNIFEEXPORT){
        if(!exportKnife())
            return false;
    }

    exportFiles<<backDb;
    QFile file(compressFile);
    if(file.exists())
        file.remove();
    if(compressFiles(srcFile,exportFiles,type)) {
        return true;
    } else {
        QFile file(compressFile);
        if(file.exists())
            file.remove();
       return false;
    }
}





//接口 导入方案接口
/***************************
 *
 *参数 Compressfile  导入的方案名
 *参数 type
 *    TASKIMPORT 方案导入参数
 *    ROBOTIMPORT 机器人导入参数
 *    ALARMIMPORT  告警代码导入参数
 *    KNIFEIMPORT      刀具库导入
 *
 *****************************/
bool SolutionSet::importSolution(QString compressedfile,TASKTYPE type)
{
    QString sFName;
    QFile unCompFile(compressedfile);

    if(!unCompFile.open(QIODevice::ReadWrite))
        return false;

    QStringList programList;
    QByteArray byteversion = unCompFile.read(2);
    QString version(byteversion);

    if(version == "11"){
        if(type != TASKIMPORT)
            return false;
    }else if(version =="21"){
        if(type != ROBOTIMPORT)
            return false;
    }else if(version == "31"){
        if(type != KNIFEIMPORT)
            return false;
    }else if(version =="41"){
        if(type!=  ALARMIMPORT)
            return false;
    }else {return false;}
    while(!unCompFile.atEnd())
    {

        int sFNameLen;
        unCompFile.read((char*)&sFNameLen,sizeof(sFNameLen));
        qDebug()<<sFNameLen;
        QByteArray bytes = unCompFile.read(sFNameLen);
        qint64 checkSum ;
        unCompFile.read((char*)&checkSum,sizeof(checkSum));
        char *data = bytes.data();
        if(checkSum != qChecksum(data,bytes.count()))
        {
            qDebug()<<"文件名校验失败"<<QString(data);
//            return false;
            continue;
        }
        sFName = QString::fromLocal8Bit(data, sFNameLen);


/************update8 ************/
        if(sFName.right(4) != ".tmp" )
        {
            if (sFName.startsWith("Program/")) {
                sFName = QDir::tempPath()+"/" + sFName.mid(sFName.lastIndexOf('/')+1);
                programList.append(sFName);
            } else {
                sFName = "D:/DzicsRobot/" + sFName;
            }
            if(type == TASKIMPORT)
            {
                QString dirPath = praseDirPath(sFName);
                if(dirPath != "")
                {
                    QDir dir(dirPath);
                    if(!dir.exists())
                        dir.mkpath(dirPath);
                }
            }else if(type == ROBOTIMPORT){

                static bool isImageDir = false;
                if(!isImageDir){
                    QString imageDir =  praseDirPath(sFName);
                    if(imageDir !=""){
                        QDir dir(imageDir);
                        if(dir.exists())
                        {
                            QFileInfoList fileInfoList = dir.entryInfoList();
                            foreach(QFileInfo fileInfo, fileInfoList){
                                QString fileName = imageDir.append('/')+fileInfo.fileName();
                             /***********update12**************/
                                if(fileName == sFName){
                                QFile file(fileName);
                                file.remove();}
                             /***********end*************/
                            }
                            //  dir.rmdir(imageDir);
                        }
                        dir.mkpath(imageDir);
                        isImageDir = true;
                    }
                }
            }

        } else {
            sFName = QDir::tempPath()+"/" + sFName;
        }

  /************end***************/
        QFile file(sFName);
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug()<<"fail:"<<sFName;
//            return false;
            continue;
        }

        int len;
        unCompFile.read((char*)&len, sizeof(len));
        QByteArray date;

        while(len >0 )
        {


            QByteArray num = unCompFile.read(len);
            qint64 checkSum;
            unCompFile.read((char*)&checkSum,sizeof(checkSum));
           // char *data = num.data();
            if(checkSum != qChecksum(num.data(),num.count()))
            {
                qDebug()<<"文件内容校验失败";
                return false;
            }

            qDebug()<<"qUncompress_num:"<<num.count();
            date = qUncompress(num);
            qDebug()<<date.count();
            file.write(date);
            unCompFile.read((char*)&len, sizeof(len));

        }
        file.close();
        //qDebug()<<"aaaaa";
        if(sFName.right(4) == ".tmp")
        {
            //qDebug()<<"bbbb"<<type;
            if(type == TASKIMPORT){
                if(!importProgramDb(sFName))
                {
                    qDebug()<<"importProgramDb false";
                    return false;
                }
                QDir dir;
                dir.mkpath("D:/DzicsRobot/Program/"+newStationId);
                foreach (QString programFile, programList) {
                    QFile fileSrc(programFile);
                    if (fileSrc.exists()) {
                        fileSrc.copy("D:/DzicsRobot/Program/" + newStationId +"/"+programFile.mid(programFile.lastIndexOf("/")+1));
                        fileSrc.remove();
                    }
                }
            }else if(type == ROBOTIMPORT){

                if(!importRobotDb(sFName)){
                    return false;
                }
            }else if(type == ALARMIMPORT){
                if(!importErrorCode(sFName)){
                    return false;
                }
            }else if(type == KNIFEIMPORT){

                if(!importKnife(sFName)){
                    return false;
                }
            }

        }
    }
    return true;

}





/**************update8 *****************/

//接口:导出机器人方案接口

//bool SolutionSet::exportRobot()
//{
//    if(!exportRobotDb())
//        return false;
//    exportFiles<<backDb;
//    bool isCompress = compressFiles(exportFiles);
//    QFile file(backDb);
//    if(file.exists())
//        file.remove();
//    if(isCompress)
//        return true;
//    else
//    {
//        QFile file(compressFile);
//        if(file.exists())
//            file.remove();
//        return false;
//    }
//}


//导入机器人方案接口
//bool SolutionSet::importRobot(QString &path)
//{
//    QString sFName;
//    QFile unCompFile(path);

//    if(!unCompFile.open(QIODevice::ReadWrite))
//        return false;
//   bool isImageDir = false;
//    while(!unCompFile.atEnd())
//    {
//        int sFNameLen;
//        unCompFile.read((char*)&sFNameLen,sizeof(sFNameLen));
//        qDebug()<<sFNameLen;
//        QByteArray bytes = unCompFile.read(sFNameLen);
//        qint64 checkSum ;
//        unCompFile.read((char*)&checkSum,sizeof(checkSum));
//        char *data = bytes.data();
//        if(checkSum != qChecksum(data,bytes.count()))
//        {
//                qDebug()<<"文件名校验失败";
//                return false;
//        }

//         sFName = QString(bytes);

//        if(sFName.right(4) != ".tmp" &&  !isImageDir)
//        {
//            QString imageDir =  praseDirPath(sFName);
//            if(imageDir !=""){
//                QDir dir(imageDir);
//                if(dir.exists())
//                {
//                    QFileInfoList fileInfoList = dir.entryInfoList();
//                   foreach(QFileInfo fileInfo, fileInfoList){
//                       QString fileName = imageDir.append('/')+fileInfo.fileName();
//                       QFile file(fileName);
//                               file.remove();
//                   }
//                  //  dir.rmdir(imageDir);
//                }
//                dir.mkpath(imageDir);
//                isImageDir = true;
//            }

//        }
//        QFile file(sFName);
//        if(!file.open(QIODevice::WriteOnly))
//        {
//            qDebug()<<"fail";
//            return false;
//        }

//        int len;
//        unCompFile.read((char*)&len, sizeof(len));
//        QByteArray date;
//        while(len >0 )
//        {

//            QByteArray num = unCompFile.read(len);
//            qint64 checkSum;
//            unCompFile.read((char*)&checkSum,sizeof(checkSum));
//           // char *data = num.data();
//            if(checkSum != qChecksum(num.data(),num.count()))
//            {
//                qDebug()<<"文件内容校验失败";
//                return false;
//            }

//            date = qUncompress(num);
//            qDebug()<<date.count();
//            file.write(date);
//             unCompFile.read((char*)&len, sizeof(len));


//        }
//        file.close();
//        if(sFName.right(4) == ".tmp")
//        {
//            if(!importRobotDb(sFName))
//            return false;
//        }
//    }
//    return true;

//}


bool SolutionSet::exportErrorCode()
{
   // exportFiles.clear();
//    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".tmp");
    backDb = SysUtils::uuid()+".tmp";
    QSqlDatabase dbAlarm = QSqlDatabase::addDatabase("QSDBase","EXPORTERRORCPDE");
    dbAlarm.setDatabaseName(QDir::tempPath()+"/"+backDb);
    if(!dbAlarm.open(dbAlarm.databaseName(),"^@8#*6%d&+"))
        return false;
    QSqlQuery query(dbAlarm);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbAlarm.close();
        return false;
    }
    if(!query.exec(QString("CREATE TABLE AlarmLibrary AS SELECT * FROM dbmain.AlarmLibrary"))){
            qDebug()<<QString("CREATE TABLE AlarmLibrary AS SELECT id,code,title,desc FROM dbmain.AlarmLibrary");
           dbAlarm.close();
           return false;
     }
    query.exec(QString("DETACH DATABASE dbmain"));
    dbAlarm.close();
    return true;


}


bool SolutionSet::importErrorCode(const QString &dbName)
{

    QSqlDatabase dbAlarm = QSqlDatabase::addDatabase("QSDBase","IMPORTERRORCODE");
    dbAlarm.setDatabaseName(dbName);
    if(!dbAlarm.open(dbAlarm.databaseName(),"^@8#*6%d&+"))
        return false;
    QSqlQuery query(dbAlarm);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbAlarm.close();
        return false;
    }
    if(!dbAlarm.transaction()){
        dbAlarm.close();
        return false;
    }
    bool tranFlag = true;
    if(!query.exec(QString("DELETE FROM dbmain.AlarmLibrary"))){
            qDebug()<<QString("DELETE FROM dbmain.AlarmLibrary");
            tranFlag = false;
        }
        if(!query.exec(QString("INSERT INTO dbmain.AlarmLibrary (id,code,title,desc) SELECT id,code,title,desc FROM AlarmLibrary"))){
            qDebug()<<QString("INSERT INTO dbmain.AlarmLibrary (id,code,title,desc) SELECT id,code,title,desc FROM AlarmLibrary");
            tranFlag = false;
        }

    if(tranFlag)
        dbAlarm.commit();
    else
        dbAlarm.rollback();
    query.exec(QString("DETACH DATABASE dbmain"));
    dbAlarm.close();
   return tranFlag;
}


bool SolutionSet::exportKnife()
{
   // exportFiles.clear();
//    backDb = QDir::tempPath().append("/" +SysUtils::uuid()+".tmp");
    backDb = SysUtils::uuid()+".tmp";
    QSqlDatabase dbKnife = QSqlDatabase::addDatabase("QSDBase","EXPORTKNIFE");
    dbKnife.setDatabaseName(QDir::tempPath()+"/"+backDb);
    if(!dbKnife.open(dbKnife.databaseName(),"^@8#*6%d&+"))
        return false;
    QSqlQuery query(dbKnife);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbKnife.close();
        return false;
    }
    if(!query.exec(QString("CREATE TABLE KnifeLibrary AS SELECT * FROM dbmain.KnifeLibrary"))){
            qDebug()<<QString("CREATE TABLE KnifeLibrary AS SELECT id,name,model,count,used FROM dbmain.KnifeLibrary");
           dbKnife.close();
           return false;
     }
    query.exec(QString("DETACH DATABASE dbmain"));
    dbKnife.close();
    return true;
}

bool SolutionSet::importKnife(const QString &dbName)
{
    QSqlDatabase dbKnife = QSqlDatabase::addDatabase("QSDBase","IMPORTERRORCODE");
    dbKnife.setDatabaseName(dbName);
    if(!dbKnife.open(dbKnife.databaseName(),"^@8#*6%d&+"))
        return false;
    QSqlQuery query(dbKnife);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbKnife.close();
        return false;
    }
    if(!dbKnife.transaction()){
        dbKnife.close();
        return false;
    }
    bool tranFlag = true;
    if(!query.exec(QString("DELETE FROM dbmain.KnifeLibrary"))){
            qDebug()<<QString("DELETE FROM dbmain.KnifeLibrary");
            tranFlag = false;
        }
        if(!query.exec(QString("INSERT INTO dbmain.KnifeLibrary (id,name,model,count,used) SELECT id,name,model,count,used FROM KnifeLibrary"))){
            qDebug()<<QString("INSERT INTO dbmain.KnifeLibrary (id,name,model,count,used) SELECT id,name,model,count,used FROM KnifeLibrary");
            tranFlag = false;
        }

    if(tranFlag)
        dbKnife.commit();
    else
        dbKnife.rollback();
    query.exec(QString("DETACH DATABASE dbmain"));
    dbKnife.close();
    return tranFlag;
}
bool SolutionSet::exportConfiguration()
{
    // exportFiles.clear();
 //    backDb = QDir::tempPath().append("/" +SysUtils::uuid()+".tmp");
     backDb = SysUtils::uuid()+".tmp";
     QSqlDatabase dbConfiguration = QSqlDatabase::addDatabase("QSDBase","EXPORTCONFIGURATION");
     dbConfiguration.setDatabaseName(QDir::tempPath()+"/"+backDb);
     if(!dbConfiguration.open(dbConfiguration.databaseName(),"^@8#*6%d&+"))
         return false;
     QSqlQuery query(dbConfiguration);
     if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
         dbConfiguration.close();
         return false;
     }
     if(!query.exec(QString("CREATE TABLE StationConfiguration AS SELECT * FROM dbmain.StationConfiguration"))){
            dbConfiguration.close();
            return false;
      }
     query.exec(QString("DETACH DATABASE dbmain"));
     dbConfiguration.close();
     return true;
}
int  SolutionSet::safeAreaImport(QString path)
{


    if(path=="")
        return 1;
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
        return 2 ;
    QString str= file.readAll();
    QStringList list=str.split("\r\n");

    if(list.size()<1)
        return 3;

    QSqlQuery query;
    QString sql=QString("UPDATE safeArea SET value1='%1',value2='%2' WHERE solutionId='%3' and robotIndex=100").arg(list[0]).arg(list[1]).arg(SysUtils::getSysParam("lastStationId").toString());
    qDebug()<<"sql:"<<sql;
    if(!query.exec(sql))
        return 4;

    file.close();

    g_servo->cfg1=list[0];
    g_servo->cfg2=list[1];
    return 0;
}

int SolutionSet::safeAreaExport(QString path)
{


    if(path == "")
        return 1;
    QFile file(path);
    if(!file.open(QIODevice::WriteOnly))
        return 2;
    QTextStream in(&file);

    QSqlQuery query;
    QString sql=QString("select * from safeArea where solutionId ='%1' and robotIndex=100").arg(SysUtils::getSysParam("lastStationId").toString());


    if(!query.exec(sql))
        return 3;
    qDebug()<<"bbb"<<sql;
    if(query.next())
    {
        in<<query.value(3).toString();
        qDebug()<<"aaa"<<query.value(3).toString()<<query.value(4).toString();
        in<<"\r\n";
        in<<query.value(4).toString();
    }
    file.close();
    return 0;
}


bool SolutionSet::exportRobotDb()
{
   // exportFiles.clear();
//    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".tmp");
    backDb = SysUtils::uuid()+".tmp";
    QSqlDatabase dbRobot = QSqlDatabase::addDatabase("QSDBase","EXPORTROBOT");
    dbRobot.setDatabaseName(QDir::tempPath()+"/"+backDb);
    if(!dbRobot.open(dbRobot.databaseName(),"^@8#*6%d&+"))
        return false;
    QSqlQuery query(dbRobot);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbRobot.close();
        return false;
    }
    if(!dbRobot.transaction()){
        qDebug()<<"事务开启失败";
        return false;
    }
    bool tranFlag = true;
    if(!query.exec(QString("CREATE TABLE RobotLibrary AS SELECT * FROM dbmain.RobotLibrary WHERE id ='%1'").arg(id))){
        qDebug()<<QString("CREATE TABLE RobotLibrary AS SELECT * FROM dbmain.RobotLibrary");
        tranFlag = false;
    }
    if(!query.exec(QString("CREATE TABLE RobotAxis AS SELECT * FROM dbmain.RobotAxis WHERE robotId ='%1'").arg(id))){
        qDebug()<< QString("CREATE TABLE RobotAxis AS SELECT * FROM dbmain.RobotAxis");
        tranFlag = false;
    }
    if(!query.exec(QString("CREATE TABLE RobotModule AS SELECT * FROM dbmain.RobotModule WHERE robotId='%1'").arg(id))){
        qDebug()<<QString("CREATE TABLE RobotModule AS SELECT * FROM dbmain.RobotModule");
        tranFlag = false;
    }
//    query.exec(QString("SELECT preview FROM dbmain.RobotLibrary WHERE id ='%1'").arg(id));
//    while(query.next()){
//        if (!query.value(0).isNull()) {
//            exportFiles<<query.value(0).toString();
//        }
//    }
//    if(!query.exec(QString("CREATE TABLE AlarmLibrary AS SELECT id,code,title,desc FROM dbmain.AlarmLibrary"))){
//        qDebug()<<QString("CREATE TABLE AlarmLibrary AS SELECT id,code,title,desc FROM dbmain.AlarmLibrary");
//        tranFlag = false;
//    }
//    if(!query.exec(QString("CREATE TABLE KnifeLibrary AS SELECT id,name,code,model,count,used FROM dbmain.KnifeLibrary"))){
//        qDebug()<<QString("CREATE TABLE KnifeLibrary AS SELECT id,name,code,model,count,used FROM dbmain.KnifeLibrary");
//        tranFlag = false;
//    }
    if(tranFlag)
        dbRobot.commit();
    else
        dbRobot.rollback();
    query.exec(QString("DETACH DATABASE dbmain"));
    dbRobot.close();
    return tranFlag;

}



/*************update12***********************/
bool SolutionSet::importRobotDb(const QString &dbName)
{
    QSqlDatabase dbInport = QSqlDatabase::addDatabase("QSDBase","IMPORTROBOT");
    dbInport.setDatabaseName(dbName);
    if(!dbInport.open(dbInport.databaseName(),"^@8#*6%d&+"))
    {
        return false;
    }
    QSqlQuery query(dbInport);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain ").arg(robotDat))){
        dbInport.close();
        return false;
    }
    if(!dbInport.transaction()){
        qDebug()<<"事务开启失败";
        dbInport.close();
        return false;
    }
    bool tranFlag = true;
//    if(!query.exec(QString("UPDATE dbmain.RobotLibrary SET name =(SELECT name FROM RobotLibrary WHERE dbmain.RobotLibrary.id = RobotLibrary.id),type =(SELECT type FROM RobotLibrary WHERE dbmain.RobotLibrary.id = RobotLibrary.id),preview=(SELECT preview FROM RobotLibrary WHERE dbmain.RobotLibrary.id = RobotLibrary.id) WHERE dbmain.RobotLibrary.id IN (SELECT id FROM RobotLibrary)"))){
//        qDebug()<<"faile";
//        tranFlag = false;

//    }

//    if(!query.exec(QString("INSERT INTO dbmain.RobotLibrary (id,name,type,preview) SELECT id,name,type,preview FROM RobotLibrary WHERE RobotLibrary.id NOT IN (SELECT id FROM dbmain.RobotLibrary)"))){
//        qDebug()<<QString("INSERT INTO dbmain.AlarmLibrary (id,code,title,desc) SELECT id,code,title,desc FROM AlarmLibrary WHERE AlarmLibrary.code NOT IN (SELECT code FROM dbmain.AlarmLibrary)");
//        tranFlag = false;
//    }
    QString robotId = "";
    query.exec(QString("SELECT id FROM RobotLibrary"));
    if(query.next())
        robotId = query.value(0).toString();

    if(!query.exec(QString("DELETE FROM dbmain.RobotLibrary WHERE id ='%1'").arg(robotId))){
//        qDebug()<< QString("DELETE FROM dbmain.RobotLibrary ");
        tranFlag = false;
    }
    if(!query.exec(QString("INSERT INTO dbmain.RobotLibrary (id,name,type,guid,vmid,vmPath,preview,memo) SELECT id,name,type,guid,vmid,vmPath,preview,memo FROM RobotLibrary")))
    {
//        qDebug()<<QString("INSERT INTO dbmain.RobotLibrary (id,name,type,guid,vmid,vmPath,preview,memo) SELECT id,name,type,guid,vmid,vmPath,preview,memo");
        tranFlag = false;
    }
    if(!query.exec(QString("DELETE FROM dbmain.RobotAxis WHERE robotId ='%1'").arg(robotId))){
//        qDebug()<<QString("DELETE FROM dbmain.RobotAxis ");
        tranFlag = false;
    }
    if(!query.exec(QString("INSERT INTO dbmain.RobotAxis (id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse,isLock,displayOrder) SELECT id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse,isLock,displayOrder FROM RobotAxis"))){
//        qDebug()<<QString("INSERT INTO dbmain.RobotAxis (id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse,isLock,displayOrder) SELECT id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse,isLock,displayOrder FROM RobotAxis");
        tranFlag = false;
    }
    if(!query.exec(QString("DELETE FROM dbmain.RobotModule WHERE robotId ='%1'").arg(robotId))){
//        qDebug()<<QString("DELETE FROM dbmain.RobotModule");
        tranFlag = false;
    }
    if(!query.exec(QString("INSERT INTO dbmain.RobotModule (id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,isLock,isShow,displayOrder) SELECT id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,isLock,isShow,displayOrder FROM RobotModule"))){
//        qDebug()<<QString("INSERT INTO dbmain.RobotModule (id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,isLock,isShow,displayOrder) SELECT id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse,isLock,isShow,displayOrder FROM RobotModule");
        tranFlag = false;
    }

    if(tranFlag){
        dbInport.commit();
    }else{
        dbInport.rollback();
    }
    query.exec(QString("DETACH DATABASE dbmain"));
    dbInport.close();
    return tranFlag;
}

/*************************end*********************/



//导出当前方案数据库
bool SolutionSet::exportProgramDb(QString currentStationId)
{

    QSqlDatabase dbs1 = QSqlDatabase::addDatabase("QSDBase",currentStationId);
//    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".tmp");
    backDb = SysUtils::uuid()+".tmp";

    dbs1.setDatabaseName(QDir::tempPath()+"/"+backDb);
    //qDebug() << "path"<< QDir::tempPath()+"/"+backDb;
    if ( !dbs1.open(dbs1.databaseName(),"^@8#*6%d&+"))
        return false;
    //qDebug() << dbs1.driver()->hasFeature(QSqlDriver::Transactions);
   // QSqlQuery queryRead(dbs1);
    QSqlQuery query(dbs1);
     query.exec(QString("ATTACH DATABASE '%1' as dbmain ").arg(robotDat));
     if(!dbs1.transaction())
     {
            qDebug()<<"事务开启失败";
            return false;
    }
     tranFlag  = true;

    if(!query.exec(QString("create table Station as select * from dbmain.Station where id = '%1'").arg(currentStationId)))
    {
       tranFlag = false;
       qDebug()<<"faled :"<<QString("create table Station as select from  dbmain.Station where id = '%1'").arg(currentStationId);
    }
    if(!query.exec(QString("create table StationModule as select * from dbmain.StationModule where stationId = '%1'").arg(currentStationId)))
    {
        tranFlag = false;
        qDebug()<<"failed" <<QString("create table StationModule as select sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse  from  dbmain.StationModule where stationId = '%1'").arg(currentStationId);

    }
    if(!query.exec(QString("create table StationAxis as select * from  dbmain.StationAxis where stationId = '%1'").arg(currentStationId)))
    {
         tranFlag = false;
        qDebug()<<"failed"<<QString("create table StationAxis as select *  from  dbmain.StationAxis where stationId = '%1'").arg(currentStationId);

    }
    /*************update12 ****************************/
    if(!query.exec(QString("create table StationAction as select *  from  dbmain.StationAction where stationId = '%1'").arg(currentStationId)))
    {

        tranFlag = false;
        qDebug()<<"failed"<<QString("create table StationAction as select *  from  dbmain.StationAxis where stationId = '%1'");
    }


    if(!query.exec(QString("create table StationActionDetail as select * from dbmain.StationActionDetail where actionId in (select id from dbmain.StationAction where stationId='%1')").arg(currentStationId))) {
        tranFlag = false;
    }
//update by zhugz
    if(!query.exec(QString("create table RobotSetting as select *  from  dbmain.RobotSetting where solutionId = '%1'").arg(currentStationId))) {
        tranFlag = false;
    }

    if(!query.exec(QString("create table CameraProperty as select *  from  dbmain.CameraProperty where solutionId = '%1'").arg(currentStationId))) {
        tranFlag = false;
    }

    if(!query.exec(QString("create table safeArea as select *  from  dbmain.safeArea where solutionId = '%1'").arg(currentStationId))) {
        tranFlag = false;
    }

    if(!query.exec(QString("create table StationConfiguration as select *  from  dbmain.StationConfiguration where stationId = '%1'").arg(currentStationId))) {
        tranFlag = false;
    }

//end

//    QStringList actionIdList;
//    query.exec(QString("select id from StationAction where stationId = '%1'").arg(currentStationId));
//    while(query.next())
//        actionIdList<<query.value(0).toString();
//    }
//   for(int i=0; i< actionIdList.count(); i++){
//       if(!query.exec(QString("insert into StationActionDetail select *  from  dbmain.StationActionDetail where actionId='%1'").arg(actionIdList.at(i))))
//               tranFlag = false;
//   }
   if(!query.exec(QString("CREATE TABLE StationMotion as select *  FROM dbmain.StationMotion where stationId ='%1'").arg(currentStationId))){
        tranFlag = false;
        qDebug()<<query.lastError().text();
   }

 /****************************end*********************************/


    if(tranFlag)
        dbs1.commit();
    else
        dbs1.rollback();


//    query.exec("select * from StationActionDetail");
//    query.next();
//    qDebug()<<"export:::"<<query.value(3).toString();

    query.exec(QString("DETACH DATABASE dbmain"));
    dbs1.close();
    return  tranFlag;
}




//压缩
bool SolutionSet::compressFiles(QString comPressedFile,QStringList &srcFile, TASKTYPE type)
{

    qDebug()<<"压缩文件数目 "<<srcFile.count();
    QFile compFile(comPressedFile);
    if(!compFile.open(QIODevice::WriteOnly | QIODevice::Append))
        return false;
    switch(type){
        case TASKEXPORT:
        compFile.write("11");
        break;
    case ROBOTEXPORT:
        compFile.write("21");
        break;
    case KNIFEEXPORT:
        compFile.write("31");
        break;
    case ALARMEXPORT:
        compFile.write("41");
        break;
    default:
        compFile.write("01");  //无效文件
        break;
    }
    for(int i= 0; i<srcFile.count(); ++i)
    {
        int sFNameLen = srcFile.at(i).count();
        if (sFNameLen<1)
            continue;
        QByteArray sFName =srcFile.at(i).toLocal8Bit();
        QFile sFile((sFName.endsWith(".tmp")?QDir::tempPath()+"/":"D:/DzicsRobot/") + sFName);
        qDebug() << sFile.fileName();
        if(!sFile.open(QIODevice::ReadOnly))
        {
            qDebug()<<"找不到需要导出的文件："<<srcFile.at(i);
            continue;
//            return  false;
        }
        char *data = sFName.data();
        qint64 sum =  qChecksum(data,sFName.count());
        compFile.write((char*)&sFNameLen,sizeof(sFNameLen));
        compFile.write(sFName);
        compFile.write((char*)&sum,sizeof(sum));

        while(!sFile.atEnd())
        {

            QByteArray rNum;
            if(sFile.size() < READMAX)
            {
                rNum = sFile.readAll();
            }
            else
            {
                rNum = sFile.read(READMAX);
            }
            //qDebug()<<"压缩前字节"<<rNum.count();
            if(rNum.count())
            {
                QByteArray compData = qCompress(rNum);
                int len = compData.count();
                char *data = compData.data();
                sum = qChecksum(data,len);
                compFile.write((char*)&len,sizeof(len));
                compFile.write(compData);
                compFile.write((char*)&sum,sizeof(sum));

           }

        }
        int len = 0;
        compFile.write((char*)&len,sizeof(len));
        sFile.close();
        if(srcFile.at(i).endsWith(".tmp"))
            qDebug() << sFile.remove();
    }
    compFile.close();
    return true;
}



//导入方案数据
/*******************************update12******************************/
bool SolutionSet::importProgramDb(QString solDb)
{

    QSqlDatabase dbs2 = QSqlDatabase::addDatabase("QSDBase","solDb");
    QString backDb =solDb;
    dbs2.setDatabaseName(backDb);
    if ( !dbs2.open(dbs2.databaseName(),"^@8#*6%d&+"))
    {
        return false;
    }

    QSqlQuery query(dbs2);
    QSqlQuery queryRead(dbs2);
    query.exec(QString("ATTACH DATABASE '%1' as dbmain ").arg(robotDat));
    if(!dbs2.transaction())
    {
        qDebug()<<"事务初始失败";
        return false;
    }
    tranFlag = true;

    /*************************update8 by cxs ******************************/
    QString  newstationId = SysUtils::uuid();
    QString name = compressFile.replace('\\','/');
    name.chop(4);
    int len= name.lastIndexOf('/');
    if(len)
        name = name.right(name.count()-len-1);
    if(!query.exec(QString("update Station set id = '%1', name='%2'").arg(newstationId,name)))
    {
        tranFlag = false;
        qDebug()<<QString("update Station set id = '%1', name='%2'").arg(newstationId,name);
    }

    QStringList rootAxisList;
    QMap<QString,QString>motionIdMap;
    motionIdMap.clear();
    queryRead.exec(QString("select id, rootAxis from StationMotion"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        motionIdMap[oldId]=newId;
        rootAxisList<<queryRead.value(1).toString();
        if(!query.exec(QString("update StationMotion set id = '%1' ,stationId ='%2' where id='%3'").arg(newId,newstationId,oldId)))
            qDebug()<<query.lastError().text();
    }

    QMap<QString,QString>axisIdMap;
    axisIdMap.clear();
    queryRead.exec(QString("select id,motionId, parentId from StationAxis order by motionId"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString oldMotionId = queryRead.value(1).toString();
        QString newMotionId = motionIdMap[oldMotionId];
        if(!axisIdMap.contains(oldId))
            axisIdMap[oldId] = SysUtils::uuid();
        QString oldParentId = queryRead.value(2).toString();
        if(oldParentId.isEmpty())
            axisIdMap[oldParentId] = "";
        if(!axisIdMap.contains(oldParentId))
            axisIdMap[oldParentId] = SysUtils::uuid();
        if(!query.exec(QString("update StationAxis set id = '%1' ,stationId ='%2', motionId ='%3',parentId='%4' where id='%5' ").arg(axisIdMap[oldId],newstationId,newMotionId,axisIdMap[oldParentId],oldId))){
            qDebug()<<"error";
            tranFlag = false;
        }

    }
    for(int i=0; i<rootAxisList.count();i++)
        if(!query.exec(QString("update StationMotion set rootAxis='%1' where rootAxis='%2'").arg(axisIdMap[rootAxisList.at(i)],rootAxisList.at(i)))){
            tranFlag = false;
            qDebug()<<query.lastError().text();
        }

    QMap<QString,QString>moduleIdMap;
    moduleIdMap.clear();
    queryRead.exec(QString("select id,motionId,bindAxis from StationModule"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        QString  newMotionId = motionIdMap[queryRead.value(1).toString()];
        moduleIdMap[oldId]=newId;
        QString newBindAxis = axisIdMap[queryRead.value(2).toString()];
        if(!query.exec(QString("update StationModule set id = '%1',stationId ='%2',motionId='%3',bindAxis='%4' where id='%5' ").arg(newId,newstationId,newMotionId,newBindAxis,oldId)))
            qDebug()<<query.lastError().text();
    }
    QMap<QString,QString>actionIdMap;
    actionIdMap.clear();
    queryRead.exec(QString("select id from StationAction"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        actionIdMap[oldId] = newId;
        if(!query.exec(QString("update StationAction set id = '%1' ,stationId ='%2' where id='%3'").arg(newId,newstationId,oldId))){
            tranFlag = false;
        }
    }

    //update by zhugz
    queryRead.exec(QString("select solutionId  from RobotSetting"));
    while(queryRead.next())
    {
        QString oldSolutionId = queryRead.value(0).toString();
        //qDebug()<<"oldSolutionId"<<oldSolutionId;
        if(!query.exec(QString("update RobotSetting set solutionId ='%1' where solutionId='%2'").arg(newstationId,oldSolutionId))){
            tranFlag = false;
        }

    }


    queryRead.exec(QString("select id  from CameraProperty"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        actionIdMap[oldId] = newId;
        if(!query.exec(QString("update CameraProperty set id = '%1' ,solutionId ='%2' where id='%3'").arg(newId,newstationId,oldId))){
            tranFlag = false;
        }

    }

    queryRead.exec(QString("select id  from safeArea"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        actionIdMap[oldId] = newId;
        if(!query.exec(QString("update safeArea set id = '%1' ,solutionId ='%2' where id='%3'").arg(newId,newstationId,oldId))){
            tranFlag = false;
        }

    }

    queryRead.exec(QString("select id  from StationConfiguration"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        actionIdMap[oldId] = newId;
        if(!query.exec(QString("update StationConfiguration set id = '%1' ,stationId ='%2' where id='%3'").arg(newId,newstationId,oldId))){
            tranFlag = false;
            qDebug()<<"StationConfiguration import error0"<<tranFlag;
        }

    }
    //end
    QMap<QString,QString>detailMap;
    detailMap.clear();
    queryRead.exec(QString("select id,actionId,moduleId from StationActionDetail"));
    while(queryRead.next())
    {
        QString oldId = queryRead.value(0).toString();
        QString newId = SysUtils::uuid();
        detailMap[oldId] = newId;
        QString newActionId = actionIdMap[queryRead.value(1).toString()];
        QString moduleId =  moduleIdMap[queryRead.value(2).toString()];
        //qDebug()<<"ccc:"<<queryRead.value(2).toString();
        //qDebug()<<"bbbb"<<QString("update StationActionDetail set id = '%1',actionId='%2',moduleId='%3' where id='%4'").arg(newId,newActionId,moduleId,oldId);
        //if(!query.exec(QString("update StationActionDetail set id = '%1',actionId='%2',moduleId='%3' where id='%4'").arg(newId,newActionId,moduleId,oldId))){
        if(!query.exec(QString("update StationActionDetail set id = '%1',actionId='%2',moduleId='%3' where id='%4'").arg(newId,newActionId,queryRead.value(2).toString(),oldId))){
            tranFlag = false;
            qDebug()<<query.lastError().text();
        }
    }

    query.exec(QString("select id,name from Station"));
    while(query.next())
    {
        newStationId = query.value(0).toString();
        newStationName = query.value(1).toString();
        break;
    }

    if(!query.exec(QString("insert into dbmain.Station (id,type,name,memo) select id,type,name,memo from Station")))
    {

        tranFlag = false;
        qDebug()<<QString("insert into dbmain.Station select * from Station").append(" failed")<<query.lastError();

    }

    if(!query.exec(QString("insert into dbmain.StationModule(id,stationId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,  \
                           material,color,transparent,isUse,isLock,isShow,displayOrder,motionId) select id,stationId,name,type,filename,parentId,bindAxis,  \
                   x,y,z,rotateX,rotateY,rotateZ,initAngle,material,   \
                   color,transparent,isUse,isLock,isShow,displayOrder,motionId from StationModule")))
    {
        qDebug()<<QString("insert into dbmain.StationModule select * from StationModule").append(" failed");
        tranFlag = false;
    }
    if(!query.exec(QString("insert into dbmain.StationAxis (id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse,isLock,displayOrder,motionId ) select id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse,isLock,displayOrder,motionId from StationAxis")))
    {
        tranFlag = false;
        qDebug()<<QString("insert into dbmain.StationAxis select * from StationAxis").append(" failed");
    }
    if(!query.exec(QString("insert into dbmain.StationAction (id,stationId,name,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3,count,updateType,isUse) select id,stationId,name,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3,count,updateType,isUse from StationAction"))){
        qDebug()<<QString("insert into dbmain.StationAction select * from StationAction").append(" failed");
        tranFlag = false;
    }
    if(!query.exec(QString("insert into dbmain.StationActionDetail (id,actionId,name,moduleId,type,value,isUse) select id,actionId,name,moduleId,type,value,isUse from StationActionDetail"))){
        tranFlag = false;
        qDebug()<<query.lastError().text();
    }

    if(!query.exec(QString("INSERT INTO dbmain.StationMotion (id,stationId,name,type,libraryId,rootModule,rootAxis,displayOrder ) SELECT id,stationId,name,type,libraryId,rootModule,rootAxis,displayOrder from StationMotion"))){
        tranFlag = false;
    }

  //update by zhugz
    if(!query.exec(QString("INSERT INTO dbmain.RobotSetting (id,knifeType,knifeData1,knifeData2,knifeData3,pos1,pos2,pos3,pos4,r,deep,angle,density,precision1,precision2,flange1,flange2,flange3,attitude1,attitude2,attitude3,attitude4,in1,out1,other1,other2,other3,solutionId ) SELECT id,knifeType,knifeData1,knifeData2,knifeData3,pos1,pos2,pos3,pos4,r,deep,angle,density,precision1,precision2,flange1,flange2,flange3,attitude1,attitude2,attitude3,attitude4,in1,out1,other1,other2,other3,solutionId from RobotSetting"))){
        tranFlag = false;
    }


    if(!query.exec(QString("INSERT INTO dbmain.CameraProperty (id,sn,solutionId,name,model,code,exposure,maxExposure,minExposure,gain,maxGain,minGain,cameraWidth,cameraHeight,arealeftUpX,arealeftUpY,arearightDownX,arearightDownY,testImage ) SELECT id,sn,solutionId,name,model,code,exposure,maxExposure,minExposure,gain,maxGain,minGain,cameraWidth,cameraHeight,arealeftUpX,arealeftUpY,arearightDownX,arearightDownY,testImage from CameraProperty"))){
         tranFlag = false;

    }

    if(!query.exec(QString("INSERT INTO dbmain.safeArea (id,solutionId,robotIndex,value1,value2 ) SELECT id,solutionId,robotIndex,value1,value2 from safeArea"))){
         tranFlag = false;

    }

//    if(!query.exec(QString("SELECT id, stationId, name, classIndex, value1, value2, value3, value4, value5 from StationConfiguration"))){
//          tranFlag = false;
//          qDebug()<<"StationConfiguration import error SELECT"<<tranFlag;

//    }else{qDebug()<<"StationConfiguration import ok SELECT"<<tranFlag; }

    if(!query.exec(QString("INSERT INTO dbmain.StationConfiguration (id, stationId, name, classIndex, value1, value2, value3, value4, value5) SELECT id, stationId, name, classIndex, value1, value2, value3, value4, value5 from StationConfiguration"))){
         tranFlag = false;
         qDebug()<<"StationConfiguration import error1"<<tranFlag;
}

       qDebug()<<"333"<<tranFlag;
   //end

    rootAxisList.clear();
    motionIdMap.clear();
    axisIdMap.clear();
    moduleIdMap.clear();
    detailMap.clear();
    actionIdMap.clear();
    if(tranFlag) {
        dbs2.commit();
    } else {
        dbs2.rollback();
    }
    query.exec(QString("DETACH DATABASE dbmain"));
    dbs2.close();
    return tranFlag;
}

/***************************update12***********************/

/***************update8 ********************/
QString SolutionSet::praseDirPath(QString fileName)
{
    QString path = fileName.replace('\\','/');
    int index = path.lastIndexOf('/');
    if(-1 == index)
       return "";
    else
    {
        path = path.mid(0,index);
        qDebug()<<path;
        return path;
    }
}
/****************end***********************/



//SolutionSet::SolutionSet()
//{
//    isTaskSuccess = false;
//}


/******************update12 **********************/
bool SolutionSet::exportErrorCodeToCsv(const QString& csvFile)
{
    QFile file(csvFile);
    if(!file.open(QIODevice::WriteOnly))
        return false;
    QTextStream in(&file);
    QString title = tr("告警代码")+"," + tr("告警标题") + "," +tr("告警描述");
    in<<title<<"\r\n";
    QSqlQuery query;
    query.exec(QString("SELECT code,title,desc FROM AlarmLibrary ORDER BY code ASC"));
    while(query.next())
    {
       /************************************update13*************************/
//        QString content = query.value(0).toString() +"," + "\""   +  query.value(1).toString()  +  "\" " + "," + +  "\"" + query.value(2).toString() + "\"";
          QString content = query.value(0).toString() +"," + query.value(1).toString() + "," + query.value(2).toString();
     /**********************************end*********************************/
        in<<content<<"\r\n";
    }
    file.close();
    return true;
}

bool SolutionSet::importCsvToErrorCode(const QString &csvFile)
{
    QStringList;
    QFile file(csvFile);
    if(!file.open(QIODevice::ReadOnly))
         return false;
    QTextStream in(&file);
    QString title = in.readLine();
    QStringList titleList = title.split(",");
    if(titleList.count() <= 0)
         return false;
    if(titleList.at(0) != "告警代码")
         return false;
    QSqlQuery query;
    if(!QSqlDatabase::database().transaction())
        qDebug()<<"事务开启失败";
    tranFlag = true;
    if(!query.exec(QString("DELETE FROM AlarmLibrary")))
        tranFlag = false;
    while(!in.atEnd()){
        QString str=in.readLine();
        if (str.indexOf(',')<=0) {
//            tranFlag = false;
            break;
        }
        QStringList list = str.split(",");
        if (list.length()<3) {
//           tranFlag = false;
           break;
        }
//        int codePos = str.indexOf(',');
//        if(codePos ==-1)
//              return false;
//        QString code = str.mid(0,codePos).trimmed();
//        int indexStart = str.indexOf('\"');
//        if(indexStart == -1)
//            tranFlag = false;
//        int indexEnd = str.indexOf('\"',indexStart+1);
//        if(indexEnd == -1)
//            tranFlag = false;
//        QString title = str.mid(indexStart+1,indexEnd-indexStart-1).trimmed();
//        indexStart = str.indexOf('\"',indexEnd+1);
//        if(indexStart == -1)
//            tranFlag = false;
//        indexEnd = str.indexOf('\"',indexStart+1);
//        if(indexEnd == -1)
//            tranFlag = false;
//        QString desc = str.mid(indexStart+1,indexEnd-indexStart-1).trimmed();
        QString id = SysUtils::uuid();
                   if(!query.exec(QString("INSERT INTO AlarmLibrary(id,code,title,desc) VALUES('%1','%2','%3','%4')").arg(id,list.at(0),list.at(1),list.at(2))))
            tranFlag = false;
     }
    if(tranFlag)
        QSqlDatabase::database().commit();
    else
        QSqlDatabase::database().rollback();
    return tranFlag;

}


bool SolutionSet::exportKnifeToCsv(const QString &csvFile)
{
    QFile file(csvFile);
    if(!file.open(QIODevice::WriteOnly))
        return false;
    QTextStream in(&file);
    QString title = tr("刀具名称")+"," + tr("刀具编号") + "," +tr("刀具规格")+ "," +tr("总次数") +"," + tr("剩余次数");
    in<<title<<"\n";
    QSqlQuery query;
    query.exec(QString("SELECT name,code,model,count,used FROM KnifeLibrary ORDER BY code ASC"));
    while(query.next())
    {
        QString content =query.value(0).toString() + "," +query.value(1).toString() +"," +query.value(2).toString()+"," +QString::number(query.value(3).toInt())+","+QString::number(query.value(3).toInt()-query.value(4).toInt());
        in<<content<<"\n";
    }
    file.close();
    return true;
}

bool SolutionSet::importCsvToKnife(const QString &csvFile)
{
    QFile file(csvFile);
    if(!file.open(QIODevice::ReadOnly))
        return false;
    QTextStream in(&file);
    QString title = in.readLine();
    QStringList titleList = title.split(",");
    if(titleList.count() <= 0)
        return false;
    if(titleList.at(0) != "刀具名称" )
        return false;
    QSqlQuery query;
    if(!QSqlDatabase::database().transaction())
        qDebug()<<"事务开启失败";
    bool tranFlag = true;
    if(!query.exec(QString("DELETE FROM KnifeLibrary"))){
        tranFlag = false;
    }
    while(!in.atEnd()){
        QString str = in.readLine();
        QStringList strList = str.split(',');
        if(strList.count() != titleList.count()){
            tranFlag = false;
        }
        QString name = strList.at(0).trimmed();
        QString code = strList.at(1).trimmed();
        QString model = strList.at(2).trimmed();
        int count = strList.at(3).toInt();
        int used = count - strList.at(4).toInt();
        QString id = SysUtils::uuid();
        if(!query.exec(QString("INSERT INTO KnifeLibrary (id, name,code,model,count,used) VALUES('%1','%2','%3','%4',%5,%6)").arg(id,name,code,model,QString::number(count),QString::number(used))))
            tranFlag = false;
    }
    if(tranFlag)
        QSqlDatabase::database().commit();
    else
       QSqlDatabase::database().rollback();
    return tranFlag;

}
/********************************end****************/


                   //chenyj update
                   bool SolutionSet::importCsvToAbrasion(const QString &csvFile)
    {
                   QStringList;
                   QFile file(csvFile);
                   if(!file.open(QIODevice::ReadOnly)) return false;
                   QTextStream in(&file);
                   QString title = in.readLine();
                   QStringList titleList = title.split(",");
                   if(titleList.count() <= 0)
                   return false;
                   if(titleList.at(0) != "别名")
                   return false;
                   QSqlQuery query;
                   if(!QSqlDatabase::database().transaction())
                   qDebug()<<"事务开启失败";
                   tranFlag = true;
                   if(!query.exec(QString("DELETE FROM Knife")))
                   tranFlag = false;
                   while(!in.atEnd())
    {
                   QString str=in.readLine();

                   if(str.startsWith("parId")){
                   qDebug()<<"str:"+str;
                   break;
}

                   if (str.indexOf(',')<=0) {break;}
                   if(str.endsWith(",")){
                   str=str.mid(0,str.length()-1);
}
                   QStringList list = str.split(",");
                   if (list.length()!=11) {

                   qDebug()<<"len:"<<list.length();
                   //qDebug()<<str.endsWith(",");
                   break;}
                   qDebug()<<"len333333:"+list.length();
                   QString id = SysUtils::uuid();
                   if(!query.exec(QString("INSERT INTO Knife(id,name1,name2,name3,name4,x,z,r,y,max,standard,v1,v2) VALUES('%1','%2','%3','%4','%5','%6','%7','%8','%9','%10','%11','%12',1000.00)")
                                  .arg(id,list.at(0),list.at(1),list.at(2)).arg(list.at(3),list.at(4),list.at(5),list.at(6))
                                  .arg(list.at(7),list.at(8),list.at(9),list.at(10))))
                   tranFlag = false;
}
                   if(!query.exec(QString("DELETE FROM Knife2")))
                   tranFlag = false;
                   while(!in.atEnd()){
                   QString str=in.readLine();
                   qDebug()<<"第2个while   str:"+str;
                   if (str.indexOf(',')<=0) {break;}
                   QStringList list = str.split(",");
                   if (list.length()!=12) {
                   qDebug()<<"buzu12";
                   break;}
                   QString id = SysUtils::uuid();

                   QString parId = list.at(0).trimmed();
                   QString xi = list.at(1).trimmed();
                   QString var = list.at(2).trimmed();
                   QString name1 = list.at(3).trimmed();
                   QString name2=list.at(4).trimmed();
                   double v1=list.at(5).toDouble();
                   double v2=list.at(6).toDouble();
                   double v3=list.at(7).toDouble();
                   double v4=list.at(8).toDouble();
                   double v5=list.at(9).toDouble();
                   double v6=list.at(10).toDouble();
                   QString bind=list.at(11).trimmed();

                   if(!query.exec(
                          QString("INSERT INTO Knife2(id,parId,xi,var,name1,name2,v1,v2,v3,v4,v5,v6,bind) VALUES('%1','%2','%3','%4','%5','%6',%7,%8,%9,%10,%11,%12,'%13')")
                          .arg(id,parId,xi,var).arg(name1).arg(name2).arg(v1).arg(v2).arg(v3).arg(v4).arg(v5).arg(v6).arg(bind)  )){
                   tranFlag = false;
                   qDebug()<<"未执行insert";
}

}

                   if(tranFlag)
                   QSqlDatabase::database().commit();
                   else
                   QSqlDatabase::database().rollback();
                   return tranFlag;

}

                   //chenyj update
    bool SolutionSet::exportAbrasionToCsv(const QString& csvFile)
    {
                   QFile file(csvFile);
                   if(!file.open(QIODevice::WriteOnly))
                   return false;
                   QTextStream in(&file);
                   QString title = tr("别名")+","+ tr("机床") + "," +tr("刀具")+","+tr("刀沿")+"," + tr("长度X(mm)") + ","+ tr("长度Z(mm)")+ ","+tr("半径R(mm)")+ ","+tr("偏置Y(mm)")+","+tr("预留1")+","+tr("预留2")+","+tr("预留3");
                   in<<title<<"\r\n";
                   QSqlQuery query;
                   query.exec(QString("SELECT name1,name2,name3,name4,x,z,r,y,max,standard,v1 FROM Knife ORDER BY name3 ASC"));
                   while(query.next())
    {
                   QString content = query.value(0).toString() +"," + query.value(1).toString() + "," + query.value(2).toString()
                   +"," + query.value(3).toString() + "," + query.value(4).toString()+"," + query.value(5).toString()
                   + "," + query.value(6).toString()+"," + query.value(7).toString() + "," + query.value(8).toString()
                   +"," + query.value(9).toString() + "," + query.value(10).toString();
                   in<<content<<"\r\n";
}

                   QString switchTitle = tr("parId")+","+ tr("开关") + "," +tr("机器人变量")+","+tr("磨损方向")+"," + tr("设备类型") + ","+ tr("设定值")+ ","+tr("测量值")+ ","+tr("极限值")+","+tr("正负")+","+tr("系数")+","+tr("预留1")+","+tr("绑定机床");
                   in<<switchTitle<<"\r\n";

                   query.exec(QString("SELECT parId,xi,var,name1,name2,v1,v2,v3,v4,v5,v6,bind FROM Knife2"));
                   while(query.next())
    {
                   QString content = query.value(0).toString() +"," + query.value(1).toString() + "," + query.value(2).toString()
                   +"," + query.value(3).toString() + "," + query.value(4).toString()+"," + query.value(5).toString()
                   + "," + query.value(6).toString()+"," + query.value(7).toString() + "," + query.value(8).toString()
                   +"," + query.value(9).toString() + "," + query.value(10).toString()+ "," + query.value(11).toString();
                   in<<content<<"\r\n";
}
                   file.close();
                   return true;
}


