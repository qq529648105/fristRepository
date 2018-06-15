#ifndef PROJECT1_H
#define PROJECT1_H

#include <QObject>
#include "Global.h"


class project1 : public QObject
{
    Q_OBJECT
public:
    explicit project1(QObject *parent = 0);
    
signals:
    
public slots:
    void zt_2ndLine(Mat &m3,Rect roi,params& param);
    void zt_lineA2(Mat &src,Rect roi,params& param);
    void zt_3rdLine(Mat &src,Rect roi,params& param,bool blackHat);
    int zt_1stLine(Mat &m5,Point &p,float &angle);
    int calcWork(Mat img,Point &p,float &angle,int c1=603,int c2=623);
    void xl_circle3(Mat& rgb,Mat& img1,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int,params &param);

    void xl_circle4(Mat& rgb,Mat& img1,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int,params &param);
    void xl_blackObj(Mat& src,Mat& rgb,Rect roi,params &param);
    void zt_calcWork_6(Mat &m3,Rect roi,params& param,double &angle);
    void calcMeter(Mat& img,Rect rect,bool& NG,int &offX,int &offY,params param);
    Mat rotateImg(Mat &src);
    void rotateRoi(Mat &src,Rect roi);
    void detectContours2();
    void calcMeter1(IplImage* img,IplImage* rgb,bool &NG,int &offX,int &offY);
    void calcMeter2(IplImage* img,IplImage* rgb,bool &NG,int &offX,int &offY);
    void calcMeter3(IplImage* img,IplImage* rgb,bool &NG,int &offX,int &offY);
    int icecream(Mat &src, Point p1, Point p2, int grayVal, Point &pos2);
    int icecream2(Mat &src, Rect &roi, int grayVal, Point &pos2);
    int icecream3(Mat &src, Rect &roi, int grayVal, Point &pos2);
    void find2Circle(IplImage* img, IplImage* rgb, Point &p1, Point &p2);
    bool elevatorInit(Mat& src,Mat& rgb);
};

#endif // PROJECT1_H
