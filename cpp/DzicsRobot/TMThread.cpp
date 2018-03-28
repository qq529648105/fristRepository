/****************************************************************************
**
**  Created By:  Daiyl  at  2015-8-3
**  Copyright (c) 2015 Dzics.com, All rights reserved.
**
****************************************************************************/

#include "TMThread.h"

#include <qmath.h>
#include <QPainter>
#include <QDateTime>
#include "Global.h"
#include "VisionDetection.h"

TMThread::TMThread(int imageWidth, int imageHeight)
{
    this->imageWidth = imageWidth;
    this->imageHeight = imageHeight;

}

TMThread::~TMThread(void)
{
    cvReleaseImage(&imgGray);
    cvReleaseImageHeader(&imgMain);
    delete pbyGetImageBuffer;
    delete qImgMain;
}

void TMThread::openCPK() {
    try{
        fileCPK.setFileName("D:\\DzicsRobot\\CPK\\CPK"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+".cpk");
        fileCPK.open(QFile::ReadWrite | QFile::Truncate | QFile::Text);
    } catch(...) {
    }
}

void TMThread::appendCPK(int savePage, QList<float> *valueList, QList<char> *valueStateList) {
    try {
//        fileCPK.write(QString(QString::number(pMeasureData[i].fValue)+",").toLocal8Bit());
    } catch(...) {
    }
}

