#ifndef PROJECT3_H
#define PROJECT3_H
#include <QObject>
#include "Global.h"
#include <QSharedMemory>

class project3 : public QObject
{
    Q_OBJECT
public:
    explicit project3(QObject *parent = 0);

signals:
    void addLog(QString str);
public slots:
    void regex();
    void test();
    void zt_oneTrangle(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_Iline(Mat &src,Rect roi,params& param);
    void zt_6thLine(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_zt01Match(Mat &src, Mat &rgb, Rect roi, params &param);
    void zt_GLine(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_GLineGN5(Mat &src, Mat &rgb, Mat &src2, Rect roi, params& param);
    void zt_GLineGN5_2(Mat &src, Mat &rgb, Rect roi, params& param);
    void zt_GLineNI(Mat &src,Mat &rgb,Rect roi,params& param);
    void loadShapePoint(int amount);

    void rgbPic(IplImage *img, bool isHsv);
    bool calcI(Mat &src,Point &cen,Point &font,int area,int len,Rect &rect,int difVal);
    void clothEdge(Mat &src,Rect roi,params& param);
    void similarity();
    void splitRGB(Mat &rgb);
    void zt_3rdLineQC(Mat &src,Rect roi,params& param,bool blackHat);
    void zt_not3rdLine(Mat &src,Rect roi,params& param);
    int matchShapes2(std::vector<Point> &contours, int m);
    void cmpPicPixel(Mat &src1, Mat &src2,Mat &src3, Rect roi, int val);

    void zt_JLine(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_JLineQC(Mat &src,Mat &rgb,Rect roi,params& param);
    void zt_JLineMatch(Mat &src, Mat &rgb, Rect roi, params &param);
    void commonMatch(Mat &src,Mat &rgb,Rect roi,params& param);
    void countMatch(Mat &src,Mat &rgb,Rect roi,params& param);
    void sz_cloth2(Mat &src,Mat &rgb,Rect roi,params& param);
private slots:

    double contoursFeature1(std::vector<Point> &dstContours,std::vector<Point> &_contour2, int mIndex, double parameter);
    double contoursFeature2(std::vector<Point> &dstContours,std::vector<Point> &_contour2, int mIndex, double parameter);

    void insertFeature(std::vector<int> &srcArr,int index,Point cen,std::vector<Point> &contours,int amount,int radius);
private:
    int s_modelNum;
    std::vector<std::vector<int>> tempArr;
    std::vector<std::vector<double>> tempDisArr;
    std::vector<std::vector<double>> tempDisArr2;

};
#endif // PROJECT3_H
