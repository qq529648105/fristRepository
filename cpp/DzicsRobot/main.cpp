#include <singleApp/qtsingleapplication.h>
#include "MainWindow.h"

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QVariant>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>
#include <QUuid>
#include <QSettings>

#include <SysUtils.h>
#include <QProcess>

#include <windows.h>
#include <winuser.h>
#include <tlhelp32.h>
#include <string.h>

#define SOCRASS
//#ifdef _DEBUG
//#include "vld.h"
//#endif


//extern int _tmain(int , char**);
QTranslator languageTranslator;
QString robotDat;
MainWindow *mainWindow = NULL;
bool isOpenXLContoller=false;


QString getUsedMemory(DWORD pid)
{
    char pidChar[25];
    //将DWORD类型转换为10进制的char*类型
    _ultoa(pid,pidChar,10);

    //调用外部命令
    QProcess p;
    p.start("tasklist /FI \"PID EQ " + QString(pidChar) + " \"");
    p.waitForFinished();
    //得到返回结果
    QString result = QString::fromLocal8Bit(p.readAllStandardOutput());
    //关闭外部命令
    p.close();

    //替换掉","
    result = result.replace(",","");
    //匹配 '数字+空格+K'部分。
    QRegExp rx("(\\d+)(\\s)(K)");
    //初始化结果
    QString usedMem("");
    if(rx.indexIn(result) != -1){
        //匹配成功
        usedMem = rx.cap(0);
    }
    //截取K前面的字符串，转换为数字，供换算单位使用。

    usedMem = usedMem.left(usedMem.length() - 1);
    //换算为MB的单位
    return QString::number(usedMem.toDouble() / 1024) + " MB";
}

DWORD getProcessByName(QString proName)
{
    PROCESSENTRY32 pe32;
    pe32.dwSize = sizeof(pe32);
    HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    BOOL bMore = ::Process32First(hProcessSnap, &pe32);

    while(bMore)
    {

       // printf(" 进程名称：%s \n", pe32.szExeFile);
      QString name= QString::fromWCharArray( pe32.szExeFile);

     // qDebug()<<"info "<<getUsedMemory(pe32.th32ProcessID);

      if(name==proName)
      {
            return pe32.th32ProcessID;
      }
        bMore = ::Process32Next(hProcessSnap, &pe32);
    }
    return 0;
}

int main(int argc, char *argv[])
{


//for (int var = 0; var < 10; ++var) {
//    qDebug(SysUtils::uuid().toLocal8Bit().data());
//}
//return 0;

    QtSingleApplication app(argc, argv);

    if (app.sendMessage("WakeUp", 50))
        return 0;
//    qChecksum()
//    QStringList paramList;
//    paramList.append("start");
//    paramList.append("DzicsRobotService");
//    QProcess::startDetached("sc", paramList);

    WinExec("taskkill /f /t /im DzicsRobotService.exe", SW_HIDE);
    Sleep(300);

    //rong lk
        WinExec("taskkill /f /t /im workObjCal.exe", SW_HIDE);
    //end
        Sleep(300);
    //rong lk
        WinExec("workObjCal.exe", SW_HIDE);
    //end

    WinExec("net start DzicsRobotService", SW_HIDE);
    WinExec("net start DzicsDensoService", SW_HIDE);
    WinExec("net start DzicsURService", SW_HIDE);
    WinExec("net start DzicsFanucService", SW_HIDE);
    WinExec("net start DzicsKawasakiService", SW_HIDE);
    WinExec("net start DzicsRobotService2", SW_HIDE);
    WinExec("net start DzicsDensoService2", SW_HIDE);
    WinExec("net start DzicsURService2", SW_HIDE);
    WinExec("net start DzicsFanucService2", SW_HIDE);
    WinExec("net start DzicsKawasakiService2", SW_HIDE);
    QProcess p1;
   // p1.startDetached("sqlbrowser.exe");




    if(getProcessByName("DzicsXLContollerCom.exe"))
    {

        isOpenXLContoller=true;
    }

//    SysUtils::externalPrint(false);

//    QTextCodec::setCodecForLocale( QTextCodec::codecForName("UTF-8") );
//    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("UTF-8") );
//    QTextCodec::setCodecForTr( QTextCodec::codecForName("UTF-8") );
    QTextCodec::setCodecForLocale( QTextCodec::codecForName("GBK") );
    QTextCodec::setCodecForCStrings( QTextCodec::codecForName("GBK") );
    QTextCodec::setCodecForTr( QTextCodec::codecForName("GBK") );

	QSettings *configIniRead = new QSettings("D:\\DzicsRobot\\ROBOT.INI", QSettings::IniFormat);
//    robotDat = configIniRead->value("/Robot/DAT").toString();
    robotDat = "D:\\DzicsRobot\\DzicsRobot.dat";
	delete configIniRead;

//    languageTranslator.load("DzicsRobot_zh_CN");
    languageTranslator.load("qt_zh_CN");
    app.installTranslator(&languageTranslator);

    QSqlDatabase db = QSqlDatabase::addDatabase("QSDBase");
    db.setDatabaseName(robotDat);//"DzicsRobot.dat");
    if ( !db.open(db.databaseName(),"^@8#*6%d&+") )
    {
        QMessageBox::critical(NULL, QObject::tr("系统错误"), QObject::tr("系统初始化失败！"));
        return 1;
    }

    if (!SysUtils::checkDatabase()) {
        QMessageBox::critical(NULL, QObject::tr("系统错误"), QObject::tr("系统初始化失败！"));
        return 2;
    }

//    // 增加模型文件
//    QSqlQuery query;
//    QString uuid = SysUtils::uuid().toString();
//    QString fileId = SysUtils::uuid().toString();
//    query.exec(QString("INSERT INTO Module(id,code,name,type,fileId,material,color,parentId,isUse,isLock) VALUES('%1','chedao','车刀',1,'%2',19,16777215,'',1,0)").arg(uuid,fileId));
//    SysUtils::addFile(fileId, SysUtils::readFile(SysUtils::getSysParam("modelPath").toString()+"/chedao.brep"), "chedao");


////    query.exec("CREATE TABLE test(id INTEGER PRIMARY KEY AUTOINCREMENT,name VARCHAR(40) NOT NULL)");
////    query.exec("INSERT INTO test(name) VALUES('abcd中文efg')");
//
//    while(query.next()){
////        QMessageBox::information(NULL, QObject::tr("Coll中国ection"), query.value(1).toString());
//    }

//_tmain(argc, argv);
//return 0;


    app.setAutoSipEnabled(true); // 自动弹出系统软键盘

    mainWindow = new MainWindow();
    app.setActivationWindow(mainWindow);
    mainWindow->show();


    return app.exec();
}
