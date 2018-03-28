#ifndef CAPTURECAMERA_H
#define CAPTURECAMERA_H


#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QImage>
#include "cv.h"
#include "basecapture.h"

#include "VimbaCPP/Include/VimbaCPP.h"
#include "Manta_G_201BCamera.h"
using namespace AVT::VmbAPI;


class FrameObserver2 : public IFrameObserver
{

public:
    FrameObserver2( CameraPtr pCamera );// : IFrameObserver( pCamera )
    FrameObserver2( CameraPtr pCamera,int x );
    void FrameReceived( const FramePtr pFrame );
    IplImage *img;
    int FrameIndex;


};

class CaptureCamera : public BaseCapture
{
    Q_OBJECT
public:

    explicit CaptureCamera(QString ip, int index, QObject *parent = 0);
    ~CaptureCamera();
    void stopCaptureCamera();
    void startCaptureCamera();

    double getExpourseValue();
    void setExpourseValue(double);
    int captureIndex;

    //avt
    void resumeCamera();
    bool initCamera(const char* cameraIp);
    VmbErrorType AcquisitionStart ();

    int camWidth;
    int camHeight;

protected:
    void run();

private:

    IplImage *img, *zoomIpImg;
    CameraPtr myCamera;
    FeaturePtr pFeature;
    VmbErrorType openErr;
    QString ip;

signals:
    void signalCamera();
    void addLog(QString message, QString time);

public slots:

    
};

#endif // CAPTURECAMERA_H
