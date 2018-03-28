#include "CaptureCamera2.h"
#include "highgui.h"
#include "cv.h"
#include <QMutex>
#include <QDebug>
#include <QMessageBox>

extern IplImage* cameraIpImg;
extern QString  cameraImage;
extern QMutex lock;
extern QMap<int,IplImage*> IpImgCache;
extern QMutex mylock[5];
extern int cur_cameraIndex;
extern int fNum[5];
extern int version;


static int camX = 0;
static int camY = 0;


CaptureCamera2::CaptureCamera2(int index,int width,int height, QObject *parent)
{



    camWidth  = width;
    camHeight = height;
//    if(version==2)
//    {
//        camWidth =2592;
//        camHeight=1944;
//    }

    runFlag = false;



    captureIndex=index;
    img = cvCreateImage(cvSize( camWidth, camHeight ), IPL_DEPTH_8U, 1);
    cameraIpImg = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
    zoomIpImg = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
    IpImgCache[index] = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 1);
//    img = cvCreateImageHeader(cvSize( camWidth, camHeight ), IPL_DEPTH_8U, 1);
//    qImg = new QImage(QSize(camWidth,camHeight), QImage::Format_RGB888);

//    cameraIpImg = cvCreateImageHeader( cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
//    cameraIpImg->imageData = (char*)qImg->bits();



}

CaptureCamera2::~CaptureCamera2()
{
    runFlag = false;
    qDebug()<<"~CaptureCamera2"<<&m_hCam;
    //MVStopGrab(m_hCam);
    //MVCloseCam(m_hCam);
    MVTerminateLib();

}

void CaptureCamera2::startCaptureCamera()
{

    this->start();
//    this->run();

}

double CaptureCamera2::getExpourseValue()
{
    double t;
    MVGetExposureTime(m_hCam,&t);
    return t;
}

void CaptureCamera2::setExpourseValue(double t)
{
    qDebug()<<"this"<<QThread::currentThreadId();
    MVSetExposureTime(m_hCam,t);
}

bool CaptureCamera2::initCamera(const char *cameraIp)
{
    return true;
}

void CaptureCamera2::resumeCamera()
{

}


void CaptureCamera2::stopCaptureCamera()
{
    runFlag = false;
//    MVStopGrab(m_hCam);
    cvResetImageROI(cameraIpImg);
    cvZero(cameraIpImg);
    cvResetImageROI(zoomIpImg);
    cvZero(zoomIpImg);
}



//void CaptureCamera2::CaptureCamera2()
//{
//    char*  ch;
//    QByteArray ba = "E:\\qt\\project\\DzicsRobot\\B0.PNG";//cameraImage.toLatin1();
//    ch=ba.data();
////    lock.lock();
//    cvReleaseImage(&cameraIpImg);
//    cameraIpImg = cvLoadImage(ch,1);
//    cvCvtColor(cameraIpImg,cameraIpImg,CV_BGR2RGB);
////    lock.unlock();
//    emit signalCamera();
//}


//int StreamCB(MV_IMAGE_INFO *pInfo, long nUserVal)
//{
//    qDebug("StreamCB %d\n", pInfo->nSizeX);

//    lock.lock();
//    cvSetData( img, pInfo->pImageBuffer, pInfo->nSizeX );   // * 3
//    cvCvtColor( img, cameraIpImg, CV_GRAY2RGB );
//    lock.unlock();
//    return 1;
//}

int __stdcall StreamCB(MV_IMAGE_INFO *pInfo, long nUserVal)
{
    CaptureCamera2 *pDlg = (CaptureCamera2 *)nUserVal;
    return ( pDlg->onStreamCB(pInfo) );
}

int CaptureCamera2::onStreamCB(MV_IMAGE_INFO *pInfo)
{
   // qDebug("Stream %d %d %d\n", pInfo->nImageSizeAcq, pInfo->nBlockId, pInfo->nMissingPackets);
    if(pInfo->nMissingPackets==0)
    {

        ++fNum[captureIndex];
        mylock[captureIndex].lock();
        //qDebug()<<"aaa"<<captureIndex;
        cvSetData( IpImgCache[captureIndex], pInfo->pImageBuffer, pInfo->nSizeX );   // * 3

        mylock[captureIndex].unlock();
        //emit addLog(QString::number(fNum[captureIndex]),"");

    }
    return 1;
}

IplImage *CaptureCamera2::getZoomImage(CvRect rect)
{
//    lock.lock();
//    cvCvtColor( img, zoomIpImg, CV_GRAY2RGB );
    cvResetImageROI(zoomIpImg);
    cvCvtColor(img, zoomIpImg, CV_GRAY2RGB);
    cvSetImageROI(zoomIpImg, rect);
//    lock.unlock();
    return zoomIpImg;
}

void CaptureCamera2::run()
{
    if(captureIndex==0)
    {
        MVInitLib();
        QThread::msleep(1000);
        MVUpdateCameraList();

    }

    int nCams = 0;
    MVGetNumOfCameras(&nCams);
    if( nCams == 0 )
    {
        qDebug()<<"无相机！"<<captureIndex;
        return;
    }
qDebug()<<"this"<<QThread::currentThreadId();

    MVSTATUS_CODES r;

    MVOpenCamByIndex(captureIndex,&m_hCam);
    if( m_hCam == NULL )
    {
        if( r == MVST_ACCESS_DENIED )
        {
            qDebug()<<"相机禁止访问！";
            return;

        }
    }
     runFlag = true;





    int n1=-1000;
    sec1=0;


       //    QMessageBox::information(NULL,"1","","ok");

       MVStartGrab(m_hCam, StreamCB, (long)this);

       qDebug()<<"相机采集开始"+QString::number(captureIndex);
       QThread::msleep(2000);
       while(runFlag){

           ++sec1;

           if(n1==fNum[captureIndex]&&!(sec1%9))
           {

               addLog("相机重连"+QString::number(captureIndex),"");

//                MVStopGrab(m_hCam);
//                MVTerminateLib();
//                QThread::msleep(2000);
//                MVInitLib();
//                QThread::msleep(1000);
                MVUpdateCameraList();
                int nCams = 0;
                MVGetNumOfCameras(&nCams);
                if(nCams==0)
                    continue;
                MVOpenCamByIndex(captureIndex,&m_hCam);
                emit addLog(QString("相机%1开始采集").arg(captureIndex),"");
                MVStartGrab(m_hCam, StreamCB, (long)this);
                //QThread::msleep(2000);
           }
           n1=fNum[captureIndex];
           //emit addLog(QString("n1 %1 fNum %2 sec1 %3").arg(n1).arg(fNum[captureIndex]).arg(sec1%9),"");
           QThread::msleep(300);
           //qDebug()<<captureIndex<<runFlag<<++num3;
           emit signalCamera();


       }

       MVStopGrab(m_hCam);

}
