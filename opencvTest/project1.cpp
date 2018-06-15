 #include "project1.h"
IplImage *gray640,*thresh640;
extern void topBlack(Mat &src,bool black);
project1::project1(QObject *parent) :
    QObject(parent)
{
    gray640=cvCreateImage(cvSize(640,480),8,1);
    thresh640=cvCreateImage(cvSize(640,480),8,1);
}


Point calcFont2(Mat &m,Point p,Point p2,int len)
{
    //偏移量的定义
    int offset = -10;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m, m, CV_MOP_OPEN, element);
    else
        morphologyEx(m, m, CV_MOP_CLOSE, element);

    int val=0,val2=0;

    int t1=m.at<uchar>(p.y,p.x);
    int t2=m.at<uchar>(p.y-len,p.x);
    int t3=m.at<uchar>(p.y+len,p.x);
    int t4=m.at<uchar>(p2.y,p2.x);
    int t5=m.at<uchar>(p2.y-len,p2.x);
    int t6=m.at<uchar>(p2.y+len,p2.x);
    val=t2+t3-2*t1;
    val2=t5+t6-2*t4;

    if(DEBUG)
    {
        m.at<uchar>(p.y,p.x)=0;
        m.at<uchar>(p.y-len,p.x)=0;
        m.at<uchar>(p.y+len,p.x)=0;
        m.at<uchar>(p2.y,p2.x)=0;
        m.at<uchar>(p2.y-len,p2.x)=0;
        m.at<uchar>(p2.y+len,p2.x)=0;
        m.at<uchar>((p.y+p2.y)/2,(p.x+p2.x)/2)=255;
        qDebug()<<"calcFont 6个点灰度值"<<t2<<t1<<t3<<t5<<t4<<t6;
        qDebug()<<"val1 val2"<<val<<val2;
        imshow("aadfs",m);
    }

    return val>val2?p:p2;
}

typedef struct{
     bool calcCen;
     int x;
     int y;
     int size;
     double moveX;
     double moveY;
     std::vector<Point> p1;

 }xy_size;


void sortCircle(xy_size circleList[],int num,int,int cenY)
{
    if(num<2)
        return;
    xy_size temp;
    for(int i=0;i<num;i++)
    {
        for(int j=1;j<num-i;j++)
        {
            if(circleList[j-1].x>circleList[j].x)
            {
                temp=circleList[j-1];
                circleList[j-1]=circleList[j];
                circleList[j]=temp;

            }
        }
    }

    int k=0,k2=0;
    xy_size t1[50],t2[50];

    for(int i=0;i<num;i++)
    {
        if(circleList[i].y<=cenY)
        {
            t1[k++]=circleList[i];

        }
    }
    for(int i=num-1;i>=0;i--)
    {
        if(circleList[i].y>cenY)
        {
            t2[k2++]=circleList[i];

        }
    }
    for(int i=0;i<num;i++)
    {
        if(i<k)
            circleList[i]=t1[i];
        else
            circleList[i]=t2[i-k];
    }



}



void detectOther2(xy_size circleList[],int num,int &num1,int len1,int dVal)
{
    xy_size t1[50];
    int len[50];

    int n=0,index=0;
    for(int i=1;i<num;i++)
    {
        Point p1(circleList[i-1].x,circleList[i-1].y);
        Point p2(circleList[i].x,circleList[i].y);
        len[i-1]= common::getLineLen(p1,p2);
    }
//找到正常的孔index
    for(int i=1;i<num;i++)
    {
        if(abs(len[i-1]-len1)<dVal&&abs(len[i]-len1)<dVal)
        {
            n=i;
            break;
        }
    }

//重新排序
    for(int i=0;i<num;i++)
    {

        if(n+i>=num)
        {
            t1[i]=circleList[n+i-num];

        }
        else
        {
            t1[i]=circleList[n+i];
        }
    }

    circleList[index]=t1[index];
    for(int i=1;i<num;i++)
    {
        Point p1(t1[i].x,t1[i].y);
        Point p2(circleList[index].x,circleList[index].y);

        if(abs(common::getLineLen(p1,p2)-len1)>dVal)//排除污点
            continue;

         circleList[++index]=t1[i];

    }
    num1=index+1;


}


int getAvgBright(Mat &m,Point p,int len)
{
    int b1=0;
    //qDebug()<<"pos1"<<p.x<<p.y;
    for(int y1=p.y-len;y1<p.y+len;y1++)
    {
        if(y1<0||y1>m.rows-1)
        {
            continue;
        }
        for(int x1=p.x-len;x1<p.x+len;x1++)
        {
            if(x1<0||x1>m.cols-1)
            {
                continue;
            }
           b1+= m.at<uchar>(y1,x1);
        }
    }
    return b1/len/len/4;
}

bool zt_2ndFeature(vector<Point> &contours,Rect &roi,Point cen,int step,int step2,int len1,int objAngle,int offset,int curToCen)
{
    int index1,index2;
    int x1=cen.x;
    int y1=cen.y;

    for(int j2=0;j2<contours.size()-step2;j2++)
    {



        //qDebug()<<"contours point x y"<<contours[k][j2].x+roi.x<<contours[k][j2].y+roi.y<<"front";
        index2=j2+step2;
        if(index2>=contours.size())
            index2%=contours.size();

        if(objAngle==90)
        {
            if(contours[j2].y-y1>curToCen)
            {
                if(contours[index2].x-contours[j2].x<-offset)
                {

                    if(DEBUG) circle( g_dstImage2, Point(contours[j2].x+roi.x, contours[j2].y+roi.y), 1, Scalar(0,0,255), 3, CV_AA);
                    //qDebug()<<"calcObjFeature 2"<<contours[k].size()<<"x y x "<<contours[k][j2].x+roi.x<<contours[k][j2].y+roi.y<<contours[k][index2].y-y1;
                    return false;
                }
            }
            if(contours[j2].y-y1<-curToCen)
            {
                if(contours[index2].x-contours[j2].x>offset)
                {

                    if(DEBUG) circle( g_dstImage2, Point(contours[j2].x+roi.x, contours[j2].y+roi.y), 1, Scalar(0,0,255), 3, CV_AA);
                    //qDebug()<<"calcObjFeature 2"<<contours[k].size()<<"x y x "<<contours[k][j2].x+roi.x<<contours[k][j2].y+roi.y<<contours[k][j2].y-y1;
                    return false;
                }
            }
            for(int j3=0;j3<40&&abs(contours[j2].y-y1)>curToCen;j3++)
            {

                index1=j3+j2+step;
                if(index1>=contours.size())
                    index1%=contours.size();


                if(abs(contours[j2].x-contours[index1].x)<offset&&abs(contours[j2].y-contours[index1].y)<offset)
                {
                    if(DEBUG)
                    {
                        circle( g_dstImage2, Point(contours[j2].x+roi.x, contours[j2].y+roi.y), 1, Scalar(0,0,255), 3, CV_AA);
                    }
                        return false;
                }
            }

        }
        else
        {

            if(contours[j2].x-x1>curToCen)
            {
                if(contours[index2].y-contours[j2].y>offset)
                {

                    if(DEBUG)
                    {
                        qDebug()<<"aa";
                        circle( g_dstImage2, Point(contours[j2].x+roi.x, contours[j2].y+roi.y), 1, Scalar(0,255,0), 3, CV_AA);
                    }

                    return false;
                }
            }

            if(contours[j2].x-x1<-curToCen)
            {

                if(contours[index2].y-contours[j2].y<-offset)
                {

                    if(DEBUG)
                    {
                        qDebug()<<"bb";
                        circle( g_dstImage2, Point(contours[j2].x+roi.x, contours[j2].y+roi.y), 1, Scalar(0,255,0), 3, CV_AA);

                    }
                    return false;
                }
            }

            for(int j3=0;j3<40&&abs(contours[j2].x-x1)>curToCen;j3++)
            {

                index1=j3+j2+step;
                if(index1>=contours.size())
                    index1%=contours.size();


                if(abs(contours[j2].x-contours[index1].x)<offset&&abs(contours[j2].y-contours[index1].y)<offset)
                {
                    if(DEBUG)
                    {
                        qDebug()<<"cc";
                        circle( g_dstImage2, Point(contours[j2].x+roi.x, contours[j2].y+roi.y), 1, Scalar(0,0,255), 3, CV_AA);
                    }
                        return false;
                }
            }

        }

    }

    return true;

}

void project1::zt_2ndLine(Mat &m3, Rect roi, params &param)
{

    int cenX=param.var1[0]-roi.x;//中心
    int cenY=param.var1[1]-roi.y;
    int minDistance=param.var1[2];//中心与字距离
    int maxDistance=param.var1[3];//中心与字距离
    unsigned int minSize=param.var1[4];//obj size
    unsigned int maxSize=param.var1[5];
    int thresh=param.var1[6];//二值
    int rectLen=param.var1[7];//矩形长
    int rectLen2=param.var1[8];

    double angleOffset=param.var2[0];//角度偏差

    int calcShapeFlag=param.var1[29];

    param.var2[19]=360;
    Mat m1,m2,m5,m6;
    Mat m4=m3(roi).clone();

    int x1,y1,distance1,len1,len2;

    int step=15,step2=8;
    double d;



    medianBlur(m4, m5, 9);

    int i;
    bool loop=true;

    for(i=thresh;i>30;i-=5)
    //for(i=104;i>103;i-=5)
    {

        if(!loop)
            break;
        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);
        threshold(m5,m6,i,255,THRESH_BINARY);



        std::vector<std::vector<Point>> contours;

        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        // Print contours' length


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];


        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            if(contours[k].size()<minSize||contours[k].size()>maxSize)
                continue;


            End_Rage2D1 = minAreaRect(Mat(contours[k]));

            cvBoxPoints(End_Rage2D1,rectpoint);

            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));
            //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),3,Scalar(255,255,255),3);

            if(distance1<minDistance||distance1>maxDistance)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<len2)//取最大长度
                len1=len2;
            qDebug()<<"bbbb"<<len1<<rectLen<<"size"<<contours[k].size();
            if(len1<rectLen||len1>rectLen2)
                continue;

            if(m6.at<uchar>(contours[k][0].y-1,contours[k][0].x))
            {
                continue;
            }

qDebug()<<"ccc";
            if(!zt_2ndFeature(contours[k],roi,Point(x1,y1),step,step2,rectLen,90,5,30)&&calcShapeFlag!=82000)
                continue;


            if(DEBUG)
                qDebug()<<"x1 y1"<<x1+roi.x<<y1+roi.y<<"len"<<len1<<"size"<<contours[k].size()<<"i:"<<i;

            double angle=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
            //角度偏移
            if(angleOffset+angle>180)
                angle=angleOffset+angle-360;
            else if(angleOffset+angle<-180)
                angle=angleOffset+angle+360;
            else
                angle+=angleOffset;
            //end

            param.var2[19]=angle;

            if(DEBUG)
            {
                imshow("thresh1",m6);

            }
            loop=false;
            break;


        }

    }
}

void project1::zt_lineA2(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;
    int cenY=param.var1[1]-roi.y;
    int distance=param.var1[2];
    unsigned int minSize=param.var1[3];
    unsigned int maxSize=param.var1[4];
    int thresh=param.var1[5];
    int square=param.var1[6];
    int rectLen=param.var1[7];
    int rectLen2=param.var1[8];
    int disDif=param.var1[9];//圆心与字距离最大偏差
    double angleOffset=param.var2[0];

    param.var1[29]=0;
    Mat m1,m2,m5,m6;
    Mat m4=src(roi).clone();

    int x1,y1,distance1,len1,len2;

    int step=15,step2=8;

    medianBlur(m4, m5, 3);

    int i;
    bool loop=true;
    for(i=thresh;i>69;i-=10)
    {

        if(!loop)
            break;
        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);
        threshold(m5,m6,i,255,THRESH_BINARY);



        std::vector<std::vector<Point>> contours;

        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        // Print contours' length


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];


        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            if(contours[k].size()<minSize||contours[k].size()>maxSize)
                continue;


            End_Rage2D1 = minAreaRect(Mat(contours[k]));

            cvBoxPoints(End_Rage2D1,rectpoint);

            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            //distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));
            //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),3,Scalar(255,255,255),3);

            //if(common::getLineLen(Point(x1,y1),Point(cenX,cenY))>distance)
            if(abs(y1-cenY)>distance)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<len2)//取最大长度
                len1=len2;

            if(len1<rectLen||len1>rectLen2)
                continue;


            if(m6.at<uchar>(contours[k][0].y-1,contours[k][0].x))
            {
                continue;
            }


            if(!zt_2ndFeature(contours[k],roi,Point(x1,y1),step,step2,rectLen,0,8,65))
                continue;


            if(DEBUG)
                qDebug()<<"x1 y1"<<x1+roi.x<<y1+roi.y<<"len"<<len1<<"size"<<contours[k].size()<<"i:"<<i;




            if(DEBUG)
            {
                imshow("thresh1",m6);

                common::drawRect(g_dstImage2,Point(roi.x,roi.y),distance,90,0,Scalar(255,0,0));
            }
            loop=false;
            param.var1[29]=1;
            break;


        }

    }
}



