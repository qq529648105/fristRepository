#include "CaptureCamera.h"
#include "highgui.h"
#include "cv.h"
#include <QMutex>
#include <QDebug>
#include <QMessageBox>
#include "Manta_G_201BCamera.h"
extern IplImage* cameraIpImg;
extern QMap<int,IplImage*> IpImgCache;
extern int cur_cameraIndex;
extern QString  cameraImage;
extern QMutex lock;
extern QMutex mylock[5];

int fNum[5]={0};
//HANDLE m_hCam;

static int camX = 0;
static int camY = 0;

VimbaSystem &sys= VimbaSystem :: GetInstance ();

CaptureCamera::CaptureCamera(QString ip,int index,QObject *parent)
{

    camWidth  = 1624;
    camHeight = 1234;
    runFlag = false;
    openErr=VmbErrorOther;
    captureIndex=index;

    img = cvCreateImage(cvSize( camWidth, camHeight ), IPL_DEPTH_8U, 1);
    cameraIpImg = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);

    zoomIpImg = cvCreateImage(cvSize(camWidth, camHeight), IPL_DEPTH_8U, 3);


    fNum[captureIndex]=0;
    this->ip=ip;
    initCamera(ip.toLocal8Bit().data());

    qDebug()<<"相机ip"<<ip;
}

CaptureCamera::~CaptureCamera()
{
    qDebug()<<"~CaptureCamera()"<<captureIndex;
    stopCaptureCamera();

}

void CaptureCamera::startCaptureCamera()
{

    this->start();


}




void CaptureCamera::stopCaptureCamera()
{
//    cvResetImageROI(cameraIpImg);
//    cvZero(cameraIpImg);
//    cvResetImageROI(zoomIpImg);
//    cvZero(zoomIpImg);
    runFlag=false;

    VmbErrorType err;
    qDebug()<<"cam "<<openErr;
    if(openErr == VmbErrorSuccess)
    {
        err = myCamera->GetFeatureByName( "AcquisitionStop", pFeature );
        err = pFeature->RunCommand ();
        err = myCamera-> EndCapture ();
        err = myCamera-> FlushQueue ();
        err = myCamera-> RevokeAllFrames ();
        err = myCamera->Close ();
    }
    err = sys.Shutdown ();
    openErr=VmbErrorOther;

}


void CaptureCamera::run()
{

    if(runFlag==false)
    {



        if(openErr != VmbErrorSuccess)
            return;
        VmbErrorType err;

        err = myCamera->GetFeatureByName( "AcquisitionStart", pFeature );
        err = pFeature ->RunCommand ();




    }
    runFlag=true;

    while(runFlag)
    {

        QThread::msleep(500);
        if(captureIndex==cur_cameraIndex)
        {

        emit signalCamera();
            //qDebug()<< getExpourseValue();
        }

    }

}

void CaptureCamera::resumeCamera()
{
    fNum[captureIndex]=0;
    stopCaptureCamera();
    initCamera(ip.toLocal8Bit().data());
    startCaptureCamera();

}


//avt



FrameObserver2::FrameObserver2(CameraPtr pCamera): IFrameObserver( pCamera )
{

}


FrameObserver2::FrameObserver2(CameraPtr pCamera, int x): IFrameObserver( pCamera )
{
    FrameIndex=x;
    IpImgCache[FrameIndex] = cvCreateImage(cvSize(1624, 1234 ), IPL_DEPTH_8U, 1);


}



void FrameObserver2::FrameReceived(const FramePtr pFrame)
{

    VmbFrameStatusType eReceiveStatus;
    if( VmbErrorSuccess == pFrame ->GetReceiveStatus( eReceiveStatus ) )
    {
        if ( VmbFrameStatusComplete == eReceiveStatus )
        {

            unsigned int imageSize;
            pFrame->GetImageSize((VmbUint32_t&) imageSize );
            if(imageSize==2004016)
            {

                unsigned char *pBuffer;
                pFrame->GetImage((VmbUchar_t*&)pBuffer );

                ++fNum[FrameIndex];
                mylock[FrameIndex].lock();
                cvSetData(IpImgCache[FrameIndex], pBuffer,1624);

                mylock[FrameIndex].unlock();


            }

        }
        else
        {
            qDebug()<<"///"<<eReceiveStatus;

            // Put your code here to react on an unsuccessfully received frame
        }
    }
    // When you are finished copying the frame , re -queue it
    m_pCamera ->QueueFrame( pFrame );


}





double CaptureCamera::getExpourseValue()
{
    VmbErrorType error;
    error = myCamera->GetFeatureByName("ExposureTimeAbs",pFeature);

    double value;
    error = pFeature->GetValue(value);

    return value;
}


void CaptureCamera::setExpourseValue(double value){
    VmbErrorType error;
    error = myCamera->GetFeatureByName ("ExposureTimeAbs", pFeature);
    error = pFeature->SetValue(value);

}



bool CaptureCamera::initCamera(const char* cameraIp)
{
    VmbErrorType err;
    err = sys.Startup();
    if(err != VmbErrorSuccess){
        emit addLog(QString("初始化失败 %1").arg(captureIndex),"");
        return false;
    }

    bool isInit = false;
    FramePtrVector frames(3);
    VmbInt64_t nPLS;

    openErr=sys.OpenCameraByID( cameraIp,VmbAccessModeFull ,myCamera );

    if ( VmbErrorSuccess != openErr){


        emit addLog(QString("相机%1打开失败").arg(captureIndex),"");
        qDebug()<<QString("相机%1打开失败").arg(captureIndex);
        return isInit;
    }

        FrameObserver2 *myObserver = new FrameObserver2(myCamera,captureIndex);
        myObserver->FrameIndex=captureIndex;

        IFrameObserverPtr pObserver(myObserver);
        err = myCamera -> GetFeatureByName( "PayloadSize", pFeature );

        err = pFeature ->GetValue( nPLS );
        for ( FramePtrVector :: iterator iter = frames.begin ();frames.end() != iter;++iter )
        {
            ( *iter ). reset( new Frame( nPLS ) );
            err = ( *iter )->RegisterObserver( pObserver );
            err = myCamera-> AnnounceFrame( *iter );
        }


    err = myCamera-> StartCapture ();

    for ( FramePtrVector :: iterator iter = frames.begin ();frames.end() != iter;++iter )
    {
        err = myCamera-> QueueFrame( *iter );
    }
    return true;
}





