#include "CaptureCameraHik.h"
#include <opencv2/highgui/highgui.hpp>
#include <QMutex>
#include <QDebug>
#include "Global.h"
using namespace cv;
extern IplImage* cameraIpImg;
extern QMap<int,IplImage*> IpImgCache;
extern int cur_cameraIndex;
extern QString  cameraImage;
extern QMutex lock;
extern QMutex mylock[5];

extern int fNum[5];
extern bool saveCache[5];


CaptureCameraHik::CaptureCameraHik(QString ip, int index, QObject *parent)
{
        m_hDevHandle = NULL;
        runFlag = FALSE;
        captureIndex=index;



        //fNum[captureIndex]=0;
        this->ip=ip;
        initCamera(ip.toLocal8Bit().data());

        qDebug()<<"���ip"<<ip;



        camWidth  = 0;
        camHeight = 0;
        img = NULL;
        cameraIpImg = NULL;
        zoomIpImg = NULL;
        IpImgCache[index] = NULL;
        //m_nSaveImageType = MV_Image_Bmp;
        //m_pBufForDriver = NULL;
        //m_nBufSizeForDriver = 0;
}

CaptureCameraHik::~CaptureCameraHik()
{

    if (m_hDevHandle)
    {
        MV_CC_DestroyHandle(m_hDevHandle);
        m_hDevHandle = NULL;
        runFlag = FALSE;
    }

//    if(img!=NULL)
//    {
//        cvReleaseImage(&img);
//        img = NULL;
//    }

//    if(cameraIpImg!=NULL)
//    {
//        cvReleaseImage(&cameraIpImg);
//        cameraIpImg = NULL;
//    }

//    if(zoomIpImg!=NULL)
//    {
//        cvReleaseImage(&zoomIpImg);
//        zoomIpImg = NULL;
//    }

//    if(IpImgCache[captureIndex]!=NULL)
//    {
//            mylock[captureIndex].lock();
//            qDebug()<<"last save";
//            cvReleaseImage(&IpImgCache[captureIndex]);
//            IpImgCache[captureIndex] = NULL;
//            mylock[captureIndex].unlock();
//    }
}


// ע��ͼ�����ݻص�
int CaptureCameraHik::RegisterImageCallBack(void(__stdcall* cbOutput)(unsigned char * pData,
                                                                MV_FRAME_OUT_INFO_EX* pFrameInfo,
                                                                void* pUser),
                                         void* pUser)
{
    if (NULL == m_hDevHandle)
    {
        return MY_FAIL;
    }

    return MV_CC_RegisterImageCallBackEx(m_hDevHandle, cbOutput, pUser);
}

// ע����Ϣ�쳣�ص�
int     CaptureCameraHik::RegisterExceptionCallBack(void(__stdcall* cbException)(unsigned int nMsgType,
                                                                            void* pUser),
                                             void* pUser)
{
    if (NULL == m_hDevHandle)
    {
        return MY_FAIL;
    }

    return MV_CC_RegisterExceptionCallBack(m_hDevHandle, cbException, pUser);
}

//void __stdcall CaptureCameraHik::ReconnectDevice(unsigned int nMsgType, void* pUser)
//{
//    void* pThread = (HANDLE)_beginthreadex(NULL, 0, ReconnectThread, pUser, 0, NULL);
//    if (NULL == pThread)
//    {
//        //MessageBox(TEXT("����ʧ�ܣ�"), TEXT("����"), MB_OK | MB_ICONWARNING);
//        return;
//    }
//}
bool CaptureCameraHik::initCamera(const char* cameraIp)
{
    int nRet;
    qDebug()<<"��ʼ��������豸";
    {
        nRet = MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);
        if (MV_OK != nRet)
        {
            qDebug()<<"�������"<<captureIndex;
            return false;
        }
    }

    qDebug()<<"�ҵ�����豸"<<m_stDevList.nDeviceNum;
    if(captureIndex>=m_stDevList.nDeviceNum)
    {
        return false;
    }
    nRet = MV_CC_CreateHandle(&m_hDevHandle, m_stDevList.pDeviceInfo[captureIndex]);
    if (NULL == m_hDevHandle)
    {
        return false;
    }


    if (MV_OK != nRet)
    {
        return false;
    }else
    {
        qDebug()<<"��������豸����ɹ�"<<captureIndex;
        nRet = MV_CC_OpenDevice(m_hDevHandle);
        if (MV_OK != nRet)
        {
            qDebug()<<"�����ֹ���ʣ�"<<captureIndex;
            MV_CC_DestroyHandle(&m_hDevHandle);
            runFlag = false;
            return false;
        }else
        {

        }
    }

    nRet = RegisterImageCallBack(ImageCallBack, this);
    if (MV_OK != nRet)
    {
        return  false;
    }
    qDebug()<<"ע�����ͼ��ص������ɹ�"<<captureIndex;
    nRet = -1;
    nRet = MV_CC_StartGrabbing(m_hDevHandle);
    if (MV_OK == nRet)
    {
        runFlag = TRUE;
    }else
    {
        return false;
    }

    qDebug()<<"��������ɼ�ͼ��ɹ�"<<captureIndex;
    return  true;
}

