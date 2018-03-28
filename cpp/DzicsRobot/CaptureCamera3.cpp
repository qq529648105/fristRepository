#include "CaptureCamera3.h"
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
static int camX = 0;
static int camY = 0;


CaptureCamera3::CaptureCamera3(int index,int width,int height, QObject *parent)
{
    camWidth  = width;
    camHeight = height;
    num2=0;
    runFlag = false;

    captureIndex=index;
    img = cvCreateImage(cvSize( camWidth, camHeight ), IPL_DEPTH_8U, 1);
    cameraIpImg = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
    zoomIpImg = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);
    IpImgCache[index] = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 1);

    cap=new VideoCapture(0);

    if(!cap->isOpened())
    {
        qDebug()<<"相机打开失败";
        return ;
    }
    runFlag=true;

}

CaptureCamera3::~CaptureCamera3()
{
    stopCaptureCamera();
}

void CaptureCamera3::startCaptureCamera()
{

    this->start();
//    this->run();

}

double CaptureCamera3::getExpourseValue()
{
   return 50;
}

void CaptureCamera3::setExpourseValue(double t)
{

}

bool CaptureCamera3::initCamera(const char *cameraIp)
{
    return true;
}

void CaptureCamera3::resumeCamera()
{

}


void CaptureCamera3::stopCaptureCamera()
{
    runFlag = false;

    cap->release();
    cvResetImageROI(cameraIpImg);
    cvZero(cameraIpImg);
    cvResetImageROI(zoomIpImg);
    cvZero(zoomIpImg);
}









IplImage *CaptureCamera3::getZoomImage(CvRect rect)
{
//    lock.lock();
//    cvCvtColor( img, zoomIpImg, CV_GRAY2RGB );
    cvResetImageROI(zoomIpImg);
    cvCvtColor(img, zoomIpImg, CV_GRAY2RGB);
    cvSetImageROI(zoomIpImg, rect);
//    lock.unlock();
    return zoomIpImg;
}

void CaptureCamera3::run()
{

    while(runFlag){
        QThread::msleep(200);
        ++fNum[captureIndex];

        *cap>>frame;
        qDebug()<<"frame"<<frame.cols<<frame.rows;
        cvtColor(frame, fGray, CV_BGR2GRAY);
        IpImgCache[captureIndex]=&IplImage(fGray);
        //qDebug()<<"size:"<<IpImgCache[captureIndex]->imageSize<<IpImgCache[captureIndex]->width<<IpImgCache[captureIndex]->height;
        //imshow("当前视频",fGray);
//        mylock[captureIndex].lock();
//        //qDebug()<<"aaa"<<captureIndex;
//        IpImgCache[captureIndex]=&IplImage(frame);
//        ++fNum[captureIndex];
//        mylock[captureIndex].unlock();


        emit signalCamera();

    }



}
