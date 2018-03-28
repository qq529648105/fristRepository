#include "ScreeCapture.h"
#include<QString>
#include <QPixmap>
#include <QTimer>
#include <qapplication.h>
#include <QDir>
#include <QTime>
#include <QMutex>
#include <QDebug>


#define MAXSIZE 300
typedef struct PIXINFO{
    int index;
    QImage image;
}pixInfo;


QList<pixInfo>pixList;
QMutex myMutex;

SaveCapterThread ::SaveCapterThread(QString &sTempFolder)
{
    tempFolder = sTempFolder;
    runFlag = true;
}

SaveCapterThread::~SaveCapterThread()
{


}



void SaveCapterThread::run()
{
    msleep(200);
    while(runFlag)
    {
        QThread::msleep(10);
        if(pixList.isEmpty())
        {

            runFlag = false;

        }
        else
        {

            myMutex.lock();

            QImage pixMap = pixList.at(0).image;
            QString num = QString::number( pixList.at(0).index);
            qDebug()<<"get  "<<pixList.at(0).index;
             pixList.pop_front();
               myMutex.unlock();
            while(num.count()<6)
                num.insert(0,'0');
            QString filename =  QString("img_%1.png").arg(num);
            filename = tempFolder +"/" +filename;
            QTime time;
            time.start();
            pixMap.save(filename);
            qDebug()<<time.elapsed()<<" save time"<<filename;

        }

    }
    msleep(10);


}



ScreeCapture::ScreeCapture():interMsec(40),
    screenHeight(0),screenWidth(0),
    mscreen(NULL),dirIndex(1)
{
    pixIndex =0 ;
    tempDir = QDir::tempPath();

}


ScreeCapture::~ScreeCapture()
{


}

void ScreeCapture::SetCaptureInter(int msec)
{
   interMsec = msec;
}

void ScreeCapture::MakeTempFolder(QString folder)
{

    tempFolder =tempDir +"/"+folder;
    QDir myDir;
    if(!myDir.exists(tempFolder))
        myDir.mkdir (tempFolder);
    else
    {

        myDir.mkdir(tempFolder.append(QString("_%1").arg(dirIndex)));

    }
    dirIndex++;

}

bool ScreeCapture::CapterScreen()
{

   if(pixList.size()>MAXSIZE)
   {
       qDebug()<<"wait";
       return false;
   }
   pixmap=QPixmap::grabWidget(mscreen,0,0,screenWidth,screenHeight);
   QImage image = pixmap.toImage();
   pixInfo imageInfo ={pixIndex, image};
   pixList.push_back(imageInfo);
   pixIndex++;
   return true;
}


/***********************
函数: 开始截屏
返回类型： void
参数:
*screen  需要截屏的QWidget 对象指针
 width   QWidget 的宽度
 height  QWidget 长度
 msec    截屏的间隔毫秒数
***********************/
void ScreeCapture::StartCapScreen(QWidget *screen ,int width, int height,int msec)
{
    pixList.clear();
    MakeTempFolder("myPicture");
    mscreen = screen;
    screenHeight = height;
    screenWidth = width;
    interMsec = msec;
    connect(&interTime,SIGNAL(timeout()),this,SLOT(CapterScreen()));
    CapterScreen();
    saveCapterThread = new SaveCapterThread(tempFolder);
    saveCapterThread1 = new  SaveCapterThread(tempFolder);
    saveCapterThread2 = new  SaveCapterThread(tempFolder);
    connect(saveCapterThread,SIGNAL(finished()),saveCapterThread,SLOT(deleteLater()));
    //connect(saveCapterThread1,SIGNAL(finished()),saveCapterThread1,SLOT(deleteLater()));
    saveCapterThread->start();
    //saveCapterThread1->start();

    interTime.start(interMsec);
}




/***************************
 * 函数:  停止截屏
 * 返回类型  QString
 * 返回存放截屏图片目录的临时文件夹路径
 *
 *******************************/

QString ScreeCapture::StopCapScreen()
{

    pixIndex =0;
    disconnect(&interTime,SIGNAL(timeout()),this,SLOT(CapterScreen()));
    interTime.stop();
    SaveCapterThread *saveThread1 = new SaveCapterThread(tempFolder);
    SaveCapterThread *saveThread2 = new SaveCapterThread(tempFolder);
    SaveCapterThread *saveThread3 = new SaveCapterThread(tempFolder);
    connect(saveThread1,SIGNAL(finished()),saveThread1,SLOT(deleteLater()));
    connect(saveThread2,SIGNAL(finished()),saveThread2,SLOT(deleteLater()));
    connect(saveThread3,SIGNAL(finished()),saveThread3,SLOT(deleteLater()));
    saveThread1->start();
    saveThread2->start();
    saveThread3->start();
    return tempFolder;

}

