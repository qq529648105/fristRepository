#ifndef CAMERAPROPERTY_H
#define CAMERAPROPERTY_H

#include <QObject>

class CameraProperty : public QObject
{
    Q_OBJECT
public:
    explicit CameraProperty(QObject *parent = 0);
    QString id;
    int sn;
    QString name;
    QString  model;
    int code;
    int solutionId;
    int maxExposure;
    int maxGain;
    int minExposure;
    int minGain;
    int exposure;
    int gain;
    int cameraWidth;
    int cameraHeight;
    int arealeftUpX;
    int arealeftUpY;
    int arearightDownX;
    int arearightDownY;
    QString testImage;
    int picNum;
    int interval;
    

    
};

#endif // CAMERAPROPERTY_H
