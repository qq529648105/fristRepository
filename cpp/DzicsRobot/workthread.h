#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QObject>

class workThread : public QObject
{
    Q_OBJECT
public:
    explicit workThread(QObject *parent = 0);
private:
    int dectedNum[5];
    bool connCam[5];
    int triggerTime;
signals:
    void addLog(QString message, QString time);
    void reConn();

public slots:
    void serviceTimer();
    void rebootServiceSlot();
    void detectCam();
};

#endif // WORKTHREAD_H
