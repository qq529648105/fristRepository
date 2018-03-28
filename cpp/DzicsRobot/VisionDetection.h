#ifndef DETECTION_H
#define DETECTION_H

#include "highgui.h"
#include "cv.h"
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
using namespace cv;
typedef struct _CICLEPAR{
    int discx;
    int discy;
    float r;
}CICLEPAR;


typedef struct _CICLEPARDIFF{
    float discx;
    float discy;
    float r;


}CICLEPARDIFF;


typedef struct _CICLEDCEOFFSET{
    float x;
    float y;

}CICLEDCEOFFSET;

//检测参数 这3个参数需要显示
extern float rivetDia;  //工件内部铆钉直径mm
extern float rivetDceDiff; //铆钉圆心位置和圆盘圆心位置距离差值mm
extern float diskDia;  //圆盘直径mm
/************************update12.9*************/
extern int armDirection;   //手臂方向
/****************************update12.9********/


//接口函数  检测
/********************
 *rivetminDia  铆钉最小直径要求mm  6.0mm
 *rivetmaxDia  最大直径要求        6.5 mm
 *dcediff   圆心距偏离最大值超过不合格  0.2mm
 *以上3个参数是检测要求参数 可能参数会微变 还没定
 *
 *threshold 2值图阈值 目前150
 *pixelScale 像素对应距离比 目前1像素对应0.0252mm
 *
 *区域可以选择划分 有可能要检测其他东西最好可以设置多个区域 就当做模板 最好可以保存
 *************************/
bool detecitonWorkPiece(IplImage* viewImage, CvRect areaRect,float rivetminDia, float rivetmaxDia,float dcediff,int threshold, float pixelScale);


//接口函数 检测未装配完铆钉圆心
/***************************8
 *threshold 200
 *Dia  4.9mm        //铆钉直径
 *pixelScale 0.0252  //像素距离比
 *返回的是 圆心X Y像素坐标 和像素banjing
 **************************/
CICLEPAR  getCicularDce(IplImage* viewImage, CvRect areaRect,int threshold, float pixelScale,float Dia );


//接口函数 检测未装配完底座圆心
/***************************8
 *threshold 60
 *Dia  11.6mm        //底座圆半径
 *pixelScale 0.03  //像素距离比
 *返回的是 底座圆心X Y像素坐标 和像素banjing
**************************************/
CICLEPAR  getBaseCicularDce(IplImage* viewImage, CvRect areaRect,int threshold, float pixelScale,float Dia );


/****************************
接口求得 铆钉和底座圆心偏差 X Y单位mm 针对图像X Y轴方向
  rivetCicle 铆钉 圆心 XY 坐标像素
  baseCicle 底座圆心 X Y坐标像素
  pixelScale 0.0252
 ****************************/
CICLEDCEOFFSET getDceOffset(CICLEPAR rivetCicle, CICLEPAR baseCicle, float pixelScale);




/****************update12.16***************************
  areaRect  就是传给我的图的大小就行了 我不去掉了
  threshold 灰度图阈值 设为40 40以上白  40以下黑
  threshol1  间隙边缘阈值 ,目前设为3
*************************************************/
bool detectGap(IplImage* viewImage,CvRect areaRect,int threshold, int threshod1);
void rotateRoi(Mat &src,Rect roi);
void cppTemplateMatch(Mat &src,Mat &temp,Point &p,Rect &roi,int method);
int gap1(Mat &src, Rect roi, int minWidth, int difVal);

#endif // DETECTION_H
