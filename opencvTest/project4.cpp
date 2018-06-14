#include "project4.h"
#include <QDebug>
#include <string.h>
#include <QTime>
#include <QFile>
#include <QRegExp>
#include <QStringList>
#include <QDir>
#include <algorithm>
extern Mat g_dstImage1;
extern Mat g_dstImage2;
extern int g_nElementShape;
extern int thresh;
extern int max_thresh;
extern int offsetSign;
extern bool DEBUG;
project4::project4(QObject *parent) :
    QObject(parent)
{
}
int calcDefect(vector<Point> &contours,int step,int pos,int xyDif,int xEnd,int yEnd)
{
   int num=0;
   bool b1,b2,b3,b4;
   b1=b2=b3=b4=false;
   for(unsigned int i=0;i<contours.size()-pos;i+=step)
   {
        if(!b1&&abs(contours[i].x-contours[i+pos].x)>xyDif)
        {
            ++num;
            b1=true;
            circle(g_dstImage2,contours[i],3,Scalar(255,0,0),2);

        }
        if(abs(contours[i].x-contours[i+pos].x)<xEnd)
        {

            b1=false;
        }


        //y
        if(!b3&&abs(contours[i].y-contours[i+pos].y)>xyDif)
        {
            ++num;
            b3=true;
            circle(g_dstImage2,contours[i],3,Scalar(0,255,0),2);

        }
        if(abs(contours[i].y-contours[i+pos].y)<yEnd)
        {

            b3=false;
        }



   }
   return num;
}
void project4::defect(Mat &src, Mat &rgb, Rect roi, params &param)
{

    unsigned int objMinSize=param.var1[0];
    unsigned int objMaxSize=param.var1[1];
    int thresh=param.var1[2];//二值
    int step=param.var1[3];
    int pos=param.var1[4];//下一位置
    int angleDif=param.var1[5];//角度差
    int xyDif=param.var1[6];//偏差
    int xEnd=param.var1[7];//x结束
    int yEnd=param.var1[8];//y结束
    Mat m1,m2;
    param.var1[29]=0;
    int x1,y1;
    Mat m5=src(roi);
    line(m5,Point(0,0),Point(m5.cols,0),Scalar(255,255,255),5);
    line(m5,Point(0,m5.rows),Point(m5.cols,m5.rows),Scalar(255,255,255),5);
    threshold(m5,m1,thresh,255,THRESH_BINARY);
    threshold(m5,m2,thresh,255,THRESH_BINARY);

    std::vector<std::vector<Point>> contours;
    findContours(m1,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    for (unsigned int k=0 ; k<contours.size(); k++)
    {
        if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*2)
            continue;

        if(common::contourType(src,contours[k],thresh,true))
            continue;

        End_Rage2D1 = minAreaRect(Mat(contours[k]));
        cvBoxPoints(End_Rage2D1,rectpoint);
        x1=End_Rage2D1.center.x;
        y1=End_Rage2D1.center.y;

        qDebug()<<"found";
        int num=calcDefect(contours[k],step,pos,xyDif,xEnd,yEnd);
        qDebug()<<"num"<<num;
        param.var1[29]=num;
        imshow("thresh2",m2);
        break;



    }

}

void project4::zt_LLine(Mat &src, Mat &rgb, Rect roi, params &param)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize=param.var1[2];
    unsigned int objMaxSize=param.var1[3];
    int thresh=param.var1[4];//二值
    int rMinLen=param.var1[5];//宽
    int rMaxLen=param.var1[6];//长
    int redius=param.var1[7];//半径
    int pos1=param.var1[8];//
    int pos2=param.var1[9];//
    int pos3=param.var1[10];//
    int pos4=param.var1[11];//
    double angleOffset=param.var2[18];//角度偏移
    Mat m1,m2;

    double angle1;

    int x1,y1,len1,len2;
    Mat m5=src(roi).clone();

    medianBlur(m5, m5, 3);

    circle(m5,Point(cenX,cenY),redius,Scalar(0,0,0),3);
    line(m5,Point(cenX+pos1,cenY+pos2),Point(cenX+pos3,cenY+pos4),Scalar(0,0,0),6);

    line(rgb,Point(param.var1[0]+pos1,param.var1[1]+pos2),Point(param.var1[0]+pos3,param.var1[1]+pos4),Scalar(0,0,0),6);
    circle(rgb,Point(param.var1[0],param.var1[1]),redius,Scalar(0,0,0),3);
    for(unsigned int thresh1=254;thresh1>thresh;thresh1-=10)
    {
        threshold(m5,m1,thresh1,255,THRESH_BINARY);

        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];


        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

            if(common::contourType(src,contours[k],thresh,false))
                continue;

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;

            if(common::getLineLen(Point(x1,y1),Point(cenX,cenY))>80)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<rMinLen||len2>rMaxLen)
                continue;

            circle(rgb,Point(contours[k][300].x+roi.x,contours[k][300].y+roi.y),3,Scalar(255,0,0),2);



            int posLen=common::getLineLen(contours[k][0],contours[k][300]);

            angle1=posLen<rMinLen/2?90:-90;

            param.var2[19]=common::anlgeOffset(angle1,angleOffset);

            return;



        }

    }
}
