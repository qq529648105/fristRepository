/*******************
展会电梯
修改函数
ispos
ispos2
findMidLine
************************/
#include "project.h"
#include <QDebug>
#include "common.h"
#include <string.h>
int arr1[1000],arr2[1000],arr3[1000],grayArr[10];
int groundLen1=4;//黑线离上地槛长度
int groundLen2=6;//黑线离下地槛长度
Point pArr[500]={0};//中间黑线存入坐标
int _step=3;//像素
int step1=10;//中线两边距离减少范围
int midLineHeight=4;//中间黑线最大高度
int difMidBright=40;//底黑线差值
int difWhiteToBlack=70;//黑线到地槛差值
int addUpGround=0;//允许上地槛值稍微变暗

int _blackVal=185;//黑线灰度值(小于)
int groundVar=100;//地槛波动值
int sideDistance=120;


project::project(QObject *parent) :
    QObject(parent)
{

}


bool findMidLine(Mat &src,Mat &rgb,int grayVal,int _step,Point p1,Point p2)
{
    int y;
    int val,val1,val2,val3,val4,val5,val10=-1;
    double ratio=(p2.y-p1.y)*1.0/(p2.x-p1.x);

    for(int i=p1.x;i<p2.x;i+=_step)
    {

        y=p1.y+(i-p1.x)*ratio;
        val1=src.at<uchar>(y,i);
        val2=src.at<uchar>(y-1,i);
        val3=src.at<uchar>(y+1,i);
        val4=src.at<uchar>(y-2,i);
        val5=src.at<uchar>(y+2,i);
        val=val1<val2?val1:val2;
        val=val<val3?val:val3;
        val=val<val4?val:val4;
        val=val<val5?val:val5;


       // circle( rgb, Point(i,y ), 3, Scalar(0,255,0), 1, CV_AA);
        //qDebug()<<"aaaa"<<y;
//       if(val>grayVal&&val2>grayVal&&val3>grayVal)
//       {
//         //  qDebug()<<"aaa"<<(i-p1.x)*ratio<<"i-p1.x"<<i-p1.x<<"ratio"<<ratio;
//           qDebug("mid line is block! val=%d! va12=%d val3=%d x=%d  y=%d",val,val2,val3,i,y);
//           return false;
//       }

        if(val10>0)
        {

            //qDebug()<<"aaaa"<<abs(val-val10)<<val<<val10;
            if(abs(val-val10)>25)
            {
                circle( rgb, Point(i,y ), 8, Scalar(255,255,255), 1, CV_AA);
               return false;
            }
        }
        val10=val;
    }
    return true;

}

bool isPos2(Mat &src,Mat &rgb,int grayVal,int _step,int x,int y,bool isLeft)
{
//  left <-      right ->
    bool flag=false;

    int i=x;
    int up;
    int mid;

    int down;
    int tmp;
    int val,val2=-1;
    int v1=3;
    int v2=4;
    while(1)
    {

        mid= src.at<uchar>(y,i);
        up = src.at<uchar>(y-1,i);
        down = src.at<uchar>(y+1,i);
        tmp=(mid<=up)?mid:up;
        val=(tmp>down)?down:tmp;
        if(isLeft)
        {

            i-=_step;
            if(i<0)
                break;
            if(i>120&&i<160)
            {

//                //qDebug()<<"aaaa"<<i<<y<<src.at<uchar>(y+v1,i);
                if( src.at<uchar>(y+v1,i)-val>30&&src.at<uchar>(y+v2,i)-val>30&&src.at<uchar>(y-v1,i)-val>30&&src.at<uchar>(y-v2,i)-val>30)
                {

                    //circle( rgb, Point(i,y ), 8, Scalar(255,255,255), 1, CV_AA);

                      break;
                }
            }

        }else
        {

            i+=_step;
            if(i>src.cols)
                break;
            if(i<src.cols-sideDistance&&i>src.cols-160)
            {
                //qDebug()<<"aaaa"<<src.at<uchar>(y+v1,i)<<src.at<uchar>(y+v2,i)<<src.at<uchar>(y-v1,i)<<src.at<uchar>(y-v2,i)<<val<<i;
                if( src.at<uchar>(y+v1,i)-val>30&&src.at<uchar>(y+v2,i)-val>30&&src.at<uchar>(y-v1,i)-val>30&&src.at<uchar>(y-v2,i)-val>30)
                {


                    break;
                }
            }

        }



        if(val==up){
            --y;
        }else if(val==down){
            ++y;
        }
        if(!isLeft&&i>src.cols/4*3||isLeft&&i<src.cols/4)
        {

            circle( rgb, Point(i,y ), 3, Scalar(0,0,255), 1, CV_AA);
        }



       //qDebug()<<"val1"<<val;
       // if(val>90&&abs(i-x)>6)
        if(val-grayVal>30&&abs(i-x)>6)
           break;
//        if(val2>0)
//        {
//                if(abs(val-val2)>difMidBright+20)
//                break;

//        }

 //       val2=val;



    }
    //qDebug()<<"i:"<<i<<"left or right"<<isLeft;
    if(i>src.cols-sideDistance&&!isLeft) flag=true;

    if(i<sideDistance&&isLeft) flag=true;

    return flag;

}


