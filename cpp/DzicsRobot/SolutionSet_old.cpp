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



/***********压缩文件头2个字节代表压缩文件的版本*********
 *
 * "11" 方案压缩格式
 * "21"  机器人方案压缩格式
 * "31" 道具文件压缩格式
 *、"41" 告警代码文件压缩格式
 *其他字符 代表无效文件
 ***************************************************/
extern QString robotDat;

#define READMAX 1024UL*1024*100




void SolutionSet::setTaskExport(QString currentStationId, QString &path, QStringList &fileList)
{
     compressFile = path += ".sol";
     curStationId = currentStationId;
     exportFiles = fileList;
     taskType = TASKEXPORT;

}


void SolutionSet::setTaskImport(QString compressedfile)
{
    compressFile = compressedfile;
    taskType =TASKIMPORT;
}


/**************update8 **************************/
void SolutionSet::setTaskRobotExport(QString &path)
{
    compressFile = path += ".rob";
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
 *      ALARMCODEEXPORT  告警代码导出参数
 *      KNIFEEXPORT      刀具库导出
 *
 *srcFile  压缩后文件
 ********************************************/
bool SolutionSet::exportSolution(QString srcFile, TASKTYPE type )
{

    if(type ==  TASKEXPORT){
        if(!exportProgramDb( curStationId))
        {
            qDebug()<<"导入失败";
            return false;
        }

    }
    else if(type == ROBOTEXPORT){
        if(!exportRobotDb())
            return false;
    }else if(type == ALARMCODEEXPORT){
        if(!exportErrorCode())
            return false;

    }else if(type == KNIFEEXPORT){
        if(!exportKnife())
            return false;
    }


    exportFiles<<backDb;
    if(compressFiles(srcFile,exportFiles,type))
        return true;
    else
    {
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
 *    ALARMCODEIMPORT  告警代码导入参数
 *    KNIFEIMPORT      刀具库导入
 *
 *****************************/
bool SolutionSet::importSolution(QString compressedfile,TASKTYPE type)
{
    QString sFName;
    QFile unCompFile(compressedfile);

    if(!unCompFile.open(QIODevice::ReadWrite))
        return false;
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
        if(type!=  ALARMCODEIMPORT)
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
                qDebug()<<"文件名校验失败";
                return false;
        }
        sFName = QString(bytes);
/************update8 ************/
        if(sFName.right(4) != ".db3" )
        {
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
                                QFile file(fileName);
                                file.remove();
                            }
                            //  dir.rmdir(imageDir);
                        }
                        dir.mkpath(imageDir);
                        isImageDir = true;
                    }
                }
            }
        }
  /************end***************/
        QFile file(sFName);
        if(!file.open(QIODevice::WriteOnly))
        {
            qDebug()<<"fail";
            return false;
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

            date = qUncompress(num);
            qDebug()<<date.count();
            file.write(date);
             unCompFile.read((char*)&len, sizeof(len));


        }
        file.close();
        if(sFName.right(4) == ".db3")
        {
            if(type == TASKIMPORT){
            if(!importProgramDb(sFName))
            return false;
            }else if(type == ROBOTIMPORT){

                if(!importRobotDb(sFName)){
                    return false;
                }
            }else if(type == ALARMCODEIMPORT){
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

//        if(sFName.right(4) != ".db3" &&  !isImageDir)
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
//        if(sFName.right(4) == ".db3")
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
    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".db3");
    QSqlDatabase dbAlarm = QSqlDatabase::addDatabase("QSQLITE","EXPORTERRORCPDE");
    dbAlarm.setDatabaseName(backDb);
    if(!dbAlarm.open())
        return false;
    QSqlQuery query(dbAlarm);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbAlarm.close();
        return false;
    }
    if(!query.exec(QString("CREATE TABLE AlarmCode AS SELECT id,code,title,desc FROM dbmain.AlarmCode"))){
            qDebug()<<QString("CREATE TABLE AlarmCode AS SELECT id,code,title,desc FROM dbmain.AlarmCode");
           dbAlarm.close();
           return false;
     }
    query.exec(QString("DETACH DATABASE dbmain"));
    dbAlarm.close();
    return true;


}