Point QC_Arr[100];
int QC_Index[100];
int QC_WhiteRatio[100];
int QC_Shadow[100];
Point QC_Noise[500];
int calcGray_QC(Mat &src,std::vector<Point> &contours)
{

    int v1,v2,v3,v4,v5;
    int m_gray=0;
    for(int i=0;i<contours.size();i++)
    {
       // qDebug()<<"*****"<<src.at<uchar>(contours[i].y,contours[i].x);
       // m_gray+=src.at<uchar>(contours[i].y,contours[i].x);
v1=src.at<uchar>(contours[i].y,contours[i].x);
v2=src.at<uchar>(contours[i].y+2,contours[i].x);
v3=src.at<uchar>(contours[i].y-2,contours[i].x);
v4=v1>v2?v1:v2;
v5=v4>v3?v4:v3;
m_gray+=v5;
//        m_gray=src.at<uchar>(contours[i].y+2,contours[i].x);
//        if(m_gray>gray)
//            gray=m_gray;
//        m_gray=src.at<uchar>(contours[i].y-2,contours[i].x);
//        if(m_gray>gray)
//            gray=m_gray;
    }

    //qDebug()<<"avg"<<m_gray/contours.size();
    return m_gray/contours.size();
}

bool calcFont_QC(int num,Point arr[])
{
    bool flag=false;

    for(int i=0;i<num;i++)
    {
        for(int j=i+1;j<num;j++)
        {
            if(abs(arr[i].x-arr[j].x)<15&&abs(arr[i].y-arr[j].y)<38)
            {
                flag= true;
            }

        }
    }
    return flag;

}

int grayValDif(Mat &src,std::vector<Point> &contours)
{
    int min1=255,grayVal,max1=0,grayVal2;

    if(contours.size()<10)
        return 0;

    for(int i=0;i<contours.size();i++)
    {
        grayVal=src.at<uchar>(contours[i].y-2,contours[i].x);
        grayVal2=src.at<uchar>(contours[i].y+2,contours[i].x);

        if(abs(grayVal-grayVal2)>max1)
            max1=abs(grayVal-grayVal2);
//        if(grayVal<min1)
//            min1=grayVal;

//        if(grayVal>max1)
//        {
//             max1=grayVal;

//        }
    }

    //return min1;
    return max1;
}

int contourGrayDif(Mat &src,std::vector<Point> &contours,int &avgBlackOut,int offX,int offY)
{
    if(contours.size()==0)
        return 0;
    unsigned int avgBlack=255,avgBright=0,tmp,avgBlack2=255,avgBright2=0;


    for(int i=0;i<contours.size();i++)
    {
       tmp= src.at<uchar>(contours[i].y+offY,contours[i].x+offX);
       if(tmp>avgBright) avgBright=tmp;
       if(tmp<avgBlack) avgBlack=tmp;
    }
    for(int i=0;i<contours.size();i++)
    {
       tmp= src.at<uchar>(contours[i].y+offY*2,contours[i].x+offX);
       if(tmp>avgBright2) avgBright2=tmp;
       if(tmp<avgBlack2) avgBlack2=tmp;
    }
    avgBlackOut=avgBlack<avgBlack2?avgBlack:avgBlack2;
    return avgBright-avgBlack;

}

double whiteRatio(Mat &src,Point &cen,int gray,int grayDif)
{
    int step=30;
    int step1=15;
    int ret=100,temp=0;
    int curVal;

    if(cen.x-step<0||src.cols<cen.x+step)
        return ret;
    if(cen.y-step1<0||src.rows<cen.y+step1)
        return ret;
    for(int i=cen.y-step;i<cen.y+step;i++)
    {
        for(int j=cen.x-step1;j<cen.x+step1;j++)
        {

            curVal=src.at<uchar>(i,j);
            if(curVal+grayDif>gray)
                ++temp;


        }
    }

    return temp*1.0/((step*2)*(step1*2))*100;
}


void project1::zt_3rdLine(Mat &src,Rect roi,params& param,bool blackHat)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测NI最小值
    unsigned int objMaxSize=param.var1[3];//检测NI最大值
    int distance=param.var1[4];//字体到中心点距离
    int thresh=param.var1[5];//二值
    double angleOffset=param.var2[0];//角度偏移




    double angle=360;//工件角度
    int px=0;//字体坐标
    int py=0;
    int size1;
    int fontGrayVal,fontGrayDif;
    int angle1,angle2,x1,y1,len1,len2,distance1,min1=255,max1=0;
    bool loop=true;
    double area;

    Mat m5;
    m5=src(roi).clone();

    medianBlur(m5, m5,3);
    Mat m6=m5.clone();
    Mat m7=m5.clone();

    topBlack(m5,blackHat);


    Mat m2;


    Mat m1;
    int a1,a2,a3,b1,c1,v1,v2,smoothNum,a4,shadowVal;
    int errorFlag=0;
    if(blackHat)
        thresh=40;

    std::vector<std::vector<Point>> contours;
    for(int i=thresh;i>9;)
    //for(int i=117;i>114;)
    {
if(DEBUG) qDebug()<<"errorFlag"<<errorFlag<<"i:"<<i;

        errorFlag=0;
        if(i>25)
        {

            i-=2;
        }
        else
            i--;



        a1=0;
        a2=0;
        a3=0;
        b1=0;
        c1=0;
        max1=0;
        min1=255;
        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);
        threshold(m5,m2,i,255,THRESH_BINARY);



        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        //qDebug()<<"contours size"<<contours.size();
        //for ( ; itContours!=contours.end(); ++itContours)

        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            if(a1>=99||c1>499)
                break;

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

//            if(contours[k].size()==52||contours[k].size()==59)
//            {
//                qDebug()<<"x1 y1"<<x1+roi.x<<y1+roi.y<<distance<<distance1;
//            }

            if(abs(distance-distance1)>35)
                continue;


            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));



            //字体在中心点相连角度
            if(abs(angle1)<25||abs(angle1)>155)
            {

                if(contours[k].size()>10&&contours[k].size()<objMaxSize)
                {
                    QC_Noise[c1++]=Point(x1,y1);

                }


                if(contours[k].size()>objMinSize-15)
                    ++b1;

                if(!blackHat)
                {
                    if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                        continue;

                }
                else
                {

//circle( m6, Point(x1,y1), 3, Scalar(255,255,255), 1, CV_AA);
                    if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*4)
                        continue;

                    if(abs(angle1)>13&&abs(angle1)<167)
                        continue;

                }
                //qDebug()<<"all x y"<<x1+roi.x<<y1+roi.y<<"i:"<<i<<"find a1"<<a1;


                len1=common::getLineLen(rectpoint[0],rectpoint[1]);
                len2=common::getLineLen(rectpoint[1],rectpoint[2]);

                if(len1>len2)
                {
                    angle2=common::calcAngle(rectpoint[0],rectpoint[1]);
                }
                else
                {
                    angle2=common::calcAngle(rectpoint[1],rectpoint[2]);
                }


                if(len1<len2)//取最大长度
                {
                    len1^=len2;
                    len2^=len1;
                    len1^=len2;

                }



                if(!blackHat)
                {
                    if(abs(angle2)>30)
                    {
                        errorFlag=1;
                        continue;
                    }
                    if(abs(angle2)>20&&len1>37)//矩形角度
                    {
                        errorFlag=2;
                        continue;
                    }

                    if(len1>44||len1<19)
                    {
                        errorFlag=3;
                        continue;
                    }

                    if(len1*1.0/len2<1.3)
                    {
                        errorFlag=4;
                        continue;
                    }
                }
                else
                {

                    //qDebug()<<"11111"<<len1<<len2<<contours[k].size();
                    //circle( m6, Point(x1,y1), 3, Scalar(255,255,255), 1, CV_AA);
                    if(len1>70||len1<13)
                    {
                        errorFlag=100;
                        continue;
                    }

                    if(len1*1.0/len2<1.3&&contours[k].size()<100)
                    {
                        errorFlag=105;
                        continue;
                    }

                }



                //特征数量

                if(abs(angle1)<30)
                    ++a2;
                if(abs(angle1)>150&&a1<100)
                    ++a3;

                fontGrayVal=calcGray_QC(m7,contours[k]);

                if(fontGrayVal>120)
                    fontGrayDif=40;
                else if(fontGrayVal>55)
                    fontGrayDif=20;
                else if(fontGrayVal>50)
                    fontGrayDif=10;
                else
                    fontGrayDif=5;



                int whiteRatioVal=whiteRatio(m7,Point(x1,y1),fontGrayVal,fontGrayDif);
                int contoursGrayDif=contourGrayDif(m7,contours[k],shadowVal,0,-2);
                //if(DEBUG) qDebug()<<"fontGrayVal"<<fontGrayVal<<whiteRatioVal<<"x y"<<x1+roi.x<<y1+roi.y<<"i"<<i<<"size"<<contours[k].size();
                if(!blackHat)
                {


                    if(DEBUG) qDebug()<<"contoursGrayDif"<<contoursGrayDif<<"x y"<<x1+roi.x<<y1+roi.y<<"i"<<i<<"size"<<contours[k].size()<<"angle"<<angle2<<"len1"<<len1<<"whiteRatioVal"<<whiteRatioVal<<fontGrayVal<<"shadowVal"<<shadowVal;




                    int grayDif=20;
                    if(i>40)
                        grayDif=35;
                    else if(i>25)
                        grayDif=25;
                    else if(i>20)
                        grayDif=20;
                    else if(i>10)
                        grayDif=12;

                    if(contoursGrayDif<grayDif)
                    {
                        errorFlag=5;
                        continue;
                    }

//                    area=contourArea(contours[k]);
//                    qDebug()<<"area:"<<area<<"len1*len2:"<<len1*len2<<"len1 len2 "<<len1<<len2;

                }


                a4= common::Smoothlevel(contours[k],5,20,1);
                smoothNum=a4;

                if(DEBUG) qDebug()<<"smoothNum"<<smoothNum;
                if(a4>9&&len2<15)
                {
                    if(DEBUG) qDebug()<<"smoothNum warning"<<a4;
                    errorFlag=6;
                    continue;
                }
                if(blackHat)
                {
                    if( smoothNum>20)
                        continue;
                }

                QC_Arr[a1]=Point(x1,y1);

                QC_Index[a1]=k;

                QC_WhiteRatio[a1]=whiteRatioVal;
                QC_Shadow[a1]=shadowVal;
                    ++a1;




                if(blackHat)
                {

                    if(a4>8&&len2<25)
                    {
                        errorFlag=101;
                        continue;
                    }



                    if(contours[k].size()<objMinSize+25)
                        continue;





                    circle( m6, Point(x1,y1), 3, Scalar(255,255,255), 1, CV_AA);
                    if(angleOffset+angle1>180)
                        angle1=angleOffset+angle1-360;
                    else if(angleOffset+angle1<-180)
                        angle1=angleOffset+angle1+360;
                    else
                        angle1+=angleOffset;
                    px=x1;
                    py=y1;
                    angle=angle1;

//                    drawContours(m6,contours,
//                                 -1, // draw all contours
//                                 Scalar(125), // in black
//                                 1); // with a thickness of 2

                    if(DEBUG)  qDebug()<<"size:"<<contours[k].size()<<"x1 y1"<<x1<<y1<<"i"<<i<<"contoursGrayDif"<<contoursGrayDif<<"len1 len2"<<len1<<len2;

                    loop=false;

                }