bool project::isPos(Mat &src,Mat &rgb,int grayVal,int _step,int x,int y,bool isLeft)
{
//left ->      right  <-
    bool flag=false;

    int i=x;
    int up;
    int mid;

    int down;
    int tmp;
    int val,val2=-1;
    int v1=3;
    int v2=4;

    //排除误判
    if(isLeft)
    {
        if(x<src.cols/3*2)
        {
            if(y-14>0)
            {
                if(grayVal-src.at<uchar>(y-10,x)>25&&grayVal-src.at<uchar>(y-12,x)>25&&grayVal-src.at<uchar>(y-14,x)>25)
                {
                   return false;
                }
            }
        }
    }
    else
    {
        if(x>src.cols/3)
        {

            if(y+14<src.rows)
            {
                if(grayVal-src.at<uchar>(y+10,x)>25&&grayVal-src.at<uchar>(y+12,x)>25&&grayVal-src.at<uchar>(y+14,x)>25)
                {
//qDebug()<<"aaaa";
                   return false;

                }
            }
        }
    }
    while(1)
    {
        mid= src.at<uchar>(y,i);
        up = src.at<uchar>(y-1,i);
        down = src.at<uchar>(y+1,i);
        tmp=(mid<=up)?mid:up;
        val=(tmp>down)?down:tmp;
        if(isLeft)
        {

            i+=_step;
            if(i>src.cols)
                break;
            //circle( rgb, Point(i,y ), 3, Scalar(100,100,100), 1, CV_AA);

            if(i-x<8)
            { //排除误判

                int y1=0,y2=0;
                for(int n=0;n<20;n++)
                {

                    if(src.at<uchar>(y-n,i)-val>difWhiteToBlack/2)
                    {
                        break;
                    }
                    else
                    {
                        y1=y-n;
                    }

                }

                for(int n=0;n<20;n++)
                {
                    if(src.at<uchar>(y+n,i)-val>difWhiteToBlack/2)
                    {
                        break;
                    }
                    else
                    {
                        y2=y+n;
                    }

                }

                if(y2-y1>10)
                    return false;
            }

            if(i>src.cols-sideDistance)
            {
                if( src.at<uchar>(y+v1,i)-val>30&&src.at<uchar>(y+v2,i)-val>30&&src.at<uchar>(y-v1,i)-val>30&&src.at<uchar>(y-v2,i)-val>30)
                {
                    //qDebug()<<"aaa"<<y<<i<<src.at<uchar>(y+v1,i)<<val;
                    break;
                }
            }
        }
        else
        {
            i-=_step;
            if(i<0)
                break;

            if(i+x<8)
            { //排除误判

                int y1=0,y2=0;
                for(int n=0;n<20;n++)
                {

                    if(src.at<uchar>(y-n,i)-val>difWhiteToBlack/2)
                    {
                        break;
                    }
                    else
                    {
                        y1=y-n;
                    }

                }

                for(int n=0;n<20;n++)
                {
                    if(src.at<uchar>(y+n,i)-val>difWhiteToBlack/2)
                    {
                        break;
                    }
                    else
                    {
                        y2=y+n;
                    }

                }

                if(y2-y1>10)
                {

                    return false;
                }
            }


            if(i<sideDistance)
            {
                // qDebug()<<"i:"<<i<<"val "<<src.at<uchar>(y+3,i)<<src.at<uchar>(y-3,i)<<"gray"<<grayVal;
                if( src.at<uchar>(y+v1,i)-val>30&&src.at<uchar>(y+v2,i)-val>30&&src.at<uchar>(y-v1,i)-val>30&&src.at<uchar>(y-v2,i)-val>30)
                {
                    //qDebug()<<"aaa";//<<i<<y<<src.at<uchar>(y+v1,i)<<curGray;
                    break;
                }
            }

        }


        if(val==up){
            --y;
        }else if(val==down){
            ++y;
        }
        if(isLeft&&i>src.cols/4*3||!isLeft&&i<src.cols/4)
        {

            circle( rgb, Point(i,y ), 3, Scalar(0,255,0), 1, CV_AA);
        }
//        if(val-grayVal>difMidBright)
//        {
//            break;
//        }
//circle( rgb, Point(i,y ), 3, Scalar(255,255,0), 1, CV_AA);
        if(val2>0)
        {

            if(abs(val-val2)>difMidBright)
            {
                // circle( rgb, Point(i,y ), 3, Scalar(0,0,0), 1, CV_AA);
                break;
            }

        }

        val2=val;



    }

    if(i<sideDistance&&!isLeft) flag=true;

    if(i>src.cols-sideDistance&&isLeft) flag=true;

    return flag;

}

