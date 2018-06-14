#include "common.h"
#include <QDebug>
extern Mat g_dstImage2;
namespace common {

inline bool getCircleCenter(Point p1,Point p2,Point p3,Point& p)
{
    bool flag=false;

    if(p1.x==p2.x)
        p2.x++;
    if(p1.x==p3.x)
        p3.x++;

    double u=(p1.x*p1.x-p2.x*p2.x+ p1.y*p1.y-p2.y*p2.y)/(2*p1.x-2*p2.x);
    double v=(p1.x*p1.x-p3.x*p3.x+ p1.y*p1.y-p3.y*p3.y)/(2*p1.x-2*p3.x);
    double k1=(p1.y-p2.y)*1.0/(p1.x-p2.x);
    double k2=(p1.y-p3.y)*1.0/(p1.x-p3.x);
    if(abs(k1-k2)<0.000001)
        return false;
    p.y=(u-v)/(k1-k2);
    p.x=v-(u-v)*k2/(k1-k2);

    if(p.x>0&&p.y>0&&p.x<2600&&p.y<2000)
    {
        flag=true;
    }
    else
    {
        p.x=0;
        p.y=0;
    }
    return flag;

}

inline double mathAngle(const double &a, const double &b)
{
    //double c=sqrt(a*a*1.0+b*b*1.0);
    //return acos(b*1.0/c)*180/3.1415;
    return acos(b*1.0/sqrt(a*a*1.0+b*b*1.0))*180/3.1415;
}

inline double calcAngle(Point p1,Point p2)
{
    if(p1.x==p2.x)
        return 90;

    if(p1.y==p2.y)
        return 0;

    if(p1.x>p2.x)
    {

        Point p3=p1;
        p1=p2;
        p2=p3;

    }
    double d1=p2.y-p1.y;
    double d2=abs(p2.x-p1.x);
    if(p2.y<p1.y)
        return mathAngle(d1,d2)*-1.0;
    else
        return mathAngle(d1,d2);
}

int calcAngle1(const Point &p1,const Point &p2)
{

    if(p1.x==p2.x)
        return 90;

    if(p1.y==p2.y)
        return 0;

    int angle=acos(abs(p2.x-p1.x)*1.0/sqrt((p2.y-p1.y)*(p2.y-p1.y)*1.0+(p2.x-p1.x)*(p2.x-p1.x)*1.0))*180/3.1415;
    if(p1.x<p2.x)
    {
        if(p2.y<p1.y)
            return angle*-1;
        else
            return angle;
    }
    else
    {
        if(p2.y<p1.y)
            return angle;
        else
            return angle*-1;
    }


}

inline int getLineLen(Point p1,Point p2)
{
    return sqrt((p1.x-p2.x*1.0)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

int getLineLen1(const Point &p1,const Point &p2)
{
    return (int)sqrt((p1.x-p2.x*1.0)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

//振挺using
double calcAngle2(Point p1,Point p2)//p1中心点 p2物体坐标
{
    double d1=abs(p2.y-p1.y);
    double d2=abs(p2.x-p1.x);
    if(p2.x>p1.x)
    {
        if(p2.y<p1.y)
        {
            return mathAngle(d1,d2)-180;
        }else
        {
            return 180-mathAngle(d1,d2);
        }
    }
    else
    {
        if(p2.y<p1.y)
        {

            return mathAngle(d1,d2)*-1;
        }else
        {
            return mathAngle(d1,d2);
        }
    }



}


Point xyCalibration(double angle,Point p1,Point p2)//p1中心点
{
    if(p1==p2)
        return p1;
    double d1=abs(p2.y-p1.y);
    double d2=abs(p2.x-p1.x);
    double angle1=mathAngle(d1,d2);
    int len=getLineLen(p1,p2);
    int signX,signY;
    double angle3=angle1+angle;

    if(p2.x<p1.x&&p2.y<p1.y||p2.x>p1.x&&p2.y>p1.y)
        angle3=angle1+angle*-1;

    if(p1.x==p2.x)
    {
        if(p2.y<p1.y)
        { signY=-1; signX=angle>0?-1:1;}
        else
        {signY=1;signX=angle>0?1:-1;}
    }
    else if(p1.y==p2.y)
    {
        if(p2.x<p1.x)
        { signX=-1; signY=angle>0?1:-1;}
        else
        { signX=1;signY=angle>0?-1:1;}
    }
    else
    {
        signX=(p2.x>p1.x?1:-1);
        signY=(p2.y>p1.y?1:-1);
        if(angle3>90) signX= signX>0?-1:1;
        if(angle3<0)  signY= signY>0?-1:1;
    }

    int x=signX*len*abs(cos(angle3*3.1415/180))+p1.x;//xOffset
    int y=signY*len*abs(sin(angle3*3.1415/180))+p1.y;//yOffset
    //qDebug()<<"X:"<<x<<"Y:"<<y<<"|"<<sin((angle1+angle)*3.1415/180)<<"p2.x"<<p2.x<<"p2.y"<<p2.y<<"angle"<<angle<<"p1.x"<<p1.x<<"p1.y"<<p1.y;
    return Point(x,y);
}


int circleFeature(std::vector<Point> &contours,Point cen,int step,int move,int offset)
{
    int size=contours.size();
    int num=0;
    Point p;
    bool success;

    for(int i=0;i+step*2<size;i+=move)
    {
        success= getCircleCenter(contours[i],contours[step+i],contours[step*2+i],p);
        if(success)
        {
            //qDebug()<<"aaa"<<p.x<<p.y<<"cen"<<cen.x<<cen.y;
            if(contours.size()==688)
                qDebug()<<"yyyy"<<getLineLen(p,cen);
            if(abs(p.x-cen.x)<offset&&abs(p.y-cen.y)<offset)
                 ++num;
        }
    }

    return num;
}


int trangleCalc(Mat &rgb,std::vector<Point> &contours,int sideLen,int index,Rect &roi)
{
    int size=contours.size();
    int i=index,j=index;
    int i2,j2,i3,j3;
    int k=0,num=0;
    int times=5;
    int step=5;//size/4/times;
    if(step>size/4)
        return 0;
    //qDebug()<<"size"<<size<<step<<sideLen;
    while(1)
    {
        if(k>times)
            break;

        i2=i+step;
        j2=j-step;
        i3=i+2*step;
        j3=j-2*step;
        if(i>=size) i-=size;
        if(i2>=size) i2-=size;
        if(i3>=size) i3-=size;
        if(j<0) j+=size;
        if(j2<0) j2+=size;
        if(j3<0) j3+=size;
       // qDebug()<<"trangle"<<abs(common::calcAngle(contours[i],contours[i2])-common::calcAngle(contours[i2],contours[i3]))<<abs(common::calcAngle(contours[j],contours[j2])-common::calcAngle(contours[j2],contours[j3]));
   // circle( rgb, Point(contours[i].x+roi.x,contours[i].y+roi.y ), 3, Scalar(0,255,0), 1, 16);
                if(abs(common::calcAngle(contours[i],contours[i2])-common::calcAngle(contours[i2],contours[i3]))<12)
                    ++num;

                if(abs(common::calcAngle(contours[j],contours[j2])-common::calcAngle(contours[j2],contours[j3]))<12)
                    ++num;

        i+=step;
        j-=step;
        k++;
    }

    //qDebug()<<"jjj"<<i<<i2<<i3<<j<<j2<<j3;
    //qDebug()<<"jjj"<<contours[i].x<<contours[i2].x<<contours[i3].x<<contours[j].x<<contours[j2].x<<contours[j3].x;
    return num;
}

inline double angleDif(double angle1,double angle2,int base,bool isAbs)
{
    double n=angle1-angle2;
    if(n<base*-1)
        return 2*base+n;

    if(n>base)
        return 2*base-n;

    if(isAbs)
        return abs(n);
    else
        return n;
}

int angleDif1(int &angle1,int &angle2,int &base,bool &isAbs)
{
    double n=angle1-angle2;
    if(n<base*-1)
        return 2*base+n;

    if(n>base)
        return 2*base-n;

    if(isAbs)
        return abs(n);
    else
        return n;
}

double anlgeOffset(double angle,double offset)
{

    if(offset+angle>180)
        angle=offset+angle-360;
    else if(offset+angle<-180)
        angle=offset+angle+360;
    else
        angle+=offset;
    return angle;
}

void drawRect(Mat &src,Point cen,int distance,int angleVal,int angleFlag,Scalar &color)
{
    if(angleVal==360)
        return;
    Rect r1;
    r1.width=100;
    r1.height=100;
    if(angleFlag==90)
    {
        if(abs(angleVal)<90)
        {
            r1.x=cen.x-50-distance;
            r1.y=cen.y-50;
            if(r1.x<0) r1.x=0;
            rectangle(src,r1,color,3);

        }
        else
        {
            r1.x=cen.x-50+distance;
            r1.y=cen.y-50;
            if(r1.x+r1.width>src.cols) r1.x=src.cols-r1.width;
            rectangle(src,r1,color,3);
        }
        //qDebug()<<"r1.x"<<r1.x<<r1.y<<cen.x<<cen.y;
    }
    else if(angleFlag==0)
    {
        if(angleVal<0)
        {
            r1.x=cen.x-50;
            r1.y=cen.y-50-distance;
            if(r1.y<0) r1.y=0;
            rectangle(src,r1,color,3);

        }
        else
        {
            r1.x=cen.x-50;
            r1.y=cen.y-50+distance;
            if(r1.y+r1.height>src.rows) r1.y=src.rows-r1.height;
            rectangle(src,r1,color,3);
        }
    }

}

int Smoothlevel(vector<Point> &contours,int step,int angle,int isAbs)
{

    double d1,d2;
    int num=0;
    for(int i=0;i<contours.size()-step*2;i+=step)
    {
        d1=common::calcAngle(contours[i],contours[i+step]);
        d2=common::calcAngle(contours[i+step],contours[i+step*2]);
        if(common::angleDif(d1,d2,90,isAbs)>angle)
            ++num;
    }
    return num;
}

bool contourType(Mat &src,vector<Point> &contours,int value,bool isBlack)
{
    if(contours[0].y-1<0||contours[0].y+1>src.rows-1)
        return false;
    bool b;
    if(isBlack)
        b=src.at<uchar>(contours[0].y-1,contours[0].x)<value?1:0;
    else
        b=src.at<uchar>(contours[0].y-1,contours[0].x)>value?1:0;
        //b=src.at<uchar>(contours[0].y+1,contours[0].x)<value?1:0;
    return b;
}

void mostVal(int &err,int val)
{
    if(val>err)
        err=val;
}

bool nearbyPos(vector<Point> &arr, int dis,int flagXY)
{

    for(int i=0;i<arr.size();i++)
    {
        for(int j=i+1;j<arr.size();j++)
        {
           // qDebug()<<"getLineLen(arr[i],arr[j])"<<getLineLen(arr[i],arr[j]);

            if(flagXY==5&&getLineLen(arr[i],arr[j])<dis)
                return true;
            else if(flagXY==1&&getLineLen(arr[i],arr[j])<dis)
                return true;
            else if(flagXY==3&&getLineLen(arr[i],arr[j])<dis)
                return true;
        }
    }
    return false;
}

int circleRediusFeature(std::vector<Point> &contours, int step, int move, int rLen1, int rLen2)
{

    int num=0,temp;
    Point p;
    bool success;

    for(int i=0;i+step*2<contours.size();i+=move)
    {
        success= getCircleCenter(contours[i],contours[step+i],contours[step*2+i],p);
        if(success)
        {
            if(contours.size()==321)
            {
                //qDebug()<<"aaa"<<contours[i].x<<contours[i].y<<contours[step+i].x<<contours[step+i].y<<contours[step*2+i].x<<contours[step*2+i].y;
           // circle(g_dstImage2,p,5,Scalar(0,0,255),2);
            // circle(g_dstImage2,contours[i],5,Scalar(0,0,255),2);
             // circle(g_dstImage2,contours[step+i],5,Scalar(0,0,255),2);
              // circle(g_dstImage2,contours[step*2+i],5,Scalar(0,0,255),2);

            }
            temp=getLineLen(p,contours[i]);
            if(temp>rLen1&&temp<rLen2)
            {


                 ++num;
            }
        }
    }

    return num;
}

}