//查找两个相同特征
                if(loop&&calcFont_QC(a1,QC_Arr))
                {


//qDebug()<<"white per"<< whiteRatioVal<<"brightest val"<<fontGrayVal;
                    loop=false;
                }

            }

        }
 qDebug()<<"errorId"<<errorFlag;
        param.var1[27]=i;
        param.var1[26]=a1;
        if(!loop)
        {
            loop=true;
            int c1Num;

            Point nP1;
            if(DEBUG) qDebug()<<"b1"<<b1<<"a1 a2 a3"<<a1<<a2<<a3<<"i"<<i;
            if(!blackHat)
            {
                for(int i=0;i<a1;i++)
                {
                    for(int j=i+1;j<a1;j++)
                    {
                        if(abs(QC_Arr[i].x-QC_Arr[j].x)>=15||abs(QC_Arr[i].y-QC_Arr[j].y)>=38)
                            continue;

                        if(QC_WhiteRatio[i]>36&&QC_WhiteRatio[j]>36)
                        {
                            errorFlag=7;
                            continue;
                        }
                         v1= grayValDif(m7,contours[QC_Index[i]]);
                         v2= grayValDif(m7,contours[QC_Index[j]]);
    //qDebug()<<"x1 y1"<<QC_Arr[i].x+roi.x<<QC_Arr[i].y+roi.y<<QC_Arr[(i+1)%a1].x+roi.x<<QC_Arr[(i+1)%a1].y+roi.y<<"QC size"<<contours[QC_Index[i]].size()<<"v1"<<v1;

                        angle1=common::calcAngle2(Point(cenX,cenY),QC_Arr[i]);
                        if(angleOffset+angle1>180)
                            angle1=angleOffset+angle1-360;
                        else if(angleOffset+angle1<-180)
                            angle1=angleOffset+angle1+360;
                        else
                            angle1+=angleOffset;


                        if(DEBUG) qDebug()<<"v1 v2"<<v1<<v2<<"y1 y2"<<QC_Arr[i].y<<QC_Arr[j].y;


                        c1Num=0;
                        for(int k=0;k<c1;k++)
                        {

                            circle( m6,QC_Noise[k], 2, Scalar(200,200,200), 1, CV_AA);
                            nP1.x=(QC_Arr[i].x+QC_Arr[j].x)/2;
                            nP1.y=(QC_Arr[i].y+QC_Arr[j].y)/2;
                            if(common::getLineLen(QC_Noise[k],nP1)<40)
                                ++c1Num;
                        }
                        if(DEBUG) qDebug()<<"c1Num"<<c1Num<<"a1"<<a1;


                        if(c1Num>6)
                        {
                            if(v1<30&&v2<30)
                            {
                                errorFlag=8;
                                continue;
                            }

    //                             if(abs(angle1)>10&&abs(angle1)<170)
    //                                 continue;
                        }

                        if(c1Num>9)
                        {
                            errorFlag=9;
                            continue;
                        }

                        if(a2>1&&a3>1&&abs(a2-a3)<2)
                        {
                            errorFlag=10;
                            continue;
                        }

                        //if(b1>19)
                        if(b1>9||(b1>7&&c1Num>7))
                        {
                            errorFlag=11;
                            continue;
                        }
                        if(QC_Shadow[i]>50&&QC_Shadow[j]>50)
                        {

                            errorFlag=12;
                            //continue;
                        }

                            px=QC_Arr[i].x;
                            py=QC_Arr[i].y;
                            angle=angle1;

                            param.var2[19]=angle;
                            param.var1[29]=py;
                            param.var1[28]=px;
                            rectangle(m6,Rect(px-25,py-25,50,50),Scalar(255,255,255));

                            goto END;
                    }
               }



            }
            if(blackHat)
            {

                if(a2>0&&a3>0)
                {
                    errorFlag=102;

                    continue;
                }
                c1Num=0;
                for(int k=0;k<c1;k++)
                {


                    nP1.x=px;
                    nP1.y=py;
                    if(common::getLineLen(QC_Noise[k],nP1)<40)
                        ++c1Num;
                }




                if(DEBUG) qDebug()<<"black c1Num"<<c1Num<<abs(angle);
                errorFlag=103;
                if(a1<4&&c1Num<6&&b1<6&&param.var1[27]>9)
                {
                    errorFlag=104;
                    if(abs(angle)<15||abs(angle)>165)
                    {


                        param.var2[19]=angle;
                        param.var1[29]=py;
                        param.var1[28]=px;
                        rectangle(m6,Rect(px-25,py-25,50,50),Scalar(0,0,0),4);

                        if(DEBUG) qDebug()<<"blackHat a1 a2 a3"<<a1<<a2<<a3<<"c1Num"<<c1Num<<"smoothNum"<<smoothNum<<"angle"<<angle;
                        goto END;
                    }
                }

            }

        }


    }


END:

             if(DEBUG)
             {

                 imshow("m6",m6);
                 imshow("aaaaa",m2);

             }


             return;


}

int project1::zt_1stLine(Mat &m5,Point &p,float &angle)
{

    Mat m1,m3;
    int len,len1,len2;

    Point p1[4];
    std::vector<Point> cen(50);
    int minLen=590;
    int maxLen=635;
    Mat m2=g_dstImage2.clone();
    medianBlur(m5, m3,9);

    int thresh=254;
    bool found=false;
    int ret=-1;
    std::vector<std::vector<Point>> contours;


    for(int i=thresh;i>70;i-=5)
    {
        threshold(m3,m1,i,255,THRESH_BINARY);

        imshow("Threshold",m1);

        cen.clear();

        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];



        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<600||contours[k].size()>1200)
                continue;


            //contourFeature(contours[k]);


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            
            cvBoxPoints(End_Rage2D1,rectpoint);

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(abs(len1-len2)>90)
                continue;


            if(m3.at<uchar>(contours[k][0].y-1,contours[k][0].x)>i)
                continue;

            cen.push_back(End_Rage2D1.center);

            qDebug()<<"i:"<<i<<"size"<<contours[k].size()<<"轮廓长度"<<arcLength(contours[k],true)<<"x1 y1"<<End_Rage2D1.center.x<<End_Rage2D1.center.y;
            for(int i = 0;i< 4;i++)
            {
                //CvArr* s=(CvArr*)&result;
                //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
                //line(m2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
            }

            // QString words=QString("%1").arg(contours[k].size());
            // putText( m2, words.toStdString(), Point(End_Rage2D1.center.x-10,End_Rage2D1.center.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

            //qDebug()<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2;
        }

        // draw black contours on white image

        int j1=0;


        if(cen.size()>2)//排除错误的区域
        {
            for(int i=0;i<cen.size();i++)
            {
                for(int n=i+1;n<cen.size();n++)
                {
                    len=common::getLineLen(cen[i],cen[n]);
                    // qDebug()<<"bbbb"<<len;
                    //qDebug()<<"n"<<i<<n;
                    if(len>minLen&&len<maxLen)
                    {
                        p1[j1]=cen[i];
                        p1[j1+1]=cen[n];
                        j1+=2;
                        found=true;

                        // qDebug()<<"aaaa"<<len<<j1;
                    }
                }
            }
            if(j1==4)
            {

                int b1=0,b2=0;
                b1= getAvgBright(m3,p1[0],15)+getAvgBright(m3,p1[1],15);
                b2= getAvgBright(m3,p1[2],15)+getAvgBright(m3,p1[3],15);
                if(b2>b1)
                {
                    p1[0]=p1[2];
                    p1[1]=p1[3];
                }

            }

        }

        if(cen.size()==2)
        {

            len=common::getLineLen(cen[0],cen[1]);
           // qDebug()<<"i"<<i<<"len"<<len<<minLen<<maxLen;
            if(len>minLen&&len<maxLen)
            {
                p1[0]=cen[0];
                p1[1]=cen[1];
                found=true;
                qDebug()<<"pos1"<<p1[0].x<<p1[0].y<<p1[1].x<<p1[1].y;
            }



        }

        if(found)
        {
            qDebug()<<"i:"<<i;
            ret=0;
            break;
        }
    }
     putText( m2, "pos", p1[0],CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0),2 );
     putText( m2, "pos", p1[1],CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 255, 0),2 );

     angle=common::calcAngle(p1[0],p1[1]);
     p.x=(p1[0].x+p1[1].x)/2;
     p.y=(p1[0].y+p1[1].y)/2;

     drawContours(m2,contours,
                  -1, // draw all contours
                  Scalar(125), // in black
                  1); // with a thickness of 2

     imshow("Contours",m2);
     return ret;
}

int project1::calcWork(Mat img,Point &p,float &angle,int c1,int c2)
{
    Mat cimg;
    medianBlur(img, img, 9);
    cvtColor(img, cimg, COLOR_GRAY2BGR);

    vector<Vec3f> circles;
    HoughCircles(img, circles, CV_HOUGH_GRADIENT, 1, 50,
                 100, 30,
                 14, 20 // change the last two parameters
                 // (min_radius & max_radius) to detect larger circles
                 );
    for( size_t i = 0; i < circles.size(); i++ )
    {
        Vec3i c = circles[i];
        circle( cimg, Point(c[0], c[1]), c[2], Scalar(0,0,255), 3, CV_AA);
        circle( cimg, Point(c[0], c[1]), 2, Scalar(0,255,0), 3, CV_AA);
    }


    //calc start

    qDebug()<<"size:"<<circles.size();

    if(circles.size()<3)
    {

        namedWindow("detected circles",0);
        resizeWindow("detected circles",800,600);
        imshow("detected circles", cimg);
        return -1;
    }

    Point p1,p2,p3,p4;
    int temp,j1=-1,j2=-1,j3=-1,j4=-1;
    //三个圆点之间比较

    for( size_t i = 1; i < 3; i++ )
    {
        temp=sqrt((circles[i-1][0]-circles[i][0])*(circles[i-1][0]-circles[i][0])+ (circles[i-1][1]- circles[i][1])*(circles[i-1][1]- circles[i][1]));
        if(abs(temp-c1)<8)
        {
            j1=i-1;
            j2=i;

        }
        if(abs(temp-c2)<8)
        {
            j3=i-1;
            j4=i;
        }
         qDebug()<<temp;
    }

    temp=sqrt((circles[0][0]-circles[2][0])*(circles[0][0]-circles[2][0])+ (circles[0][1]- circles[2][1])*(circles[0][1]- circles[2][1]));

    if(abs(temp-c1)<8)
    {
        if(j1==-1)
        {
            j1=0;
            j2=2;

        }

    }
    if(abs(temp-c2)<8)
    {   if(j3==-1)
        {
            j3=0;
            j4=2;
        }
    }
    //end
qDebug()<<j1<<j2<<j3<<j4<<temp;
    if(-1==j1 || -1 == j2 || -1==j3 || -1==j4 )
        return -1;

    p1.x=circles[j1][0];
    p1.y=circles[j1][1];
    p2.x=circles[j2][0];
    p2.y=circles[j2][1];

    angle=common::calcAngle(p1,p2);

    p3.x=circles[j3][0];
    p3.y=circles[j3][1];
    p4.x=circles[j4][0];
    p4.y=circles[j4][1];

    if(p3.x==p4.x)
    {
        p.x=p3.x;
        p.y=(p3.y>p4.y)?(p3.y-p4.y)/2+p4.y:(p4.y-p3.y)/2+p3.y;
    }
    if(p3.y==p4.y)
    {
        p.y=p3.y;
        p.x=(p3.x>p4.x)?(p3.x-p4.x)/2+p4.x:(p4.x-p3.x)/2+p3.x;
    }


    p.x=(p3.x>p4.x)?p4.x:p3.x;
    p.y=(p3.y>p4.y)?p4.y:p3.y;

    p.x+=abs(p3.x-p4.x)/2;
    p.y+=abs(p3.y-p4.y)/2;


//    if(p3.x==p4.x){p.x=p3.x;p.y=abs(p4.y-p3.y)/2;}
//    if(p3.y==p4.y){p.x=abs(p3.x-p4.x)/2;p.y=p3.y;}
//    if(p3.x>p4.x){Point p5=p3;p3=p4;p4=p5;}

//    a=abs(p4.y-p3.y);
//    b=abs(p4.x-p3.x);
//    c=sqrt(a*a*1.0+b*b);
//    if(a>b){a^=b;b^=a; a^=b;}

//    if(p3.y<p4.y)
//    {
//        if(p4.x-p3.x>p4.y-p3.y)
//        { p.x=p3.x+b/2;
//            p.y=p3.y+a/2;
//        }
//        else
//        {
//            p.x=p3.x+a/2;
//            p.y=p3.y+b/2;
//        }

//    }
//    else
//    {
//        if(p4.x-p3.x>p3.y-p4.y)
//        {
//            p.x=p3.x+b/2;
//            p.y=p3.y-a/2;
//        }
//        else
//        {
//            p.x=p3.x+a/2;
//            p.y=p3.y-b/2;
//        }

//    }

    circle( cimg, Point(p.x, p.y), 3, Scalar(255,0,0), 3, CV_AA);

    namedWindow("detected circles",0);
    resizeWindow("detected circles",800,600);
    imshow("detected circles", cimg);
    return 0;
    //end
}