void project::find2(Mat &src,Mat &rgb,Point &p,bool isLeft)
{



    int left_x=640/4;
    int right_x=640/4*3;
    int up_x=480/4;
    int down_x=480/4*3;

    bool flag=false;

    int tmpBright;
    Point leftP;
    Point rightP;
    leftP.x=0;
    rightP.x=0;
    int curGray;//找到最左或最右当前灰度值

    int i;
    bool loop=true;
     if(isLeft)
         i=left_x;
     else
         i=right_x;


    while(loop)
    {


        if(isLeft)
        {
            if(i>src.cols/2+100)
                break;
            i+=_step;
        }
        else
        {
            if(i<src.cols/2-100)
                break;
            i-=_step;

        }





        for(int j=up_x;j<down_x;j++)
        {
            if(loop==false)
                break;

//qDebug()<<"aaa"<<j<<i;
            tmpBright=src.at<uchar>(j,i);

            if(tmpBright<_blackVal)
            {


                if(abs(src.at<uchar>(j+1,i)-tmpBright)<difMidBright+60)
                {


                    if(src.at<uchar>(j-groundLen1,i)+addUpGround-src.at<uchar>(j+1,i)>difWhiteToBlack&&src.at<uchar>(j-groundLen1,i)-tmpBright+addUpGround>difWhiteToBlack)
                    {

                        for(int k=1;k<midLineHeight+1;k++)
                        {

                            if(src.at<uchar>(j+k,i)-tmpBright>difWhiteToBlack&&src.at<uchar>(j+k+1,i)-tmpBright>difWhiteToBlack&&src.at<uchar>(j+k+2,i)-tmpBright>difWhiteToBlack)
                            {


                                if(isLeft)
                                {
                                    leftP.x=i;
                                    leftP.y=j;

                                }
                                else
                                {
                                    rightP.x=i;
                                    rightP.y=j;
// circle( rgb, Point(i,j ), 3, Scalar(0,255,255), 1, CV_AA);
                                }
                                flag=false;


                                if(isLeft&&leftP.x)
                                {

                                    curGray=src.at<uchar>(leftP.y,leftP.x);
                                    //qDebug()<<"left x"<<leftP.x;

                                    flag=isPos(src,rgb,curGray,_step,leftP.x,leftP.y-1,isLeft);

                                    if(flag)
                                    {
                                        //qDebug()<<"aaaaa";
                                        //circle( rgb, Point(i,j), 3, Scalar(0,0,0), 1, CV_AA);
                                        flag=isPos2(src,rgb,curGray,_step,leftP.x,leftP.y-1,isLeft);

                                    }

                                    if(flag)
                                    {
                                        qDebug()<<"找到黑线最左侧点"<<leftP.x<<leftP.y;
                                        p=leftP;
                                        circle( rgb, Point(leftP.x,leftP.y ), 3, Scalar(0,0,255), 1, CV_AA);
                                        loop=false;
                                    }
                                }

                                if(!isLeft&&rightP.x)
                                {


                                    curGray=src.at<uchar>(rightP.y,rightP.x);

                                    flag=isPos(src,rgb,curGray,_step,rightP.x,rightP.y-1,isLeft);
                                    if(flag)
                                    {

                                        flag=isPos2(src,rgb,curGray,_step,rightP.x,rightP.y-1,isLeft);
                                    }



                                    if(flag)
                                    {

                                         //qDebug()<<"aaaaa:"<<src.at<uchar>(j-groundLen1,i)<<src.at<uchar>(j+1,i)<<j<<i;
                                        qDebug()<<"找到黑线最右侧点"<<rightP.x<<rightP.y;
                                        p=rightP;
                                        circle( rgb, Point(rightP.x,rightP.y ), 3, Scalar(255,0,0), 1, CV_AA);
                                        loop=false;
                                    }
                                }



                                //circle( rgb, Point(i,j+k ), 3, Scalar(0,255,0), 1, CV_AA);

                                break;


                            }
                        }
                    }
                }
            }

        }
    }





}



bool closeState(Mat &src)
{
    int num=0;
    for(int i=src.rows/4;i<src.rows/4*3;i+=5)
    {
        for(int j=src.cols/4;j<src.cols/4*3;j+=5)
        {
        if(src.at<uchar>(i,j)>30)
            ++num;
        if(num>10) break;
        }
    }
    if(num>10)
        return false;
    else
        return true;
}

int minGray(Mat &src)
{
    int num=255;
    for(int i=0;i<src.rows;i++)
    {
        if(src.at<uchar>(i,src.cols/2)<num)
            num=src.at<uchar>(i,src.cols/2);
    }
    return num;
}
bool detectBlackObj(Mat &src,int x,int y,int preVal,int standardVal)
{
    int n=0;
    for(int i=y-1;i<y+2;i++)
    {
        for(int j=x-1;j<x+2;j++)
        {
            if(abs(src.at<uchar>(y,x)-preVal)>standardVal)
                ++n;
        }
    }
    if(n==9)
        return true;
    else
        return false;
}

