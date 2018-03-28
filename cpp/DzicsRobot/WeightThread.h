#ifndef WEIGHTTHREAD_H
#define WEIGHTTHREAD_H

#include<QThread>
#include<QTime>

unsigned int CRC16Check ( uchar *arr_buff, uchar len);

class QextSerialPort;

class WeightThread : public QThread
{
    Q_OBJECT
public:
    WeightThread();
    char readBuf[1024];
    bool openSerial(QString& comName, int bandRet);
    int sendSerialCmd(unsigned char* cmd ,int cmdLen);
    int readSerailData(char *readBuf);
    int praseReadData(char* buf, int bufLen);
    QextSerialPort *serialCom;
    void startWeightThread();
    void stopWeightThread();
    void setRecvTimeOut(int msec);
    void startTask(int taskId);

    QTime *interTime;
    QTime *recvTime;

public slots:
    void getSerialData();

protected:
    void run();
private:
    int  taskId;
    bool isThreadStart; //线程启动标志
    int  recvNum;
    int  recvOutMsec;  //接收超时时间
    int  interMsec;   //
signals:
    void sendValue(float value);

};

#endif // WEIGHTTHREAD_H
