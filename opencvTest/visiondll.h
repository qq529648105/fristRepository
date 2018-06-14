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
    int result;        //-1 ���ʧ�� 1 ���ɹ�
    float offsetX;     //ƫ����X
    float offsetY;     //ƫ����Y
    float offseZ;
    float angle;       //��ת�Ƕ�
}DETECTRESULT;

//#ifdef __cplusplus
//extern "C"  {
//#endif

/*****************��ͦ�Ӿ����� *******************/

/**************************************************
�ӿڣ� zt_getTemplateCenter
���� �������׼ģ���Բ��
����:  x Բ������x
       y Բ������y
�������: dectImage
		  binThreshold 2ֵͼ��ֵ��200��
***************************************************/
__declspec(dllexport) myPoint zt_getTemplateCenter(IplImage *dectImage,int binThreshold);

/**********************************************
�ӿ�: zt_visulLocation
����: �����λ����תƫ��У����Ŀǰ���ڱ�׼��Բ�λ�����
����: result��   0 ���ʧ��  1 ���ɹ�
      offsetX��  ������X�᷽��ƫ����
	  offsetY��  ������Y�᷽��ƫ����
	  offsetZ��  ������Z�᷽��ƫ����������ʹ�ã�
	  angle��    ��������ת�Ƕ�
�������:
	  dectImage�����ͼ��IplImage ����
	  dectArea�� �������
	  leftArea:  ��׼ģ�������������
	  binThreshold: ��ֵͼ��ֵ ��200��
	  offset: ��ȡ����������ֵ��60��
	  tcenterX����׼ģ��Բ��ͼ��ϵ����X
	  tcenterY����׼ģ��Բ��ͼ��ϵ����Y
	  angleCoord����������ת�Ƕ���ͼ����ת�Ƕ�У׼ֵ
	  pixelRatio��ͼ��ռ��������ϵ�� (�ռ����/ͼ�����)
	  offsetX: ������X�᷽��������
	  offsetY: ������Y�᷽��������
**************************************************/
__declspec(dllexport) DETECTRESULT  zt_visulLocation(IplImage *dectImage,CvRect dectArea, CvRect leftArea, int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY);


/**************************************************
�ӿڣ� zt_matchTemplate
���� ��ģ��ƥ��
����:  ƥ��ϵ�� ��method=1 ʱ ԽСƥ��Խ�ߣ�
�������: dectImage1  ���ͼ��
		  tp  ģ��ͼ��
		  dectArea ���ͼ������
		  method ��ⷽ����1��
/*******************************************/
__declspec(dllexport) long int  zt_matchTemplate(IplImage *dectImage1,IplImage *tp,CvRect dectArea, int method);

//Mat& imgͼ��,int &num����Բ��������,int &num2ë������, xy_arr[],��ë������, otherPoint�۵�����
__declspec(dllexport) void xl_circle(Mat& img,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int &otherNum,params &param);

__declspec(dllexport) void zt_calcWork2(Mat &m3,Rect roi,params& param,double &angle);
__declspec(dllexport) void detectWork(Mat &src,Rect roi,params& param,int method);

//#ifdef __cplusplus
//}
//#endif

#endif
