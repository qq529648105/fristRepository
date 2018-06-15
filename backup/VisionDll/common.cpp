#include "common.h"
//#pragma optimize( "", on )
//#pragma optimize( "", off )

inline int getLineLen(Point p1,Point p2)
{
    return sqrt((p1.x-p2.x*1.0)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
}

int getLineLen1(const Point &p1,const Point &p2)
{
    return (int)sqrt((p1.x-p2.x*1.0)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
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


bool getCircleCenter(Point p1,Point p2,Point p3,Point& p)
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
            if(abs(p.x-cen.x)<offset&&abs(p.y-cen.y)<offset)
                 ++num;
        }
    }
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


int Smoothlevel(vector<Point> &contours,int step,int angle,int isAbs)
{

    double d1,d2;
    int num=0;
    for(int i=0;i<contours.size()-step*2;i+=step)
    {
        d1=calcAngle(contours[i],contours[i+step]);
        d2=calcAngle(contours[i+step],contours[i+step*2]);
        if(angleDif(d1,d2,90,isAbs)>angle)
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

bool nearbyPos(vector<Point> &arr, int dis,int flagXY)
{

    for(int i=0;i<arr.size();i++)
    {
        for(int j=i+1;j<arr.size();j++)
        {
          
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
            temp=getLineLen(p,contours[i]);
            if(temp>rLen1&&temp<rLen2)
                 ++num;
        }
    }

    return num;
}