/****************************************************************************
**
**  Created By:  Daiyl  at  2015-8-3
**  Copyright (c) 2015 Dzics.com, All rights reserved.
**
****************************************************************************/

#ifndef TMTHREAD_H
#define TMTHREAD_H

#include <QThread>
#include <QImage>
#include <QFile>

#include "opencv/cv.h"
#include "TMIF.h"

class TMThread : public QThread
{
    Q_OBJECT
public:
    TMThread(int imageWidth, int imageHeight);
    ~TMThread(void);

    virtual void run();

signals:
    void updateImage( QImage *image );
    void updateValue( QList<float> *valueList, QList<char> *valueStateList );
    void updateMeasure( QImage *image, float values[], bool valids[] );
    void updateGapResult(bool result, QString params);
    void addLog(QString message, QString time);
public slots:
//    double* saveValues(int pageNo = 0);
    bool getMeasure(int savePage);  // savePage 0:不保存，1:保存第1页，2:保存第2页，3:保存第3页
    bool getMeasureGap(int xStart,int yStart,int xEnd,int yEnd, int threshold, int threshod1, QString params);  //   threshold 灰度图阈值 设为40 40以上白  40以下黑    threshol1  间隙边缘阈值 ,目前设为3
    void tmGapDetect(QString param, QString var);
private:
    int openState;  // RC_OK
    int imageWidth, imageHeight;
    BYTE* pbyGetImageBuffer;
    IplImage *imgGray, *imgMain,*tmImage1;
    QImage *qImgMain;
    QFile fileCPK;

    static const int OUT_CNT = 16;

    bool openTM();
    bool closeTM();

    void openCPK();
    void appendCPK(int savePage, QList<float> *valueList, QList<char> *valueStateList);
    void closeCPK();

};

#endif // TMTHREAD_H
