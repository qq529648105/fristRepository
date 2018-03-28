#ifndef CaptureCamera3_H
#define CaptureCamera3_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QImage>
#include <QObject>
#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "basecapture.h"

using namespace cv;
class CaptureCamera3 :  public BaseCapture
{
    Q_OBJECT
public:
    explicit CaptureCamera3(int index, int width, int height, QObject *parent = 0);
    ~CaptureCamera3();
    void stopCaptureCamera();
    void startCaptureCamera();
    double getExpourseValue();
    void setExpourseValue(double);
    bool initCamera(const char* cameraIp);
    void resumeCamera();

    IplImage *getZoomImage(CvRect rect);
    int captureIndex;
    int camWidth;
    int camHeight;

protected:
    void run();
private:
    int num2,num3;
    IplImage *img, *zoomIpImg;
    Mat frame,fGray;
    VideoCapture *cap;


signals:
    void signalCamera();
    void addLog(QString message, QString time);
public slots:

};

#endif // CaptureCamera3_H