void TMThread::closeCPK() {
    try {
        fileCPK.close();
    } catch(...) {
    }
}
bool open1=true;
bool TMThread::getMeasure(int savePage)  // savePage 0:不保存，1:保存第1页，2:保存第2页，3:保存第3页
{
//    qDebug("getMeasure");
START:
    if (openState != RC_OK) {
        addLog("重新打开基恩士","");
        openTM();
    }

    if(openState == RC_OK) {
        try {
            TMIF_HEAD head = TMIF_HEAD_A;
            BYTE byThin    = 2;		// byThin	0:None,	1:1/2,	2:1/4
            TMIF_REQAREA reqArea;
            reqArea.nXStart = 0;
            reqArea.nYStart = 0;
            reqArea.nXEnd   = TMIF_IMAGE_WIDTH  - 1;
            reqArea.nYEnd   = TMIF_IMAGE_HEIGHT - 1;
            TMIF_GET_AREA_INFO areaInfo;
//            TMIF_OUTNO outNo = TMIF_OUTNO_1;
//            TMIF_MEASUREDATA measureData;
            TMIF_MEASUREDATA* pMeasureData = new TMIF_MEASUREDATA[OUT_CNT];

            int ret = TMIF_GetImage(head, byThin, &reqArea, &areaInfo, pbyGetImageBuffer, TMIF_OUTNO_ALL, pMeasureData);


            int nWidth  = (areaInfo.nXEnd - areaInfo.nXStart + 1) / (int)pow(2.0, byThin);
            int nHeight = (areaInfo.nYEnd - areaInfo.nYStart + 1) / (int)pow(2.0, byThin);

            if(nWidth!=272||nHeight!=256)
            {
                if(open1)
                {
                    open1=false;
                    bool c1=closeTM();
                    addLog(QString("关闭基恩士%1").arg(c1),"");

                    QThread::msleep(100);
                    openState=RC_OTHER_ERR_FAILED;
                    goto START;
                }
                else
                {
                    open1=true;
                }

            }
    //        qDebug("\nGetMaster: %d, %f, %d, %d", ret, measureData.fValue, nWidth, nWidth);

            //updated by zhugz 20170616

            //addLog(QString("%1,%2").arg(nWidth).arg(nHeight),"");
            if(nWidth==272&&nHeight==256)
            {
            IplImage *imgTm = cvCreateImageHeader(cvSize( nWidth, nHeight ), IPL_DEPTH_8U, 1);
            imgTm->imageData = (char *)pbyGetImageBuffer;

            cvResize(imgTm, imgGray);
            cvCvtColor(imgGray, imgMain, CV_GRAY2RGBA);
            }
            else
            {
                addLog(QString("基恩士图片异常 w:%1 h:%2").arg(nWidth).arg(nHeight),"");
                return false;
            }


            //end

    //        CvFont font;
    //        cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,1.0,1.0,0,1);
    //        CvScalar textColor = CV_RGB(255,0,0);
    //        cvPutText(imgMain, (QString::number(measureData.fValue, 'g', 7) + "mm").toLocal8Bit().data(), cvPoint(3, 15), &font,textColor);
    //        cvShowImage("a", imgMain);

            QPainter painter(qImgMain);
            painter.setPen(Qt::DotLine);
            painter.drawLine(0, 84, 168, 84);
            painter.drawLine(84, 0, 84, 168);
            int validCount = 0,n1=0,n2=0,n3=0;
            QList<float> *valueList = new QList<float>();
            QList<char> *valueStateList = new QList<char>();

            QFile fileCSV("D:\\DzicsRobot\\CPK\\"+QDate::currentDate().toString("yyyyMMdd").toLocal8Bit()+"_"+QString::number(savePage)+".csv");
            if (savePage>0) {
                openCPK();
                fileCSV.open(QFile::Append);
                fileCSV.write(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss,").toLocal8Bit());
            }

            for (int i=0;i<OUT_CNT;i++) {
                //if(!_finite(pMeasureData[i].fValue)) pMeasureData[i].fValue=-1;//updated by zhugz
                valueList->append(pMeasureData[i].fValue);//pMeasureData[i].FloatResultpMeasureData[i].fValue);
                //addLog(QString("tmRet %1").arg(pMeasureData[i].MeasureResult),"");
                if (pMeasureData[i].FloatResult == TMIF_FLOATRESULT_VALID
                        && pMeasureData[i].MeasureResult == TMIF_GO) {
                            validCount++;
                            ++n1;
                            valueStateList->append(1);  // 通过
                            //mainWindow->writeLog("基恩士通过");

//                            appendCPK(savePage, i, pMeasureData[i].fValue);
                            if (savePage>0) fileCSV.write(QString(QString::number(pMeasureData[i].fValue)+",").toLocal8Bit());
                } else if (pMeasureData[i].FloatResult == TMIF_FLOATRESULT_WAITING) {
                    validCount++;
                    ++n2;
                    valueStateList->append(3);  // 未配置
                    //mainWindow->writeLog("基恩士未配置");
                } else {
                    valueStateList->append(2);  // 不通过
                    ++n3;
                    //mainWindow->writeLog("基恩士不通过");
//                    appendCPK(savePage, i, pMeasureData[i].fValue);
                    if (savePage>0) fileCSV.write(QString(QString::number(pMeasureData[i].fValue)+",").toLocal8Bit());
                }
            }

            //addLog(QString("基恩士检测%1 %2 %3").arg(n1).arg(n2).arg(n3),"");
            if (savePage>0) {
                appendCPK(savePage, valueList, valueStateList);

                closeCPK();
                fileCSV.write(validCount == OUT_CNT ? "通过" : "失败");
                fileCSV.write("\r\n");
                fileCSV.close();
            }

            if (validCount == OUT_CNT) {
                painter.setPen( Qt::green );
                painter.drawText(135, 16, "通过");
            } else {
                painter.setPen( Qt::red );
                painter.drawText(135, 16, "失败");
            }

//            if (pMeasureData[0].FloatResult == TMIF_FLOATRESULT_VALID) {
//                if (pMeasureData[0].MeasureResult != TMIF_HI)
//                    painter.setPen( Qt::green );
//                else
//                    painter.setPen( Qt::red );

//                painter.drawText(125,12,(QString().sprintf("%0.3f", pMeasureData[0].fValue).left(5)+"mm"));
//            }

            //updated by zhugz
            emit updateImage(qImgMain);
            //end

            emit updateValue(valueList, valueStateList);
//            emit updateMeasure(qImgMain, values, valids);

            return true;
//                    msleep(50);
        } catch (...) {
//                    msleep(100);
            emit addLog("cpk保存异常","");
        }
    }

    return false;
}

//double* TMThread::saveValues(int pageNo)
//{
//    if (openState != RC_OK) {
//        openTM();
//    }

//    double values[6]={0,0,0,0,0,0};
//    if (openState == RC_OK) {
//        try {
//            TMIF_MEASUREDATA* measurementData = new TMIF_MEASUREDATA[OUT_CNT];
//            int ret = TMIF_GetMeasureValue(TMIF_OUTNO_ALL, measurementData);
//            if (ret == RC_OK) {

//            } else {
//                delete[] measurementData;
//            }

//            getImage( TMIF_OUTNO_ALL );

//        } catch (...) {
//        }
//    }
//    return values;
//}

bool TMThread::openTM()
{
    try {
//    TMIF_OPENPARAM_ETHERNET config;
//    config.IPAddress.S_un.S_addr = inet_addr("192.168.16.210");
//    config.wPort     = (DWORD)24689;
//    config.wReserve  = 0;
//    openState = TMIF_EtherOpen(&config);
        openState = TMIF_UsbOpen();
//        qDebug("\nOpen: %d", openState);

        pbyGetImageBuffer = new BYTE[TMIF_IMAGE_WIDTH * TMIF_IMAGE_HEIGHT];
        imgGray = cvCreateImage(cvSize( imageWidth, imageHeight ), IPL_DEPTH_8U, 1);
        qImgMain = new QImage(imageWidth, imageHeight, QImage::Format_ARGB32);
        imgMain = cvCreateImageHeader(cvSize( imageWidth, imageHeight ), IPL_DEPTH_8U, 4);
        imgMain->imageData = (char *)qImgMain->bits();
        return openState == RC_OK;
    } catch(...) {
        return false;
    }

}

bool TMThread::closeTM()
{
    try {
        return TMIF_Close() == RC_OK;
    } catch (...) {
        return false;
    }
}

void TMThread::run()
{

    if (openTM()) {
        getMeasure( 0 );    // 获取初始数据
    } else if(openState != RC_OK) {
        if (openTM()) {
            getMeasure( 0 );    // 获取初始数据
        }
    }

//    while(openState == RC_OK) {
//        getMeasure( 0 );
//        sleep(100);
//    }

    exec();

//    while(openState == RC_OK) {
//        try {
//            TMIF_HEAD head = TMIF_HEAD_A;
//            BYTE byThin    = 2;		// byThin	0:None,	1:1/2,	2:1/4
//            TMIF_REQAREA reqArea;
//            reqArea.nXStart = 0;
//            reqArea.nYStart = 0;
//            reqArea.nXEnd   = TMIF_IMAGE_WIDTH  - 1;
//            reqArea.nYEnd   = TMIF_IMAGE_HEIGHT - 1;
//            TMIF_GET_AREA_INFO areaInfo;
//            TMIF_OUTNO outNo = TMIF_OUTNO_ALL;
////            TMIF_MEASUREDATA measureData;
//            TMIF_MEASUREDATA* pMeasureData = new TMIF_MEASUREDATA[OUT_CNT];

//            int ret = TMIF_GetImage(head, byThin, &reqArea, &areaInfo, pbyGetImageBuffer, outNo, pMeasureData);

//            int nWidth  = (areaInfo.nXEnd - areaInfo.nXStart + 1) / (int)pow(2.0, byThin);
//            int nHeight = (areaInfo.nYEnd - areaInfo.nYStart + 1) / (int)pow(2.0, byThin);

//    //        qDebug("\nGetMaster: %d, %f, %d, %d", ret, measureData.fValue, nWidth, nWidth);

//            IplImage *imgTm = cvCreateImageHeader(cvSize( nWidth, nHeight ), IPL_DEPTH_8U, 1);
//            imgTm->imageData = (char *)pbyGetImageBuffer;

//            cvResize(imgTm, imgGray);
//            cvCvtColor(imgGray, imgMain, CV_GRAY2RGBA);

//    //        CvFont font;
//    //        cvInitFont(&font,CV_FONT_HERSHEY_PLAIN,1.0,1.0,0,1);
//    //        CvScalar textColor = CV_RGB(255,0,0);
//    //        cvPutText(imgMain, (QString::number(measureData.fValue, 'g', 7) + "mm").toLocal8Bit().data(), cvPoint(3, 15), &font,textColor);
//    //        cvShowImage("a", imgMain);

//            QPainter painter(qImgMain);
//            painter.setPen(Qt::DotLine);
//            painter.drawLine(0, 84, 168, 84);
//            painter.drawLine(84, 0, 84, 168);
//            if (pMeasureData[0].FloatResult == TMIF_FLOATRESULT_VALID) {
//                if (pMeasureData[0].MeasureResult != TMIF_HI)
//                    painter.setPen( Qt::green );
//                else
//                    painter.setPen( Qt::red );

//                painter.drawText(125,12,(QString().sprintf("%0.3f", pMeasureData[0].fValue).left(5)+"mm"));
//            }

//            emit updateImage(qImgMain);
////             float values[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
////             bool valids[16]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
////            emit updateMeasure(qImgMain, values, valids);

//            msleep(50);
//        } catch (...) {
//            msleep(100);
//        }
//    }

    closeTM();


}

#include "highgui.h"
extern bool detectGap(IplImage* viewImage,CvRect areaRect,int threshold, int threshod1);
bool TMThread::getMeasureGap(int xStart,int yStart,int xEnd,int yEnd, int threshold, int threshod1, QString params)  //   threshold 灰度图阈值 设为40 40以上白  40以下黑    threshol1  间隙边缘阈值 ,目前设为3
{
    if (openState != RC_OK) {
        openTM();
    }

    if(openState == RC_OK) {
        try {
            TMIF_HEAD head = TMIF_HEAD_A;
            BYTE byThin    =0;//  2;		// byThin	0:None,	1:1/2,	2:1/4
            TMIF_REQAREA reqArea;
            reqArea.nXStart = xStart;//563;//0;
            reqArea.nYStart = yStart;//483;//0;
            reqArea.nXEnd   = xEnd;//718;//TMIF_IMAGE_WIDTH  - 1;
            reqArea.nYEnd   = yEnd;//638;//TMIF_IMAGE_HEIGHT - 1;
            TMIF_GET_AREA_INFO areaInfo;
            TMIF_OUTNO outNo = TMIF_OUTNO_1;
            TMIF_MEASUREDATA measureData;

            int ret = TMIF_GetImage(head, byThin, &reqArea, &areaInfo, pbyGetImageBuffer, outNo, &measureData);

            int nWidth  = (areaInfo.nXEnd - areaInfo.nXStart + 1);
            int nHeight = (areaInfo.nYEnd - areaInfo.nYStart + 1);

    //        qDebug("\nGetMaster: %d, %f, %d, %d", ret, measureData.fValue, nWidth, nWidth);

            IplImage *imgTm = cvCreateImageHeader(cvSize( nWidth, nHeight ), IPL_DEPTH_8U, 1);
            imgTm->imageData = (char *)pbyGetImageBuffer;
            cvSaveImage("TempGapImage.PNG", imgTm);   // Debug

            cvResize(imgTm, imgGray);
            cvCvtColor(imgGray, imgMain, CV_GRAY2RGBA);
            emit updateImage(qImgMain);

            bool result = detectGap(imgTm, cvRect(0,0,nWidth, nHeight), threshold, threshod1);

            emit updateGapResult(result, params);

            return true;
//                    msleep(50);
        } catch (...) {
//                    msleep(100);
        }
    }

    return false;
}
//updated by zhugz
void TMThread::tmGapDetect(QString param, QString var)
{


    QStringList inputList = param.split(",");
    while (inputList.length()<6) inputList.append("");

    int x1=inputList[0].toInt();
    int y1=inputList[1].toInt();
    int x2=inputList[2].toInt();
    int y2=inputList[3].toInt();
    int minWidth=inputList[4].toInt();
    int difVal=inputList[5].toInt();

    if (openState != RC_OK) {
        openTM();
    }

    if(openState == RC_OK) {
        try {
            TMIF_HEAD head = TMIF_HEAD_A;
            BYTE byThin    = 0;		// byThin	0:None,	1:1/2,	2:1/4
            TMIF_REQAREA reqArea;
            reqArea.nXStart = 0;
            reqArea.nYStart = 0;
            reqArea.nXEnd   = TMIF_IMAGE_WIDTH  - 1;
            reqArea.nYEnd   = TMIF_IMAGE_HEIGHT - 1;
            TMIF_GET_AREA_INFO areaInfo;
//            TMIF_OUTNO outNo = TMIF_OUTNO_1;
//            TMIF_MEASUREDATA measureData;
            TMIF_MEASUREDATA* pMeasureData = new TMIF_MEASUREDATA[OUT_CNT];

            int ret = TMIF_GetImage(head, byThin, &reqArea, &areaInfo, pbyGetImageBuffer, TMIF_OUTNO_ALL, pMeasureData);

            int nWidth  = (areaInfo.nXEnd - areaInfo.nXStart + 1) / (int)pow(2.0, byThin);
            int nHeight = (areaInfo.nYEnd - areaInfo.nYStart + 1) / (int)pow(2.0, byThin);


            QTime t;
            t.start();

            addLog(QString("获取基因士图像：%1").arg(ret),"");
            if(ret==1)
                addLog("错误请求参数","");
            else if(ret ==2)
                addLog("没有图像数据","");
            else if(ret>2)
                addLog("其他错误","");

            tmImage1=cvCreateImageHeader(cvSize( nWidth, nHeight ), IPL_DEPTH_8U, 1);
            tmImage1->imageData = (char *)pbyGetImageBuffer;
            Rect roi;
            roi.x=x1;
            roi.y=y1;
            roi.width=x2-x1;
            roi.height=y2-y1;

            int ret1=gap1(Mat(tmImage1),roi,minWidth,difVal);
            mainWindow->savedDetectImg(tmImage1,"tmImage",QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss_")+QString::number(ret1));
            emit updateGapResult(ret1, var);
            emit addLog(QString("间隙检测耗时:%1 ms").arg(t.elapsed()),"");



        } catch (...) {
                addLog("间隙检测异常","");
        }
    }


}
