#ifndef SCREECAPTURE_H
#define SCREECAPTURE_H
#include<QPixmap>
#include <QTimer>
#include <QThread>

class QString;
class QWidget;

class SaveCapterThread : public QThread
{
    Q_OBJECT
public:
    SaveCapterThread(QString &sTempFolder);
    ~SaveCapterThread();
    bool runFlag;
    QString tempFolder;

protected:
    void run();


};

class ScreeCapture:public QObject
{
    Q_OBJECT
public:
    ScreeCapture();
    ~ScreeCapture();
    void SetCaptureInter(int msec);
    void MakeTempFolder(QString folder);
    SaveCapterThread* saveCapterThread;
    SaveCapterThread *saveCapterThread1;
     SaveCapterThread *saveCapterThread2;
public slots:
    bool CapterScreen();
    //接口 开始截屏 停止截屏
    void StartCapScreen(QWidget *screen,int width, int height,int msec);

    QString StopCapScreen();

private:
    int pixIndex; //图片索引
    int dirIndex;   //文件夹索引
    int interMsec;  //截屏间隔
    QTimer interTime;
    QPixmap pixmap;
    int screenWidth;
    int screenHeight;
    QWidget *mscreen;
    QString tempDir;
    QString tempFolder;
};





#endif // SCREECAPTURE_H
