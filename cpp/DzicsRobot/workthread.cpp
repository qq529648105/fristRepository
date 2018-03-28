#include "workthread.h"
#include "Global.h"

workThread::workThread(QObject *parent) :
    QObject(parent)
{  
}
extern QString getUsedMemory(DWORD pid);
extern DWORD getProcessByName(QString proName);

void workThread::serviceTimer()
{
    qDebug()<<"bbb";
    triggerTime++;
    if(triggerTime%360==0)
    {
        DWORD pid=getProcessByName("DzicsRobotService.exe");
        if(pid)
        {
            mainWindow->writeLog("DzicsRobotService.exe "+getUsedMemory(pid));
        }
    }

    if(mainWindow->heartBeat2==mainWindow->heartBeat)
    {
        emit addLog("服务重启","");

        WinExec("taskkill /f /t /im DzicsRobotService.exe", SW_HIDE);
        Sleep(500);
        WinExec("net start DzicsRobotService", SW_HIDE);

        Sleep(2000);

        emit reConn();


    }
    mainWindow->heartBeat2=mainWindow->heartBeat;

}

void workThread::rebootServiceSlot()
{

    emit addLog("手动重启服务","");

    WinExec("taskkill /f /t /im DzicsRobotService.exe", SW_HIDE);
    Sleep(500);
    WinExec("net start DzicsRobotService", SW_HIDE);

    Sleep(2000);

    emit reConn();
}

void workThread::detectCam()
{

    for(int i=0;i<g_camera->ipAraay.length();i++)
    {

        if(dectedNum[i]==fNum[i])
        {
            emit addLog(QString("相机%1正在重连...").arg(i),"");
            if(g_camera->captureCamera[i])
                g_camera->captureCamera[i]->resumeCamera();
        }
        dectedNum[i]=fNum[i];

    }

}