bool openTheDoor(Mat &src,Mat &rgb,Point p1,Point p2)
{

    int flag=false;
    if(!findMidLine(src,rgb,_blackVal,_step,p1,p2))
    {
        qDebug()<<"mid is block ";
        return true;
    }
    int obj=0;
    int num=0;

    if(p1.x>=pArr[0].x)
    {

        for(int i=0;i<src.rows-100;i++)
        {
                if(pArr[i].x>p2.x)
                {
                    break;
                }

                if(pArr[i].x&&pArr[i].x>p1.x)
                {

                    //比较中线
//                    if(abs(src.at<uchar>(pArr[i].y,pArr[i].x)-arr1[i])>30)
//                        ++obj;


                    //比较上下地槛
                    if(pArr[i].x<p2.x-step1&&pArr[i].x>p1.x+step1)
                    {

                        //if(abs(src.at<uchar>(pArr[i].y+groundLen2,pArr[i].x)-arr3[i])>groundVar)
                        //qDebug()<<"aaaa"<<arr3[i]<<src.at<uchar>(pArr[i].y+groundLen2,pArr[i].x)<<"x y"<<pArr[i].x<<pArr[i].y+groundLen2;
                        if(detectBlackObj(src,pArr[i].x,pArr[i].y+groundLen2,arr3[i],groundVar))
                             ++obj;
                            //if(abs(src.at<uchar>(pArr[i].y+groundLen2,pArr[i].x)-arr3[i])>groundVar)
                               //++obj;

                        if(detectBlackObj(src,pArr[i].x,pArr[i].y-groundLen1,arr2[i],groundVar))
                             ++obj;
//
                       // qDebug()<<"obj"<<obj;
                        //circle( rgb, Point(pArr[i].x,pArr[i].y-groundLen1-2 ), 3, Scalar(rand()&255,rand()&255,rand()&255), 1, CV_AA);

                       // printf("i:%d up %d up2 %d  \n",i,abs(src.at<uchar>(pArr[i+1].y,pArr[i+1].x)-arr2[i+1]),arr2[i+1]);

                    }

                    //printf("obj %d  len %d \n",obj,doorCurLen);
                    if(obj>4)
                    {
                        //qDebug()<<"obj"<<obj;
                        flag= true;
                    }
                }


        }
    }


    //存入比对数据
    while(1)
    {
        if(p1.x+num*_step>p2.x)
            break;

        arr1[num]=src.at<uchar>(pArr[num].y,pArr[num].x);
        if(pArr[num].y-groundLen1>0&&pArr[num].y+groundLen2<src.rows)
        {
            arr2[num]=src.at<uchar>(pArr[num].y-groundLen1,pArr[num].x);
            //qDebug()<<"aaaa arr2"<<pArr[num].y-groundLen1<<"arr3"<<pArr[num].y+groundLen2<<"arr1"<<pArr[num].y;
            arr3[num]=src.at<uchar>(pArr[num].y+groundLen2,pArr[num].x);
            if(pArr[num].x>p1.x+step1&&pArr[num].x<p2.x-step1)
            {
                circle( rgb, Point(pArr[num].x,pArr[num].y -groundLen1), 1, Scalar(0,255,0), 1, CV_AA);
                 circle( rgb, Point(pArr[num].x,pArr[num].y +groundLen2), 1, Scalar(0,255,0), 1, CV_AA);
            }

        }
        ++num;

    }


    return flag;
}


bool detecting=true;
int frame=0;
int closeDoor=0;//N帧后关门
bool setState=false;//设置门状态
void project::edvia(Mat src,Mat rgb)
{
    qDebug()<<"<<src.cols<<src.rows"<<src.cols<<src.rows<<rgb.cols<<rgb.rows;
    Point p1(0,0);
    Point p2(0,0);
    Point p3(0,0);

    int tempX1,tempY1,tempX2,tempY2;
    int minGray1;
   static bool storeXY=true;

    if(1)
    {
        minGray1= minGray(src);
        qDebug()<<"minGray1"<<minGray1;
        qDebug()<<"closeState"<<closeState(src);


        find2(src,rgb,p1,1);


        find2(src,rgb,p2,0);




        tempX1=p1.x;
        tempY1=p1.y;
        tempX2=p2.x;
        tempY2=p2.y;
        if(1)
        {

            storeXY=false;
            //存入坐标数据
            double ratio=abs(tempY1-tempY2)*1.0/abs(tempX1-tempX2);
            int num=0;
            int t1=1;
            if(tempY1>tempY2)
                t1=-1;

            while(1)
            {
                if(tempX1+num*_step>p2.x)
                    break;



                pArr[num].x=tempX1+num*_step;
                pArr[num].y=tempY1+num*_step*ratio*t1;
               // qDebug()<<"bbbb"<<pArr[num].y<<ratio<<num*_step*ratio*t1;
                //printf("num:%d  x: %d\n",num,pArr[num].x);
                //circle( rgb, Point(pArr[num].x,pArr[num].y ), 3, Scalar(255,0,255), 1, CV_AA);
                ++num;


            }
        }



        if(p2.x-p1.x>110&&setState)
        {
            ++closeDoor;
            if(closeDoor>120)
            {
               // OperateIO(IORELAPY1, OFF);
                closeDoor=0;

            }
        }

        if(p1.x&&p2.x&&p2.x-p1.x<15)
        {
            detecting=false;
            qDebug()<<"detecting=false";
        }
    }


    if(1)
    {

        if(p1.x==0)
        {
            //OperateIO(IORELAPY1, ON);
            setState=true;
            closeDoor=0;
           qDebug()<<"left side is block";

        }

        if(p2.x==0)
        {
            //OperateIO(IORELAPY1, ON);
            setState=true;
            closeDoor=0;
           qDebug()<<"right side is block";

        }

        if(p1.x&&p2.x)
        {


            bool b= openTheDoor(src,rgb,p1,p2);


            if(b)
            {
                //OperateIO(IORELAPY1, ON);
                setState=true;
                closeDoor=0;
                qDebug()<<"obj is block";
            }
            //qDebug()<<"aaaa"<<pArr[0].x;

        }

    }


    imshow("rgb",rgb);

}


