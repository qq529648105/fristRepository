#include "VideoRecord.h"
#include <QPixmap>
#include <QWidget>
#include <QTime>
#include <QDir>
#include <QDebug>
#include <QFileDialog>

#define MAXSIZE 200

QList<QImage>videoList;
CvVideoWriter* videoWriter = 0;

WriteViedoThread::WriteViedoThread()
{
    stopVideoFlag = false;
    runFlag = true;
}

WriteViedoThread::~WriteViedoThread()
{


}



void WriteViedoThread::run()
{

    static int count = 0;
    while(runFlag )
    {
        if(!videoList.isEmpty())
        {
           QTime time;
            time.start();
            QImage qImage =  videoList.at(0);
            IplImage *IplImageBuffer = cvCreateImageHeader(cvSize(qImage.width(), qImage.height()), IPL_DEPTH_8U, 4);
            IplImageBuffer->imageData = (char *)qImage.bits();
            IplImage *IplImageBufferdes = cvCreateImage(cvSize(qImage.width(), qImage.height()), IPL_DEPTH_8U, 3);
            cvCvtColor(IplImageBuffer  , IplImageBufferdes,CV_RGBA2RGB);
            if(!cvWriteFrame( videoWriter, IplImageBufferdes))
                qDebug()<<"��Ƶд��ʧ��";
              else
         //      qDebug()<<"��Ƶд��ɹ�"<<count<<" "<<time.elapsed();
            count++;
//            QString s = QString("%1.png").arg(count);
//            cvSaveImage(s.toStdString().data(),IplImageBufferdes);
            cvReleaseImage(&IplImageBufferdes);
            videoList.pop_front();
            qDebug()<<time.elapsed();
        }
        else if(stopVideoFlag)
        {
            break;
        }
        msleep(1);
    }
    cvReleaseVideoWriter(&videoWriter);
    msleep(1);

    qDebug()<<"end t";
}







VideoRecord::VideoRecord(QObject *parent) :
    QObject(parent),capMsec(40),screenWidget(NULL)
{


}


IplImage * VideoRecord::QImageToIplImage(const QImage *qImage)
{

    int width = qImage->width();
    int height = qImage->height();
    CvSize Size;
    Size.height = height;
    Size.width =  width;
    IplImage *IplImageBuffer = cvCreateImageHeader(cvSize(qImage->width(), qImage->height()), IPL_DEPTH_8U, 4);
    IplImageBuffer->imageData = (char *)qImage->bits();
    IplImage *IplImageBufferdes = cvCreateImage(cvSize(qImage->width(), qImage->height()), IPL_DEPTH_8U, 3);
    //cvConvertImage(IplImageBuffer, IplImageBuffer1, CV_CVTIMG_SWAP_RB);
    cvCvtColor(IplImageBuffer  , IplImageBufferdes,CV_RGBA2RGB);
    return IplImageBufferdes;

}

void VideoRecord::grabScreen()
{
    if (writeVideoThread ->stopVideoFlag)
        return;
    capTimer.stop();

//   if(videoList.size()>MAXSIZE)
//        return;
//    QPixmap pixmap = QPixmap::grabWidget(screenWidget);
    QPixmap pixmap = QPixmap::grabWindow(screenWidget->winId());
    QImage qimage2 = pixmap.toImage();
    videoList.push_back(qimage2);

    capTimer.start(capMsec);
}


/************
�ӿڲ���  *scrWidget  ��������
         msec   �������ں��� һ������40MS
         cvCreateVideoWriter  ����¼����Ƶ��ʽ
 ***************/
void VideoRecord::StartVedioRecord(QWidget *scrWidget, int msec)
{
    videoList.clear();
    screenWidget = scrWidget;
    screenHeight = scrWidget->height();
    screenWidth = scrWidget->width();
    int fps =  10;//1000/msec / 5;
//    tempDir = QDir::tempPath().append("/");
//    QString str = tempDir +"test.avi";
//    char*  fileName;
//    QByteArray ba = str.toLatin1();
//    fileName=ba.data();

    QString fileName = QFileDialog::getSaveFileName(scrWidget, tr(" ��ѡ�񷽰���Ƶ�����ļ�"),
                                "",
                                tr("������Ƶ (*.avi)"));
    if (fileName.isEmpty()) return;

   // videoWriter = cvCreateVideoWriter("test.avi",0 ,fps,cvSize(screenWidth,screenHeight),3);
    videoWriter = cvCreateVideoWriter(fileName.toUtf8().constData(), CV_FOURCC('X', 'V', 'I', 'D') ,fps,cvSize(screenWidth,screenHeight),3);
    if(!videoWriter)
        qDebug()<<"failed";
//    if(capMsec != msec)
        capMsec = msec;
    connect(&capTimer,SIGNAL(timeout()),this,SLOT(grabScreen()));
    capTimer.start(msec);
   //   qDebug()<<"��ǰ��ʼʱ��"<<QTime::currentTime().toString("HH:mm:ss");
//    grabScreen();
    writeVideoThread = new WriteViedoThread();
    connect(writeVideoThread,SIGNAL(finished()),writeVideoThread,SLOT(deleteLater()));
    writeVideoThread->start();

}


/****************************
 *�ӿ� ֹͣ¼��
 *���� ¼���ļ�������·��
 *****************************/

QString VideoRecord::StopVedioRecord()
{
    capTimer.stop();
    disconnect(&capTimer,SIGNAL(timeout()),this,SLOT(grabScreen()));
    writeVideoThread ->stopVideoFlag = true;
// qDebug()<<"��ǰ����ʱ��"<<QTime::currentTime().toString("HH:mm:ss");
    return  tempDir;

}
