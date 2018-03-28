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
    //�ӿ� ��ʼ���� ֹͣ����
    void StartCapScreen(QWidget *screen,int width, int height,int msec);

    QString StopCapScreen();

private:
    int pixIndex; //ͼƬ����
    int dirIndex;   //�ļ�������
    int interMsec;  //�������
    QTimer interTime;
    QPixmap pixmap;
    int screenWidth;
    int screenHeight;
    QWidget *mscreen;
    QString tempDir;
    QString tempFolder;
};





#endif // SCREECAPTURE_H
