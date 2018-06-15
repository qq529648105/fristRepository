#ifndef VISIONRIVOET_H
#define VISIONRIVOET_H
#include "highgui.h"
#include "cv.h"
#include "opencv2/opencv.hpp"
#include "math.h"
#define INFINITE_NUM 99999999
#define EPSINON 0.00001
#define CRACK_POINT_NUM 128
#define LEAP_NUM_THRESHOLD 3
#define CRACK_REGION_RADIUS 10
#define FILE_PATH_LENTH 256
#define INTERCEPT_RATIO 1.73
#define DETECT_WINDOW_NUM 32  //检测的滑动窗口的块数
#define PI_VALUE 3.1415926
#define IMPRINT_REGION_RATIO 0.4				  //检测印痕区域的必须大于0.5
#define IMPRINT_RADIUS_RATIO 0.5
#define SHIM_REGION_SIZE 300   //检测的垫片区域的大小
#define IMPRINT_WINDOW_SHRINK_RATIO 0.84 // 垫片环上检测区域的缩放比率，排除内外边缘的亮度影响
#define SHIM_INNER_SHRINK_RATIO 0.65 // 垫片外圆的区域缩放比率，排除外边缘的亮度影响
#define FAR_END_REGION_SIZE 5 // 垫片外圆的区域缩放比率，排除外边缘的亮度影响
#define CANDIDATE_COUNTER_NUM 512 // canny提取边缘后，候选Contours数目(counter大小10~40)
#define _DEBUG

typedef struct _CircleParam
{
	//Video Codec
	int				x;		//圆心x
	int				y;
	double			radius;
	double			maxDiameter;
	double			minDiameter;
	//构造函数初始化
	_CircleParam()
	{
		x = 0;
		y = 0;
		radius = 0;
		maxDiameter = 0;
        minDiameter = 0;
	}
}CircleParam;

/**********************************************
接口: getCrackRegion
功能: 铆钉裂缝检测

输入参数:
	  pROI		 检测图像IplImage 类型
	  rectROI	 检测的区域(如果不指定，检测区域为整个图像)
输出参数:
params     返回检测结果,params.var1[0],params.var1[1],params.var2[0],params.var2[1]分别表示是 
                        is_qualified(>0合格，=0不合格)crack_num(裂缝数目)、 max_radius(最大直径)、min_radius(最小直径)
返回: result：   -1 检测失败 0 检测不合格 1 检测合格
**************************************************/
int getCrackRegion(IplImage* pROI, CvRect rectROI, params& param);

/**********************************************
接口: detectDiameterAndCrackPoint
功能: 新的铆钉裂缝检测

输入参数:
	  pROI		 检测图像IplImage 类型
	  rectROI	 检测的区域(如果不指定，检测区域为整个图像)
输出参数:
params     返回检测结果,params.var1[0],params.var1[1],params.var2[0],params.var2[1], params.var2[2]分别表示是 
                        is_qualified(>0合格，=0不合格)crack_num(裂缝数目)、 max_radius(最大直径)、min_radius(最小直径)、像素/毫米比值
返回: result：   -1 检测失败 0 检测不合格 1 检测合格
**************************************************/
int detectDiameterAndCrackPoint(IplImage* pROI, CvRect rectROI, params& param);
/**********************************************
接口: shimDetect
功能: 垫片正反检测

输入参数:
	  pIplsrc		 检测图像IplImage 类型
	  rectROI	 检测的区域(如果不指定，检测区域为整个图像)
输出参数:
params     返回检测结果,params.var1[0](0未检测到印痕，1检测到印痕)
返回: result：   -1 检测失败  0未检测到印痕，1检测到印痕
**************************************************/
int shimDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
/**********************************************
接口: ringDetect
功能: 垫片正反检测

输入参数:
	  pIplsrc		 检测图像IplImage 类型
	  rectROI	 检测的区域(如果不指定，检测区域为整个图像)
输出参数:
params     返回检测结果,params.var1[0](0未检测到反面，1检测正面)
返回: result：   -1 检测失败  0未检测到反面，1检测正面
**************************************************/
int ringDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
/**********************************************
接口: rivoetDirectionDetect
功能: 工件方向检测

输入参数:
	  pIplsrc	 检测图像IplImage 类型
	  rectROI	 检测的区域(如果不指定，检测区域为整个图像)
输出参数:
params     返回检测结果,params.var1[0](=1向下，=2向右，=360无法计算),params.var1[1](0表示无料，1表示有料)
返回: result：   -1 检测失败  1检测成功
**************************************************/
int rivoetDirectionDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
/**********************************************
接口: rivoetDefectDetect
功能: 垫片缺陷检测

输入参数:
	  pIplsrc	 检测图像IplImage 类型
	  rectROI	 检测的区域(如果不指定，检测区域为整个图像)
输出参数:
params     返回检测结果,params.var1[0](=1合格，=0不合格)
返回: result：   -1 检测失败  1检测成功
**************************************************/
int rivoetDefectDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
#endif