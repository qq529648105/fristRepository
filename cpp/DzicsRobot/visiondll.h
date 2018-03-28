#ifndef VISIONDLL_H
#define VISIONDLL_H
#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"

//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <opencv.hpp>

using namespace cv;
typedef struct _point{
	int x;
	int y;
}myPoint;

typedef struct{
     bool calcCen;
     int x;
     int y;
     int size;
     double moveX;
     double moveY;
   //  std::vector<Point> p1;

 }xy_size_1;
typedef struct{
    int var1[30];
    double var2[20];
}params;
typedef struct _DETECTRESULT{
    int result;        //-1 检测失败 1 检测成功
    float offsetX;     //偏移量X
    float offsetY;     //偏移量Y
    float offseZ;
    float angle;       //旋转角度
}DETECTRESULT;

//#ifdef __cplusplus
//extern "C"  {
//#endif

/*****************振挺视觉分析 *******************/

/**************************************************
接口： zt_getTemplateCenter
功能 ：计算标准模板的圆心
返回:  x 圆心坐标x
       y 圆心坐标y
输入参数: dectImage
		  binThreshold 2值图阈值（200）
***************************************************/
__declspec(dllexport) myPoint zt_getTemplateCenter(IplImage *dectImage,int binThreshold);

/**********************************************
接口: zt_visulLocation
功能: 零件定位，旋转偏移校正（目前用于标准的圆形活塞）
返回: result：   0 检测失败  1 检测成功
      offsetX：  机器人X轴方向偏移量
	  offsetY：  机器人Y轴方向偏移量
	  offsetZ：  机器人Z轴方向偏移量（暂无使用）
	  angle：    机器人旋转角度
输入参数:
	  dectImage：检测图像IplImage 类型
	  dectArea： 检测区域
	  leftArea:  标准模板左边特征区域
	  binThreshold: 二值图阈值 （200）
	  offset: 提取特征轮廓阈值（60）
	  tcenterX：标准模板圆心图像系坐标X
	  tcenterY：标准模板圆心图像系坐标Y
	  angleCoord：机器人旋转角度与图像旋转角度校准值
	  pixelRatio：图像空间坐标比例系数 (空间距离/图像距离)
	  offsetX: 机器人X轴方向修正量
	  offsetY: 机器人Y轴方向修正量
**************************************************/
__declspec(dllexport) DETECTRESULT  zt_visulLocation(IplImage *dectImage,CvRect dectArea, CvRect leftArea, int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY);


/**************************************************
接口： zt_matchTemplate
功能 ：模板匹配
返回:  匹配系数 （method=1 时 越小匹配越高）
输入参数: dectImage1  检测图像
		  tp  模板图像
		  dectArea 检测图像区域
		  method 检测方法（1）
/*******************************************/
__declspec(dllexport) long int  zt_matchTemplate(IplImage *dectImage1,IplImage *tp,CvRect dectArea, int method);

//Mat& img图像,int &num所有圆孔数数量,int &num2毛刺数量, xy_arr[],有毛刺坐标, otherPoint污点坐标
__declspec(dllexport) void xl_circle(Mat& img,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int &otherNum,params &param);

__declspec(dllexport) void zt_calcWork2(Mat &m3,Rect roi,params& param,double &angle);
__declspec(dllexport) void detectWork(Mat &src,Rect roi,params& param,int method);

//#ifdef __cplusplus
//}
//#endif

#endif
