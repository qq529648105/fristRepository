#ifndef VISUALINSPECTION_H
#define VISUALINSPECTION_H
#include "highgui.h"
#include "cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QPoint>


//振挺3机器人项目视觉分析
typedef struct _DETECTRESULT{
    int result;        //-1 检测失败 1 检测成功
    float offsetX;     //偏移量X
    float offsetY;     //偏移量Y
    float offseZ;
    float angle;       //旋转角度
}DETECTRESULT;



QPoint getTemplateCenter(IplImage *dectImage,int binThreshold);
DETECTRESULT  visulInspection(IplImage *dectImage,CvRect dectArea, CvRect leftArea, int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY);
DETECTRESULT  visulInspection1(IplImage *dectImage,CvRect dectArea, CvRect leftArea, int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY);

DETECTRESULT  visulInspectionBak(IplImage *dectImage,int binThreshold,CvRect dectArea,CvRect leftArea,CvRect rightArea,double pixelRatio,int offset);
long int  isWorkPieceRollback(IplImage *dectImage1,IplImage *tp,CvRect dectArea, int time);
double  matchTemplate(IplImage *dectImage,IplImage *tempImage,int method);
bool  isWorkPieceRollback1(IplImage *dectImage,CvRect areaRect,IplImage *tp1,IplImage *tp2,IplImage *tp3,IplImage *tp4, int methold);
int isWorkPieceR2(IplImage *dectImage1,IplImage *tp,CvRect dectArea, int time);
#endif // VISUALINSPECTION_H