void project1::xl_circle3(Mat &rgb,Mat& img1,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int,params &param)
{

    Mat img=img1(roi);
    medianBlur(img, img, 9);
    int len1[10];
    int c_size1=param.var1[0];//小圆孔size
    int c_size2=param.var1[1];//大圆孔size
    int c_size3=param.var1[2];//最大圆size
    int min_radius=param.var1[3];//最小内圆环半径
    int max_radius=param.var1[4];//最大内圆环半径
    int dVal=param.var1[5];//实际圆孔间距和测量圆孔间距差值,越小越匹配度越低
    int cen_distance1=param.var1[6];//圆孔离圆心距离(所有风扇小孔的平均值)
    len1[0]=param.var1[7];//最大圆孔间距
    len1[1]=param.var1[8];//小圆间距
    len1[2]=param.var1[9];//只有6个小圆孔间距
    Point mark(param.var1[10]-roi.x,param.var1[11]-roi.y);//机器人基准点
    Point center(param.var1[12]-roi.x,param.var1[13]-roi.y);//圆心
    int brightVal=param.var1[14];//毛刺灰度值
    int thresh=param.var1[15];//二值参数
    int limitVal=param.var1[16];//圆孔离圆心距离与param.var1[6]差值范围
    double pixelRatio=param.var2[0];//像素比
    double rectDifVal=param.var2[1];//圆外接矩形长宽比
    double angle2=param.var2[2];//坐标角度偏差
    double offsetX=param.var2[3];//X偏移毫米
    double offsetY=param.var2[4];//Y偏移毫米
    //int s_num=0,b_num=0;//排除污点后大圆，小圆孔数量


    int j1=0;
    int distance;
    int cenX=center.x;
    int cenY=center.y;
    int x1,y1,shape_size;
    Point calibratePoint;

    xy_size detectedCircleList[100];
    xy_size smallCircleList[100];
    xy_size bigCircleList[50];




    //start -------------------------------------------------------------------------------------

 //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(img,img,thresh,255,THRESH_BINARY);


     std::vector<std::vector<Point>> contours;

     findContours(img,
                  contours, // a vector of contours
                  CV_RETR_TREE, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     //std::cout << "Contours: " << contours.size() << std::endl;
     std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];

     for (unsigned int k=0 ; k<contours.size(); k++)
     {

         if(contours[k].size()>c_size3-100&&contours[k].size()<c_size3+150)
         {
             End_Rage2D1 = minAreaRect(Mat(contours[k]));
             cvBoxPoints(End_Rage2D1,rectpoint);
             cenX=End_Rage2D1.center.x;
             cenY=End_Rage2D1.center.y;
             qDebug()<<"cen"<<cenX<<cenY;
         }
     }

     for (unsigned int k=0 ; k<contours.size(); k++)
     {
         if(contours[k].size()<c_size1-dVal||contours[k].size()>c_size2+500)
             continue;
         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;


        distance=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


        if(distance<min_radius||distance>max_radius)
            continue;


        //qDebug()<<"aaaa"<<cen_distance1-distance<<limitVal<<"|"<<contours[k].size()<<c_size2-dVal;
        if(abs(cen_distance1-distance)>limitVal&&contours[k].size()<=c_size2-dVal)
            continue;

        xy_size curObj;
        curObj.calcCen=false;
        curObj.x=x1;
        curObj.y=y1;
        curObj.size=contours[k].size();
        curObj.p1=contours[k];



        double line1= common::getLineLen(Point(rectpoint[0].x,rectpoint[0].y),Point(rectpoint[1].x,rectpoint[1].y));
        double line2= common::getLineLen(Point(rectpoint[2].x,rectpoint[2].y),Point(rectpoint[1].x,rectpoint[1].y));

        if(abs(line1/line2-1)>rectDifVal)
        {
            curObj.calcCen=true;
            //continue;


           // qDebug()<<"line1 2"<<abs(line1/line2-1)<<x1<<y1;
        }

        detectedCircleList[num++]=curObj;


         //QString words=QString("%1").arg(End_Rage2D1.angle);
        // putText( m2, words.toStdString(), Point(End_Rage2D1.center.x-10,End_Rage2D1.center.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
        // std::cout << "Size: " << contours[k].size()<<" angle:"<<End_Rage2D1.angle << std::endl;
     }

     //--------------------------------------------------------------end

     int bigNum=0,smallNum=0;
     for(int k=0;k<num;k++)
     {
         if(detectedCircleList[k].size>c_size2-dVal)
         {
             bigCircleList[bigNum++]=detectedCircleList[k];
             //qDebug()<<"p1"<<detectedCircleList[k].p1[0].x<<detectedCircleList[k].p1[0].y;
         }
         else
         {
             smallCircleList[smallNum++]=detectedCircleList[k];
         }
     }
     //sortCircle(bigCircleList,bigNum,cenX,cenY);

     sortCircle(detectedCircleList,num,cenX,cenY);
//     if(num>22)
//     {


//         if(bigNum>10)
//         {

//             detectOther2(bigCircleList,bigNum,b_num,len1[0],dVal);
//             otherNum+=bigNum-b_num;


//         }

//         if(smallNum>12)
//         {

//             detectOther2(smallCircleList,smallNum,s_num,len1[1],dVal);
//             otherNum+=smallNum-s_num;


//         }

//         num=22;

//     }
//    //圆环上有6个小孔
//     if(num>6&&num<22)
//     {

//         detectOther2(smallCircleList,smallNum,s_num,len1[2],dVal);
//         otherNum+=smallNum-s_num;

//        num=6;
//     }

//     if(b_num==0) b_num=bigNum;
//     for(int i=0;i<b_num;i++)
//     {
//         detectedCircleList[i]=bigCircleList[i];

//     }
//     if(b_num==0) s_num=smallNum;
//     for(int i=0;i<s_num;i++)
//     {
//         detectedCircleList[i+b_num]=smallCircleList[i];
//     }





//qDebug()<<"yyyyy"<<detectedCircleList[4].p1[120].x;
     for(int k=0;k<num;k++)
     {

         x1=detectedCircleList[k].x;
         y1=detectedCircleList[k].y;

         shape_size=detectedCircleList[k].size;

         int bright;
         bright=img.at<uchar>(y1-1,x1-1);
         bright+=img.at<uchar>(y1-1,x1);
         bright+=img.at<uchar>(y1-1,x1+1);
         bright+=img.at<uchar>(y1,x1-1);
         bright+=img.at<uchar>(y1,x1);
         bright+=img.at<uchar>(y1,x1+1);
         bright+=img.at<uchar>(y1+1,x1-1);
         bright+=img.at<uchar>(y1+1,x1);
         bright+=img.at<uchar>(y1+1,x1+1);
         bright/=9;

         if(bright<brightVal&&j1<50)//去毛刺圆孔,j1(下标)
         {

             xy_arr[j1].x=x1;
             xy_arr[j1].y=y1;
             xy_arr[j1].size=shape_size;
             //xy_arr[j1].p1=detectedCircleList[k].p1;

             xy_arr[j1].calcCen=detectedCircleList[k].calcCen;

             if(DEBUG)
             {
                 circle( rgb,Point(x1+roi.x,y1+roi.y), 35, Scalar(0,0,255), 3, CV_AA);
             }

             Point p1,p2,p3,pCen,pPre;
             bool success;
             if(xy_arr[j1].calcCen)
             {

                 for(int i=20;i<shape_size-20;i+=20)
                 {

                     p1.x=detectedCircleList[k].p1[i-20].x;
                     p1.y=detectedCircleList[k].p1[i-20].y;
                     p2.x=detectedCircleList[k].p1[i].x;
                     p2.y=detectedCircleList[k].p1[i].y;
                     p3.x=detectedCircleList[k].p1[i+20].x;
                     p3.y=detectedCircleList[k].p1[i+20].y;




                     success=common::getCircleCenter(p1,p2,p3,pCen);

                     if(abs(pCen.x-pPre.x)<4&&abs(pCen.y-pPre.y)<4&&success)
                     {
                          //qDebug()<<"i:"<<i<<"pCen1111"<<xy_arr[j1].x<<xy_arr[j1].y<<pCen.x<<pCen.y;
                          xy_arr[j1].x=pCen.x;
                          xy_arr[j1].y=pCen.y;

                          break;
                     }
                     pPre=pCen;

                 }
             }


             calibratePoint=common::xyCalibration(angle2,mark,Point(x1,y1));


             xy_arr[j1].moveX=(calibratePoint.x-mark.x)*pixelRatio+offsetX;
             xy_arr[j1].moveY=(calibratePoint.y-mark.y)*pixelRatio+offsetY;

             j1++;
         }
     }


   num2=j1;





}

void project1::xl_circle4(Mat &rgb,Mat& img1,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int,params &param)
{
    Point center(param.var1[0]-roi.x,param.var1[1]-roi.y);//圆心
    Point robotCen(param.var1[2]-roi.x,param.var1[3]-roi.y);//机器人基准点
    int minSize=param.var1[4];//圆孔size
    int maxSize=param.var1[5];//圆孔size
    int c_size3=param.var1[6];//找圆心需要的size
    int minDistance=param.var1[7];//圆孔离圆心距离
    int maxDistance=param.var1[8];//圆孔离圆心距离

    int thresh=param.var1[9];//二值参数
    int step=param.var1[10];//毛刺step
    int b=param.var1[11];//RGB
    int g=param.var1[12];
    int r=param.var1[13];
    int rgbDif=param.var1[14];//rgb范围

    double pixelRatio=param.var2[0];//像素比
    double rectDifVal=param.var2[1];//圆外接矩形长宽比
    double angle2=param.var2[2];//坐标角度偏差
    double offsetX=param.var2[3];//X偏移毫米
    double offsetY=param.var2[4];//Y偏移毫米


    int j1=0;
    int distance,len1,len2;
    int cenX=center.x;
    int cenY=center.y;
    int x1,y1,shape_size;
    Point calibratePoint;

    xy_size detectedCircleList[100];


    Mat img=img1(roi);


    Mat mask(img.rows,img.cols,CV_8UC1,Scalar(0));
    for(int i=0;i<img.rows;i++)
    {
        for(int j=0;j<img.cols;j++)
        {

            if(abs(img.at<Vec3b>(i,j)[0]-r)+ abs(img.at<Vec3b>(i,j)[1]-g)+ abs(img.at<Vec3b>(i,j)[2]-b)<rgbDif)
            {

                mask.at<uchar>(i,j)=255;
            }
        }
    }

    //start -------------------------------------------------------------------------------------


    //threshold(img,img,thresh,255,THRESH_BINARY);
    std::vector<std::vector<Point>> contours;

     findContours(mask,
                  contours, // a vector of contours
                  CV_RETR_TREE, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     //std::cout << "Contours: " << contours.size() << std::endl;
     std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];

//     for (unsigned int k=0 ; k<contours.size(); k++)
//     {

//         if(contours[k].size()>c_size3-100&&contours[k].size()<c_size3+150)
//         {
//             End_Rage2D1 = minAreaRect(Mat(contours[k]));
//             cvBoxPoints(End_Rage2D1,rectpoint);
//             cenX=End_Rage2D1.center.x;
//             cenY=End_Rage2D1.center.y;
//             qDebug()<<"cen"<<cenX<<cenY;
//         }
//     }
     qDebug()<<"size:"<<contours.size();
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
         if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;
         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;



        distance=common::getLineLen(Point(x1,y1),Point(cenX,cenY));

        if(distance<minDistance||distance>maxDistance)
            continue;


        xy_size curObj;
        curObj.calcCen=false;
        curObj.x=x1;
        curObj.y=y1;
        curObj.size=contours[k].size();
        curObj.p1=contours[k];


        len1=common::getLineLen(rectpoint[0],rectpoint[1]);
        len2=common::getLineLen(rectpoint[1],rectpoint[2]);

        if(len1>len2)
        {
            len1^=len2;
            len2^=len1;
            len1^=len2;

        }


        if(len2*1.0/len1<rectDifVal)
        {
            curObj.calcCen=true;

        }

        detectedCircleList[num++]=curObj;


         //QString words=QString("%1").arg(End_Rage2D1.angle);
        // putText( m2, words.toStdString(), Point(End_Rage2D1.center.x-10,End_Rage2D1.center.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
        // std::cout << "Size: " << contours[k].size()<<" angle:"<<End_Rage2D1.angle << std::endl;
     }

     //--------------------------------------------------------------end



     sortCircle(detectedCircleList,num,cenX,cenY);






//qDebug()<<"yyyyy"<<detectedCircleList[4].p1[120].x;
     for(int k=0;k<num;k++)
     {

         x1=detectedCircleList[k].x;
         y1=detectedCircleList[k].y;

         shape_size=detectedCircleList[k].size;

         if(j1<50)//去毛刺圆孔,j1(下标)
         {

             xy_arr[j1].x=x1;
             xy_arr[j1].y=y1;
             xy_arr[j1].size=shape_size;

             xy_arr[j1].calcCen=detectedCircleList[k].calcCen;

             if(DEBUG)
             {
                 circle( rgb,Point(x1+roi.x,y1+roi.y), 35, Scalar(0,0,255), 3, CV_AA);
             }

             Point p1,p2,p3,pCen,pPre;
             bool success;
             if(xy_arr[j1].calcCen)
             {

                 for(int i=step;i<shape_size-step;i+=step)
                 {

                     p1.x=detectedCircleList[k].p1[i-step].x;
                     p1.y=detectedCircleList[k].p1[i-step].y;
                     p2.x=detectedCircleList[k].p1[i].x;
                     p2.y=detectedCircleList[k].p1[i].y;
                     p3.x=detectedCircleList[k].p1[i+step].x;
                     p3.y=detectedCircleList[k].p1[i+step].y;




                     success=common::getCircleCenter(p1,p2,p3,pCen);

                     if(abs(pCen.x-pPre.x)<4&&abs(pCen.y-pPre.y)<4&&success)
                     {
                          //qDebug()<<"i:"<<i<<"pCen1111"<<xy_arr[j1].x<<xy_arr[j1].y<<pCen.x<<pCen.y;
                          xy_arr[j1].x=pCen.x;
                          xy_arr[j1].y=pCen.y;

                          break;
                     }
                     pPre=pCen;

                 }
             }


             calibratePoint=common::xyCalibration(angle2,robotCen,Point(x1,y1));


             xy_arr[j1].moveX=(calibratePoint.x-robotCen.x)*pixelRatio+offsetX;
             xy_arr[j1].moveY=(calibratePoint.y-robotCen.y)*pixelRatio+offsetY;

             j1++;
         }
     }


   num2=j1;



}