int avgGray(Mat &src)
{
    int step=2;
    int v1=0;
    int y=src.rows/3/step;
    int x=src.cols/step;
    for(int i=src.rows/3;i<src.rows/3*2;i+=step)
    {
        for(int j=0;j<src.cols;j+=step)
        {
            v1+=src.at<uchar>(i,j);
        }
    }
    qDebug()<<"v1"<<v1<<y<<x;
    return v1*1.0/(x*y);
}
void project::edviaGray(Mat &src)
{
    int step=3;
    int grayVal=avgGray(src);
    qDebug()<<"avgGray"<<grayVal;
}


//bool find9P(Mat &src,int x,int y)
//{
//    bool f=true;
//    if(x+3>src.cols||y+3>src.rows)
//        return false;
//    for(int i=0;i<3;i++)
//    {


//        if(!src.at<uchar>(y+i,x)||!src.at<uchar>(y+i,x+1))//||!src.at<uchar>(y+i,x+2))
//        {
//            f=false;
//            break;
//        }


//    }
//    return f;
//}


bool find9P(Mat &src,int x,int y)
{
    bool f=true;
    if(x+2>src.cols||y+2>src.rows)
        return false;
    for(int i=0;i<2;i++)
    {


        if(!src.at<uchar>(y+i,x)||!src.at<uchar>(y+i,x+1))//||!src.at<uchar>(y+i,x+2))
        {
            f=false;
            break;
        }


    }
    return f;
}

//int project::cmpPicPixel2(Mat &src1, Mat &src2,Mat &rgb, Rect &roi,Point setLeft,Point setRight, int val,int &doorWidth)
//{


//    Point cen((setLeft.x+setRight.x)/2,(setLeft.y+setRight.y)/2);
//    Mat m(src1.rows,src1.cols,CV_8U,Scalar(0,0,0));
//    int y2=setLeft.y>setRight.y?setLeft.y:setRight.y;
//    for(int j=roi.x;j<roi.x+roi.width;j++)
//    {

//        for(int k=roi.y;k<roi.y+roi.height;k++)
//        {

//            if(abs(src1.at<uchar>(k,j)-src2.at<uchar>(k,j))>val&&abs(src1.at<uchar>(k-1,j)-src2.at<uchar>(k-1,j))>val)//&&abs(src1.at<uchar>(k+1,j)-src2.at<uchar>(k+1,j))>val)
//                m.at<uchar>(k,j)=255;
//        }
//    }


//    Point pLeft(0,0),pRight(0,0);

//    //to left
//    for(int j=cen.x;j>roi.x;j--)
//    {
//        for(int k=roi.y;k<roi.y+roi.height;k++)
//        {
//            if(k-y2<11&&m.at<uchar>(k,j)==255)
//            {
//                if(find9P(m,j,k))
//                {
//                    pLeft.x=j;
//                    pLeft.y=k;


//                    goto FoundLeft;
//                }
//                else
//                {
//                    continue;
//                }
//            }
//        }
//    }
//FoundLeft:


//    //to right
//    for(int j=cen.x;j<roi.x+roi.width;j+=3)
//    {
//        for(int k=roi.y;k<roi.y+roi.height;k++)
//        {
//            if(m.at<uchar>(k,j)==255)
//            {
//                if(k-y2<11&&find9P(m,j,k))
//                {
//                    pRight.x=j;
//                    pRight.y=k;

//                    goto FoundRight;