bool SolutionSet::importErrorCode(const QString &dbName)
{

    QSqlDatabase dbAlarm = QSqlDatabase::addDatabase("QSQLITE","IMPORTERRORCODE");
    dbAlarm.setDatabaseName(dbName);
    if(!dbAlarm.open())
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
    if(!query.exec(QString("DELETE FROM dbmain.AlarmCode"))){
            qDebug()<<QString("DELETE FROM dbmain.AlarmCode");
            tranFlag = false;
        }
        if(!query.exec(QString("INSERT INTO dbmain.AlarmCode (id,code,title,desc) SELECT id,code,title,desc FROM AlarmCode"))){
            qDebug()<<QString("INSERT INTO dbmain.AlarmCode (id,code,title,desc) SELECT id,code,title,desc FROM AlarmCode");
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
    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".db3");
    QSqlDatabase dbKnife = QSqlDatabase::addDatabase("QSQLITE","EXPORTKNIFE");
    dbKnife.setDatabaseName(backDb);
    if(!dbKnife.open())
        return false;
    QSqlQuery query(dbKnife);
    if(!query.exec(QString("ATTACH DATABASE '%1' as dbmain").arg(robotDat))){
        dbKnife.close();
        return false;
    }
    if(!query.exec(QString("CREATE TABLE KnifeLibrary AS SELECT id,name,model,count,used FROM dbmain.KnifeLibrary"))){
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
    QSqlDatabase dbKnife = QSqlDatabase::addDatabase("QSQLITE","IMPORTERRORCODE");
    dbKnife.setDatabaseName(dbName);
    if(!dbKnife.open())
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


bool SolutionSet::exportRobotDb()
{
   // exportFiles.clear();
    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".db3");
    QSqlDatabase dbRobot = QSqlDatabase::addDatabase("QSQLITE","EXPORTROBOT");
    dbRobot.setDatabaseName(backDb);
    if(!dbRobot.open())
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
    if(!query.exec(QString("CREATE TABLE RobotLibrary AS SELECT* FROM dbmain.RobotLibrary"))){
        qDebug()<<QString("CREATE TABLE RobotLibrary AS SELECT* FROM dbmain.RobotLibrary");
        tranFlag = false;
    }
    if(!query.exec(QString("CREATE TABLE RobotAxis AS SELECT* FROM dbmain.RobotAxis"))){
        qDebug()<< QString("CREATE TABLE RobotAxis AS SELECT* FROM dbmain.RobotAxis");
        tranFlag = false;
    }
    if(!query.exec(QString("CREATE TABLE RobotModule AS SELECT* FROM dbmain.RobotModule"))){
        qDebug()<<QString("CREATE TABLE RobotModule AS SELECT* FROM dbmain.RobotModule");
        tranFlag = false;
    }
    query.exec(QString("SELECT preview FROM dbmain.RobotLibrary"));
    while(query.next()){
        exportFiles<<query.value(0).toString();
    }
//    if(!query.exec(QString("CREATE TABLE AlarmCode AS SELECT id,code,title,desc FROM dbmain.AlarmCode"))){
//        qDebug()<<QString("CREATE TABLE AlarmCode AS SELECT id,code,title,desc FROM dbmain.AlarmCode");
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




bool SolutionSet::importRobotDb(const QString &dbName)
{
    QSqlDatabase dbInport = QSqlDatabase::addDatabase("QSQLITE","INPORTROBOT");
    dbInport.setDatabaseName(dbName);
    if(!dbInport.open())
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
//        qDebug()<<QString("INSERT INTO dbmain.AlarmCode (id,code,title,desc) SELECT id,code,title,desc FROM AlarmCode WHERE AlarmCode.code NOT IN (SELECT code FROM dbmain.AlarmCode)");
//        tranFlag = false;
//    }
    if(!query.exec(QString("DELETE FROM dbmain.RobotLibrary "))){
        qDebug()<< QString("DELETE FROM dbmain.RobotLibrary ");
        tranFlag = false;
    }
    if(!query.exec(QString("INSERT INTO dbmain.RobotLibrary (id,name,type,preview) SELECT id,name,type,preview FROM RobotLibrary")))
    {
        qDebug()<<QString("INSERT INTO dbmain.RobotLibrary (id,name,type,preview) SELECT id,name,type,preview FROM RobotLibrary");
        tranFlag = false;
    }
    if(!query.exec(QString("DELETE FROM dbmain.RobotAxis "))){
        qDebug()<<QString("DELETE FROM dbmain.RobotAxis ");
        tranFlag = false;
    }
    if(!query.exec(QString("INSERT INTO dbmain.RobotAxis (id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse) SELECT id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse FROM RobotAxis"))){
        qDebug()<<QString("INSERT INTO dbmain.RobotAxis (id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse) SELECT id,robotId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse FROM RobotAxis");
        tranFlag = false;
    }
    if(!query.exec(QString("DELETE FROM dbmain.RobotModule"))){
        qDebug()<<QString("DELETE FROM dbmain.RobotModule");
        tranFlag = false;
    }
    if(!query.exec(QString("INSERT INTO dbmain.RobotModule (id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse) SELECT id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse FROM RobotModule"))){
        qDebug()<<QString("INSERT INTO dbmain.RobotModule (id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse) SELECT id,robotId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,material,color,transparent,isUse FROM RobotModule");
        tranFlag = false;
    }
//    if(!query.exec(QString("DELETE FROM dbmain.AlarmCode"))){
//        qDebug()<<QString("DELETE FROM dbmain.AlarmCode");
//        tranFlag = false;
//    }
//    if(!query.exec(QString("INSERT INTO dbmain.AlarmCode (id,code,title,desc) SELECT id,code,title,desc FROM AlarmCode"))){
//        qDebug()<<QString("INSERT INTO dbmain.AlarmCode (id,code,title,desc) SELECT id,code,title,desc FROM AlarmCode");
//        tranFlag = false;
//    }
//    if(!query.exec(QString("DELETE FROM dbmain.KnifeLibrary"))){
//        qDebug()<<QString("DELETE FROM dbmain.KnifeLibrary");
//        tranFlag = false;
//    }
//    if(!query.exec(QString("INSERT INTO dbmain.KnifeLibrary (id,name,code,model,count,used) SELECT id,name,code,model,count,used FROM KnifeLibrary"))){
//        qDebug()<<QString("INSERT INTO dbmain.KnifeLibrary (id,name,code,model,count,used) SELECT id,name,code,model,count,used FROM KnifeLibrary");
//        tranFlag = false;
//    }
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

    QSqlDatabase dbs1 = QSqlDatabase::addDatabase("QSQLITE",currentStationId);
    backDb =QDir::tempPath().append("/" +SysUtils::uuid()+".db3");
    //QString backDb = path.append(".db3");
    dbs1.setDatabaseName(backDb);
    if ( !dbs1.open())
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
    if(!query.exec(QString("create table StationModule as select* from dbmain.StationModule where stationId = '%1'").arg(currentStationId)))
    {
        tranFlag = false;
        qDebug()<<"failed" <<QString("create table StationModule as select sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse  from  dbmain.StationModule where stationId = '%1'").arg(currentStationId);

    }
    if(!query.exec(QString("create table StationAxis as select * from  dbmain.StationAxis where stationId = '%1'").arg(currentStationId)))
    {
         tranFlag = false;
        qDebug()<<"failed"<<QString("create table StationAxis as select *  from  dbmain.StationAxis where stationId = '%1'").arg(currentStationId);

    }
    if(!query.exec(QString("create table StationAction as select *  from  dbmain.StationAction where stationId = '%1'").arg(currentStationId)))
    {
        tranFlag = false;
        qDebug()<<"failed"<<QString("create table StationAxis as select *  from  dbmain.StationAxis where stationId = '%1'");
    }
    if(!query.exec(QString("create table StationActionDetail as select *  from  dbmain.StationActionDetail where 1=0" )))
    {
        tranFlag = false;
        qDebug()<<"failed"<<QString("create table StationActionDetail as select *  from  dbmain.StationActionDetail where 1=0" );
    }
 /********************update8 by cxs 6.3***************************/
    if(!query.exec(QString("CREATE TABLE StationMotion as select id,stationId,name,type,libraryId,robotNo FROM dbmain.StationMotion where stationId ='%1'").arg(currentStationId))){
        tranFlag = false;
        qDebug()<<"failed:"<<QString("CREATE TABLE StationMotion as select id,stationId,name,type,libraryId,robotNo FROM dbmain.StationMotion");
    }

 /****************************end*********************************/
  /*  QString  newstationId = SysUtils::uuid();
    QString name = compressFile;
    name.chop(4);
    int len= name.lastIndexOf('/');
    if(len)
        name = name.right(name.count()-len-1);
    if(!query.exec(QString("update Station set id = '%1', name='%2'  where id = '%3' ").arg(newstationId,name,currentStationId)))
    {
        tranFlag = false;
        qDebug()<<QString("update Station set id = '%1', name='%2' where id = '%3' ").arg(newstationId,name,currentStationId).append( "falied");
    }
    QStringList idList;
    QStringList newIdList;
    queryRead.exec(QString("select id from StationModule"));

    while(queryRead.next())
    {

        QString id = queryRead.value(0).toString();
        idList<<id;
        QString idNew = SysUtils::uuid();
        newIdList<<idNew;
//        if(query.exec(QString("update StationModule set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,stationId,id)))
//            qDebug()<<"Success" <<QString("update StationModule set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,stationId,id);
    }

    query.prepare(QString("update StationModule set id = ? ,stationId ='%1' where id=?").arg(newstationId));
    query.addBindValue(newIdList);
    query.addBindValue(idList);
    if(!query.execBatch())
    {
        tranFlag = false;
        qDebug()<<"Bach module failed";
    }
   idList.clear();
   newIdList.clear();
   queryRead.exec(QString("select id from StationAxis"));
   while(queryRead.next())
    {
        QString id = queryRead.value(0).toString();
        idList<<id;
        QString idNew = SysUtils::uuid();
        newIdList<<idNew;
//        if(query.exec(QString("update StationAxis set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id)))
//            qDebug()<<"Success"<<QString("update StationAxis set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id);
    }
   query.prepare(QString("update StationAxis set id = ? ,stationId ='%1' where id=?").arg(newstationId));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach Axis failed";
   }
    idList.clear();
    newIdList.clear();


   queryRead.exec(QString("select id from StationAction"));
   while(queryRead.next())
   {
        QString id = queryRead.value(0).toString();
        idList<<id;
        QString idNew = SysUtils::uuid();
        newIdList<<idNew;
//        if(!query.exec(QString("update StationAction set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id)))
//            qDebug()<<QString("update StationAction set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id).append("failed");

        if(!query.exec(QString("insert into StationActionDetail(id,actionId,name,type,moduleId,value,isUse)  select id,'%1',name,type,moduleId,value,isUse from dbmain.StationActionDetail where actionId = '%2'").arg(idNew,id)))
            qDebug()<<"failed";
      //  if(query.exec())
   }
   query.prepare(QString("update StationAction set id = ? ,stationId ='%1' where id=?").arg(newstationId));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach Action failed";
   }
  idList.clear();
  newIdList.clear();


    queryRead.exec(QString("select id from StationActionDetail"));
            while(queryRead.next())
    {
         QString id = queryRead.value(0).toString();
         idList<<id;
         QString idNew = SysUtils::uuid();
         newIdList<<idNew;
//       if(!query.exec(QString("update StationActionDetail set id ='%1' where id ='%2'").arg(idNew,id)))
//             qDebug()<<"failed;
    }
   query.prepare(QString("update StationActionDetail set id = ?  where id=?"));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach ActionDeatial failed";
    }
    idList.clear();
    newIdList.clear();*/

    if(tranFlag)
        dbs1.commit();
    else
        dbs1.rollback();

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
    case ALARMCODEEXPORT:
        compFile.write("41");
        break;
    default:
        compFile.write("01");  //无效文件
        break;
    }
    for(int i= 0; i<srcFile.count(); ++i)
    {
        int sFNameLen = srcFile.at(i).count();
        QByteArray sFName =srcFile.at(i).toLatin1();
        char *data = sFName.data();
        qint64 sum =  qChecksum(data,sFName.count());
        compFile.write((char*)&sFNameLen,sizeof(sFNameLen));
        compFile.write(sFName);
        compFile.write((char*)&sum,sizeof(sum));
        QFile sFile(srcFile.at(i));
        if(!sFile.open(QIODevice::ReadOnly))
        {
            qDebug()<<"找不到需要导出的组件";
            return  false;
        }
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
        //if(srcFile.at(i).right(4) == ".db3")
       // sFile.remove();
    }
    compFile.close();
    return true;
}





bool SolutionSet::importProgramDb(QString solDb)
{
    QSqlDatabase dbs2 = QSqlDatabase::addDatabase("QSQLITE","solDb");
    QString backDb =solDb;
    dbs2.setDatabaseName(backDb);
    if ( !dbs2.open())
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
    QString name = compressFile;
    name.chop(4);
    int len= name.lastIndexOf('/');
    if(len)
        name = name.right(name.count()-len-1);
    if(!query.exec(QString("update Station set id = '%1', name='%2'   ").arg(newstationId,name)))
    {
        tranFlag = false;
        qDebug()<<QString("update Station set id = '%1', name='%2'   ").arg(newstationId,name);
    }
    QStringList idList;
    QStringList newIdList;
    queryRead.exec(QString("select id from StationModule"));

    while(queryRead.next())
    {

        QString id = queryRead.value(0).toString();
        idList<<id;
        QString idNew = SysUtils::uuid();
        newIdList<<idNew;
//        if(query.exec(QString("update StationModule set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,stationId,id)))
//            qDebug()<<"Success" <<QString("update StationModule set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,stationId,id);
    }

    query.prepare(QString("update StationModule set id = ? ,stationId ='%1' where id=?").arg(newstationId));
    query.addBindValue(newIdList);
    query.addBindValue(idList);
    if(!query.execBatch())
    {
        tranFlag = false;
        qDebug()<<"Bach module failed";
    }
   idList.clear();
   newIdList.clear();
   queryRead.exec(QString("select id from StationAxis"));
   while(queryRead.next())
    {
        QString id = queryRead.value(0).toString();
        idList<<id;
        QString idNew = SysUtils::uuid();
        newIdList<<idNew;
//        if(query.exec(QString("update StationAxis set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id)))
//            qDebug()<<"Success"<<QString("update StationAxis set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id);
    }
   query.prepare(QString("update StationAxis set id = ? ,stationId ='%1' where id=?").arg(newstationId));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach Axis failed";
   }
    idList.clear();
    newIdList.clear();


   queryRead.exec(QString("select id from StationAction"));
   while(queryRead.next())
   {
        QString id = queryRead.value(0).toString();
        idList<<id;
        QString idNew = SysUtils::uuid();
        newIdList<<idNew;
//        if(!query.exec(QString("update StationAction set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id)))
//            qDebug()<<QString("update StationAction set id = '%1',stationId ='%2' where id='%3' ").arg(idNew,newstationId,id).append("failed");

        if(!query.exec(QString("insert into StationActionDetail(id,actionId,name,type,moduleId,value,isUse)  select id,'%1',name,type,moduleId,value,isUse from dbmain.StationActionDetail where actionId = '%2'").arg(idNew,id)))
            qDebug()<<"failed";
      //  if(query.exec())
   }
   query.prepare(QString("update StationAction set id = ? ,stationId ='%1' where id=?").arg(newstationId));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach Action failed";
   }
  idList.clear();
  newIdList.clear();


    queryRead.exec(QString("select id from StationActionDetail"));
            while(queryRead.next())
    {
         QString id = queryRead.value(0).toString();
         idList<<id;
         QString idNew = SysUtils::uuid();
         newIdList<<idNew;

    }
   query.prepare(QString("update StationActionDetail set id = ?  where id=?"));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach ActionDeatial failed";
    }
    idList.clear();
    newIdList.clear();



    queryRead.exec(QString("select id from StationMotion"));
    while(queryRead.next())
    {
         QString id = queryRead.value(0).toString();
         idList<<id;
         QString idNew = SysUtils::uuid();
         newIdList<<idNew;

    }
   query.prepare(QString("update StationMotion set id = ? ,stationId ='%1' where id=?").arg(newstationId));
   query.addBindValue(newIdList);
   query.addBindValue(idList);
   if(!query.execBatch())
   {
       tranFlag = false;
       qDebug()<<"Bach StationMotion failed";
    }
    idList.clear();
    newIdList.clear();

 /****************end********************************************/

    query.exec(QString("select id,name from Station"));
    while(query.next())
    {
        newStationId = query.value(0).toString();
        newStationName = query.value(1).toString();
        break;
    }

    if(!query.exec(QString("insert into dbmain.Station (id,type,name) select id,type,name from Station")))
    {
        tranFlag = false;
        qDebug()<<QString("insert into dbmain.Station select* from Station").append(" failed");

    }
    if(!query.exec(QString("insert into dbmain.StationModule(id,stationId,name,type,filename,parentId,bindAxis,x,y,z,rotateX,rotateY,rotateZ,initAngle,  \
                           material,color,transparent,isUse) select id,stationId,name,type,filename,parentId,bindAxis,  \
                           x,y,z,rotateX,rotateY,rotateZ,initAngle,material,   \
                           color,transparent,isUse from StationModule")))
    {
        qDebug()<<QString("insert into dbmain.StationModule select* from StationModule").append(" failed");
        tranFlag = false;
    }
    if(!query.exec(QString("insert into dbmain.StationAxis (id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse) select id,stationId,sign,name,type,parentId,startX,startY,startZ,endX,endY,endZ,symbol,minValue,maxValue,originOffset,isUse  from StationAxis")))
    {
        tranFlag = false;
        qDebug()<<QString("insert into dbmain.StationAxis select* from StationAxis").append(" failed");
    }
    if(!query.exec(QString("insert into dbmain.StationAction (id,stationId,name,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3,count,updateType,isUse) select id,stationId,name,type,code,operator,value,extraTrigger,code2,operator2,value2,extraTrigger2,code3,operator3,value3,count,updateType,isUse from StationAction"))){
        qDebug()<<QString("insert into dbmain.StationAction select* from StationAction").append(" failed");
        tranFlag = false;
    }
    if(!query.exec(QString("insert into dbmain.StationActionDetail (id,actionId,name,type,moduleId,value,isUse) select id,actionId,name,type,moduleId,value,isUse from StationActionDetail"))){
        tranFlag = false;
        qDebug()<<QString("insert into dbmain.StationActionDetail select* from StationActionDetail").append(" failed");
    }
/******************update 8 by  cxs *********************/
    if(!query.exec(QString("INSERT INTO dbmain.StationMotion (id,stationId,name,type,libraryId,robotNo ) SELECT id,stationId,name,type,libraryId,robotNo from StationMotion"))){
         tranFlag = false;
         qDebug()<<"SQL FAILED";
    }
/**********************end**************************************/
    if(tranFlag)
        dbs2.commit();
    else
        dbs2.rollback();
    query.exec(QString("DETACH DATABASE dbmain"));
    dbs2.close();
    return tranFlag;
}



/***************update8 ********************/
QString SolutionSet::praseDirPath(QString fileName)
{
    QString path = fileName;
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



SolutionSet::SolutionSet()
{
    isTaskSuccess = false;
}
