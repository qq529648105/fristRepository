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

//������ ��3��������Ҫ��ʾ
extern float rivetDia;  //�����ڲ�í��ֱ��mm
extern float rivetDceDiff; //í��Բ��λ�ú�Բ��Բ��λ�þ����ֵmm
extern float diskDia;  //Բ��ֱ��mm
/************************update12.9*************/
extern int armDirection;   //�ֱ۷���
/****************************update12.9********/


//�ӿں���  ���
/********************
 *rivetminDia  í����Сֱ��Ҫ��mm  6.0mm
 *rivetmaxDia  ���ֱ��Ҫ��        6.5 mm
 *dcediff   Բ�ľ�ƫ�����ֵ�������ϸ�  0.2mm
 *����3�������Ǽ��Ҫ����� ���ܲ�����΢�� ��û��
 *
 *threshold 2ֵͼ��ֵ Ŀǰ150
 *pixelScale ���ض�Ӧ����� Ŀǰ1���ض�Ӧ0.0252mm
 *
 *�������ѡ�񻮷� �п���Ҫ�������������ÿ������ö������ �͵���ģ�� ��ÿ��Ա���
 *************************/
bool detecitonWorkPiece(IplImage* viewImage, CvRect areaRect,float rivetminDia, float rivetmaxDia,float dcediff,int threshold, float pixelScale);


//�ӿں��� ���δװ����í��Բ��
/***************************8
 *threshold 200
 *Dia  4.9mm        //í��ֱ��
 *pixelScale 0.0252  //���ؾ����
 *���ص��� Բ��X Y�������� ������banjing
 **************************/
CICLEPAR  getCicularDce(IplImage* viewImage, CvRect areaRect,int threshold, float pixelScale,float Dia );


//�ӿں��� ���δװ�������Բ��
/***************************8
 *threshold 60
 *Dia  11.6mm        //����Բ�뾶
 *pixelScale 0.03  //���ؾ����
 *���ص��� ����Բ��X Y�������� ������banjing
**************************************/
CICLEPAR  getBaseCicularDce(IplImage* viewImage, CvRect areaRect,int threshold, float pixelScale,float Dia );


/****************************
�ӿ���� í���͵���Բ��ƫ�� X Y��λmm ���ͼ��X Y�᷽��
  rivetCicle í�� Բ�� XY ��������
  baseCicle ����Բ�� X Y��������
  pixelScale 0.0252
 ****************************/
CICLEDCEOFFSET getDceOffset(CICLEPAR rivetCicle, CICLEPAR baseCicle, float pixelScale);




/****************update12.16***************************
  areaRect  ���Ǵ����ҵ�ͼ�Ĵ�С������ �Ҳ�ȥ����
  threshold �Ҷ�ͼ��ֵ ��Ϊ40 40���ϰ�  40���º�
  threshol1  ��϶��Ե��ֵ ,Ŀǰ��Ϊ3
*************************************************/
bool detectGap(IplImage* viewImage,CvRect areaRect,int threshold, int threshod1);
void rotateRoi(Mat &src,Rect roi);
void cppTemplateMatch(Mat &src,Mat &temp,Point &p,Rect &roi,int method);
int gap1(Mat &src, Rect roi, int minWidth, int difVal);

#endif // DETECTION_H