//                }
//                else
//                {
//                    continue;
//                }
//            }
//        }
//    }
//FoundRight:
//    //circle(m,pLeft,3,Scalar(255,255,255),3);
//    //circle(m,pRight,3,Scalar(255,255,255),3);
//    //roi.height=(setLeft.y>setRight.y?setLeft.y:setRight.y)-roi.y+10;
//    //rectangle(m,roi,Scalar(255,255,255));
//        imshow("thresh",m);
//        qDebug()<<"left right"<<pLeft.x<<pRight.x;
//        doorWidth=0;
//        int dif=0;
//    if(pLeft.x&&pRight.x)
//    {
//        qDebug()<<"found left right";
//        circle(rgb,pLeft,5,Scalar(0,0,255));
//        circle(rgb,pRight,5,Scalar(0,0,255));
//        circle(rgb,cen,5,Scalar(0,255,0));
//        dif=abs(common::getLineLen(pLeft,cen)-common::getLineLen(pRight,cen));
//        doorWidth=common::getLineLen(pLeft,pRight);

//    }
//    return dif;

//}


bool shaking(Mat &src,Point cen)
{
    int y,n1=0;
    for(int i=cen.y-20;i<cen.y+20;i++)
    {
        if(src.at<uchar>(i,cen.x)==255)
        {
            y=i;
            n1=0;
            for(int j1=cen.x;j1>cen.x-25;j1--)
            {

                if(!src.at<uchar>(y,j1))
                {
                    if(src.at<uchar>(y-1,j1)==255)
                        --y;
                    else if(src.at<uchar>(y+1,j1)==255)
                        ++y;
                    else
                        break;
                }
                ++n1;

            }

            y=i;

            for(int j1=cen.x;j1<cen.x+25;j1++)
            {
                if(!src.at<uchar>(y,j1))
                {

                    if(src.at<uchar>(y-1,j1)==255)
                        --y;
                    else if(src.at<uchar>(y+1,j1)==255)
                        ++y;
                    else
                        break;
                }
                ++n1;

            }
            //circle(src,Point(cen.x,i),5,Scalar(255,255,255));
            //qDebug()<<"ccc"<<n1;
            if(n1>48)
                return true;

        }
    }

    return false;
}