void project1::xl_blackObj(Mat &src,Mat &rgb,  Rect roi, params &param)
{
    param.var1[29]=0;
    Mat m1;
    int len1,len2,allNum,x1,y1;
    Mat m2=src.clone();

    medianBlur(m2, m2,3);

    int cenX= param.var1[0];//圆心X
    int cenY= param.var1[1];//圆心Y
    unsigned int minSize=param.var1[2];//小圆孔size
    int radius=param.var1[3];//半径
    int grayVal=param.var1[4];//异物灰度值
    int maxLen= param.var1[5];//过滤最小长度大于
    int minLen=param.var1[6];//过滤最小长度小于
    //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(m2,m1,grayVal,255,THRESH_BINARY);

     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


     std::cout << "Contours: " << contours.size() << std::endl;
     //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     //for ( ; itContours!=contours.end(); ++itContours)
     allNum=0;
     for (unsigned int k=0 ; k<contours.size(); k++)
     {

         if(contours[k].size()<minSize)
             continue;


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;

         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

         if(len1>len2)
         {
             len1^=len2;
             len2^=len1;
             len1^=len2;
         }

         if(len1<minLen||len1>maxLen)
             continue;
//qDebug()<<"size"<<contours[k].size()<<common::getLineLen(Point(x1,y1),Point(cenX,cenY))<<radius;
         if(common::getLineLen(Point(x1,y1),Point(cenX,cenY))>radius)
             continue;

         ++allNum;
         if(allNum>0)
         {
             param.var1[29]=1;
             param.var1[27]=x1;
             param.var1[28]=y1;

             qDebug()<<"cen"<<x1<<y1<<"size"<<contours[k].size();
             break;
         }

     }



}



void project1::zt_calcWork_6(Mat &m3,Rect roi,params& param,double &angle)
{
    int cenX=param.var1[0]-roi.x;
    int cenY=param.var1[1]-roi.y;
    //qDebug()<<"cen"<<param.var1[0]<<"roi.x"<<roi.x;

    int distance=param.var1[2];
    unsigned int minSize=param.var1[3];
    unsigned int maxSize=param.var1[4];
    int thresh=param.var1[5];
    int square=param.var1[6];
    int rectLen=param.var1[7];
    int rectLen2=param.var1[8];//大半圆矩形长度
    double angleOffset=param.var2[0];
    Mat m1,m2,m5;
    Mat m4=m3(roi).clone();
    bool foundBigArea=false;
    bool foundSmallArea=false;


    int x1,y1,distance1,j,x2,y2,x3,y3,len1,len2;

    double d;
    int sizeArr[2]={0};
    Point p1[2];

    medianBlur(m4, m5, 9);
    medianBlur(m4, m2, 3);




    int offset = -10;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m1, m1, MORPH_TOPHAT , element);
    else
        morphologyEx(m1, m1, MORPH_BLACKHAT, element);

//结束

    for(int i=thresh;i>70;i-=5)
    {
        qDebug()<<"thresh:"<<i;

        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);


        std::vector<std::vector<Point>> contours;

        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        // Print contours' length


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];

        j=0;
        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            if(contours[k].size()<minSize||contours[k].size()>maxSize)
                continue;


            End_Rage2D1 = minAreaRect(Mat(contours[k]));

            cvBoxPoints(End_Rage2D1,rectpoint);

            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(abs(distance-distance1)>20)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<len2)//取最大长度
                len1=len2;
            if(len1<rectLen)
                continue;
            //qDebug()<<"Len"<<len1<<"rectlen"<<rectLen;


            d=abs(distance1*1.0/distance-1);

            //qDebug()<<"d:"<<d<<"设定值"<<distance<<"实际值"<<distance1<<"x1 y1"<<x1+roi.x<<y1+roi.y;
            x3=abs(x1-cenX);
            y3=abs(y1-cenY);
            if(cenX>x1)
            {
                if(distance>distance1)
                    x2=x1-x3*d;
                else
                    x2=x1+x3*d;

            }
            else
            {
                if(distance>distance1)
                    x2=x1+x3*d;
                else
                    x2=x1-x3*d;
            }
            if(cenY>y1)
            {
                if(distance>distance1)
                    y2=y1-y3*d;
                else
                    y2=y1+y3*d;
            }
            else
            {
                if(distance>distance1)
                    y2=y1+y3*d;
                else
                    y2=y1-y3*d;
            }

            if(len1>rectLen2)
            {
                p1[1].x=x2;
                p1[1].y=y2;
                sizeArr[1]=contours[k].size();
                foundBigArea=true;
                if(foundSmallArea)
                {
                    if(abs(p1[0].x-p1[1].x)+abs(p1[0].y-p1[1].y)<150)
                        foundSmallArea=false;
                }
            }
            else
            {
                p1[0].x=x2;
                p1[0].y=y2;
                sizeArr[0]=contours[k].size();
                foundSmallArea=true;
            }

            if(j>1)
                break;

            j++;


        }
        if(foundBigArea&&foundSmallArea)
            break;
    }
     if(!foundBigArea&&!foundSmallArea)
     {
         angle=360;
         return;
     }


     if(foundBigArea&&foundSmallArea==false)
     {
         qDebug()<<"通过中心点计算小半圆";
         p1[0].x=cenX-(p1[1].x-cenX);
         p1[0].y=cenY-(p1[1].y-cenY);

     }
     if(foundBigArea==false&&foundSmallArea)
     {
         qDebug()<<"通过中心点计算大半圆";
         p1[1].x=cenX-(p1[0].x-cenX);
         p1[1].y=cenY-(p1[0].y-cenY);

     }

 //********通过字体得到NI坐标

     Point NI;
     Point font=calcFont2(m2,p1[0],p1[1],square);
     if(!foundBigArea)
        NI= font;
     else
        NI=p1[1];
//     qDebug()<<QString("p1(%1,%2) p2(%3,%4)").arg(p1[0].x+roi.x).arg(p1[0].y+roi.y).arg(p1[1].x+roi.x).arg(p1[1].y+roi.y);
//*********end

     angle=common::calcAngle2(Point(cenX,cenY),NI);
     //角度偏移
     if(angleOffset+angle>180)
         angle=angleOffset+angle-360;
     else if(angleOffset+angle<-180)
         angle=angleOffset+angle+360;
     else
         angle+=angleOffset;
     //end
     if(foundBigArea)
         qDebug()<<"找到大半圆";
     if(foundSmallArea)
         qDebug()<<"找到小半圆";
     qDebug()<<"result x y"<<NI.x+roi.x<<NI.y+roi.y;




}


void project1::calcMeter(Mat& img,Rect rect,bool& NG,int &offX,int &offY,params param)
{

    Mat m3=img(rect).clone();
    Mat m1;
    int len1,len2,x1,y1,b1,b2;

    medianBlur(m3, m3,5);

   unsigned int minSize= param.var1[0];//圆孔最小size
   unsigned int maxSize= param.var1[1];//圆孔最大size
   int dif1= param.var1[2];//长宽像素差
   int brightDif= param.var1[3];//圆孔与圆环亮度差
   int thresh=param.var1[4];//二值
   int r=param.var1[5];//圆心至圆环半径
   int cenX= param.var1[6]-rect.x;//标定中心点X
   int cenY= param.var1[7]-rect.y;//标定中心点Y
   int maxOffset=param.var1[8];//最大偏移像素

    //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(m3,m1,thresh,255,THRESH_BINARY);

     imshow("Threshold",m1);
     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     std::cout << "Contours: " << contours.size() << std::endl;
     std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     //for ( ; itContours!=contours.end(); ++itContours)
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
         if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;


         //contourFeature(contours[k]);


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;
         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);
         if(abs(len1-len2)>dif1)
             continue;

         //亮度差计算
         b1=(m3.at<uchar>(y1-3,x1)+m3.at<uchar>(y1+3,x1)+m3.at<uchar>(y1,x1-3)+m3.at<uchar>(y1,x1+3))/4;
         b2=(m3.at<uchar>(y1-r,x1)+m3.at<uchar>(y1+r,x1)+m3.at<uchar>(y1,x1-r)+m3.at<uchar>(y1,x1+r))/4;
         qDebug()<<"aaaaa"<<b1<<b2;
         if(b2-b1<brightDif)
             continue;


         if(abs(x1-cenX)<maxOffset&&abs(y1-cenY)<maxOffset)
         {
             NG=false;
             offX=cenX-x1;
             offY=cenY-y1;
             break;
         }


     }




}
void project1::rotateRoi(Mat &src,Rect roi)
{
    if(roi.x+roi.width>src.cols||roi.y+roi.height>src.rows)
        return;
    Mat m1=src(roi).clone();

    for(int i=0;i<roi.height;i++)
    {
        for(int j=0;j<roi.width;j++)
        {
            src.at<uchar>(i+roi.y,j+roi.x)=m1.at<uchar>(roi.height-i-1,roi.width-j-1);
        }
    }
}
Mat project1::rotateImg(Mat &src)
{
    Point2f srcTriangle[3];
    Point2f dstTriangle[3];
    //定义一些Mat变量
    Mat rotMat( 2, 3, CV_32FC1 );
    Mat warpMat( 2, 3, CV_32FC1 );
    Mat srcImage, dstImage_warp, dstImage_warp_rotate;

    //【2】加载源图像并作一些初始化
    srcImage = src.clone();

    // 设置目标图像的大小和类型与源图像一致
    dstImage_warp = Mat::zeros( srcImage.rows, srcImage.cols, srcImage.type() );

    //【3】设置源图像和目标图像上的三组点以计算仿射变换
    srcTriangle[0] = Point2f( 0,0 );
    srcTriangle[1] = Point2f( static_cast<float>(srcImage.cols - 1), 0 );
    srcTriangle[2] = Point2f( 0, static_cast<float>(srcImage.rows - 1 ));

    dstTriangle[0] = Point2f( static_cast<float>(srcImage.cols*0.0), static_cast<float>(0));
    dstTriangle[1] = Point2f( static_cast<float>(srcImage.cols*1), static_cast<float>(0));
    dstTriangle[2] = Point2f( static_cast<float>(0), static_cast<float>(srcImage.rows*1));

    //【4】求得仿射变换
    warpMat = getAffineTransform( srcTriangle, dstTriangle );

    //【5】对源图像应用刚刚求得的仿射变换
    warpAffine( srcImage, dstImage_warp, warpMat, dstImage_warp.size() );

    //【6】对图像进行缩放后再旋转
    // 计算绕图像中点顺时针旋转50度缩放因子为0.6的旋转矩阵
    Point center = Point( dstImage_warp.cols/2, dstImage_warp.rows/2 );
    double angle = -180.0;
    double scale = 1;
    // 通过上面的旋转细节信息求得旋转矩阵
    rotMat = getRotationMatrix2D( center, angle, scale );
    // 旋转已缩放后的图像
    warpAffine( dstImage_warp, dstImage_warp_rotate, rotMat, dstImage_warp.size() );
    return dstImage_warp_rotate;
}
int hough_line(Mat src)
{
    //【1】载入原始图和Mat变量定义
    Mat srcImage = src;//imread("1.jpg");  //工程目录下应该有一张名为1.jpg的素材图
    Mat midImage,dstImage;//临时变量和目标图的定义

    //【2】进行边缘检测和转化为灰度图
    Canny(srcImage, midImage, 50, 200, 3);//进行一此canny边缘检测

    cvtColor(midImage,dstImage, CV_GRAY2BGR);//转化边缘检测后的图为灰度图

    //【3】进行霍夫线变换
    vector<Vec4i> lines;//定义一个矢量结构lines用于存放得到的线段矢量集合
    HoughLinesP(midImage, lines, 1, CV_PI/180, 80, 50, 10 );

    //【4】依次在图中绘制出每条线段
    for( size_t i = 0; i < lines.size(); i++ )
    {
        Vec4i l = lines[i];
        line( dstImage, Point(l[0], l[1]), Point(l[2], l[3]), Scalar(186,88,255), 1, CV_AA);
    }

    //【5】显示原始图

    imshow("【原始图】", srcImage);

    //【6】边缘检测后的图
    imshow("【边缘检测后的图】", midImage);

    //【7】显示效果图
    imshow("【效果图】", dstImage);

    //waitKey(0);

    return 0;
}