void CaptureCameraHik::run()
{
//    int nRet = RegisterImageCallBack(ImageCallBack, this);
//    if (nRet == MV_OK)
//    {
//        m_bStartGrabbing = TRUE;
//    }else
//    {
//        return;
//    }
    int nRet;
    int n1=-1000;
    int sec1 = 0;
    while(runFlag)
    {
        ++sec1;

        if(n1==fNum[captureIndex]&&!(sec1%9))
        {
            qDebug()<<"���������"<<captureIndex;
            addLog("�������"+QString::number(captureIndex),"");
            MV_CC_EnumDevices(MV_GIGE_DEVICE | MV_USB_DEVICE, &m_stDevList);

            nRet  = MV_CC_CreateHandle(&m_hDevHandle, m_stDevList.pDeviceInfo[captureIndex]);
            nRet = MV_CC_OpenDevice(m_hDevHandle);
            if (MV_OK != nRet)
            {
                qDebug()<<"�����ֹ���ʣ�"<<captureIndex;
                MV_CC_DestroyHandle(&m_hDevHandle);
                //m_bStartGrabbing = false;
            }else
            {
                emit addLog(QString("���%1��ʼ�ɼ�").arg(captureIndex),"");
                nRet = RegisterImageCallBack(ImageCallBack, this);

                if (MV_OK == nRet)
                {
                    //m_bStartGrabbing = TRUE;
                    nRet = MV_CC_StartGrabbing(m_hDevHandle);
                    if (MV_OK == nRet)
                    {
                        qDebug()<<"�����󣬵�������ɼ�ͼ��ɹ���"<<captureIndex;
                    }
                }
            }

        }
       n1=fNum[captureIndex];
       //qDebug()<<"��������źţ�"<<captureIndex;
       emit signalCamera();


       QThread::msleep(300);
    }

}

void CaptureCameraHik::startCaptureCamera()
{
    this->start();
}

void CaptureCameraHik::stopCaptureCamera()
{
    if (NULL == m_hDevHandle)
    {
        return;
    }


    int nRet = MV_CC_StopGrabbing(m_hDevHandle);
    if (MV_OK != nRet)
    {
        return ;
    }
    runFlag = FALSE;
}

int CaptureCameraHik::SaveImage(MV_SAVE_IMAGE_PARAM_EX* pstParam)
{
    if (NULL == pstParam)
    {
        return MY_FAIL;
    }

    return MV_CC_SaveImageEx(pstParam);
}

