#ifndef PROJECT_H
#define PROJECT_H

#include <QObject>
#include "Global.h"
#include <QTimer>
#include <Windows.h>

using namespace cv;
class project : public QObject
{
    Q_OBJECT
public:
    explicit project(QObject *parent = 0);
    void edvia(Mat src, Mat rgb);
    bool isPos(Mat &src,Mat &rgb,int grayVal,int _step,int x,int y,bool isLeft);
    void find2(Mat &src,Mat &rgb,Point &p,bool isLeft);
    void find2_r(Mat &src,Mat &rgb,Point &p,bool isLeft);
    void edviaGray(Mat &src);
    //int cmpPicPixel2(Mat &src1, Mat &src2, Mat &rgb, Rect &roi,Point setLeft,Point setRight, int val, int &doorWidth);
    int cmpPicPixel2(Mat &src1, Mat &src2, Mat &rgb, Point setLeft,Point setRight, int val, int &doorWidth,Point &getLeft,Point &getRight);
    bool findBlackLine(Mat &src,Mat &rgb,int grayVal,int _step,Point p1,Point p2);
    int staticBlackObj(Mat &src, Mat &rgb, int grayVal, Point p1, Point p2, int maxNum);
    bool lenIsNormal(Mat &src,Point left,Point right);
signals:
    
public slots:
    
};

#endif // PROJECT_H
