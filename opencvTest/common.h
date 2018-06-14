#ifndef COMMON_H
#define COMMON_H
#include <opencv2/core/core.hpp>
using namespace cv;
namespace common {
bool getCircleCenter(Point p1,Point p2,Point p3,Point& p);//三点确定圆心
double mathAngle(const double &a,const double &b);
double calcAngle(Point p1, Point p2);//线段角度 返回值范围 -89 ~ 90
int calcAngle1(const Point &p1, const Point &p2);
int getLineLen(Point p1, Point p2);//得到两点距离
int getLineLen1(const Point &p1,const Point &p2);
double calcAngle2(Point p1,Point p2);//两点角度 返回值范围 -179 ~ 180 p1中心点 p2物体坐标
Point xyCalibration(double angle,Point p1,Point p2);//p1机器中心点  坐标系转换
int circleFeature(std::vector<Point> &contours,Point cen,int step,int move,int offset);//查找半圆特征 返回特征数量
int circleRediusFeature(std::vector<Point> &contours,int step,int move,int rLen1,int rLen2);
int trangleCalc(Mat &rgb,std::vector<Point> &contours,int sideLen,int index,Rect &roi);//

double angleDif(double angle1, double angle2, int base, bool isAbs);
int angleDif1(int &angle1, int &angle2, int &base, bool &isAbs);
double anlgeOffset(double angle,double offset);
void drawRect(Mat &src,Point cen,int distance,int angleVal,int angleFlag,Scalar &color);
int Smoothlevel(vector<Point> &contours, int step, int angle, int isAbs);
bool contourType(Mat &src, vector<Point> &contours, int value, bool isBlack);
void mostVal(int &err,int val);
bool nearbyPos(vector<Point> &arr, int dis, int flagXY);

}
#endif // COMMON_H