int project::cmpPicPixel2(Mat &src1, Mat &src2,Mat &rgb,Point setLeft,Point setRight, int val,int &doorWidth,Point &getLeft,Point &getRight)
{


    Point cen((setLeft.x+setRight.x)/2,(setLeft.y+setRight.y)/2);
    Mat m(src1.rows,src1.cols,CV_8U,Scalar(0,0,0));

    double ratio=(setRight.y-setLeft.y)*1.0/(setRight.x-setLeft.x);
    int startY=setLeft.y-ratio*(-40)-5;


    int var_y=20;

    if(setLeft.x-40<0||setRight.x+40>640||startY<0||startY>400)
        return 0;

    for(int j=setLeft.x-40;j<setRight.x+40;j++)
    {
        if(j%8==0)
        {
            circle(rgb,Point(j,startY+(j-setLeft.x)*ratio),1,Scalar(255,255,255));

        }
        for(int k=startY+(j-setLeft.x)*ratio;k<30+startY+(j-setLeft.x)*ratio;k++)
        {

            if(abs(src1.at<uchar>(k,j)-src2.at<uchar>(k,j))>val&&abs(src1.at<uchar>(k-1,j)-src2.at<uchar>(k-1,j))>val)//&&abs(src1.at<uchar>(k+1,j)-src2.at<uchar>(k+1,j))>val)
                m.at<uchar>(k,j)=255;
        }
    }

    if(shaking(m,Point((setRight.x+setLeft.x)/2,(setRight.y+setLeft.y)/2)))
    {
        qDebug()<<"shaking true";
        imshow("thresh",m);
        doorWidth=-1;
        return 0;
    }


    Point pLeft(0,0),pRight(0,0);

    //to left
    for(int j=cen.x;j>setLeft.x-20;j--)
    {

        if(j%8==0)
        {
            circle(rgb,Point(j,var_y+cen.y+(j-cen.x)*ratio-5),1,Scalar(255,255,255));

        }
        for(int k=cen.y+(j-cen.x)*ratio-5;k<var_y+cen.y+(j-cen.x)*ratio-5;k++)
        {
            if(m.at<uchar>(k,j)==255)
            {
                if(find9P(m,j,k))
                {
                    pLeft.x=j;
                    pLeft.y=k;

                    goto FoundLeft;
                }

                else
                {
                    continue;
                }
            }
        }
    }
FoundLeft:


    //to right
    for(int j=cen.x;j<setRight.x+20;j++)
    {

        if(j%8==0)
        {
            circle(rgb,Point(j,var_y+cen.y+(j-cen.x)*ratio-5),1,Scalar(255,255,255));

        }
        for(int k=cen.y+(j-cen.x)*ratio-5;k<var_y+cen.y+(j-cen.x)*ratio-5;k++)
        {
            if(m.at<uchar>(k,j)==255)
            {
                qDebug()<<"abc"<<j<<k<<find9P(m,j,k);
                if(find9P(m,j,k))
                {
                    pRight.x=j;
                    pRight.y=k;

                    goto FoundRight;

                }
                else
                {
                    continue;
                }
            }
        }
    }
FoundRight:
    //circle(m,pLeft,3,Scalar(255,255,255),3);
    //circle(m,pRight,3,Scalar(255,255,255),3);
    //roi.height=(setLeft.y>setRight.y?setLeft.y:setRight.y)-roi.y+10;
    //rectangle(m,roi,Scalar(255,255,255));

        qDebug()<<"left"<<pLeft.x<<pLeft.y<<" right"<<pRight.x<<pRight.y;
        doorWidth=0;
        int dif=0;
    if(pLeft.x&&pRight.x)
    {
        getLeft=pLeft;
        getRight=pRight;
        //qDebug()<<"found left right";

        dif=abs(common::getLineLen(pLeft,cen)-common::getLineLen(pRight,cen));

        if(dif>18)
        {

            if(common::getLineLen(pLeft,cen)<common::getLineLen(pRight,cen))
            {

                if(m.at<uchar>(pLeft.y,pLeft.x-2)==0||m.at<uchar>(pLeft.y+2,pLeft.x-2)==0||m.at<uchar>(pLeft.y+3,pLeft.x-2)==0)
                {
                    qDebug()<<"repaire left";
                    circle(rgb,getLeft,5,Scalar(255,255,255));

                    getLeft.x=cen.x-common::getLineLen(pRight,cen);
                    getLeft.y=cen.y+cen.y-pRight.y;


                    circle(rgb,getLeft,7,Scalar(0,0,255));

                    dif=0;

                }
            }
            else
            {
                if(m.at<uchar>(pRight.y,pRight.x+4)==0||m.at<uchar>(pRight.y+2,pRight.x+4)==0||m.at<uchar>(pRight.y+3,pRight.x+4)==0)
                {
                    qDebug()<<"repaire right";
                    circle(rgb,getRight,5,Scalar(255,255,255));

                    getRight.x=cen.x+common::getLineLen(pLeft,cen);
                    getRight.y=cen.y+cen.y-pLeft.y;


                    circle(rgb,getRight,7,Scalar(0,0,255));


                    dif=0;
                }
            }
        }

        doorWidth=common::getLineLen(getLeft,getRight);


    }

    circle(rgb,getLeft,5,Scalar(0,0,255));
    circle(rgb,getRight,5,Scalar(0,0,255));
    circle(rgb,cen,5,Scalar(0,255,0));
    //circle(m,getRight,3,Scalar(0,0,0),3);
    qDebug()<<"aaa"<<common::getLineLen(pLeft,cen)<<common::getLineLen(pRight,cen);
    qDebug()<<"lenIsNormal:"<<lenIsNormal(m,getLeft,getRight);
    imshow("thresh",m);
    return dif;

}
bool project::lenIsNormal(Mat &src,Point left,Point right)
{
    if(src.at<uchar>(left.y+3,left.x)==255||src.at<uchar>(left.y+3,left.x-1)==255||src.at<uchar>(left.y+3,left.x-2)==255)
    {
        return true;
    }
    if(src.at<uchar>(right.y+3,right.x)==255||src.at<uchar>(right.y+3,right.x+1)==255||src.at<uchar>(right.y+3,right.x+2)==255)
    {
        return true;
    }
    return false;
}

bool project::findBlackLine(Mat &src,Mat &rgb,int grayVal,int _step,Point p1,Point p2)
{

    int val,val1,val2,val3,val4,val5,val10,y;
    //double ratio=(p2.y-p1.y)*1.0/(p2.x-p1.x);
    Point cen((p2.x+p1.x)/2,(p2.y+p1.y)/2);


 //circle( rgb,cen, 8, Scalar(255,255,255), 1, CV_AA);

    int leftNum=0,rightNum=0;

    for(int y1=cen.y-6;y1<cen.y+7;y1+=2)
    {

        val10=-1;
        y=y1;
        for(int i=cen.x;i<610;i+=_step)
        {

            val1=src.at<uchar>(y,i);
            val2=src.at<uchar>(y-1,i);
            val3=src.at<uchar>(y+1,i);
            val4=src.at<uchar>(y-2,i);
            val5=src.at<uchar>(y+2,i);
            val=val1<val2?val1:val2;
            val=val<val3?val:val3;
            val=val<val4?val:val4;
            val=val<val5?val:val5;

            if(val==val2||val==val4)
                y--;

            if(val==val3||val==val5)
                y++;

            if(val10>0)
            {
                if(abs(val-val10)>25)
                {
                    // circle( rgb, Point(i,y ), 4, Scalar(255,255,255), 1, CV_AA);
                    ++leftNum;
                    break;
                }
            }
            val10=val;
        }

    }

    if(leftNum>=7)
        return false;



    for(int y1=cen.y-6;y1<cen.y+7;y1+=2)
    {
        val10=-1;
        y=y1;
        for(int i=cen.x;i>30;i-=_step)
        {
            val1=src.at<uchar>(y,i);
            val2=src.at<uchar>(y-1,i);
            val3=src.at<uchar>(y+1,i);
            val4=src.at<uchar>(y-2,i);
            val5=src.at<uchar>(y+2,i);
            val=val1<val2?val1:val2;
            val=val<val3?val:val3;
            val=val<val4?val:val4;
            val=val<val5?val:val5;
            if(val==val2||val==val4)
                y--;

            if(val==val3||val==val5)
                y++;

            //circle( rgb, Point(i,y ), 3, Scalar(255,255,255), 1, CV_AA);

            if(val10>0)
            {
                if(abs(val-val10)>25)
                {
                    circle( rgb, Point(i,y ), 4, Scalar(255,255,255), 1, CV_AA);
                    ++rightNum;
                    break;
                }
            }
            val10=val;
        }


    }
    if(rightNum>=7)
        return false;

    return true;
}