void project1::detectContours2()
{
    string image_name ="1.jpg";
    Mat image = imread(image_name,0);
    if (!image.data)
        return;





    // 从文件中加载原图
    IplImage *pSrcImage = cvLoadImage(image_name.data(), CV_LOAD_IMAGE_UNCHANGED);

    // 转为2值图

    //cvThreshold(pSrcImage,pSrcImage,200,255,THRESH_BINARY_INV);
    //namedWindow("Binary Image");
    //imshow("Binary Image",Mat(pSrcImage));

   // image = Mat(pSrcImage,true);

    //imwrite("binary.jpg",image);
    Mat result(image.size(),CV_8U,cv::Scalar(255));
    // Get the contours of the connected components
    std::vector<std::vector<Point>> contours;

    findContours(image,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours
    imshow("aaaa",image);
    qDebug()<<"size:"<<contours.size();
    return;
    // Print contours' length


    std::cout << "Contours: " << contours.size() << std::endl;
    std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
    RotatedRect     End_Rage2D1;

    for ( ; itContours!=contours.end(); ++itContours)
    {
        End_Rage2D1 = minAreaRect(Mat(itContours[0]));
        QString words=QString("%1").arg(End_Rage2D1.angle);
        putText( result, words.toStdString(), Point(End_Rage2D1.center.x-10,End_Rage2D1.center.y),CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255) );
        std::cout << "Size: " << itContours->size() << std::endl;

    }

    // draw black contours on white image

    drawContours(result,contours,
                 -1, // draw all contours
                 Scalar(0), // in black
                 2); // with a thickness of 2

    namedWindow("Contours");
    imshow("Contours",result);






    // Eliminate too short or too long contours
//    unsigned  int cmin= 10;  // minimum contour length
 //   unsigned int cmax= 1000; // maximum contour length
 //   std::vector<std::vector<Point>>::const_iterator itc= contours.begin();
//    while (itc!=contours.end()) {

//        if (itc->size() < cmin || itc->size() > cmax)
//            itc= contours.erase(itc);
//        else
//            ++itc;
//    }

    // draw contours on the original image
    Mat original= imread(image_name);
    drawContours(original,contours,
                 -1, // draw all contours
                 Scalar(255,255,0), // in white
                 2); // with a thickness of 2


    namedWindow("Contours on original");
    imshow("Contours on original",original);



    // Let's now draw black contours on white image

    result.setTo(Scalar(255));
    drawContours(result,contours,
                 -1, // draw all contours
                 Scalar(0), // in black
                 1); // with a thickness of 1
    image= imread("binary.jpg",0);

    //imshow("lll",result);
    //waitKey(0);

    // testing the bounding box
    //////////////////////////////////////////////////////////////////////////////
    //霍夫变换进行直线检测，此处使用的是probabilistic Hough transform（cv::HoughLinesP）而不是standard Hough transform（cv::HoughLines）

    Mat result_line(image.size(),CV_8U,Scalar(255));
    result_line = result.clone();


    hough_line(result_line);

    //Mat tempimage;

    //【2】进行边缘检测和转化为灰度图
    //Canny(result_line, tempimage, 50, 200, 3);//进行一此canny边缘检测
    //imshow("canny",tempimage);
    //waitKey(0);

    //cvtColor(tempimage,result_line, CV_GRAY2BGR);//转化边缘检测后的图为灰度图
    vector<Vec4i> lines;

    HoughLinesP(result_line,lines,1,CV_PI/180,80,50,10);

    for(unsigned int i = 0; i < lines.size(); i++)
    {
        line(result_line,Point(lines[i][0],lines[i][1]),Point(lines[i][2],lines[i][3]),Scalar(0,0,0),2,8,0);
    }
    namedWindow("line");
    imshow("line",result_line);


    CvBox2D     End_Rage2D;
    CvPoint2D32f rectpoint[4];
    CvMemStorage *storage = cvCreateMemStorage(0);  //开辟内存空间


    CvSeq*      contour = NULL;     //CvSeq类型 存放检测到的图像轮廓边缘所有的像素值，坐标值特征的结构体以链表形式

    cvFindContours( pSrcImage, storage, &contour, sizeof(CvContour),CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE);//这函数可选参数还有不少



    for(; contour; contour = contour->h_next)   //如果contour不为空，表示找到一个以上轮廓，这样写法只显示一个轮廓
        //如改为for(; contour; contour = contour->h_next) 就可以同时显示多个轮廓
    {

        End_Rage2D = cvMinAreaRect2(contour);

        //代入cvMinAreaRect2这个函数得到最小包围矩形  这里已得出被测物体的角度，宽度,高度，和中点坐标点存放在CvBox2D类型的结构体中，
        //主要工作基本//结束。
        cvBoxPoints(End_Rage2D,rectpoint);
        for(int i = 0;i< 4;i++)
        {
            //CvArr* s=(CvArr*)&result;
            //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
            line(result,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(125),2);
        }
\
        QString words=QString("%1").arg(End_Rage2D.angle);


        putText( result, words.toStdString(), Point(End_Rage2D.center.x-10,End_Rage2D.center.y),CV_FONT_HERSHEY_COMPLEX, 0.5, Scalar(0, 0, 255) );
        //qDebug()<<"x y :"<<End_Rage2D.center.x<<End_Rage2D.center.y;

       // std::cout <<" angle:\n"<<(float)End_Rage2D.angle << std::endl;      //被测物体旋转角度

    }


    imshow("lalalal",result);
}

