#ifndef CAPTURECAMERA2_H
#define CAPTURECAMERA2_H

#include <QObject>
#include <QThread>
#include <QReadWriteLock>
#include <QImage>
#include <QObject>
#include "MVGigE.h"
#include "cv.h"
#include "basecapture.h"

class CaptureCamera2 :  public BaseCapture
{
    Q_OBJECT
public:

    explicit CaptureCamera2(int index, int width, int height, QObject *parent = 0);
    ~CaptureCamera2();
    void stopCaptureCamera();
    void startCaptureCamera();
    double getExpourseValue();
    void setExpourseValue(double);
    bool initCamera(const char* cameraIp);
    void resumeCamera();
    int onStreamCB( MV_IMAGE_INFO *pInfo);
    IplImage *getZoomImage(CvRect rect);
    int captureIndex;
    int camWidth;
    int camHeight;

protected:
    void run();
private:
    int sec1;
    HANDLE m_hCam;


//    QImage *qImg;
//    QReadWriteLock lock;
   // void captureCamera();
    IplImage *img, *zoomIpImg;

    /*
     *	Snap �ص��������û�Ҳ���Զ���Ϊȫ�ֺ�����
     *	�����Ϊ��ĳ�Ա����������Ϊ��̬��Ա������
     */
//    static int CALLBACK SnapThreadCallback(MV_IMAGE_INFO *pInfo, long nUserVal);

signals:
    void signalCamera();
    void addLog(QString message, QString time);
public slots:

};

#endif // CAPTURECAMERA2_H