int project::staticBlackObj(Mat &src,Mat &rgb, int grayVal, Point p1, Point p2,int maxNum)
{
    double ratio=(p2.y-p1.y)*1.0/(p2.x-p1.x);
    int num=0,n1,n2,a1,a2;
    Point pLeft,pRight;
    for(int i=p1.x-30;i>50;i-=5)
    {
        n1=0;
        n2=0;
        a1=0;
        a2=0;
//circle(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),4,Scalar(90,90,200));
        for(int j=(i-p1.x)*ratio+p1.y-10;j<(i-p1.x)*ratio+p1.y+60;j++)
        {
            if(src.at<uchar>(j,i)<grayVal)
                ++n1;
            else
            {

                n1=a1>9?-100:0;
            }
            if(n1>9&&a1<n1)
                a1=n1;

            if(src.at<uchar>(j,i-10)<grayVal)
                ++n2;
            else
            {
                n2=a2>9?-100:0;
            }
            if(n2>9&&a2<n2)
            {
                a2=n2;
                pLeft.x=i;
                pLeft.y=j;
            }
        }
        //qDebug()<<"a1 a2"<<a1<<a2;
        if(a2+10<a1)
        {

            if(src.at<uchar>(pLeft.y+10,pLeft.x-10)-src.at<uchar>(pLeft.y+10,pLeft.x)>10&&src.at<uchar>(pLeft.y+9,pLeft.x-10)-src.at<uchar>(pLeft.y+9,pLeft.x)>10)
            {
                ++num;
                line(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),Point(i,(i-p2.x)*ratio+p2.y+50),Scalar(0,255,0),1);
            }
            else
            {
                qDebug()<<"staticBalck left"<<src.at<uchar>(pLeft.y+10,pLeft.x-10)<<src.at<uchar>(pLeft.y+10,pLeft.x);
                //line(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),Point(i,(i-p2.x)*ratio+p2.y+50),Scalar(0,255,0),1);
            }

        }
        if(num>maxNum)
            return 1;
    }

    num=0;
    for(int i=p2.x+30;i<590;i+=5)
    {
        n1=0;
        n2=0;
        a1=0;
        a2=0;
        //circle(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),4,Scalar(90,90,200));
        for(int j=(i-p2.x)*ratio+p2.y-10;j<(i-p2.x)*ratio+p2.y+60;j++)
        {
            if(src.at<uchar>(j,i)<grayVal)
            {

                ++n1;
            }
            else
                n1=a1>9?-100:0;
            if(n1>9&&a1<n1)
                a1=n1;

            if(src.at<uchar>(j,i+10)<grayVal)
                ++n2;
            else
                n2=a2>9?-100:0;
            if(n2>9&&a2<n2)
            {
                a2=n2;
                pRight.x=i;
                pRight.y=j;

            }

            //qDebug()<<"bbbb"<<i<<j;
        }
        //qDebug()<<"bbbb"<<a1<<a2<<i;
        if(a2+10<a1)
        {
            //line(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),Point(i,(i-p2.x)*ratio+p2.y+50),Scalar(0,255,0),1);
            //++num;

            if(src.at<uchar>(pRight.y+10,pRight.x+10)-src.at<uchar>(pRight.y+10,pRight.x)>10&&src.at<uchar>(pRight.y+9,pRight.x+10)-src.at<uchar>(pRight.y+9,pRight.x)>10)
            {
                ++num;
                line(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),Point(i,(i-p2.x)*ratio+p2.y+50),Scalar(0,255,0),1);
            }
            else
            {
                qDebug()<<"staticBlack right"<<src.at<uchar>(pRight.y+10,pRight.x+10)<<src.at<uchar>(pRight.y+10,pRight.x);
                circle(rgb,Point(pRight.x+10,pRight.y+10),3,Scalar(80,80,80) );
                circle(rgb,Point(pRight.x,pRight.y+10),3,Scalar(250,250,250) );
                //line(rgb,Point(i,(i-p2.x)*ratio+p2.y-10),Point(i,(i-p2.x)*ratio+p2.y+50),Scalar(0,255,0),1);
            }
        }
        if(num>maxNum)
            return 2;
    }



    return 0;

}

