#ifndef SYSUTILS_H
#define SYSUTILS_H

#include <QVariant>
#include <QUuid>
#include <opencv2/core/core.hpp>
#define ZERO(a)			(((a) > -1e-15) && ((a) < 1e-15))
#define EQUAL(a, b)		(fabs((a) - (b)) <= 1e-15)
#define UNUSED(x)		(void)x;


static const char X = 0;
static const char Y = 1;
static const char Z = 2;
static const char A = 3;
static const char B = 4;
static const char C = 5;
static const char U = 6;
static const char V = 7;
static const char W = 8;

struct XYZ{double x; double y; double z;};
static const XYZ xyz(double x, double y, double z){
    XYZ xyz;
    xyz.x=x; xyz.y=y; xyz.z=z;
    return xyz;
}

namespace SysUtils {

    bool checkDatabase();
    QString uuid();
    QString uuid(QUuid uuid);

    QVariant getSysParam(QString paramCode);
    bool setSysParam(QString paramCode, QVariant paramValue);
    bool addSysParam(QString paramCode, QVariant paramValue);

    bool addFile(QString id, QByteArray content, QString name = QString::null);
    bool setFile(QString id, QByteArray content);
    QByteArray getFile(QString id);
    bool setFileName(QString id, QString name);
    QString getFileName(QString id);
    QString idToFile(QString id);

    QByteArray readFile(QString fileName);
    bool writeFile(QString fileName, QByteArray data);

	void glRotate(char axis, double center[3], double angle);

    // 外部打标
    void externalPrint(bool isShow);
    // 远程协助
    void remoteHelp(bool isShow);

    // 计算条件表达式
    bool evalCondition(QString valueLeft, QString valueOperator, QString valueRight);

}

using namespace cv;
namespace common {
bool getCircleCenter(Point p1,Point p2,Point p3,Point& p);//三点确定圆心
double mathAngle(double a, double b);
double calcAngle(Point p1,Point p2);//线段角度 返回值范围 -89 ~ 90
int getLineLen(Point p1,Point p2);//得到两点距离
double calcAngle2(Point p1,Point p2);//两点角度 返回值范围 -179 ~ 180 p1中心点 p2物体坐标
int linesAngle(double angle1, double angle2, bool sign);
Point xyCalibration(double angle,Point p1,Point p2);//p1机器中心点  坐标系转换
int circleFeature(std::vector<Point> &contours,Point cen,int step,int move,int offset);//查找半圆特征 返回特征数量
int trangleCalc(Mat &rgb,std::vector<Point> &contours,int sideLen,int index,Rect &roi);//
int trangleCalc2(Mat &rgb, std::vector<Point> &contours, int sideLen, int step, Rect &roi);
void addCpk3(QStringList title,QStringList list);
double anlgeOffset(double angle,double offset);
}

#endif // SYSUTILS_H