int findMeterRing(Mat &src,IplImage *rgb,Point cen,int r,int thresh)
{
    int num=0;
    bool b[4]={0};
    for(int i=0;i<r;i++)
    {
//        cvCircle(rgb,cvPoint(cen.x,cen.y-r-i),1,cvScalar(255,0,0),1);
//        cvCircle(rgb,cvPoint(cen.x,cen.y+r+i),1,cvScalar(255,0,0),1);
//        cvCircle(rgb,cvPoint(cen.x-r-i,cen.y),1,cvScalar(255,0,0),1);
//        cvCircle(rgb,cvPoint(cen.x+r+i,cen.y),1,cvScalar(255,0,0),1);
//        qDebug()<<"aaa"<<src.at<uchar>(cen.y-r-i,cen.x)<<src.at<uchar>(cen.y+r+i,cen.x)<<src.at<uchar>(cen.y,cen.x-r-i)<<src.at<uchar>(cen.y,cen.x+r+i)<<"i:"<<i;


        if(src.at<uchar>(cen.y-r-i,cen.x)<=thresh)
        {
            b[0]=true;

        }
        if(src.at<uchar>(cen.y+r+i,cen.x)<=thresh)
        {
            b[1]=true;
        }
        if(src.at<uchar>(cen.y,cen.x-r-i)<=thresh)
        {
            b[2]=true;
        }
        if(src.at<uchar>(cen.y,cen.x+r+i)<=thresh)
        {
            b[3]=true;
        }
    }
    for(int i=0;i<4;i++)
    {
        if(b[i]==true)
            ++num;
    }
    qDebug()<<"num"<<num;
    return num;
}
bool neddleCirCen(Mat &src,Point cen,int grayVal)
{
    int flag=true;
    int x,y;
    int v1,v2,v3;

    x=cen.x;
    y=cen.y;
    for(int i=0;i<30;i++)
    {
        --y;
        v1=src.at<uchar>(y,x-1);
        v2=src.at<uchar>(y,x);
        v3=src.at<uchar>(y,x+1);

        if(v1<v2) --x;
        if(v3<v2) ++x;
        if(v1-grayVal>20&&v2-grayVal>20&&v3-grayVal>20)
            break;

        if(i>28)
        { flag=false;break;}
    }

    x=cen.x;
    y=cen.y;
    for(int i=0;i<30;i++)
    {
        ++y;
        v1=src.at<uchar>(y,x-1);
        v2=src.at<uchar>(y,x);
        v3=src.at<uchar>(y,x+1);

        if(v1<v2) --x;
        if(v3<v2) ++x;
        if(v1-grayVal>20&&v2-grayVal>20&&v3-grayVal>20)
            break;

        if(i>28)
        { flag=false;break;}
    }

    x=cen.x;
    y=cen.y;
    for(int i=0;i<30;i++)
    {
        --x;
        v1=src.at<uchar>(y-1,x);
        v2=src.at<uchar>(y,x);
        v3=src.at<uchar>(y+1,x);

        if(v1<v2) --y;
        if(v3<v2) ++y;
        if(v1-grayVal>20&&v2-grayVal>20&&v3-grayVal>20)
            break;

        if(i>28)
        { flag=false;break;}
    }

    x=cen.x;
    y=cen.y;
    for(int i=0;i<30;i++)
    {
        ++x;
        v1=src.at<uchar>(y-1,x);
        v2=src.at<uchar>(y,x);
        v3=src.at<uchar>(y+1,x);

        if(v1<v2) --y;
        if(v3<v2) ++y;
        if(v1-grayVal>20&&v2-grayVal>20&&v3-grayVal>20)
            break;

        if(i>28)
        { flag=false;break;}
    }

    return flag;
}
void project1::calcMeter1(IplImage* img,IplImage* rgb,bool &NG,int &offX,int &offY)
{

    //CvRect rect=cvRect(185,258,70,70);

    //IplImage *m1=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);

    cvCopy(img,gray640);
   //cvSetImageROI(m1,rect);
    Mat m3(img);

    int len1,len2,x1,y1,b1,b2;
    CvPoint2D32f circleCen;
    float circleR;
    CvRect boundRect;

    cvSmooth(gray640, gray640,CV_MEDIAN,3,3,0,0);

//   unsigned int minSize= 45;//圆孔最小size
//   unsigned int maxSize= 70;//圆孔最大size
//   int dif1=8;//长宽像素差
//   int brightDif= 60;//圆孔与圆环亮度差
//   int thresh=130;//二值
//   int r=17;//圆心至圆环半径
//   int cenX= 320;//-rect.x;//标定中心点X
//   int cenY= 240;//-rect.y;//标定中心点Y
//   int maxOffset=100;//最大偏移像素

    //小港测试
    unsigned int minSize= 80;//圆孔最小size
    unsigned int maxSize= 120;//圆孔最大size
    int dif1=20;//长宽像素差
    int brightDif= 20;//圆孔与圆环亮度差
    int thresh=130;//二值
    int r=36;//圆心至圆环半径
    int cenX= 320;//-rect.x;//标定中心点X
    int cenY= 240;//-rect.y;//标定中心点Y
    int maxOffset=26;//最大偏移像素


    bool loop=true;
   int rectMaxLen;
   double area;
   CvMemStorage *pStorage = NULL;
   pStorage = cvCreateMemStorage(0);
   for(int i=thresh;i>40;i-=5)
   //for(int i=thresh;i>125;i-=10)
   {

       if(!loop)
           break;
       cvThreshold(gray640,thresh640,i,255,THRESH_BINARY);
       CvSeq *pContour = NULL;


       CvPoint2D32f pt[4];




       cvFindContours(thresh640,pStorage, &pContour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

       CvBox2D box;
       CvFont font;

       cvInitFont(&font,CV_FONT_VECTOR0,1,1,0,1,8);

       for (; pContour != NULL; pContour = pContour->h_next)
       {
           box = cvMinAreaRect2(pContour,NULL);

           cvBoxPoints(box,pt);

           if(pContour->total<minSize||pContour->total>maxSize)
               continue;

           Point *p=(Point*)cvGetSeqElem(pContour,0);
           if(m3.at<uchar>(p->y-1,p->x)<i)
               continue;
           //qDebug()<<"m3.at<uchar>(p->y-1,p->x)"<<m3.at<uchar>(p->y-1,p->x)<<"i:"<<i;


           x1=box.center.x;
           y1=box.center.y;

           len1=common::getLineLen(pt[0],pt[1]);
           len2=common::getLineLen(pt[1],pt[2]);

           rectMaxLen=len1>len2?len1:len2;
           if(abs(len1-len2)>dif1)
               continue;



           if(y1+r>m3.rows||y1-r<0||x1+r>m3.cols||x1-r<0)
               continue;

           //亮度差计算
           b1=(m3.at<uchar>(y1-3,x1)+m3.at<uchar>(y1+3,x1)+m3.at<uchar>(y1,x1-3)+m3.at<uchar>(y1,x1+3))/4;
           b2=(m3.at<uchar>(y1-r,x1)+m3.at<uchar>(y1+r,x1)+m3.at<uchar>(y1,x1-r)+m3.at<uchar>(y1,x1+r))/4;

           cvCircle(rgb,cvPoint(x1,y1-r),2,cvScalar(0,255,0),1);
           cvCircle(rgb,cvPoint(x1,y1+r),2,cvScalar(0,255,0),1);
           cvCircle(rgb,cvPoint(x1-r,y1),2,cvScalar(0,255,0),1);
           cvCircle(rgb,cvPoint(x1+r,y1),2,cvScalar(0,255,0),1);

           qDebug()<<"b1 b2"<<b2<<b1<<brightDif<<"x y"<<x1<<y1<<"i:"<<i;
           if(b2<b1)
               continue;


           if(b2-b1<brightDif)
               continue;




           cvMinEnclosingCircle(pContour,&circleCen,&circleR);
           boundRect=cvBoundingRect(pContour);

           area=cvContourArea(pContour,CV_WHOLE_SEQ);
           if(len1*len2-area<50)
               continue;

           if(!neddleCirCen(m3,Point(x1,y1),b1))
               continue;

           //qDebug()<<"minRect xy"<<x1<<y1<<"minClosing"<<circleCen.x<<circleCen.y<<circleR<<"boundRect"<<boundRect.x+boundRect.width/2<<boundRect.y+boundRect.height/2;
           qDebug()<<"minRect area"<<len1*len2<<"obj area"<<area<<neddleCirCen(m3,Point(x1,y1),b1);
           //int num=findMeterRing(m3,rgb,Point(x1,y1),rectMaxLen/2+10,i);

//           if(num<3)
//               continue;
           //int num=circleFeature_c(pContour,Point(x1,y1),pContour->total/4,5,2);


        //   Point *p=(Point*)cvGetSeqElem(pContour,0);

          // qDebug()<<"point"<<p->x<<p->y;
//           for(int i = 0;i<4;++i){
//               cvLine(rgb,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(255,255,255));
//           }
           //cvDrawContours(rgb, pContour, CV_RGB(255,0,0), CV_RGB(0, 255, 0),0,2,0);

           cvPutText(rgb, QString::number(pContour->total).toLocal8Bit().data(), cvPoint(box.center.x,box.center.y), &font,CV_RGB(255, 0, 0) );
           //qDebug()<<"x1 y1 "<<x1<<y1;
           if(abs(x1-cenX)<maxOffset&&abs(y1-cenY)<maxOffset)
           {
               //qDebug()<<"num"<<num;
               NG=false;
               offX=cenX-x1;
               offY=cenY-y1;
               loop=false;
               qDebug()<<"i:"<<i;
               cvCircle(rgb,cvPoint(x1,y1),10,cvScalar(255,254,65),1);
               break;
           }
       }

   }
   cvLine(rgb,cvPoint(300,240),cvPoint(340,240),cvScalar(0,0,255));
   cvLine(rgb,cvPoint(320,220),cvPoint(320,260),cvScalar(0,0,255));
   cvReleaseMemStorage(&pStorage);
     pStorage = NULL;
     cvShowImage("img",rgb);

}

void project1::calcMeter2(IplImage* img,IplImage* rgb,bool &NG,int &offX,int &offY)
{
//黑色针
    //CvRect rect=cvRect(185,258,70,70);

    //IplImage *m1=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);

    cvCopy(img,gray640);
   //cvSetImageROI(m1,rect);
    Mat m3(img);

    int len1,len2,x1,y1,b1,b2;
    CvPoint2D32f circleCen;
    float circleR;
    CvRect boundRect;

    cvSmooth(gray640, gray640,CV_MEDIAN,3,3,0,0);

   unsigned int minSize= 60;//圆孔最小size
   unsigned int maxSize= 110;//圆孔最大size
   int dif1=11;//长宽像素差
   int brightDif= 50;//圆孔与圆环亮度差
   int thresh=160;//二值
   int r=25;//圆心至圆环半径
   int cenX= 320;//-rect.x;//标定中心点X
   int cenY= 240;//-rect.y;//标定中心点Y
   int maxOffset=100;//最大偏移像素

   bool loop=true;
   int rectMaxLen;
   double area;
   CvMemStorage *pStorage = NULL;
   pStorage = cvCreateMemStorage(0);
   for(int i=thresh;i>80;i-=5)
   //for(int i=thresh;i>125;i-=10)
   {
       if(!loop)
           break;
       cvThreshold(gray640,thresh640,i,255,THRESH_BINARY);
       CvSeq *pContour = NULL;


       CvPoint2D32f pt[4];




       cvFindContours(thresh640,pStorage, &pContour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

       CvBox2D box;
       CvFont font;

       cvInitFont(&font,CV_FONT_VECTOR0,1,1,0,1,8);

       for (; pContour != NULL; pContour = pContour->h_next)
       {
           box = cvMinAreaRect2(pContour,NULL);

           cvBoxPoints(box,pt);

           if(pContour->total<minSize||pContour->total>maxSize)
               continue;



           x1=box.center.x;
           y1=box.center.y;

           len1=common::getLineLen(pt[0],pt[1]);
           len2=common::getLineLen(pt[1],pt[2]);

           rectMaxLen=len1>len2?len1:len2;
           if(abs(len1-len2)>dif1)
               continue;



           if(y1+r>m3.rows||y1-r<0||x1+r>m3.cols||x1-r<0)
               continue;

           //亮度差计算
           b1=(m3.at<uchar>(y1-3,x1)+m3.at<uchar>(y1+3,x1)+m3.at<uchar>(y1,x1-3)+m3.at<uchar>(y1,x1+3))/4;
           b2=(m3.at<uchar>(y1-r,x1)+m3.at<uchar>(y1+r,x1)+m3.at<uchar>(y1,x1-r)+m3.at<uchar>(y1,x1+r))/4;

           cvCircle(rgb,cvPoint(x1,y1-r),2,cvScalar(0,255,0),1);
           cvCircle(rgb,cvPoint(x1,y1+r),2,cvScalar(0,255,0),1);
           cvCircle(rgb,cvPoint(x1-r,y1),2,cvScalar(0,255,0),1);
           cvCircle(rgb,cvPoint(x1+r,y1),2,cvScalar(0,255,0),1);

           qDebug()<<"b1 b2"<<b2<<b1<<brightDif<<"x y"<<x1<<y1<<"i:"<<i;
           if(b2-b1<brightDif)
               continue;


           //cvMinEnclosingCircle(pContour,&circleCen,&circleR);
           //boundRect=cvBoundingRect(pContour);

           area=cvContourArea(pContour,CV_WHOLE_SEQ);
           if(len1*len2-area<70)
               continue;

           //误识别处理
//           if(!neddleCirCen(m3,Point(x1,y1),b1))
//               continue;

           //qDebug()<<"minRect xy"<<x1<<y1<<"minClosing"<<circleCen.x<<circleCen.y<<circleR<<"boundRect"<<boundRect.x+boundRect.width/2<<boundRect.y+boundRect.height/2;
           qDebug()<<"minRect area"<<len1*len2<<"obj area"<<area;//<<neddleCirCen(m3,Point(x1,y1),b1);
           //int num=findMeterRing(m3,rgb,Point(x1,y1),rectMaxLen/2+10,i);

//           if(num<3)
//               continue;


        //   Point *p=(Point*)cvGetSeqElem(pContour,0);

          // qDebug()<<"point"<<p->x<<p->y;
//           for(int i = 0;i<4;++i){
//               cvLine(rgb,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(255,255,255));
//           }
           //cvDrawContours(rgb, pContour, CV_RGB(255,0,0), CV_RGB(0, 255, 0),0,2,0);

           cvPutText(rgb, QString::number(pContour->total).toLocal8Bit().data(), cvPoint(box.center.x,box.center.y), &font,CV_RGB(255, 0, 0) );
           //qDebug()<<"x1 y1 "<<x1<<y1;
           if(abs(x1-cenX)<maxOffset&&abs(y1-cenY)<maxOffset)
           {
               //qDebug()<<"num"<<num;
               NG=false;
               offX=cenX-x1;
               offY=cenY-y1;
               loop=false;
               qDebug()<<"i:"<<i;
               cvCircle(rgb,cvPoint(x1,y1),10,cvScalar(255,254,65),1);
               break;
           }
       }

   }
   cvLine(rgb,cvPoint(300,240),cvPoint(340,240),cvScalar(0,0,255));
   cvLine(rgb,cvPoint(320,220),cvPoint(320,260),cvScalar(0,0,255));
   cvReleaseMemStorage(&pStorage);
     pStorage = NULL;
     cvShowImage("img",rgb);

}

void project1::calcMeter3(IplImage* img,IplImage* rgb,bool &NG,int &offX,int &offY)
{
//找白色大圆
    //CvRect rect=cvRect(185,258,70,70);

    //IplImage *m1=cvCreateImage(cvGetSize(img),img->depth,img->nChannels);

    cvCopy(img,gray640);
   //cvSetImageROI(m1,rect);
    Mat m3(img);

    int len1,len2,x1,y1,b1,b2;
    CvPoint2D32f circleCen;
    float circleR;
    CvRect boundRect;

    cvSmooth(gray640, gray640,CV_MEDIAN,3,3,0,0);


    unsigned int minSize= 180;//圆孔最小size
    unsigned int maxSize= 270;//圆孔最大size
    int dif1=15;//长宽像素差
    //int brightDif= 20;//圆孔与圆环亮度差
    int thresh=160;//二值
    //int r=36;//圆心至圆环半径
    int cenX= 320;//-rect.x;//标定中心点X
    int cenY= 240;//-rect.y;//标定中心点Y
    int maxOffset=100;//最大偏移像素


    bool loop=true;
   int rectMaxLen;
   double area;
   CvMemStorage *pStorage = NULL;
   pStorage = cvCreateMemStorage(0);
   for(int i=90;i<thresh;i++)
   //for(int i=thresh;i>125;i-=10)
   {

       if(!loop)
           break;
       cvThreshold(gray640,thresh640,i,255,THRESH_BINARY);
       CvSeq *pContour = NULL;


       CvPoint2D32f pt[4];




       cvFindContours(thresh640,pStorage, &pContour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

       CvBox2D box;
       CvFont font;

       cvInitFont(&font,CV_FONT_VECTOR0,1,1,0,1,8);

       for (; pContour != NULL; pContour = pContour->h_next)
       {
           box = cvMinAreaRect2(pContour,NULL);

           cvBoxPoints(box,pt);

           if(pContour->total<minSize||pContour->total>maxSize)
               continue;

           Point *p=(Point*)cvGetSeqElem(pContour,0);
           if(m3.at<uchar>(p->y-1,p->x)>i)
               continue;


           x1=box.center.x;
           y1=box.center.y;

           len1=common::getLineLen(pt[0],pt[1]);
           len2=common::getLineLen(pt[1],pt[2]);

           rectMaxLen=len1>len2?len1:len2;
           if(abs(len1-len2)>dif1)
               continue;



//           if(y1+r>m3.rows||y1-r<0||x1+r>m3.cols||x1-r<0)
//               continue;




           //qDebug()<<"x1 y1 "<<x1<<y1;
           if(abs(x1-cenX)<maxOffset&&abs(y1-cenY)<maxOffset)
           {
               //qDebug()<<"num"<<num;
               NG=false;
               offX=cenX-x1;
               offY=cenY-y1;
               loop=false;
               qDebug()<<"i:"<<i;
               cvCircle(rgb,cvPoint(x1,y1),10,cvScalar(255,254,65),1);
               cvPutText(rgb, QString::number(pContour->total).toLocal8Bit().data(), cvPoint(box.center.x,box.center.y), &font,CV_RGB(255, 0, 0) );
               break;
           }
       }

   }


   cvLine(rgb,cvPoint(300,240),cvPoint(340,240),cvScalar(0,0,255));
   cvLine(rgb,cvPoint(320,220),cvPoint(320,260),cvScalar(0,0,255));
   cvReleaseMemStorage(&pStorage);
     pStorage = NULL;
     cvShowImage("img",rgb);

}
int findCup(Mat &src,int grayVal,bool isLeft,Point p1,Point &out)
{
    int ret=-1;

    int x1=0,y1=0,x2;

    int i1,pos1;

    if(!isLeft)
    {

        i1=-1;
    }
    else
    {

        i1=1;
    }

    for(int i=0;i<100;i++)
    {
        pos1=p1.x+i1*i;
        if(pos1>=src.cols-2||pos1<1)
            return ret;

        if( src.at<uchar>(p1.y,pos1)<grayVal&&src.at<uchar>(p1.y,pos1+i1)<grayVal&&src.at<uchar>(p1.y,pos1-i1)>grayVal&&src.at<uchar>(p1.y,pos1-i1*2)>grayVal)
        {

            x1=pos1;
            qDebug()<<"bbb"<<src.at<uchar>(p1.y,pos1);
            break;
        }

    }

    if(x1==0)
        return ret;

    x2=x1;
    if(abs(x1-p1.x)<50)
    {

        circle( g_dstImage2,Point(x1,p1.y), 5, Scalar(255,255,255), 2, CV_AA);
    }
    for(int i=p1.y;i>0;i--)
    {
        //qDebug()<<"x2"<<x2<<i;
        if(src.at<uchar>(i,x2-i1)>grayVal&&src.at<uchar>(i,x2-i1*2)>grayVal&&src.at<uchar>(i,x2)<grayVal&&src.at<uchar>(i,x2+i1)<grayVal)
        {
            continue;
        }
        else if(src.at<uchar>(i,x2-i1*2)>grayVal&&src.at<uchar>(i,x2-i1*3)>grayVal&&src.at<uchar>(i,x2)<grayVal&&src.at<uchar>(i,x2+i1)<grayVal)
            ++x2;
        else if(src.at<uchar>(i,x2)<grayVal&&src.at<uchar>(i,x2-i1)<grayVal&&src.at<uchar>(i,x2+i1*2)>grayVal&&src.at<uchar>(i,x2+i1*3)>grayVal)
            --x2;
        else
        {
            y1=i;
            qDebug()<<"aaa"<<src.at<uchar>(i,x2-i1)<<src.at<uchar>(i,x2-i1*2)<<src.at<uchar>(i,x2)<<src.at<uchar>(i,x2+i1)<<grayVal<<"i"<<i<<p1.y;

            break;
        }
    }

    if(abs(x2-x1)<40)
    {

        out.x=x2;
        out.y=y1;

        ret=0;
    }



    return ret;


}

int project1::icecream(Mat &src, Point p1,Point p2, int grayVal,Point &pos2)
{
    if(p1.x<0||p1.y<0||p2.x>src.cols||p2.y>src.rows)
        return 0;

    Point p3(0,0),p4(0,0),p5(0,0);
    int num=0;




    if(findCup(src,grayVal,false,p2,p4)==0)
    {

        circle( g_dstImage2,p4, 5, Scalar(255,255,255), 2, CV_AA);
    }

    if(findCup(src,grayVal,true,Point(p1.x,p2.y),p5)==0)
        circle( g_dstImage2,p5, 5, Scalar(255,255,255), 2, CV_AA);

    p3=p4.y<p5.y?p4:p5;



    for(int i=p1.y;i<p3.y;i++)
    {
        for(int j=p1.x;j<p3.x;j++)
        {
            if(src.at<uchar>(j,i)<grayVal)
                ++num;
        }
    }

    pos2=p3;
    return num;


}

std::vector<int> iceArr,upLineArr;
std::vector<Point> upLineXY;
int project1::icecream2(Mat &src, Rect &roi, int grayVal, Point &pos2)
{

    unsigned int objMinSize= 600;//检测字体最小值
    //unsigned int objMaxSize=200;//检测字体最大值
    int minLen=240;
    int x1,y1,len1,len2;

    Mat m5=src(roi);

    Mat m1,m2;

    threshold(m5,m1,grayVal,255,THRESH_BINARY_INV);
    threshold(m5,m2,grayVal,255,THRESH_BINARY_INV);
    std::vector<std::vector<Point>> contours;
    findContours(m1,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            qDebug()<<contours[k].size()<<"aaaa";
            if(contours[k].size()<objMinSize)
                continue;
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;

//            if(m5.at<uchar>(contours[k][0].y-1,contours[k][0].x)<grayVal)
//                continue;

            if(abs(roi.y-m5.at<uchar>(contours[k][0].y,contours[k][0].x))<5)
                continue;

            //qDebug()<<"aaa"<<contours[k].size();
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1<len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<minLen)
                continue;


            iceArr.clear();
            upLineArr.clear();
            upLineXY.clear();
            for(int i=20;i<contours[k].size();i+=20)
            {
                if(contours[k][i].y>roi.height-18)
                    continue;

                 iceArr.push_back(common::calcAngle1(contours[k][i],contours[k][i-20]));

                 upLineArr.push_back(m5.at<uchar>(contours[k][i].y-1,contours[k][i].x));

                 upLineXY.push_back(contours[k][i]);
                 //qDebug()<<"aaa"<<common::calcAngle1(contours[k][i],contours[k][i-20])<<"x y"<<contours[k][i].x<<contours[k][i].y;
                 circle(g_dstImage2,Point(roi.x+contours[k][i].x,roi.y+contours[k][i].y),3,Scalar(0,255,0),2);

            }

            for(int i=0;i<5&&iceArr.size()>5;i++)
            {
                iceArr.push_back(iceArr[i]);
                upLineArr.push_back(upLineArr[i]);
                upLineXY.push_back(upLineXY[i]);
            }


            int num=0,j;
            for(int i=1;i<iceArr.size();i++)
            {
                j=i;
                //if(j>=iceArr.size())
                //qDebug()<<"aaa"<<abs(iceArr[j]-iceArr[j-1]);
                if(abs(iceArr[j]-iceArr[j-1])<6)
                {
                    if(upLineArr[j]>grayVal)
                    {
                        ++num;
                        qDebug()<<"upLine"<<upLineArr[j]<<iceArr[j]<<"num"<<num;
                    }

                    if(num>12)
                    {

//                        findIndex=j*20;

//                        if(findIndex>contours[k].size())
//                            findIndex%=contours[k].size();

                        //pos2=contours[k][findIndex];
                        pos2.x=upLineXY[j].x+roi.x;
                        pos2.y=upLineXY[j].y+roi.y;
                        // circle(g_dstImage2,Point(roi.x+pos2.x,roi.y+pos2.y),5,Scalar(0,0,255),3);
                        qDebug()<<"pos2"<<pos2.x<<pos2.y<<"size"<<contours[k].size()<<upLineXY[j].x<<upLineXY[j].y;

                        imshow("m2",m2);
                        return num;
                    }

                }
                else
                {
                    num=0;
                }
            }

        }




    return 0;
}


int project1::icecream3(Mat &src, Rect &roi, int grayVal, Point &pos2)
{

    IplImage *grayImg=&IplImage(src);

    IplImage *m2Img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    IplImage *m1Img = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
    unsigned int objMinSize= 600;//检测字体最小值
    //unsigned int objMaxSize=200;//检测字体最大值
    int minLen=240;
    int x1,y1,len1,len2;

    Mat m5=src(roi);

    Mat m1,m2;

    // qDebug()<<"11"<<grayImg->depth<<grayImg->nSize<<grayImg->imageSize;
    cvCopy(grayImg,m2Img);


    cvSetImageROI(m2Img,cvRect(roi.x,roi.y,roi.width,roi.height));
    cvSetImageROI(m1Img,cvRect(roi.x,roi.y,roi.width,roi.height));
    cvThreshold(m2Img,m1Img,200,255,THRESH_BINARY_INV);
    CvMemStorage *pStorage = NULL;
    pStorage = cvCreateMemStorage(0);

    CvSeq *pContour = NULL;
    CvPoint2D32f rectpoint[4];
    CvBox2D box;
    cvFindContours(m1Img,pStorage, &pContour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     cvResetImageROI(m2Img);
    //printf("contours all:%d\n",pContour->total);
    for (; pContour != NULL; pContour = pContour->h_next)
    {
       // printf("size:%d\n",pContour->total);
        if(pContour->total<objMinSize)
            continue;

        box = cvMinAreaRect2(pContour,NULL);
        cvBoxPoints(box,rectpoint);



        Point *p0=(Point*)cvGetSeqElem(pContour,0);
//        if(m5.at<uchar>(p0->y-1,p0->x)<grayVal)
//            continue;

        if(abs(roi.y-m5.at<uchar>(p0->y,p0->x))<5)
            continue;

        len1=common::getLineLen(rectpoint[0],rectpoint[1]);
        len2=common::getLineLen(rectpoint[1],rectpoint[2]);

       // printf("bbb :len1 %d %d %d %d\n",len1,len2,rectpoint[0].x,rectpoint[0].y);
        if(len1<len2)
        {
            len1^=len2;
            len2^=len1;
            len1^=len2;
        }

        if(len1<minLen)
            continue;


        iceArr.clear();
        upLineArr.clear();
        upLineXY.clear();


        for(int i=20;i<pContour->total;i+=20)
        {
            CvPoint *pt = (CvPoint*) cvGetSeqElem(pContour, i);
            CvPoint *pt2 = (CvPoint*) cvGetSeqElem(pContour, i-20);
            if(pt->y>roi.height-18)
                continue;

            Point p1,p2;
            p1.x=pt->x;
            p1.y=pt->y;
            p2.x=pt2->x;
            p2.y=pt2->y;

            iceArr.push_back(common::calcAngle1(p1,p2));
            upLineArr.push_back(m5.at<uchar>(p1.y-1,p1.x));
            upLineXY.push_back(p1);


        }


        int size2=iceArr.size();
        for(int i=0;i<size2;i++)
        {
            iceArr.push_back(iceArr[i]);
            upLineArr.push_back(upLineArr[i]);
            upLineXY.push_back(upLineXY[i]);
        }

     //   circle(g_dstImage2,Point(upLineXY[0].x+roi.x,upLineXY[0].y+roi.y),3,Scalar(200,120,2),2);


        int num=0,j;
        for(int i=1;i<iceArr.size();i++)
        {
            j=i;

           // if(j==1)
                //circle(g_dstImage2,Point(upLineXY[j].x+roi.x,upLineXY[j].y+roi.y),3,Scalar(200,120,2),2);
//putText( g_dstImage2, QString("%1").arg(iceArr[j]).toStdString(),Point(upLineXY[j].x+roi.x,upLineXY[j].y+roi.y-10) ,CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
//qDebug()<<"j"<<j<<abs(iceArr[j]-iceArr[j-1]);
            //if(abs(iceArr[j]-iceArr[j-1])<8)
            if(common::angleDif(iceArr[j],iceArr[j-1],90,true)<8)
            {
                circle(g_dstImage2,Point(upLineXY[j].x+roi.x,upLineXY[j].y+roi.y),2,Scalar(200,120,2),2);
                //circle(g_dstImage2,Point(upLineXY[j].x+roi.x,upLineXY[j].y+roi.y),3,Scalar(200,120,2),2);
                if(upLineArr[j]>grayVal)
                    ++num;

                //printf("num:%d\n",num);
                if(num>10)
                {


                   // CvPoint* p3= (CvPoint*) cvGetSeqElem(pContour, findIndex);
                    pos2.x=upLineXY[j].x+roi.x;
                    pos2.y=upLineXY[j].y+roi.y;

                    cvReleaseMemStorage(&pStorage);
                    pStorage = NULL;

                    return num;
                }

            }
            else
            {
                circle(g_dstImage2,Point(upLineXY[j].x+roi.x,upLineXY[j].y+roi.y),2,Scalar(120,200,66),2);
                //qDebug()<<"aaa"<< common::angleDif(iceArr[j],iceArr[j-1],90,true)<<upLineXY[j].x+roi.x<<upLineXY[j].y+roi.y;
                putText( g_dstImage2, QString("%1").arg(common::angleDif(iceArr[j],iceArr[j-1],90,true)).toStdString(),Point(upLineXY[j].x+roi.x,upLineXY[j].y+roi.y-10) ,CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
                num=0;
            }
        }

    }
    cvReleaseMemStorage(&pStorage);
    pStorage = NULL;
    return 0;
}

void project1::find2Circle(IplImage *img, IplImage *rgb, Point &p1, Point &p2)
{
    //找黑圆

        cvCopy(img,gray640);
        Mat m3(img);
        int len1,len2,x1,y1;

        //cvSmooth(gray640, gray640,CV_MEDIAN,3,3,0,0);

        Point arr[100];

        unsigned int minSize= 20;//圆孔最小size
        unsigned int maxSize= 50;//圆孔最大size
        int dif1=3;//长宽像素差
        int maxLen=18;//圆最大直径
        int minDoorWid=90;//最小门长
        int offsetY=30;//两个圆Y偏移





        bool loop=true;
       int num;
       //double area;
       CvMemStorage *pStorage = NULL;
       pStorage = cvCreateMemStorage(0);
       for(int i=254;i>80;i-=10)
       {

           if(!loop)
               break;
           cvThreshold(gray640,thresh640,i,255,THRESH_BINARY);
           CvSeq *pContour = NULL;
           CvPoint2D32f pt[4];

           cvFindContours(thresh640,pStorage, &pContour,sizeof(CvContour),CV_RETR_LIST,CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

           CvBox2D box;
           //CvFont font;

           //cvInitFont(&font,CV_FONT_VECTOR0,1,1,0,1,8);
            num=0;
           for (; pContour != NULL; pContour = pContour->h_next)
           {
               box = cvMinAreaRect2(pContour,NULL);

               cvBoxPoints(box,pt);

               if(pContour->total<minSize||pContour->total>maxSize)
                   continue;

               Point *p=(Point*)cvGetSeqElem(pContour,0);
               if(m3.at<uchar>(p->y-1,p->x)<i)
                   continue;



               x1=box.center.x;
               y1=box.center.y;



             qDebug()<<"gray"<<m3.at<uchar>(y1-8,x1)<<m3.at<uchar>(y1+8,x1)<<m3.at<uchar>(y1,x1);
            //cvCircle(rgb,cvPoint(p->x,p->y),3,cvScalar(255,254,65),1);
               if(m3.at<uchar>(y1-8,x1)-m3.at<uchar>(y1,x1)<80||m3.at<uchar>(y1+8,x1)-m3.at<uchar>(y1,x1)<80)
                   continue;


               len1=common::getLineLen(pt[0],pt[1]);
               len2=common::getLineLen(pt[1],pt[2]);

               if(len1>maxLen||len2>maxLen)
                   continue;

               if(abs(len1-len2)>dif1)
                   continue;

            //cvCircle(rgb,cvPoint(p->x,p->y),3,cvScalar(255,254,65),1);
               qDebug()<<"len1"<<len1<<len2<<"i"<<i;
               ++num;

               if(num<100)
               {

                   arr[num-1]=Point(x1,y1);


               }
           }

           if(num>1)
           {
               for(int i=0;i<num;i++)
               {
                   for(int j=i+1;j<num;j++)
                   {
                       if(abs(arr[i].x-arr[j].x)>minDoorWid&&abs(arr[i].y-arr[j].y)<offsetY)
                       {
                           if(arr[i].x<arr[j].x)
                           {
                               p1=arr[i];
                               p2=arr[j];
                           }
                           else
                           {
                               p1=arr[j];
                               p2=arr[i];
                           }
                           qDebug()<<"found";
                           goto INITEND;
                       }
                   }
               }
           }

       }

INITEND:
       //cvLine(rgb,cvPoint(300,240),cvPoint(340,240),cvScalar(0,0,255));
       //cvLine(rgb,cvPoint(320,220),cvPoint(320,260),cvScalar(0,0,255));
       cvReleaseMemStorage(&pStorage);
         pStorage = NULL;

}

bool project1::elevatorInit(Mat &src, Mat &rgb)
{

    for(int i=550;i<630;i+=3)
    {
        for(int j=100;j<380;j+=3)
        {
            if(src.at<uchar>(j,i)==255&&src.at<uchar>(j,i+1)==255&&src.at<uchar>(j,i+2)==255)
            {
                if(src.at<uchar>(j,i-55)<80&&src.at<uchar>(j,i-70)<80&&src.at<uchar>(j,i-85)<80)
                {
                    return true;
                }

            }
        }
    }

    return false;


}
