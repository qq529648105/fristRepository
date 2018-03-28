#ifndef VISUALINSPECTION_H
#define VISUALINSPECTION_H
#include "highgui.h"
#include "cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <QPoint>


//��ͦ3��������Ŀ�Ӿ�����
typedef struct _DETECTRESULT{
    int result;        //-1 ���ʧ�� 1 ���ɹ�
    float offsetX;     //ƫ����X
    float offsetY;     //ƫ����Y
    float offseZ;
    float angle;       //��ת�Ƕ�
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
