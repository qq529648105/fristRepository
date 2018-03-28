#ifndef VIDEORECORD_H
#define VIDEORECORD_H

#include <QObject>
#include <QImage>
#include <QString>
#include <QTimer>
#include <QThread>
#include "opencv/highgui.h"
#include "opencv/cv.h"

class WriteViedoThread : public QThread
{
    Q_OBJECT
public:
    WriteViedoThread();
    ~WriteViedoThread();
    bool runFlag;
    bool stopVideoFlag;
    QString tempFolder;

protected:
    void run();


};


class VideoRecord : public QObject
{
    Q_OBJECT
public:
    explicit VideoRecord(QObject *parent = 0);
    IplImage *QImageToIplImage(const QImage * qImage);

    //½Ó¿Ú
    void StartVedioRecord(QWidget* scrWidget, int msec);
    QString StopVedioRecord();

public slots:
    void grabScreen();


private:
    QString tempDir;
    QTimer capTimer;
    int   capMsec;
    int screenHeight;
    int screenWidth;
    QWidget *screenWidget;
    WriteViedoThread *writeVideoThread;
    //CvVideoWriter* videoWriter;

    
signals:
    
public slots:
    
};

#endif // VIDEORECORD_H
