#ifndef BASECAPTURE_H
#define BASECAPTURE_H

#include <QThread>

class BaseCapture : public QThread
{
public:
    BaseCapture();
    bool runFlag;
    int picNum;
    int interval;
    int endFrame;
    int savedNum;
    virtual void stopCaptureCamera()=0;
    virtual void startCaptureCamera()=0;
    virtual bool initCamera(const char* cameraIp)=0;
    virtual double getExpourseValue()=0;
    virtual void setExpourseValue(double)=0;
    virtual void resumeCamera()=0;
    //virtual void changeExposureTimeAbs(double value)=0;
    virtual void run()=0;


};

#endif // BASECAPTURE_H
