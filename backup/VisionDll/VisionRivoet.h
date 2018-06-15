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
#define DETECT_WINDOW_NUM 32  //���Ļ������ڵĿ���
#define PI_VALUE 3.1415926
#define IMPRINT_REGION_RATIO 0.4				  //���ӡ������ı������0.5
#define IMPRINT_RADIUS_RATIO 0.5
#define SHIM_REGION_SIZE 300   //���ĵ�Ƭ����Ĵ�С
#define IMPRINT_WINDOW_SHRINK_RATIO 0.84 // ��Ƭ���ϼ����������ű��ʣ��ų������Ե������Ӱ��
#define SHIM_INNER_SHRINK_RATIO 0.65 // ��Ƭ��Բ���������ű��ʣ��ų����Ե������Ӱ��
#define FAR_END_REGION_SIZE 5 // ��Ƭ��Բ���������ű��ʣ��ų����Ե������Ӱ��
#define CANDIDATE_COUNTER_NUM 512 // canny��ȡ��Ե�󣬺�ѡContours��Ŀ(counter��С10~40)
#define _DEBUG

typedef struct _CircleParam
{
	//Video Codec
	int				x;		//Բ��x
	int				y;
	double			radius;
	double			maxDiameter;
	double			minDiameter;
	//���캯����ʼ��
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
�ӿ�: getCrackRegion
����: í���ѷ���

�������:
	  pROI		 ���ͼ��IplImage ����
	  rectROI	 ��������(�����ָ�����������Ϊ����ͼ��)
�������:
params     ���ؼ����,params.var1[0],params.var1[1],params.var2[0],params.var2[1]�ֱ��ʾ�� 
                        is_qualified(>0�ϸ�=0���ϸ�)crack_num(�ѷ���Ŀ)�� max_radius(���ֱ��)��min_radius(��Сֱ��)
����: result��   -1 ���ʧ�� 0 ��ⲻ�ϸ� 1 ���ϸ�
**************************************************/
int getCrackRegion(IplImage* pROI, CvRect rectROI, params& param);

/**********************************************
�ӿ�: detectDiameterAndCrackPoint
����: �µ�í���ѷ���

�������:
	  pROI		 ���ͼ��IplImage ����
	  rectROI	 ��������(�����ָ�����������Ϊ����ͼ��)
�������:
params     ���ؼ����,params.var1[0],params.var1[1],params.var2[0],params.var2[1], params.var2[2]�ֱ��ʾ�� 
                        is_qualified(>0�ϸ�=0���ϸ�)crack_num(�ѷ���Ŀ)�� max_radius(���ֱ��)��min_radius(��Сֱ��)������/���ױ�ֵ
����: result��   -1 ���ʧ�� 0 ��ⲻ�ϸ� 1 ���ϸ�
**************************************************/
int detectDiameterAndCrackPoint(IplImage* pROI, CvRect rectROI, params& param);
/**********************************************
�ӿ�: shimDetect
����: ��Ƭ�������

�������:
	  pIplsrc		 ���ͼ��IplImage ����
	  rectROI	 ��������(�����ָ�����������Ϊ����ͼ��)
�������:
params     ���ؼ����,params.var1[0](0δ��⵽ӡ�ۣ�1��⵽ӡ��)
����: result��   -1 ���ʧ��  0δ��⵽ӡ�ۣ�1��⵽ӡ��
**************************************************/
int shimDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
/**********************************************
�ӿ�: ringDetect
����: ��Ƭ�������

�������:
	  pIplsrc		 ���ͼ��IplImage ����
	  rectROI	 ��������(�����ָ�����������Ϊ����ͼ��)
�������:
params     ���ؼ����,params.var1[0](0δ��⵽���棬1�������)
����: result��   -1 ���ʧ��  0δ��⵽���棬1�������
**************************************************/
int ringDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
/**********************************************
�ӿ�: rivoetDirectionDetect
����: ����������

�������:
	  pIplsrc	 ���ͼ��IplImage ����
	  rectROI	 ��������(�����ָ�����������Ϊ����ͼ��)
�������:
params     ���ؼ����,params.var1[0](=1���£�=2���ң�=360�޷�����),params.var1[1](0��ʾ���ϣ�1��ʾ����)
����: result��   -1 ���ʧ��  1���ɹ�
**************************************************/
int rivoetDirectionDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
/**********************************************
�ӿ�: rivoetDefectDetect
����: ��Ƭȱ�ݼ��

�������:
	  pIplsrc	 ���ͼ��IplImage ����
	  rectROI	 ��������(�����ָ�����������Ϊ����ͼ��)
�������:
params     ���ؼ����,params.var1[0](=1�ϸ�=0���ϸ�)
����: result��   -1 ���ʧ��  1���ɹ�
**************************************************/
int rivoetDefectDetect(IplImage* pIplsrc, CvRect rectROI, params& param);
#endif