void CaptureCameraHik::resumeCamera()
{
    //fNum[captureIndex]=0;
    stopCaptureCamera();
    initCamera(ip.toLocal8Bit().data());
    startCaptureCamera();

}
// ���ץͼ�ص�, ��Ҫ������ȡͼƬ
void   __stdcall CaptureCameraHik::ImageCallBack(unsigned char* pData, MV_FRAME_OUT_INFO_EX* pFrameInfo, void* pUser)
{
    if (pFrameInfo)
    {

        CaptureCameraHik *pCaptureCameraHik = (CaptureCameraHik*)pUser;
        if (/*MV_Image_Undefined == pCaptureCameraHik->m_nSaveImageType || */NULL == pData)
        {
            return;
        }

        // ���ö�Ӧ���������

        // ���ڵ�һ�α���ͼ��ʱ���뻺�棬�� CloseDevice ʱ�ͷ�
        if (NULL == pCaptureCameraHik->zoomIpImg)
        {
            // BMPͼƬ��С��width * height * 3 + 2048(Ԥ��BMPͷ��С)
            if(pCaptureCameraHik->camWidth!=pFrameInfo->nWidth && pCaptureCameraHik->camHeight!=pFrameInfo->nHeight)
            {
                //ͼ���С���ú��ڴ��ͷ�����

                pCaptureCameraHik->camWidth  = pFrameInfo->nWidth;
                pCaptureCameraHik->camHeight = pFrameInfo->nHeight;
                if(pCaptureCameraHik->img!=NULL)
                {
                    cvReleaseImage(&pCaptureCameraHik->img);
                    pCaptureCameraHik->img = NULL;
                }
                pCaptureCameraHik->img = cvCreateImage(cvSize( pCaptureCameraHik->camWidth, pCaptureCameraHik->camHeight ), IPL_DEPTH_8U, 1);

                if(cameraIpImg!=NULL)
                {
                    cvReleaseImage(&cameraIpImg);
                    cameraIpImg = NULL;
                }
                cameraIpImg = cvCreateImage(cvSize(pCaptureCameraHik->camWidth, pCaptureCameraHik->camHeight), IPL_DEPTH_8U, 3);

                //updated by zhugz
                pCaptureCameraHik->savedNum=0;
                for(int i=0;i<pCaptureCameraHik->picNum;i++)
                    IpImgArray[pCaptureCameraHik->captureIndex][i]=cvCreateImage(cvSize( pCaptureCameraHik->camWidth, pCaptureCameraHik->camHeight ), IPL_DEPTH_8U, 1);
                //end
                if(pCaptureCameraHik->zoomIpImg!=NULL)
                {
                    cvReleaseImage(&pCaptureCameraHik->zoomIpImg);
                    pCaptureCameraHik->zoomIpImg = NULL;
                }
                pCaptureCameraHik->zoomIpImg = cvCreateImage(cvSize(pCaptureCameraHik->camWidth, pCaptureCameraHik->camHeight), IPL_DEPTH_8U, 3);

                if(IpImgCache[pCaptureCameraHik->captureIndex]!=NULL)
                {
                    cvReleaseImage(&IpImgCache[pCaptureCameraHik->captureIndex]);
                    IpImgCache[pCaptureCameraHik->captureIndex] = NULL;
                }
                IpImgCache[pCaptureCameraHik->captureIndex] = cvCreateImage(cvSize(pCaptureCameraHik->camWidth, pCaptureCameraHik->camHeight), IPL_DEPTH_8U, 1);
            }
            //pCaptureCameraHik->m_nBufSizeForDriver = pFrameInfo->nWidth * pFrameInfo->nHeight * 3 + 2048;

            //pCaptureCameraHik->m_pBufForDriver = (unsigned char*)malloc(pCaptureCameraHik->m_nBufSizeForDriver);
            //if (NULL == pCaptureCameraHik->m_pBufForDriver)
            //{
            //    return;
            //}
        }

//        MV_SAVE_IMAGE_PARAM_EX stParam = {0};
//        stParam.enImageType = 0; // ��Ҫ�����ͼ������
//        stParam.enPixelType = pFrameInfo->enPixelType;  // �����Ӧ�����ظ�ʽ
//        //stParam.nBufferSize = pCaptureCameraHik->m_nBufSizeForDriver;  // �洢�ڵ�Ĵ�С
//        stParam.nWidth      = pFrameInfo->nWidth;         // �����Ӧ�Ŀ�
//        stParam.nHeight     = pFrameInfo->nHeight;          // �����Ӧ�ĸ�
//        stParam.nDataLen    = pFrameInfo->nFrameLen;
//        stParam.pData       = pData;
        //stParam.pImageBuffer = pCaptureCameraHik->m_pBufForDriver;
//        stParam.nJpgQuality = 80;

         {

             ++fNum[pCaptureCameraHik->captureIndex];
             mylock[pCaptureCameraHik->captureIndex].lock();

             cvSetData( IpImgCache[pCaptureCameraHik->captureIndex], pData, pFrameInfo->nWidth );   // * 3

             if(pCaptureCameraHik->picNum>0&&saveCache[pCaptureCameraHik->captureIndex])
             {

                 if(fNum[pCaptureCameraHik->captureIndex]%pCaptureCameraHik->interval==0)
                 {



                     cvCopy(IpImgCache[pCaptureCameraHik->captureIndex],IpImgArray[pCaptureCameraHik->captureIndex][pCaptureCameraHik->savedNum]);

                     ++pCaptureCameraHik->savedNum;


                     if(pCaptureCameraHik->savedNum==pCaptureCameraHik->picNum)
                     {
                         pCaptureCameraHik->savedNum=0;
                         saveCache[pCaptureCameraHik->captureIndex]=false;
                         mainWindow->callVision(pCaptureCameraHik->captureIndex,pCaptureCameraHik->picNum);
                     }
                 }
             }
             mylock[pCaptureCameraHik->captureIndex].unlock();


         }
    }
}


double CaptureCameraHik::getExpourseValue()
{
    double t;

    MVCC_FLOATVALUE floatValue;

    int nRet = MV_CC_GetExposureTime(m_hDevHandle,&floatValue);
    t = floatValue.fCurValue;
    qDebug()<<nRet<<"setExpourseValue"<<QThread::currentThreadId();
    return t;
}

void CaptureCameraHik::setExpourseValue(double t)
{

    int nRet = MV_CC_SetExposureTime(m_hDevHandle,t);
    qDebug()<<nRet<<"setExpourseValue"<<QThread::currentThreadId();
}

int CaptureCameraHik::getAcquisitionLineRate()
{
    unsigned int  nValue;
    MVCC_INTVALUE pstValue;

    MV_CC_GetAcquisitionLineRate(m_hDevHandle, &pstValue);

    nValue = pstValue.nCurValue;
    return nValue;
}

void CaptureCameraHik::setAcquisitionLineRate(const unsigned int  nValue)
{
    qDebug()<<"this"<<QThread::currentThreadId();
    MV_CC_SetAcquisitionLineRate(m_hDevHandle, nValue);
}
