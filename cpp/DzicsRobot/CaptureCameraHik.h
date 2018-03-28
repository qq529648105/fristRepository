#ifndef CAPTURECAMERAHIK_H
#define CAPTURECAMERAHIK_H

//#include <QObject>
//#include <QThread>
#include <QReadWriteLock>
#include <QImage>
#include "cv.h"
#include "basecapture.h"

#include "MvErrorDefine.h"
#include "CameraParams.h"
#include "MvCameraControl.h"

#define MAX_DEVICE_NUM          4

/*���Ⱥ궨��*/
#define MAX_BUF_SIZE            (1024*1024*58)
#define MAX_XML_FILE_SIZE       (1024*1024*3)

/*���������붨��*/
typedef int Status;
#define STATUS_OK               1
#define STATUS_ERROR            0

/*����ģʽ*/
#define TRIGGER_OFF             0
#define TRIGGER_ON              1
#define TRIGGER_NOT_SET         -1

/*����Դ*/
#define SOFTWAREMODE            7
#define HAREWAREMODE            0

#define MY_OK           0
#define MY_FAIL         -1

//class ReconnectThread: public QThread
//{
//public:
//    ReconnectThread() { }
//    virtual void run()
//    {
//        if (m_bOpenDevice)
//        {
//            m_pcMyCamera->Close();

//            BOOL bConnected = FALSE;
//            while (1)
//            {
//                int nRet = MY_OK;
//                nRet = m_pcMyCamera->Open(&m_stDevInfo);
//                if (MY_OK == nRet)
//                {
//                    m_pcMyCamera->RegisterExceptionCallBack(ReconnectDevice, this);
//                    bConnected = TRUE;
//                    break;
//                }
//                else
//                {
//                    Sleep(100);
//                }
//            }

//            if (bConnected && m_bStartGrabbing)
//            {
//                m_pcMyCamera->Display(m_hwndDisplay);
//                m_pcMyCamera->StartGrabbing();
//            }
//        }
//    }
//};
class CaptureCameraHik : public BaseCapture
{
    Q_OBJECT
public:

    explicit CaptureCameraHik(QString ip, int index, QObject *parent = 0);
    ~CaptureCameraHik();
    void stopCaptureCamera();
    void startCaptureCamera();

    double getExpourseValue();
    void setExpourseValue(double);

    int getAcquisitionLineRate();
    void setAcquisitionLineRate(const unsigned int nValue);
    int captureIndex;

    void resumeCamera();
    bool initCamera(const char* cameraIp);
    int SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam);
    // ע��ͼ�����ݻص�
    int RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser),
                                    void* pUser);
    int RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType,
                                                                                void* pUser),
                                                 void* pUser);
    /*���ץͼ�ص�*/
    static void   __stdcall ImageCallBack(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser);
    int camWidth;
    int camHeight;

protected:
    void run();

private:

    IplImage *img, *zoomIpImg;
    void* m_hDevHandle;
    QString ip;

    MV_CC_DEVICE_INFO_LIST m_stDevList;             // �豸��Ϣ�б�ṹ������������洢�豸�б�

    bool  m_bStartGrabbing;                     // �Ƿ�ʼץͼ

    MV_SAVE_IAMGE_TYPE   m_nSaveImageType;      // ����ͼ���ʽ
    //unsigned char*  m_pBufForDriver;         // ���ڱ���ͼ��Ļ���
    //unsigned int    m_nBufSizeForDriver;

signals:
    void signalCamera();
    void addLog(QString message, QString time);

public slots:
};

#endif // CAPTURECAMERAHIK_H
