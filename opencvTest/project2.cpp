#include "project2.h"
#include <QDebug>
#include <string.h>
#include <QTime>
#include <QFile>
#include "mainwindow.h"
int medianBlurVal=3;
bool useBlackH;
int mousePosX=0,mousePosY=0;
extern MainWindow *mainWindow;
project2::project2(QObject *parent) :
    QObject(parent)
{

    m_client=new QUdpSocket(this);
    m_client->bind(QHostAddress::Any,7766);

    connect(m_client,SIGNAL(readyRead()),this,SLOT(processUdp()));
    udpImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U, 1);
    rgbImg=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U, 3);
    sharememory = new QSharedMemory();              //构造实例对象
    sharememory->setKey("QSharedMemoryExample");    //为实例对象指定关键字(给共享内存命名)

    if (!sharememory->attach())     //关联共享内存
    {

        emit addLog("无法关联共享内存");

    }



}
bool isShowObj(std::vector<Point> &contours,Point p2)
{
    for(int i=0;i<contours.size();i+=3)
    {
        if(common::getLineLen(contours[i],p2)<4)
            return true;
    }
    return false;
}

void thresh_callback(int, void* )
{

    Mat m1,m3,m4;
    int len1,len2,allNum=0;
    Mat m2=g_dstImage2.clone();

    Point texting;
    int contoursGrayVal;

    double arcLen1;
    //line(g_dstImage1,Point(550-90,240),Point(550-90,720),Scalar(255,255,255),6);
    //line(g_dstImage1,Point(550+90,240),Point(550+90,720),Scalar(255,255,255),6);

//    int filterVal=10;
//    bilateralFilter(g_dstImage1, m1, filterVal, filterVal*2,filterVal/2 );

    //    //闭运算
    //    //偏移量的定义
    //    int offset = -10;//偏移量
    //    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //    //自定义核
    //    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //    //进行操作
    //    if( offset < 0 )
    //        morphologyEx(m1, m1, CV_MOP_OPEN, element);
    //    else
    //        morphologyEx(m1, m1, CV_MOP_CLOSE, element);



    Mat m5;
    qDebug()<<"aaa"<<g_dstImage3.cols;
    if(useBlackH)
    {
    qDebug()<<"offset"<<offsetSign;
    int offset = offsetSign;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(g_dstImage3, m5, MORPH_TOPHAT , element);
    else
        morphologyEx(g_dstImage3, m5, MORPH_BLACKHAT, element);
    }
    else
    {
        g_dstImage1.copyTo(m5);
    }



    //结束
    //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(m5,m4,thresh,255,THRESH_BINARY);
    threshold(m5,m3,thresh,255,THRESH_BINARY);

     imshow("Threshold",m3);
     std::vector<std::vector<Point>> contours;

     findContours(m4,
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
         if(contours[k].size()>30&&contours[k].size()<100)
         {
             ++allNum;
         }
        if(contours[k].size()<20)//||contours[k].size()>340)
             continue;

        if(contourP1.x&&contourP1.y&&!isShowObj(contours[k],contourP1))
            continue;


//        if(contours[k].size()>450)
//            circle(m2,contours[k][300],5,Scalar(255,0,0),3);

 //qDebug()<<"aaaa"<<contours[k][0].x<<contours[k][0].y;
        //contourFeature(contours[k]);


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);
         Point2f center;
         float radius;
         minEnclosingCircle(Mat(contours[k]), center, radius);//点集的最小外接圆


         //qDebug()<<"aaaa"<< pointPolygonTest(contours[k],Point(0,0),false);

         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

//         if(len2>70)
//             continue;
//         if(contours[k][0].y-1&&contours[k][0].x-1)
//            contoursGrayVal=m3.at<uchar>(contours[k][0].y-1,contours[k][0].x-1);

//         if(len1<480&&len2<480)
//             continue;
//         if(contours[k].size()>50)
//         {

//            circle( m2, Point(contours[k][50].x,contours[k][50].y ), 5, Scalar(0,0,255), 1, CV_AA);
//         }

         for(int i = 0;i< 4;i++)
         {
             if(abs(rectpoint[i].y-m2.cols)>50)
                 texting=rectpoint[i];
             //CvArr* s=(CvArr*)&result;
             //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
             line(m2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
         }

         QString words=QString("%1").arg(contours[k].size());


         arcLen1=arcLength(contours[k],true);
         if(mousePosX>0)
         {
             if(common::getLineLen(End_Rage2D1.center,Point(mousePosX,mousePosY))<40)
             {
                 putText( m2, words.toStdString(), texting,CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

                 qDebug()<<"arcLen"<<arcLen1<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2<<"areaRatio"<<area/len1/len2<<"radius"<<radius;//<<"contoursGrayVal"<<contoursGrayVal<<"4 point"<<rectpoint[0].x<<rectpoint[0].y<<rectpoint[1].x<<rectpoint[1].y<<rectpoint[2].x<<rectpoint[2].y<<rectpoint[3].x<<rectpoint[3].y<<"allNum"<<allNum;//<<"pos x y"<<contours[k][0].x<<contours[k][0].y<<contours[k][1].x<<contours[k][1].y<<contours[k][2].x<<contours[k][2].y<<contours[k][contours[k].size()-1].x<<contours[k][contours[k].size()-1].y<<contours[k][contours[k].size()-2].x<<contours[k][contours[k].size()-2].y;

             }
         }
         else
         {
             putText( m2, words.toStdString(), texting,CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
             if(contourP1.x&&contourP1.y)
                 mainWindow->showResult(QString("size:%1 len %2 %3 area:%4 radius:%5").arg(contours[k].size()).arg(len1).arg(len2).arg(area).arg(radius));
             qDebug()<<"arcLen"<<arcLen1<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2<<"radius"<<radius;//<<"contoursGrayVal"<<contoursGrayVal<<"4 point"<<rectpoint[0].x<<rectpoint[0].y<<rectpoint[1].x<<rectpoint[1].y<<rectpoint[2].x<<rectpoint[2].y<<rectpoint[3].x<<rectpoint[3].y<<"allNum"<<allNum;//<<"pos x y"<<contours[k][0].x<<contours[k][0].y<<contours[k][1].x<<contours[k][1].y<<contours[k][2].x<<contours[k][2].y<<contours[k][contours[k].size()-1].x<<contours[k][contours[k].size()-1].y<<contours[k][contours[k].size()-2].x<<contours[k][contours[k].size()-2].y;

         }

         //         for(int j=0;j<contours[k].size();j+=30)
//         {
//             circle( m2, Point(contours[k][j].x,contours[k][j].y ), 5, Scalar(0,0,255), 1, CV_AA);
//         }

      //circle( m2, Point(contours[k][10].x,contours[k][10].y ), 5, Scalar(193,255,3), 1, CV_AA);
     }

     // draw black contours on white image

     drawContours(m2,contours,
                  -1, // draw all contours
                  Scalar(125), // in black
                  1); // with a thickness of 2


     imshow("Contours",m2);


}


void onMouse2(int event, int x, int y,int, void* handle)
{



    switch (event)
    {
    case CV_EVENT_LBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度
        contourP1.x=x;
        contourP1.y=y;
        break;
    case CV_EVENT_RBUTTONDOWN:     //鼠标左键按下响应：返回坐标和灰度
        contourP1.x=0;
        contourP1.y=0;
        break;

    }
}

void project2::min_rect1()
{
    namedWindow( "Contours", 0 );
    resizeWindow("Contours",800,600);
    namedWindow( "Threshold", 0 );
    resizeWindow("Threshold",1280,960);
    cv::setMouseCallback("Contours",onMouse2,this);
    createTrackbar( " Threshold:", "Contours", &thresh, max_thresh, thresh_callback );
    thresh_callback( 0, 0 );
}
int calcLineGray(Mat &src,Mat &rgb,Point &pos,bool horizontal,int times)
{
    int ret=0;
    if(horizontal)
    {
        if(pos.x+times>=src.cols)
            return -1;

        for(int i=0;i<times;i++)
        {
            ret+=src.at<uchar>(pos.y,i+pos.x);
            //circle( rgb, Point(i+pos.x,pos.y), 3, Scalar(0,255,0), 1, CV_AA);

        }
    }
    else
    {
        if(pos.y+times>=src.rows)
            return -1;
        qDebug()<<"pos"<<pos.x<<pos.y;
        for(int i=0;i<times;i++)
        {
            ret+=src.at<uchar>(i+pos.y,pos.x);
           // circle( rgb, Point(pos.x,i+pos.y), 3, Scalar(0,255,0), 1, CV_AA);

        }
    }

    return ret/times;
}

int getIndex(int allNum,int j,int step)
{
    int index1=j+step;
    if(index1>=allNum)
    {
        index1-=allNum;
    }
    if(index1<0)
    {
        //index1=allNum-abs(j-abs(step));
        index1+=allNum;
    }
    return index1;
}
Point shimCentre(Mat &src,Rect roi,params &param)
{
    Mat m1=src.clone();
    int len1,len2,leftPoint,j,index1,index2,index3,index4;



    int nearLen,nearIndex,temp1;
    Point cen,p1,p2,p3;
    Point setCen(param.var1[0]-roi.x,param.var1[1]-roi.y);
    int thresh=param.var1[2];//二值
    int minSize=param.var1[3];//圆心阴影size
    int maxSize=param.var1[4];
    int step=param.var1[5];
    int offset1=param.var1[6];//圆心偏离最大值


    Point p(0,0);

    threshold(m1,m1,thresh,255,THRESH_BINARY);

     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     //std::cout << "Contours: " << contours.size() << std::endl;

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

         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

//         if(abs(len1-len2)>90)
//             continue;



         leftPoint=2000;
         for(int i=0;i<contours[k].size();i++)
         {
             if(contours[k][i].x<leftPoint)
             {
                 leftPoint=contours[k][i].x;
                 j=i;
             }
         }
         index1=getIndex(contours[k].size(),j,step);
         index2=getIndex(contours[k].size(),j,-step);
         step= contours[k][index1].y<contours[k][index2].y?step:-step;
         index3=getIndex(contours[k].size(),j,step);
         index4=getIndex(contours[k].size(),j,step*2);

         p1.x=contours[k][j].x;
         p1.y=contours[k][j].y;
         p2.x=contours[k][index3].x;
         p2.y=contours[k][index3].y;
         p3.x=contours[k][index4].x;
         p3.y=contours[k][index4].y;
         bool success=common::getCircleCenter(p1,p2,p3,cen);

         if(!success)
             continue;

         if(abs(setCen.x-cen.x)<offset1&&abs(setCen.y-cen.y)<offset1)
         {

             p=cen;
             circle( g_dstImage2, Point(p1.x+roi.x,p1.y+roi.y ), 3, Scalar(255,0,0), 1, CV_AA);
             circle( g_dstImage2, Point(p2.x+roi.x,p2.y+roi.y ), 3, Scalar(255,0,0), 1, CV_AA);
             circle( g_dstImage2, Point(p3.x+roi.x,p3.y+roi.y ), 3, Scalar(255,0,0), 1, CV_AA);

             circle( g_dstImage2, Point(cen.x+roi.x,cen.y+roi.y ), 3, Scalar(0,255,0), 1, CV_AA);
//             nearLen=1000;
//             for(int j1=0;j1<contours[k].size();j1++)
//             {
//                 temp1=common::getLineLen(cen,contours[k][j1]);
//                 if(temp1<nearLen)
//                 {
//                     nearLen=temp1;
//                     nearIndex=j1;
//                 }
//             }


//             circle( g_dstImage2,Point(contours[k][nearIndex].x+roi.x,contours[k][nearIndex].y+roi.y), 3, Scalar(0,255,0), 1, CV_AA);
         }



//         for(int i = 0;i< 4;i++)
//         {
             //CvArr* s=(CvArr*)&result;
             //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
             //line(g_dstImage2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);


//                 if(rectpoint[i].x==contours[k][j1].x&&rectpoint[i].y==contours[k][j1].y)
//                     ;
                     //qDebug()<<"x y"<<rectpoint[i].x<<rectpoint[i].y;
                     //circle( g_dstImage2, rectpoint[i], 3, Scalar(0,255,0), 1, CV_AA);

   //      }

        // QString words=QString("%1").arg(contours[k].size());
        // putText( g_dstImage2, words.toStdString(), Point(End_Rage2D1.center.x-10,End_Rage2D1.center.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
         //qDebug()<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2;
     }

     // draw black contours on white image

//     drawContours(g_dstImage2,contours,
//                  -1, // draw all contours
//                  Scalar(125), // in black
//                  1); // with a thickness of 2


     return p;
}

Point shimCentre2(Mat &src,Rect roi,params &param)
{
    Mat m3=src.clone();
    Mat m1;
    int len1,len2;

    Point setCen(param.var1[0]-roi.x,param.var1[1]-roi.y);
    int i=param.var1[2];//二值
    int minThresh=param.var1[3];
    int minSize=param.var1[4];//圆心阴影size
    int maxSize=param.var1[5];
    int offset1=param.var1[6];//圆心偏离最大值
    int minLen= param.var1[7]=380;//rectLen
    int maxLen= param.var1[8]=405;


    bool loop=true;

    Point p(0,0);
    for(;i>minThresh;i-=5)
    {

        if(!loop)
            break;

    threshold(m3,m1,i,255,THRESH_BINARY);

     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     //std::cout << "Contours: " << contours.size() << std::endl;

     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     //for ( ; itContours!=contours.end(); ++itContours)
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
        if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;

         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);


         if(len1>len2)
         {
             len1^=len2;
             len2^=len1;
             len1^=len2;
         }
         if(len1<minLen||len2>maxLen)
         {
             if(DEBUG) qDebug()<<"len1<minLen||len2>maxLen"<<len1;
             continue;
         }



         if(abs(setCen.x-End_Rage2D1.center.x)<offset1&&abs(setCen.y-End_Rage2D1.center.y)<offset1)
         {
             p=End_Rage2D1.center;
             if(DEBUG)
             {
                 circle( g_dstImage2, End_Rage2D1.center, 3, Scalar(255,0,0), 2, CV_AA);

                qDebug()<<"circle cen i"<<i;
             }
             loop=false;
             break;
         }





     }


    }

     return p;
}







void shimDetection(Mat &src,Rect roi,params &param,bool blackHat)
{
    Mat m1=src.clone();
    Mat m3;
    int j,index1,index2,step,x1,y1;
    int nearLen,temp1,minRLen,len,len1,len2;

    Point cen1(param.var1[7],param.var1[8]);
    int minR1=param.var1[9];//圆环上小圆半径范围
    int minR2=param.var1[10];//圆环上小圆半径范围
    int rLen1=param.var1[11];//小圆环到圆心距离最小
    int rLen2=param.var1[12];//小圆环到圆心距离最大
    int size1=param.var1[13];//小圆size范围最小
    int size2=param.var1[14];//小圆size范围最大
    int thresh=param.var1[15];//二值最大
    int allnum=param.var1[16];//所有数量
    int thresh2=param.var1[17];//二值最小
    int cirLen1=param.var1[18];//小圆边离圆心最小距离
    int cirLen2=param.var1[19];//小圆边离圆心最大距离
    //out param
    param.var1[29]=0;


    Point cen,p1,p2,p3;

    bool loop=true;


    int offset = -10;//偏移量
    if(blackHat)
        offset=10;
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m1, m1, MORPH_TOPHAT , element);
    else
        morphologyEx(m1, m1, MORPH_BLACKHAT, element);




    //for(int i=37;i>36;i--)
    for( int i=thresh;i>thresh2;i--)
    {
        //qDebug()<<"i "<<i;
        if(!loop)
        {

            break;
        }
        threshold(m1,m3,i,255,THRESH_BINARY);

        imshow("m3",m3);
        std::vector<std::vector<Point>> contours;

        findContours(m3,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        // Print contours' length
        //std::cout << "Contours: " << contours.size() << std::endl;

        if(contours.size()>allnum)
            break;
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        double area;
        //for ( ; itContours!=contours.end(); ++itContours)
        param.var1[28]=0;
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<size1||contours[k].size()>size2)
                continue;




            step=(contours[k].size()-5)/4;
            //contourFeature(contours[k]);


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            area=contourArea(contours[k]);
            cvBoxPoints(End_Rage2D1,rectpoint);

            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            //         if(abs(len1-len2)>90)
            //             continue;


            nearLen=1000;
            for(int j1=0;j1<contours[k].size();j1++)
            {
                temp1=common::getLineLen(Point(x1,y1),contours[k][j1]);
                if(temp1<nearLen)
                {
                    nearLen=temp1;
                    j=j1;

                }
            }
            //circle( g_dstImage2, contours[k][nearIndex], 3, Scalar(0,255,0), 1, CV_AA);



            j=getIndex(contours[k].size(),j,contours[k].size()/2);

            index1=getIndex(contours[k].size(),j,step);
            index2=getIndex(contours[k].size(),j,-step);

            p1.x=contours[k][j].x;
            p1.y=contours[k][j].y;
            p2.x=contours[k][index1].x;
            p2.y=contours[k][index1].y;
            p3.x=contours[k][index2].x;
            p3.y=contours[k][index2].y;


            // qDebug()<<"aaaa"<<x1+roi.x<<y1+roi.y<<p2.x+roi.x<<p2.y+roi.y<<p3.x+roi.x<<p3.y+roi.y<<"index1"<<index1<<"index2"<<index2<<"j"<<j<<"step"<<step<<"size"<<contours[k].size();
            bool success=common::getCircleCenter(p1,p2,p3,cen);

            if(!success)
                continue;





            //小圆半径
            minRLen=common::getLineLen(p1,cen);
            len=common::getLineLen(cen1,cen);
           // qDebug()<<"len"<<len<<"i"<<i<<contours[k].size();
            if(len<rLen1||len>rLen2)
                continue;
            //qDebug()<<"found"<<"Contours: " << contours.size()<<"i:"<<i<<"size"<<contours[k].size();
           // qDebug()<<minRLen<<minR1<<minR2<<"aaaaaa";


            if(minRLen>minR1&&minRLen<minR2)
            {
                if(!blackHat)
                {
                //排除30至-130度之间错误判断
                if(common::calcAngle2(cen1,p1)>-130&&common::calcAngle2(cen1,p1)<30)
                {
                    if(common::getLineLen(cen1,p1)>rLen2)
                    {
                        continue;
                    }
                    if(len1>len2)
                    {
                        if(common::calcAngle(rectpoint[0],rectpoint[1])>0)
                        {
                            loop=false;
                            param.var1[29]=0;
                            break;
                        }
                    }
                    else
                    {
                        if(common::calcAngle(rectpoint[1],rectpoint[2])>0)
                        {
                            loop=false;
                            param.var1[29]=0;
                            break;
                        }
                    }

                }
                //end

                //排除90-180度之间错误判断
                if(common::calcAngle2(cen1,p1)>90&&common::calcAngle2(cen1,p1)<180)
                {
                    //circle( g_dstImage2, Point(cen.x+roi.x,cen.y+roi.y), common::getLineLen(cen,p1), Scalar(0,255,0), 1, CV_AA);
                    //qDebug()<<"aaaa"<<common::getLineLen(cen1,p1)<<rLen1;
                    if(common::getLineLen(cen1,p1)<cirLen1)
                    {
                        continue;
                    }
                }

                }
                else
                {
                    if(common::getLineLen(cen1,p1)<cirLen1||common::getLineLen(cen1,p1)>cirLen2)
                    {
                        continue;
                    }
                }
                //end
                circle( g_dstImage2, Point(p1.x+roi.x,p1.y+roi.y), 3, Scalar(255,0,0), 1, CV_AA);
                circle( g_dstImage2, Point(p2.x+roi.x,p2.y+roi.y), 3, Scalar(255,255,0), 1, CV_AA);
                circle( g_dstImage2, Point(p3.x+roi.x,p3.y+roi.y), 3, Scalar(255,255,0), 1, CV_AA);


                ++param.var1[28];
                if(param.var1[28]>=5)
                {
                    loop=false;
                    param.var1[29]=0;

                    break;
                }

                if(common::circleFeature(contours[k],cen,step,5,10)<1)
                    continue;


                drawContours(g_dstImage2,contours,
                             -1, // draw all contours
                             Scalar(125), // in black
                             1); // with a thickness of 2
                QString words=QString("%1").arg(minRLen);
                putText( g_dstImage2, words.toStdString(), Point(cen.x+roi.x-10,cen.y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

                circle( g_dstImage2, Point(cen.x+roi.x,cen.y+roi.y), common::getLineLen(cen,p1), Scalar(0,255,0), 1, CV_AA);


                qDebug()<<"x1 y1"<<x1+roi.x<<y1+roi.y;
                qDebug()<<"found"<<"Contours: " << contours.size()<<"i:"<<i<<"size"<<contours[k].size()<<"feature"<<common::circleFeature(contours[k],cen,step,5,10);
                loop=false;
                param.var1[29]=1;



                break;
            }


            //         for(int i = 0;i< 4;i++)
            //         {
            //CvArr* s=(CvArr*)&result;
            //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
            //line(g_dstImage2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
            //circle( g_dstImage2, rectpoint[i], 3, Scalar(0,255,0), 1, CV_AA);
            //      }

            // QString words=QString("%1").arg(contours[k].size());
            // putText( g_dstImage2, words.toStdString(), Point(End_Rage2D1.center.x-10,End_Rage2D1.center.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
            // qDebug()<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2;
        }


    }
//loop is end


}

int minDistanceNum(std::vector<Point> &contours,int step,int len)
{
    int num=0;

    for(int i=step;i<contours.size();i+=step)
    {
        if(common::getLineLen1(contours[i-step],contours[i])<len)
            ++num;

    }
    return num;
}

bool delegateAngle(int angle1,int angle2,int angleDif)
{
    return common::angleDif(angle1,angle2,90,1)>angleDif;
}
typedef bool (*compare1)(int angle1,int angle2,int angleDif);
compare1 callback1=delegateAngle;
int compareAngleNum(std::vector<Point> &contours,int step,int angle,int angleDif)
{
    int num=0,temp;



    for(int j=step;j<contours.size();j+=step)
    {
        temp= common::calcAngle1(contours[j-step],contours[j]);

        if(callback1(temp,angle,angleDif))
            ++num;
    }



    return num;
}

void shimDetection2(Mat &src,Rect roi,params &param,bool blackHat)
{
    Mat m1=src.clone();
    Mat m3,m4;
    int j,index1,index2,step,x1,y1;
    int nearLen,temp1,minRLen,len,len1,len2,allnum1,len3,p1toCen,p2toCen,p3toCen,angle10;
    int cirFeatureNum=1,cirFeaVal,s_level;

    Point cen1(param.var1[25],param.var1[26]);
    int minR1=param.var1[9];//圆环上小圆半径范围
    int minR2=param.var1[10];//圆环上小圆半径范围
    int rLen1=param.var1[11];//小圆环到圆心距离最小
    int rLen2=param.var1[12];//小圆环到圆心距离最大
    int size1=param.var1[13];//小圆size范围最小
    int size2=param.var1[14];//小圆size范围最大
    int thresh=param.var1[15];//二值最大
    int allnum=param.var1[16];//所有数量
    int thresh2=param.var1[17];//二值最小
    int cirLen1=param.var1[18];//小圆边离圆心最小距离
    int cirLen2=param.var1[19];//小圆边离圆心最大距离
    int size3=param.var1[20];//噪点数量

    //out param
    param.var1[29]=0;
    param.var1[28]=0;
    param.var1[27]=0;


    Point cen,p1,p2,p3,foundCen,foundCirContour;

    bool loop=true;


    int offset = -10;//偏移量
    if(blackHat)
        offset=10;
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m1, m1, MORPH_TOPHAT , element);
    else
        morphologyEx(m1, m1, MORPH_BLACKHAT, element);




    //for(int i=110;i>109;i--)
    for( int i=thresh;i>thresh2;i--)
    {
        if(i>60) i-=9;
        if(i>40&&i<60) i-=5;
        allnum1=0;
       // qDebug()<<"i "<<i;
        if(!loop)
        {
            break;
        }
        threshold(m1,m3,i,255,THRESH_BINARY);

        m4=m3.clone();
        if(DEBUG) imshow("m4",m4);
        std::vector<std::vector<Point>> contours;

        findContours(m3,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        // Print contours' length
        //std::cout << "Contours: " << contours.size() << std::endl;


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        double area;
        //for ( ; itContours!=contours.end(); ++itContours)
        param.var1[28]=0;
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<size3)
                continue;





            step=(contours[k].size()-5)/4;
            //contourFeature(contours[k]);


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            area=contourArea(contours[k]);
            cvBoxPoints(End_Rage2D1,rectpoint);

            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;

            len3=common::getLineLen(cen1,Point(x1,y1));

            if(len3<cirLen1-50||len3>cirLen2)
                continue;



                allnum1++;

                if(allnum1>allnum)
                {
                    if(DEBUG) qDebug()<<"污点太多";
                    return;

                }

            if(contours[k].size()<size1||contours[k].size()>size2)
            {

                continue;
            }

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);


            if(len1>len2)
                angle10=common::calcAngle(rectpoint[0],rectpoint[1]);
            else
                angle10=common::calcAngle(rectpoint[1],rectpoint[2]);
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }
            if(len1<18)
            {
               if(DEBUG) qDebug()<<"len1<18";
                continue;
            }


            nearLen=1000;
            for(int j1=0;j1<contours[k].size();j1++)
            {
                temp1=common::getLineLen(Point(x1,y1),contours[k][j1]);
                if(temp1<nearLen)
                {
                    nearLen=temp1;
                    j=j1;

                }
            }
            //circle( g_dstImage2, contours[k][nearIndex], 3, Scalar(0,255,0), 1, CV_AA);



            j=getIndex(contours[k].size(),j,contours[k].size()/2);

            index1=getIndex(contours[k].size(),j,step);
            index2=getIndex(contours[k].size(),j,-step);

            p1.x=contours[k][j].x;
            p1.y=contours[k][j].y;
            p2.x=contours[k][index1].x;
            p2.y=contours[k][index1].y;
            p3.x=contours[k][index2].x;
            p3.y=contours[k][index2].y;




            // qDebug()<<"aaaa"<<x1+roi.x<<y1+roi.y<<p2.x+roi.x<<p2.y+roi.y<<p3.x+roi.x<<p3.y+roi.y<<"index1"<<index1<<"index2"<<index2<<"j"<<j<<"step"<<step<<"size"<<contours[k].size();
            bool success=common::getCircleCenter(p1,p2,p3,cen);

            if(!success)
            {
                if(DEBUG) qDebug()<<"!success";
                continue;
            }


           // qDebug()<<"aaa"<<allnum1<<common::getLineLen(cen1,p1)<<cirLen1<<cirLen2;

            p1toCen=common::getLineLen(cen1,p1);
            p2toCen=common::getLineLen(cen1,p2);
            p3toCen=common::getLineLen(cen1,p3);

            //if((p1toCen+p2toCen+p3toCen)/3>)

            if(p1toCen<cirLen1||p1toCen>cirLen2)
            {
                if(DEBUG) qDebug()<<"p1toCen<cirLen1||p1toCen>cirLen2"<<cirLen1<<cirLen2<<p1toCen;
                continue;
            }


            //小圆半径
            minRLen=common::getLineLen(p1,cen);
            len=common::getLineLen(cen1,cen);
            //qDebug()<<"len"<<len<<"rLen1"<<rLen1<<rLen2;
            if(len<rLen1||len>rLen2)
            {
                if(DEBUG) qDebug()<<"len<rLen1||len>rLen2"<<len<<rLen1<<rLen2;
                continue;
            }
            //qDebug()<<"found"<<"Contours: " << contours.size()<<"i:"<<i<<"size"<<contours[k].size();
            //qDebug()<<"minRLen"<<minRLen<<minR1<<minR2;


            if(minRLen<minR1||minRLen>minR2)
            {
                if(DEBUG) qDebug()<<"minRLen<minR1||minRLen>minR2"<<minRLen;
                continue;
            }

            if(DEBUG)
            {
                circle( g_dstImage2, p1, 3, Scalar(255,0,0), 1, CV_AA);
                circle( g_dstImage2, p2, 3, Scalar(255,255,0), 1, CV_AA);
                circle( g_dstImage2, p3, 3, Scalar(255,255,0), 1, CV_AA);
            }

            if(p2toCen>cirLen2-5&&p3toCen>cirLen2-5||p1toCen>cirLen2-5&&p3toCen>cirLen2-5||p1toCen>cirLen2-5&&p2toCen>cirLen2-5)
            {
               if(DEBUG) qDebug()<<"p2 p3 abnormal";
                continue;
            }



            //qDebug()<<"circleFeature"<<cirFeaVal<<"cirFeatureNum";
            cirFeaVal=common::circleFeature(contours[k],cen,step,5,11);
            if(cirFeaVal<cirFeatureNum)
            {
                if(DEBUG) qDebug()<<"cirFeaVal<cirFeatureNum";
                continue;
            }

            int minDisNum=minDistanceNum(contours[k],20,17);

            s_level=common::Smoothlevel(contours[k],15,30,0);
            int angleNum=compareAngleNum(contours[k],10,angle10,45);


            if(len1<40)
            {
                if(angleNum>10&&minDisNum>10)
                {
                    if(DEBUG) qDebug()<<"angleNum>10&&minDisNum>10";
                    continue;
                }

                if(s_level>4&&minDisNum>4)
                {
                    if(DEBUG) qDebug()<<"s_level>4&&minDisNum>4";
                    continue;
                }
            }

            if(angleNum>20)
            {
                if(DEBUG) qDebug()<<"angleNum>20";
                continue;
            }





             if(DEBUG)
             {
            drawContours(g_dstImage2,contours,
                         -1, // draw all contours
                         Scalar(125), // in black
                         1); // with a thickness of 2
             }
            QString words=QString("%1").arg(minRLen);





            if(loop)
            {
                foundCen=cen;
                foundCirContour=p1;
                param.var1[27]=cirFeaVal;
                param.var1[28]=s_level;

                loop=false;
                if(DEBUG)
                {

                    qDebug()<<"angleNum"<<angleNum<<"minDisNum"<<minDisNum;



                    qDebug()<<"x1 y1"<<x1+roi.x<<y1+roi.y<<"max distance"<<p1toCen<<p2toCen<<p3toCen<<"circleFeature2"<< param.var1[28];
                    qDebug()<<"area"<<area<<"size:"<<contours[k].size()<<"len1 len2: " <<len1<<len2<<len1*len2<<"i:"<<i<<"feature"<<param.var1[27];

                    putText( g_dstImage2, words.toStdString(), Point(foundCen.x-10,foundCen.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

                }
            }



        }
        if(!loop)
        {
             if(DEBUG) qDebug()<<"allNum"<<allnum1;
             if(param.var1[28]>5&&param.var1[27]<2)
             {
                 if(DEBUG) qDebug()<<"param.var1[28]>5&&param.var1[27]<2";
                 return ;
             }
             else if(param.var1[28]>40&&param.var1[27]<2)
             {
                 if(DEBUG) qDebug()<<"param.var1[28]>40&&param.var1[27]<2";
                 return;
             }
             else if(allnum1>90&&param.var1[27]<4)
             {
                 if(DEBUG) qDebug()<<"allnum1>90&&param.var1[27]<4";
                 return;
             }
             else if(allnum1>60&&param.var1[27]<3)
             {
                 if(DEBUG) qDebug()<<"allnum1>60&&param.var1[27]<3";
                 return;
             }
             else if(allnum1>40&&param.var1[27]<2)
             {
                 if(DEBUG) qDebug()<<"allnum1>40&&param.var1[27]<2";
                 return;
             }


            param.var1[29]=1;


             if(DEBUG)
                 circle( g_dstImage2, foundCen, common::getLineLen(foundCen,foundCirContour), Scalar(0,255,0), 1, CV_AA);


        }


    }
//loop is end


}



void HD_circleCenter(Mat &src,Rect roi,params &param)
{

    Mat m1;
    int len1,len2,len3,x1,y1,i;



    int minRectLen=param.var1[0];//最小矩形长度
    int maxRectLen=param.var1[1];//最大矩形长度
    int minSize=param.var1[2];//size范围
    int thresh=param.var1[3];


    //out param
    param.var1[28]=0;
    param.var1[29]=0;
    bool loop = true;


    for(i=thresh;i<60;i+=5)
    {
        if(!loop)
            break;
     threshold(src,m1,i,255,THRESH_BINARY);


     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;

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

         if(abs(len1-len2)>20)
             continue;


         if(len1>minRectLen&&len1<maxRectLen&&len2>minRectLen&&len2<maxRectLen)
         {
             param.var1[28]=x1;
             param.var1[29]=y1;
             for(int i1 = 0;i1< 4;i1++)
             {
                 line(g_dstImage2,cvPointFrom32f(rectpoint[i1]),cvPointFrom32f(rectpoint[(i1+1)%4]),Scalar(0,255,0),1);
             }

             QString words=QString("%1").arg(contours[k].size());

             putText( g_dstImage2, words.toStdString(), Point(End_Rage2D1.center.x-10+roi.x,End_Rage2D1.center.y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

             loop=false;
             qDebug()<<"i:"<<i;
             break;
         }


     }



    }


}

void calcWork1Angle(Mat &src,Rect roi,params &param)
{

    Mat m1;

    int len1,len2,len3,len4,x1,y1,i;
    double angle1,angle,angle2;



    int minRectLen=param.var1[10];//最小矩形长度
    int maxRectLen=param.var1[11];//最大矩形长度
    int minSize=param.var1[12];//size范围

    int minLen= param.var1[13];//离中心点长度
    int maxLen= param.var1[14];
    int thresh= param.var1[15];

    int cenX=param.var1[28];
    int cenY=param.var1[29];
    double angleOffset=param.var2[0];//角度偏移

    //int maxSize=param.var1[2];

    //out param
    param.var2[19]=500;

    bool loop = true;

    Point p1,p2;


//for(i=25;i<26;i++)
    for(i=thresh;i<60;i++)
    {
        if(!loop)
            break;
     threshold(src,m1,i,255,THRESH_BINARY);

     imshow("Threshold1",m1);
     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     //std::cout << "Contours: " << contours.size() << std::endl;
     //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     qDebug()<<"contours.size()"<<contours.size();
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
        if(contours[k].size()<minSize)
             continue;

        //contourFeature(contours[k]);


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;
         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

         len3=len1>len2?len1:len2;


         len4=common::getLineLen(Point(cenX,cenY),Point(x1,y1));
        // qDebug()<<"bbbb"<<len4<<minLen<<maxLen<<"size"<<contours[k].size()<<"i"<<i;
         if(len4<minLen||len4>maxLen)
             continue;

         if(len1>len2)
         {
             angle2=common::calcAngle(rectpoint[1],rectpoint[2]);
         }
         else
         {
             angle2=common::calcAngle(rectpoint[0],rectpoint[1]);
         }
         if(abs(angle2-common::calcAngle(Point(cenX,cenY),Point(x1,y1)))>10)
             continue;
         //qDebug()<<"len1"<<len1<<"len2"<<len2<<minRectLen<<maxRectLen<<"x y"<<x1<<y1<<"size"<<contours[k].size()<<"i:"<<i;
         if(len3>minRectLen&&len3<maxRectLen)
         {

             for(int i1 = 0;i1< 4;i1++)
             {
                 line(g_dstImage2,cvPointFrom32f(rectpoint[i1]),cvPointFrom32f(rectpoint[(i1+1)%4]),Scalar(0,255,0),1);
             }

//             QString words=QString("%1").arg(contours[k].size());

//             putText( g_dstImage2, words.toStdString(), Point(End_Rage2D1.center.x-10+roi.x,End_Rage2D1.center.y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );



             angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));

             loop=false;


             param.var2[19]=angle1;
             qDebug()<<"angle i:"<<angle1<<i;
             break;
         }



     }

     // draw black contours on white image


    }


}

void calcWork1Angle_2(Mat &src,Rect roi,params &param)
{
//小垫片得到两点
    int cenX=param.var1[0];//中心点
    int cenY=param.var1[1];
    int minSize=param.var1[2];//最小size
    int maxSize=param.var1[3];//最小size
    int thresh=param.var1[4];//二值
    int rectMaxLen=param.var1[5];//长
    int rectMinLen=param.var1[6];//宽
    int lenDifVal=param.var1[7];//长宽偏差
    double ratio=param.var2[0];//比例

    Mat m1;
    Point p1,p2,p3,p4;
    Mat m2=src(roi);
    int i,x1,y1,len1,len2;
    bool loop=true;

    param.var1[29]=0;
    for(i=100;i>thresh;i-=5)
    {

        if(!loop)
            break;
     threshold(m2,m1,i,255,THRESH_BINARY);


     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     //for ( ; itContours!=contours.end(); ++itContours)
     for (unsigned int k=0 ; k<contours.size(); k++)
     {

        if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;

         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;

         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

         p1.x=(rectpoint[0].x+rectpoint[1].x)/2;
         p1.y=(rectpoint[0].y+rectpoint[1].y)/2;

         p2.x=(rectpoint[2].x+rectpoint[3].x)/2;
         p2.y=(rectpoint[2].y+rectpoint[3].y)/2;

         if(len1>len2)
         {
             p1.x=(rectpoint[1].x+rectpoint[2].x)/2;
             p1.y=(rectpoint[1].y+rectpoint[2].y)/2;

             p2.x=(rectpoint[0].x+rectpoint[3].x)/2;
             p2.y=(rectpoint[0].y+rectpoint[3].y)/2;

             len1^=len2;
             len2^=len1;
             len1^=len2;
         }

         if(abs(len1-rectMinLen)>lenDifVal||abs(len2-rectMaxLen)>lenDifVal)
             continue;


         p3.x=x1+(p1.x-x1)*ratio;
         p3.y=y1+(p1.y-y1)*ratio;

         p4.x=x1+(p2.x-x1)*ratio;
         p4.y=y1+(p2.y-y1)*ratio;

         param.var1[20]=p3.x;
         param.var1[21]=p3.y;

         param.var1[22]=p4.x;
         param.var1[23]=p4.y;
         param.var1[24]=x1;
         param.var1[25]=y1;

         param.var1[29]=1;

         circle(g_dstImage2,Point(p3.x+roi.x,p3.y+roi.y),10,Scalar(255,0,0),2);
         circle(g_dstImage2,Point(p4.x+roi.x,p4.y+roi.y),10,Scalar(255,0,0),2);
         circle(g_dstImage2,End_Rage2D1.center,10,Scalar(255,0,0),2);

         loop=false;
         break;



     }




    }


}


void adjustWork1Angle(Mat &src,Rect roi,params &param)
{
//角度调整
    Mat m1;

    int len1,len2,len3,len4,x1,y1,i;
    double angle1=500,angle;

    param.var1[24]=0;


   // 800,1200,50,40,300,25000

    int minSize=param.var1[16];//size
    int maxSize=param.var1[17];
    int difLen=param.var1[18];//矩形长宽差
    int thresh=param.var1[19];//二值
    int minLen=param.var1[20];//矩形长
    int areaVal=param.var1[21];//面积

    int cenX=param.var1[28];
    int cenY=param.var1[29];
    double angleOffset=param.var2[0];//角度偏移

    bool loop = true,angleDif=false;
    for(i=254;i>thresh;i-=10)
    {

        if(!loop)
            break;
     threshold(src,m1,i,255,THRESH_BINARY);

    // imshow("Threshold",m1);
     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
     //std::cout << "Contours: " << contours.size() << std::endl;
     //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
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

         len3=len1>len2?len1:len2;

         if(src.at<uchar>(contours[k][0].y-1,contours[k][0].x)>i)
             continue;

         if(abs(len1-len2)<difLen)
             continue;

         if(abs(x1-cenX)>40||abs(y1-cenY)>40)
             continue;


         if(len3<minLen)
             continue;
         area=contourArea(contours[k]);

         if(area<areaVal)
             continue;

             for(int i1 = 0;i1< 4;i++)
             {
                 line(g_dstImage2,cvPointFrom32f(rectpoint[i1]),cvPointFrom32f(rectpoint[(i1+1)%4]),Scalar(0,255,0),1);
             }

             QString words=QString("%1").arg(contours[k].size());

             putText( g_dstImage2, words.toStdString(), Point(rectpoint[0].x+roi.x,rectpoint[0].y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );


             double angleA,angleB;


             if(len1>len2)
             {
                 angleA=common::calcAngle2(Point((rectpoint[0].x+rectpoint[1].x)/2,(rectpoint[0].y+rectpoint[1].y)/2),rectpoint[0]);
                 angleB=common::calcAngle2(Point((rectpoint[0].x+rectpoint[1].x)/2,(rectpoint[0].y+rectpoint[1].y)/2),rectpoint[1]);
                 param.var1[24]=rectpoint[0].x;
                 param.var1[25]=rectpoint[0].y;
                 param.var1[26]=rectpoint[1].x;
                 param.var1[27]=rectpoint[1].y;

             }
             else
             {
                 angleA=common::calcAngle2(Point((rectpoint[1].x+rectpoint[2].x)/2,(rectpoint[1].y+rectpoint[2].y)/2),rectpoint[1]);
                 angleB=common::calcAngle2(Point((rectpoint[1].x+rectpoint[2].x)/2,(rectpoint[1].y+rectpoint[2].y)/2),rectpoint[2]);

                 param.var1[24]=rectpoint[0].x;
                 param.var1[25]=rectpoint[0].y;
                 param.var1[26]=rectpoint[1].x;
                 param.var1[27]=rectpoint[1].y;
             }

             param.var2[17]=angleA;
             param.var2[18]=angleB;
             loop=false;
             if(param.var2[19]!=500)
             {
                 int dif2,dif3;
                 if(angleA-param.var2[19]>300)
                     dif2=360-(angleA-param.var2[19]);
                 else if(angleA-param.var2[19]<-300)
                     dif2=360+angleA-param.var2[19];
                 else
                     dif2=angleA-param.var2[19];

                 if(angleB-param.var2[19]>300)
                     dif3=360-(angleB-param.var2[19]);
                 else if(angleB-param.var2[19]<-300)
                     dif3=360+angleB-param.var2[19];
                 else
                     dif3=angleB-param.var2[19];

                 if(abs(dif2)<10)
                 {
                     angle1=angleA;
                     angleDif=true;
                 }

                 if(abs(dif3)<10)
                 {
                     angle1=angleB;
                     angleDif=true;
                 }

                 if(angleDif)
                 {


                     param.var2[19]=angle1;
                 }
                 else
                 {
                     param.var2[19]=500;
                 }

             }

            // param.var2[19]=angle1;
             qDebug()<<"angle i:"<<i<<"角度计算2:"<<angle1<<"angleA B"<<angleA<<angleB<<"area"<<area;

             words=QString("result:%1").arg(angle1);

             putText( g_dstImage2, words.toStdString(), Point(200,200),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );

             break;
         }





     // draw black contours on white image


    }


}

void project2::hd_maxDiameter(Mat &src,Rect roi,params &param)
{

        Mat m1;
        Mat m2=src(roi);

        int x1,y1,len1,len2;

        double ratio=param.var2[2];//比例
        int thresh=param.var2[3];
        int minSize=param.var1[20];//size
        int maxSize=param.var1[21];

         param.var2[4]=0;//输出直径

         threshold(m2,m1,thresh,255,THRESH_BINARY);

        // imshow("Threshold",m1);
         std::vector<std::vector<Point>> contours;

         findContours(m1,
                      contours, // a vector of contours
                      CV_RETR_LIST, // retrieve the external contours
                      CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

         // Print contours' length
         //std::cout << "Contours: " << contours.size() << std::endl;
         //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
         RotatedRect     End_Rage2D1;
         CvPoint2D32f rectpoint[4];
         double area;
         //for ( ; itContours!=contours.end(); ++itContours)
         for (unsigned int k=0 ; k<contours.size(); k++)
         {
            if(contours[k].size()<minSize||contours[k].size()>maxSize)
                 continue;

             End_Rage2D1 = minAreaRect(Mat(contours[k]));
             area=contourArea(contours[k]);
             cvBoxPoints(End_Rage2D1,rectpoint);

             x1=End_Rage2D1.center.x;
             y1=End_Rage2D1.center.y;
             len1=common::getLineLen(rectpoint[0],rectpoint[1]);
             len2=common::getLineLen(rectpoint[1],rectpoint[2]);

             if(abs(len1-len2)>20)
                 continue;

             if(common::contourType(m2,contours[k],thresh,true))
                 continue;

             Point2f center;
             float radius;
             minEnclosingCircle(contours[k],center,radius);

             param.var2[4]=radius*2*ratio;
             qDebug()<<"diameter size"<<contours[k].size();


         }


}
void project2::hd_maxDiameter2(Mat &src,Rect roi,params &param)
{

        Mat m1,m8;
        Mat m2=src(roi);

        int x1,y1,len1,len2;

        double ratio=param.var2[2];//比例
        int thresh=param.var2[3];
        int minSize=param.var1[20];//size
        int maxSize=param.var1[21];
        bool isBlack=param.var1[22];
        int lenDif=param.var1[23];//长宽差
        Point cen1(param.var1[25],param.var1[26]);//中心点
         param.var2[4]=0;//输出外接矩形较小直径
         param.var2[5]=0;//输出外接矩形较大直径
         param.var2[6]=0;//输出外接圆直径
         threshold(m2,m1,thresh,255,THRESH_BINARY);
         threshold(m2,m8,thresh,255,THRESH_BINARY);

        // imshow("Threshold",m1);
         std::vector<std::vector<Point>> contours;

         findContours(m1,
                      contours, // a vector of contours
                      CV_RETR_LIST, // retrieve the external contours
                      CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

         // Print contours' length
         //std::cout << "Contours: " << contours.size() << std::endl;
         //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
         RotatedRect     End_Rage2D1;
         CvPoint2D32f rectpoint[4];
         double area;
         //for ( ; itContours!=contours.end(); ++itContours)
         for (unsigned int k=0 ; k<contours.size(); k++)
         {
            if(contours[k].size()<minSize||contours[k].size()>maxSize)
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
             if(abs(len1-len2)>lenDif)
                 continue;

             if(common::contourType(m2,contours[k],thresh,isBlack))
                 continue;

             if(common::getLineLen(cen1,Point(x1,y1))>40)
             {
                 qDebug()<<"err"<<common::getLineLen(cen1,Point(x1,y1));
                              continue;
             }

             Point2f center;
             float radius;
             minEnclosingCircle(contours[k],center,radius);

             param.var2[4]=len1*ratio;
             param.var2[5]=len2*ratio;
             param.var2[6]=radius*2*ratio;

             qDebug()<<"diameter size"<<contours[k].size()<<"thresh"<<thresh;


         }


}

void adjustWork1Angle_2(Mat &src,Rect roi,params &param)
{
//小垫片角度计算
    int minSize=param.var1[10];//最小size
    int maxSize=param.var1[11];//最小size
    int thresh=param.var1[12];//二值
    int distance=param.var1[13];//中心点距离
    int distance2=param.var1[14];//
    int disDif=param.var1[15];//偏差距离
    double angleDif=param.var2[1];//角度偏移

    param.var2[19]=360;

    Mat m1;

    Mat m2=src(roi);
    int i,x1,y1,len1,len2,len3,len4,len5;
    bool loop=true;


    for(i=100;i>thresh;i-=10)
    {

        if(!loop)
            break;
     threshold(m2,m1,i,255,THRESH_BINARY);


     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     //for ( ; itContours!=contours.end(); ++itContours)
     for (unsigned int k=0 ; k<contours.size(); k++)
     {

        if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;

         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;

         len3=common::getLineLen(End_Rage2D1.center,Point(param.var1[24],param.var1[25]));
         if(len3<distance||len3>distance2)
             continue;

         if(common::contourType(m2,contours[k],i,false))
            continue;
         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);


         if(len1>len2)
         {
             len1^=len2;
             len2^=len1;
             len1^=len2;
         }

         len4=common::getLineLen(End_Rage2D1.center,Point(param.var1[20],param.var1[21]));
         len5=common::getLineLen(End_Rage2D1.center,Point(param.var1[22],param.var1[23]));


         //qDebug()<<"aaa"<<len4<<len5<<disDif;
         if(len4<disDif)
         {
             circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),20,Scalar(0,0,255),2);
             qDebug()<<"adjust found";
             double angle=common::calcAngle2(Point(param.var1[24],param.var1[25]),Point(param.var1[20],param.var1[21]));
             param.var2[19]=common::anlgeOffset(angle,angleDif);
             loop=false;
             break;
         }

         if(len5<disDif)
         {
             circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),20,Scalar(0,0,255),2);
             qDebug()<<"adjust found";
             double angle=common::calcAngle2(Point(param.var1[24],param.var1[25]),Point(param.var1[22],param.var1[23]));
             param.var2[19]=common::anlgeOffset(angle,angleDif);
             loop=false;
             break;
         }



     }




    }


}


void workIsEmpty(Mat &src,Rect roi,params &param)
{


    param.var1[29]=0;


    Mat m1;
    int len1,len2,allNum;
    Mat m2=g_dstImage2.clone();
    medianBlur(src(roi), m1,9);


    int minSize=param.var1[0];//size范围
    int thresh=param.var1[1];//二值

    //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(m1,m1,thresh,255,THRESH_BINARY);

     imshow("Threshold",m1);
     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
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

        ++allNum;
        if(allNum>1)
        {
            param.var1[29]=1;
            break;
        }


        //contourFeature(contours[k]);


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

//         if((len1>len2?len1:len2)<minRectLen)
//             continue;

//         if(len1>len2)
//         {
//             p1=rectpoint[0];
//             p2=rectpoint[1];
//         }
//         else
//         {
//             p1=rectpoint[1];
//             p2=rectpoint[2];
//         }

//         if(abs(len1-len2)>90)
//             continue;


//         for(int i = 0;i< 4;i++)
//         {
//             //CvArr* s=(CvArr*)&result;
//             //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
//             line(m2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
//         }

         QString words=QString("%1").arg(contours[k].size());

         putText( m2, words.toStdString(), Point(End_Rage2D1.center.x-10+roi.x,End_Rage2D1.center.y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
         qDebug()<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2;
     }

     // draw black contours on white image



     imshow("Contours",m2);

}

void workIsEmpty2(Mat &src,Rect roi,params &param)
{
    param.var1[29]=1;
    Mat m1;
    int len1,len2,allNum;
    Mat m2=src.clone();

    int thresh=param.var1[0];//二值
    int offsetXY=param.var1[1];//xy偏移
    int minSize=param.var1[2];//size
    int maxSize=param.var1[3];//size
    //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(m2,m1,thresh,255,THRESH_BINARY);

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
         if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         if(abs(src.cols/2-End_Rage2D1.center.x)>offsetXY||abs(src.rows/2-End_Rage2D1.center.y)>offsetXY)
             continue;
         ++allNum;
         if(allNum>0)
         {
             param.var1[29]=0;
             qDebug()<<"cen"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"size"<<contours[k].size();
             break;
         }

//         len1=common::getLineLen(rectpoint[0],rectpoint[1]);
//         len2=common::getLineLen(rectpoint[1],rectpoint[2]);

//         if((len1>len2?len1:len2)<minRectLen)
//             continue;

//         if(len1>len2)
//         {
//             p1=rectpoint[0];
//             p2=rectpoint[1];
//         }
//         else
//         {
//             p1=rectpoint[1];
//             p2=rectpoint[2];
//         }

//         if(abs(len1-len2)>90)
//             continue;


//         for(int i = 0;i< 4;i++)
//         {
//             //CvArr* s=(CvArr*)&result;
//             //cvLine(s,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),CV_G(0,0,255),2);
//             line(m2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
//         }

      //   QString words=QString("%1").arg(contours[k].size());

        // putText( m2, words.toStdString(), Point(End_Rage2D1.center.x-10+roi.x,End_Rage2D1.center.y+roi.y),CV_FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255),2 );
         qDebug()<<"Size: " << contours[k].size()<<"<< area:"<<area<<"x y:"<<End_Rage2D1.center.x<<End_Rage2D1.center.y<<"rectLen"<<len1<<len2;
     }

     // draw black contours on white image



     imshow("Contours",m2);

}


void project2::hd_vision1()
{
   params param;
   param.var1[0]=625;//设定圆心坐标
   param.var1[1]=441;
   param.var1[2]=100;//圆心二值
   param.var1[3]=320;//圆心阴影size
   param.var1[4]=450;
   param.var1[5]=40;//圆心size step  圆心检测1使用
   param.var1[6]=20;//圆心偏离最大值

   param.var1[9]= 15;//圆环上小圆半径范围
   param.var1[10]= 22;//圆环上小圆半径范围
   param.var1[11]= 100;//小圆环到圆心距离最小
   param.var1[12]= 110;//小圆环到圆心距离最大
   param.var1[13]= 57;//小圆size范围
   param.var1[14]= 107;
   param.var1[15]= 150;//二值最大
   param.var1[16]= 140;//所有数量
   param.var1[17]=25;//二值最小
   param.var1[18]=100;//小圆边离圆心最小距离
   param.var1[19]=166;//小圆边离圆心最大距离

   Rect roi(490,310,283,277);

   Mat m;
   medianBlur( g_dstImage1(roi).clone(), m,3);
  // imwrite("ccc.jpg",m);

//   int filterVal=10;
//   bilateralFilter( g_dstImage1(roi).clone(), m, filterVal, filterVal*2,filterVal/2 );

   //call dll

  // detectWork(m,roi,param,2);

   //end

   //qt call******************************

      Point pCen= shimCentre(m,roi,param);
      qDebug()<<"cen point"<<pCen.x<<pCen.y;

      param.var1[7]=pCen.x;//计算得到圆心坐标
      param.var1[8]=pCen.y;

      shimDetection(m,roi,param,false);
      if(!param.var1[29])
      {
          shimDetection(m,roi,param,true);
      }

   //end*********************************

   if(param.var1[29])
   {
       qDebug()<<"正面"<<"比对轮廓数量"<<param.var1[28];

   }
   else
   {
       if(param.var1[28]>=5) qDebug()<<"有瑕疵"<<"比对轮廓数量"<<param.var1[28];
       qDebug()<<"反面";
   }


   imshow("Contours",g_dstImage2);
}



void project2::hd_vision2()
{
//480,600,1000,10,240,290,550,330,390,4,800,1200,50,40,300,25000,-96
    params param;
    //中心点检测
    param.var1[0]=480;//最小矩形长度
    param.var1[1]=600;
    param.var1[2]=1000;//最小size
    param.var1[3]=10;//二值

    //角度检测
    param.var1[10]=240;//最小矩形长度
    param.var1[11]=290;
    param.var1[12]=550;//最小size
    param.var1[13]=330;//离中心点长度
    param.var1[14]=390;
    param.var1[15]=4;//二值
    param.var2[0]=0;//角度偏移


    //角度确认  800,1200,50,40,300,25000
    param.var1[16]=800;//size
    param.var1[17]=1200;
    param.var1[18]=50;//矩形长宽差
    param.var1[19]=40;//二值
    param.var1[20]=300;//矩形长
    param.var1[21]=25000;//面积


    //Rect roi(3,10,1278-3,946-10);
    Rect roi(0,0,1280,960);
    Mat m;
    QTime t;
    t.start();
    medianBlur(g_dstImage1, m,3);

    // call dll
    //detectWork(m,roi,param,3);
    HD_circleCenter(m,roi,param);
    calcWork1Angle(m,roi,param);//
    adjustWork1Angle(m,roi,param);//查找白色金属


    if(param.var2[19]!=500)
    {
        if(param.var2[0]+param.var2[19]>180)
            param.var2[19]=param.var2[0]+param.var2[19]-360;
        else if(param.var2[0]+param.var2[19]<-180)
            param.var2[19]=param.var2[0]+param.var2[19]+360;
        else
            param.var2[19]+=param.var2[0];
    }

    imshow("Contours",g_dstImage2);
    qDebug()<<"中心点"<<param.var1[28]<<param.var1[29];
    qDebug()<<"angle:"<<param.var2[19]<<"Time elapsed:"<<t.elapsed();
}

void project2::hd_vision3()
{

    params param;
    param.var1[0]=150;//二值
    param.var1[1]=100;//xy偏移
    param.var1[2]=550;//size
    param.var1[3]=2000;//size
    Rect roi(0,0,1280,960);
    Mat m=g_dstImage1.clone();

    workIsEmpty2(m,roi,param);
    //detectWork(m,roi,param,4);
    if(param.var1[29])
        qDebug()<<"料盘空";
    else
        qDebug()<<"料盘有";

}


void project2::hd_vision4()
{
//
    params param;

    param.var1[0]=0;//中心点
    param.var1[1]=0;
    param.var1[2]=800;//最小size
    param.var1[3]=1200;//最小size
    param.var1[4]=19;//二值
    param.var1[5]=361;//长
    param.var1[6]=272;//宽
    param.var1[7]=50;//长宽偏差
    param.var2[0]=2;//比例


    param.var1[10]=30;//最小size
    param.var1[11]=200;//最小size
    param.var1[12]=19;//二值
    param.var1[13]=323;//中心点距离
    param.var1[14]=421;//
    param.var1[15]=73;//偏差距离
    param.var2[1]=0;//角度偏移

    param.var1[20]=400;//size
    param.var1[21]=600;
    param.var2[2]=0.0303;//比例,测量最大直径
    param.var2[3]=30;//二值
    param.var2[4]=0;//输出直径
    //Rect roi(3,10,1278-3,946-10);
    Rect roi(0,0,1280,960);
    Mat m;
    QTime t;
    t.start();
    medianBlur(g_dstImage1, m,3);

    // call dll

   // detectWork(m,roi,param,19);


    Rect roi2(527,378,250,250);
    hd_maxDiameter(m,roi2,param);

    calcWork1Angle_2(m,roi,param);
    if(param.var2[29])
    {
        qDebug()<<"adjustWork1Angle_2 start";
        adjustWork1Angle_2(m,roi,param);
    }
    imshow("rgb",g_dstImage2);
    qDebug()<<"angle:"<<param.var2[19]<<"最大直径"<<param.var2[4];

    qDebug()<<"Time elapsed:"<<t.elapsed();
}


void project2::hd_vision1_2(Mat &src,Rect roi,params& param)
{


    bool callDll=0;

    //call dll
    if(callDll)
    {
        detectWork(g_dstImage1,roi,param,2);
        if(param.var1[29])
        {
            qDebug()<<"正面"<<"比对轮廓数量"<<param.var1[28];

        }
        else
        {

            qDebug()<<"反面";
        }

        //end

    }
    else
    {
        //qt call******************************

        if(DEBUG)
            g_dstImage2= g_dstImage2(roi);

        Mat m;
        medianBlur( src(roi).clone(), m,3);

        Point pCen= shimCentre2(m,roi,param);
        if(DEBUG)
            qDebug()<<"cen point"<<pCen.x+roi.x<<pCen.y+roi.y;

        param.var1[25]=pCen.x;//计算得到圆心坐标
        param.var1[26]=pCen.y;

        if(pCen.x!=0)
            shimDetection2(m,roi,param,false);


        //end*********************************
    }
        if(DEBUG)
            imshow("Contours3",g_dstImage2);
}

void project2::hd_vision3_angle(Mat &src, Rect roi, params &param)
{
    //华东第3套角度
        int cenX=param.var1[0];//中心点
        int cenY=param.var1[1];
        int minSize=param.var1[2];//最小size
        int maxSize=param.var1[3];//最小size
        int thresh=param.var1[4];//二值
        int thresh2=param.var1[5];
        int rectMaxLen=param.var1[6];//长
        int rectMinLen=param.var1[7];//宽

        double angleOffset=param.var2[0];//角度偏移

        Mat m1;

        Mat m2=src(roi);
        int i,x1,y1,len1,len2;
        bool loop=true;


        param.var2[19]=360;
        for(i=thresh;i<thresh2;i++)
        {

            if(!loop)
                break;
         threshold(m2,m1,i,255,THRESH_BINARY);


         std::vector<std::vector<Point>> contours;

         findContours(m1,
                      contours, // a vector of contours
                      CV_RETR_LIST, // retrieve the external contours
                      CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

         RotatedRect     End_Rage2D1;
         CvPoint2D32f rectpoint[4];
         double area;
         //for ( ; itContours!=contours.end(); ++itContours)
         for (unsigned int k=0 ; k<contours.size(); k++)
         {

            if(contours[k].size()<minSize||contours[k].size()>maxSize)
                 continue;

             End_Rage2D1 = minAreaRect(Mat(contours[k]));
             area=contourArea(contours[k]);
             cvBoxPoints(End_Rage2D1,rectpoint);

             x1=End_Rage2D1.center.x;
             y1=End_Rage2D1.center.y;


             if(common::getLineLen(Point(x1,y1),Point(cenX,cenY))>100)
                 continue;


             len1=common::getLineLen(rectpoint[0],rectpoint[1]);
             len2=common::getLineLen(rectpoint[1],rectpoint[2]);


             if(len1>len2)
             {

                 len1^=len2;
                 len2^=len1;
                 len1^=len2;
             }

             if(len1<rectMinLen||len2>rectMaxLen)
                 continue;




             Point p1;
             int index,maxDis=0,temp1;
             Point cen(x1,y1);
             int j=0;
             for(int i1=0;i1<contours[k].size();i1+=2)
             {
                 temp1=common::getLineLen(contours[k][i1],cen);
//                 if(i>1190&&i<1195)
//                 {
//                     qDebug()<<"temp"<<temp1;
//                    circle(g_dstImage2,Point(contours[k][i].x+roi.x,contours[k][i].y+roi.y),3,Scalar(0,255,0),2);
//                 }
                 if(temp1>maxDis)
                 {
                     maxDis=temp1;
                     index=i1;
                 }
                 j++;
             }

             p1=contours[k][index];

             double angle1=common::calcAngle2(Point(x1,y1),p1);
             param.var2[19]=common::anlgeOffset(angle1,angleOffset);
             param.var1[29]=i;
             circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),10,Scalar(255,0,0),2);
             circle(g_dstImage2,Point(p1.x+roi.x,p1.y+roi.y),7,Scalar(0,0,255),2);

             qDebug()<<"size"<<contours[k].size()<<"maxDis"<<maxDis;
             loop=false;
             break;



         }


     }

}

int project2::gap1(Mat &src,Rect roi,int minWidth,int difVal)
{

    if(roi.width>src.cols-1||roi.height>src.rows-1||src.rows==0||src.cols==0)
        return 0;
    int threshVal,tmp1,tmp2,leftx=0,rightx=0;
    threshVal=src.at<uchar>(roi.y+roi.height/2,roi.x+roi.width/2);
        qDebug()<<"threshVal"<<threshVal<<"cenx"<<roi.x+roi.width/2;
        for(int i=roi.x+roi.width/2;i>roi.x;i--)
        {
            for(int j=roi.y;j<roi.y+roi.height;j++)
            {
                tmp1=src.at<uchar>(j,i);
                tmp2=src.at<uchar>(j,roi.x*2+roi.width-i);
                //qDebug()<<"tmp1"<<tmp1<<"tmp2"<<tmp2;
                if(leftx==0&&tmp1-threshVal>40)
                    leftx=i;

                if(rightx==0&&tmp2-threshVal>40)
                    rightx=roi.x*2+roi.width-i;


                if(leftx&&rightx)
                    goto END;

            }
        }
    END:
        qDebug()<<"aaa"<<rightx<<leftx<<minWidth;

        line(g_dstImage2,Point(leftx,roi.y),Point(leftx,roi.y+roi.height),Scalar(0,255,0),1);
        line(g_dstImage2,Point(rightx,roi.y),Point(rightx,roi.y+roi.height),Scalar(0,255,0),1);
        imshow("rgb",g_dstImage2);

    return rightx-leftx+difVal<minWidth?0:1;

}
void project2::hd_gap()
{
    //1088  1024  1794  1794
    Rect roi(363,557,707-363,622-557);
    QTime t;
    params param;
    t.start();


    int ret=gap1(g_dstImage1, roi,233,8);




    qDebug()<<"Time elapsed:"<<t.elapsed()<<"是否合格"<<ret;

}


bool semicircleCen(vector<Point> &contours,int x1,int y1,int step,int minR,int maxR,int cirFeaNum,int move,int offset,Point &cen)
{

    return true;
}

void project2::hd_smallWork_other(Mat &src, Rect roi, params &param)
{

    QString words="false";
    Mat m1=src.clone();
    Mat m3;
    int step,x1,y1,circleFea;
    int len1,len2;
    int error=0;

    Point cen1(param.var1[0],param.var1[1]);
    int rLen1=param.var1[2];//半径
    int rLen2=param.var1[3];
    int minSize=600;//小圆size范围最小
    int maxSize=8000;//小圆size范围最大
    int setOffset=param.var1[6];//与圆心偏离
    int cirFeaVal=8;//特征数量



    param.var1[26]=82000;

    threshold(m1,m3,254,255,THRESH_BINARY);



    std::vector<std::vector<Point>> contours;

    findContours(m3,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    double area;

    for (unsigned int k=0 ; k<contours.size(); k++)
    {


        if(contours[k].size()<minSize||contours[k].size()>maxSize)
        {

            continue;
        }


        step=contours[k].size()/12;

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


        common::mostVal(error,1);

        circleFea=common::circleRediusFeature(contours[k],step,50,rLen1,rLen2);

        if(circleFea<cirFeaVal)
            continue;
        common::mostVal(error,2);
        for(int i = 0;i< 4;i++)
        {
            line(g_dstImage2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
        }
        if(area/contours[k].size()<3)
            continue;

        common::mostVal(error,3);



        param.var1[26]=90000;
        param.var1[29]=82000;

        putText( g_dstImage2, words.toStdString(), Point(100,300),CV_FONT_HERSHEY_SIMPLEX, 4, Scalar(0, 0, 255),2 );

        break;

    }



}

void project2::hd_smallWork(Mat &src, Rect roi, params &param)
{

    QString words="false";
    Mat m1=src.clone();
    Mat m3;
    int step,x1,y1,circleFea;
    int len1,len2;
    int error=0;

    Point cen1(param.var1[0],param.var1[1]);
    int rLen1=param.var1[2];//半径
    int rLen2=param.var1[3];
    int minSize=param.var1[4];//小圆size范围最小
    int maxSize=param.var1[5];//小圆size范围最大
    int setOffset=param.var1[6];//与圆心偏离
    int cirFeaVal=param.var1[7];//特征数量



    param.var1[29]=82000;

    threshold(m1,m3,254,255,THRESH_BINARY);



    std::vector<std::vector<Point>> contours;

    findContours(m3,
                 contours, // a vector of contours
                 CV_RETR_LIST, // retrieve the external contours
                 CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


    RotatedRect     End_Rage2D1;
    CvPoint2D32f rectpoint[4];
    double area;

    for (unsigned int k=0 ; k<contours.size(); k++)
    {


        if(contours[k].size()<minSize||contours[k].size()>maxSize)
        {

            continue;
        }


        step=contours[k].size()/10;

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


        //common::mostVal(error,1);

        circleFea=common::circleRediusFeature(contours[k],step,20,rLen1,rLen2);
        qDebug()<<"circleFea"<<circleFea<<"x y"<<x1<<y1<<"size"<<contours[k].size();
        if(circleFea<cirFeaVal-1)
            continue;
        if(DEBUG)
        {
            common::mostVal(error,2);
            for(int i = 0;i< 4;i++)
            {
                line(g_dstImage2,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
            }
        }
        if(area/contours[k].size()>3)
        {
            break;
        }

        common::mostVal(error,3);


        param.var1[27]=x1;
        param.var1[28]=y1;
        param.var1[29]=90000;

        words="true";
        //qDebug()<<"ret"<<"true";
        qDebug()<<"size:"<<contours[k].size()<<"area"<<area<<"circleFea"<<circleFea;
        break;

    }

    putText( g_dstImage2, words.toStdString(), Point(100,300),CV_FONT_HERSHEY_SIMPLEX, 4, Scalar(0, 0, 255),2 );

    //qDebug()<<"error"<<error;
}


void project2::zt_ZT01(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int whiteLineLen=param.var1[7];//画直线长度
    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();

    line(m5,Point(cenX-whiteLineLen,0),Point(cenX-whiteLineLen,m5.rows),Scalar(255,255,255),6);
    line(m5,Point(cenX+whiteLineLen,0),Point(cenX+whiteLineLen,m5.rows),Scalar(255,255,255),6);

    Mat m1;

    medianBlur(m5, m5, 3);
    int offset =10;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m5, m5, MORPH_TOPHAT , element);
    else
        morphologyEx(m5, m5, MORPH_BLACKHAT, element);

    for(int i=255;i>thresh;i-=5)
    //for(int i=130;i>129;i-=5)
    {
        if(i>200)
            i-=20;
        if(i>160&&i<180)
            i-=10;
        int a1=0,a2=0,a3=0,lenA=0,lenB=0,preY1=0,preY2=0;

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("thresh",m1);
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
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<11&&len2<11)
                continue;

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
            angle1=common::anlgeOffset(angle1,angleOffset);
            //字体在中心点相连角度

            //circle( rgb,Point(x1+roi.x,y1+roi.y), 25, Scalar(0,0,0), 2, CV_AA);
            if(abs(angle1)<30)
            {

                if(len2>60)
                    ++a3;

                if(abs(x1+y1-lenA)>12)
                {
                    circle( rgb, Point(x1+roi.x,y1+roi.y), 3, Scalar(0,0,255), 2, CV_AA);
                    ++a1;
                }

                if(a1>1)
                {
                   if(abs(y1-preY1)>52)
                   {
                       --a1;
                       continue;
                   }
                }
               // qDebug()<<"size"<<contours.size()<<"x + y"<<x1+y1<<lenA<<"a1"<<a1;
                lenA=x1+y1;
                preY1=y1;
//                for(int i = 0;i< 4;i++)
//                {
//                    line(rgb,Point(cvPointFrom32f(rectpoint[i]).x+roi.x,cvPointFrom32f(rectpoint[i]).y+roi.y),Point(cvPointFrom32f(rectpoint[(i+1)%4]).x+roi.x,cvPointFrom32f(rectpoint[(i+1)%4]).y+roi.y),Scalar(0,255,0),1);
//                }
            }
            if(abs(angle1)>150&&abs(angle1)<=180)
            {
                if(len2>60)
                    ++a3;

                if(abs(x1+y1-lenB)>12)
                {
                    circle( rgb, Point(x1+roi.x,y1+roi.y), 3, Scalar(0,0,255), 2, CV_AA);
                    ++a2;
                }
                if(a2>1)
                {
                    if(abs(y1-preY2)>52)
                    {
                        --a2;
                       continue;
                    }
                }
                lenB=x1+y1;
                preY2=y1;
//                for(int i = 0;i< 4;i++)
//                {
//                    line(rgb,Point(cvPointFrom32f(rectpoint[i]).x+roi.x,cvPointFrom32f(rectpoint[i]).y+roi.y),Point(cvPointFrom32f(rectpoint[(i+1)%4]).x+roi.x,cvPointFrom32f(rectpoint[(i+1)%4]).y+roi.y),Scalar(255,0,0),1);
//                }
            }
            if(a1>1||a2>1||a3>0)
            {
                param.var1[28]=x1;
                param.var1[29]=y1;
                param.var2[19]=angle1;
                qDebug()<<"i:"<<i<<"a1 a2 a3"<<a1<<a2<<a3;
                return;
            }
        }


    }
}

void project2::zt_ZT01_2(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int whiteLineLen=param.var1[7];//画直线长度
    double angleOffset=param.var2[0];//角度偏移
    double angle1,objAngle;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();

    line(m5,Point(cenX-whiteLineLen,0),Point(cenX-whiteLineLen,m5.rows),Scalar(255,255,255),6);
    line(m5,Point(cenX+whiteLineLen,0),Point(cenX+whiteLineLen,m5.rows),Scalar(255,255,255),6);

    Mat m1;

    medianBlur(m5, m5, 3);
    int offset =10;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m5, m5, MORPH_TOPHAT , element);
    else
        morphologyEx(m5, m5, MORPH_BLACKHAT, element);

    for(int i=255;i>thresh;i-=5)
    //for(int i=130;i>129;i-=5)
    {
        if(i>200)
            i-=20;
        if(i>160&&i<180)
            i-=10;

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("thresh",m1);
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
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
                objAngle=common::calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                objAngle=common::calcAngle(rectpoint[2],rectpoint[1]);
            }

            if(abs(objAngle)>12)
                continue;
            if(len2<18)
                continue;

            if(len2*1.0/len1<2.1)
                continue;

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));



            if(abs(angle1)>30&&abs(angle1)<150)
                continue;


            param.var1[28]=x1;
            param.var1[29]=y1;
            angle1=common::anlgeOffset(angle1,angleOffset);
            param.var2[19]=angle1;
            circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(0,0,255)),3;
            qDebug()<<"i:"<<i<<"objAngle:"<<objAngle;
            return;

        }


    }
}


int trangleFeature(std::vector<Point> &contours,int sideLen,Rect &roi)
{
    int num=0,left1=0,right1=0,down1=0,index,num1;
    int size=contours.size();

    Point pUp=contours[0];
    for(int i=1;i<size;i++)
    {
        if(contours[i].x<contours[left1].x) left1=i;
        if(contours[i].x>contours[right1].x) right1=i;
        if(contours[i].y>contours[down1].x) down1=i;
    }

    if(abs(contours[left1].x-pUp.x)+abs(contours[left1].x-contours[down1].x)>abs(contours[right1].x-pUp.x)+abs(contours[right1].x-contours[down1].x))
        index=left1;
    else
        index=right1;

    num+=common::trangleCalc(g_dstImage2,contours,sideLen,0,roi);
    num+=common::trangleCalc(g_dstImage2,contours,sideLen,down1,roi);
    num+=common::trangleCalc(g_dstImage2,contours,sideLen,index,roi);
    return num;

}

void project2::zt_5thLine(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int rLen1=param.var1[7];//三角形外接矩形len
    int rLen2=param.var1[8];
    int rLenDif=param.var1[9];//三角形长宽差
    int sideLen=param.var1[10];//三角形最小边长
    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    medianBlur(m5, m5, 3);
//    int offset =10;//偏移量
//    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
//    //自定义核
//    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
//    //进行操作
//    if( offset < 0 )
//        morphologyEx(m5, m5, MORPH_TOPHAT , element);
//    else
//        morphologyEx(m5, m5, MORPH_BLACKHAT, element);

    int num1=0;
    for(int i=30;i<thresh;i+=5)
    {


        int a1=0,a2=0;
        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("m1",m1);
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
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<rLen1||len2>rLen2)
                continue;

            if(len2-len1>rLenDif)
                continue;


            num1=trangleFeature(contours[k],sideLen,roi);
            qDebug()<<"num1"<<num1<<"i:"<<i;

            if(num1>7)
            {


            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;
            qDebug()<<"i:"<<i<<"size"<<contours[k].size();
            rectangle(rgb,Rect(x1-30+roi.x,y1-30+roi.y,70,60),Scalar(0,0,255),3,8,0);//用矩形画矩形窗
            namedWindow("rgb",0);
            resizeWindow("rgb",700,500);
            imshow("rgb",rgb);
            return;

            }
        }


    }
}

void project2::test1(IplImage *pSrcImage)
{
    char *buf=new char[1024*1024*10];
    IplImage *aaa = cvLoadImage("F:/CPP/QT/opencvTest/image/111.bmp", 0);
    memcpy(buf,aaa->imageData,aaa->imageSize);
    IplImage *bbb= cvCreateImageHeader(cvGetSize(pSrcImage),pSrcImage->depth,pSrcImage->nChannels);
    //unsigned char *data=(unsigned char *)pSrcImage->imageData;
    cvSetData(bbb,buf,aaa->widthStep);
    //pSrcImage->imageData=buf;


   // memcpy(aaa->imageData,buf,pSrcImage->imageSize);
    cvShowImage("bbb",bbb);
}

void project2::sendUdp(QString str)
{

    emit addLog("已发送");

    QByteArray buf=str.toLocal8Bit();
    m_client->writeDatagram(buf,QHostAddress("192.168.16.45"),8888);

}
//得到左右领点坐标
void get2Pos(Mat &src,std::vector<Point> &contours,Point &p1,Point &p2)
{

    int x1=(p1.x+p2.x)/2;
    int y1=(p1.y+p2.y)/2;
    int index1=0,index2=0;
    int index3,upIndex,downIndex;
    int len1=10000,len2=10000,tmpLen;

    for(int i=x1;i>src.cols/2;i--)
    {

        if(src.at<uchar>(y1,i)==0&&src.at<uchar>(y1,i-1)==0&&src.at<uchar>(y1,i-2)==0)
        {
            index1=i;

            break;
        }
    }
    Point temp(index1,y1);

    for(int i=0;i<contours.size();i++)
    {
        if(abs(contours[i].x-temp.x)<10&&abs(contours[i].y-temp.y)<10)
        {
            index2=i;
            break;
        }
    }


    int sign=p1.x>p2.x?1:-1;
    int sign1=contours[index2-100].y<contours[index2+100].y?-1:1;
    int x2,y2;
    double ratio=abs(p1.x-p2.x)*1.0/abs(p1.y-p2.y);

    for(int i=0;i<400;i++)
    {
        x2=x1+i*ratio*sign;
        y2=y1-i;

        if(y2<0)
            break;
        for(int j=0;j<500;j++)
        {

            index3=index2+j*sign1;
            if(sign1==1)
            {
                if(index3>=contours.size())
                    index3-=contours.size();
            }
            else
            {
                if(index3<0)
                    index3=contours.size()-index3;
            }

            tmpLen=common::getLineLen(Point(x2,y2),contours[index3]);

            if(tmpLen<len1)
            {
                len1=tmpLen;
                upIndex=index3;

            }


        }

    }


    for(int i=0;i<400;i++)
    {
        x2=x1+i*ratio*sign*-1;
        y2=y1+i;
        if(y2>=src.rows)
            break;
        for(int j=0;j<500;j++)
        {

            index3=index2+j*sign1*-1;
            if(sign1==1)
            {
                if(index3>=contours.size())
                    index3-=contours.size();
            }
            else
            {
                if(index3<0)
                    index3=contours.size()-index3;
            }

            tmpLen=common::getLineLen(Point(x2,y2),contours[index3]);
            if(tmpLen<len2)
            {
                len2=tmpLen;
                downIndex=index3;
            }

        }

    }

    p1=contours[upIndex];
    p2=contours[downIndex];

}


void project2::sz_cloth(Mat &src, Mat &rgb, Rect roi, params &param)
{

    int objMinSize=param.var1[0];//size
    int objMaxSize=param.var1[1];
    int thresh=param.var1[2];//二值
    Point modelCen(param.var1[3],param.var1[4]);//模板中心
    Point modelCen2(param.var1[5],param.var1[6]);//计算出模板中心
    int len1=param.var1[7];//计算出中心与衣领垂直长度
    int offsetX=param.var1[8];//XY偏移
    int offsetY=param.var1[9];
    double pixelRatio=param.var2[0];//像素比
    double angleDif=param.var2[1];//模板角度
    double angleCamera=param.var2[2];//相机角度


    double angle,angle2,angle1;
    int x1,y1,offx,offy;
    Point p,p1,p2,center,p3;
    Mat m1,m2;
    namedWindow("rgb",0);
    resizeWindow("rgb",800,600);
    namedWindow("m1",0);
    resizeWindow("m1",800,600);
    threshold(src,m1,thresh,255,THRESH_BINARY);
    threshold(src,m2,thresh,255,THRESH_BINARY);
    imshow("m1",m1);
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
        End_Rage2D1 = minAreaRect(Mat(contours[k]));
        cvBoxPoints(End_Rage2D1,rectpoint);
        x1=End_Rage2D1.center.x;
        y1=End_Rage2D1.center.y;


        for(int i=0;i<4;i++)
        {
            if(rectpoint[i].x>x1)
            {
                if(rectpoint[i].y<y1)
                    p1=rectpoint[i];
                else
                    p2=rectpoint[i];
            }
        }
        if(common::getLineLen(p1,p2)>1600)
            continue;

        get2Pos(m2,contours[k],p1,p2);

        circle( rgb, p1, 8, Scalar(0,0,255), 3, CV_AA);
        circle( rgb, p2, 8, Scalar(0,0,255), 3, CV_AA);



        p.x=(p1.x+p2.x)/2;
        p.y=(p1.y+p2.y)/2;

        angle1=common::calcAngle(p1,p2);

        angle1=angle1>0?angle1-90:90+angle1;

        if(modelCen2.x==0)
        {

            angle2=common::calcAngle(p,modelCen);
            angle=abs(angle1-angle2);
            len1=cos(angle*3.1415/180)*common::getLineLen(p,modelCen);
            qDebug()<<"计算模板";
        }

        offx=cos(angle1*3.1415/180)*len1;
        offy=sin(angle1*3.1415/180)*len1;

        center.x=p.x-offx;
        center.y=p.y-offy;



        p3=common::xyCalibration(angleCamera,modelCen2,center);

        param.var1[28]=(modelCen2.x-p3.x)*pixelRatio+offsetX;
        param.var1[29]=(modelCen2.y-p3.y)*pixelRatio+offsetY;
        param.var2[19]=angle1-angleDif;
        qDebug()<<"aaa"<<angle1<<angleDif;
        if(abs(param.var2[19])<0.00001)
           param.var2[19]=0;


        qDebug()<<"len1"<<len1<<"center"<<center.x<<center.y<<"angle1"<<angle1<<"偏移 x y angle"<<param.var1[28]<<param.var1[29]<<param.var2[19];
        circle( rgb, p, 5, Scalar(0,0,255), 1, CV_AA);
        circle( rgb, center, 5, Scalar(0,0,255), 1, CV_AA);

        imshow("rgb",rgb);

    }

}


int armFrame=0;

void project2::processUdp()
{
    qint64 size=m_client->pendingDatagramSize();
    QByteArray array(size,0);
    m_client->readDatagram(array.data(),size);

    int index=int(array[0]);

    memcpy((void*)(udpImg->imageData+index*30720),array.data()+2,30720);


    QFile file("d:/abc.txt");
    file.open(QIODevice::WriteOnly|QIODevice::Append);
    if(index==0)
        file.write("\r\n");

    file.write(QString("%1,").arg(index).toLocal8Bit().data());

    file.close();

    if(index==0)
    {


        pic_count=0;
        pic_sum=0;
    }
    pic_count++;
    pic_sum+=index;

    if(pic_count==10&&pic_sum==45)
    {
        cvDrawLine(udpImg,cvPoint(udpImg->width/2-50,udpImg->height/2),cvPoint(udpImg->width/2+50,udpImg->height/2),cvScalar(255,255,255),1);
        cvDrawLine(udpImg,cvPoint(udpImg->width/2,udpImg->height/2-50),cvPoint(udpImg->width/2,udpImg->height/2+50),cvScalar(255,255,255),1);

        cvShowImage("abc",udpImg);

    }
}
void project2::getSharedMem()
{


    //QBuffer buffer;                //构建缓冲区
    //QDataStream out(&buffer);      //建立数据流对象，并和缓冲区关联
   // QImage image;
    //对共享内存进行读写操作
     emit addLog(QString("共享内存:%1").arg(sharememory->size()));
    if(sharememory->size()<640*480*3)
    {

        return;
    }
    sharememory->lock();           //锁定共享内存
    rgbImg->imageData=(char*)sharememory->data();

    //初始化缓冲区中的数据，setData函数用来初始化缓冲区。
    //该函数如果在open()函数之后被调用，则不起任何作用。
    //buffer.open(QBuffer::ReadOnly);  //解除注释，则setData函数不起作用，无法加载内存中数据
    //buffer.setData((char*)sharememory->constData(), sharememory->size());
    //buffer.open(QBuffer::ReadOnly);    //只读方式打开缓冲区
    //out >> image;                      //将缓冲区的数据写入QImage对象
    sharememory->unlock();         //释放共享内存
    cvShowImage("abc",rgbImg);
    //sharememory->detach();         //解除程序和共享内存的关联
    //ui->Label_Display->setPixmap(QPixmap::fromImage(image));   //显示图片
}


void project2::wx_direction0(Mat &src, Mat &rgb, params &param)
{
    Mat m1=src;
    //medianBlur(src, m1, 9);
    Point pos1(param.var1[0],param.var1[1]);
    Point pos2(param.var1[2],param.var1[3]);
    int times=param.var1[4];
    int grayVal1=param.var1[5];
    int result1= calcLineGray(m1,rgb,pos1,true,times);
    int result2= calcLineGray(m1,rgb,pos2,true,times);

    qDebug()<<"result 1 2 "<<result1<<result2;
    if(result1<grayVal1)
    {
        param.var1[29]=1;//有料方向下
        qDebug()<<"有料方向下";
    }
    else if(result2<grayVal1)
    {
        param.var1[29]=2;//有料方向右
        qDebug()<<"有料方向右";
    }
    else
        param.var1[29]=360;//无

}

void project2::wx_direction1(Mat &src, Mat &rgb,Rect roi, params &param)
{

    Mat m1,m5;
    medianBlur(src(roi), m5, 9);
    Point pos1(param.var1[10]-roi.x,param.var1[11]-roi.y);//位置上
    Point pos2(param.var1[12]-roi.x,param.var1[13]-roi.y);//位置左
    int thresh=param.var1[14];

    int objMinSize=param.var1[15];//size
    int objMaxSize=param.var1[16];

    int minLen=param.var1[17];//最小长度
    int maxLen=param.var1[18];


    //out
    param.var1[29]=360;
    int len1,len2,x1,y1;
    for(int i=20;i<thresh;i+=5)
    {

        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("m1",m1);
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
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;


            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<minLen||len2>maxLen)
                continue;

            if(abs(x1-pos1.x)<20&&abs(y1-pos1.y)<20)
            {
                param.var1[29]=1;
                circle( rgb, Point(x1+roi.x,y1+roi.y), 40, Scalar(193,255,3), 2, CV_AA);
                qDebug()<<"空料下";
                return;
            }

            if(abs(x1-pos2.x)<20&&abs(y1-pos2.y)<20)
            {
                param.var1[29]=2;
                circle( rgb, Point(x1+roi.x,y1+roi.y), 40, Scalar(193,255,3), 2, CV_AA);
                qDebug()<<"空料右";
                return;
            }


        }
    }


}
//贴边数量
int calcCircleSide(std::vector<Point> &a,std::vector<Point> &b,int minSideDis,int &getMinVal)
{

    int num1=1000;
    int num=0;
    int len;
    for(int i=0;i<a.size();i++)
    {
        for(int j=0;j<b.size();j++)
        {
            len=common::getLineLen(a[i],b[j]);
            if(len<num1)
                num1=len;
            if(len<minSideDis)
                ++num;
        }
    }
    getMinVal=num1;
    return num;
}
void project2::wx_defect(Mat &src, Rect roi, params &param)
{
    int minSize1=param.var1[20];//size
    int maxSize1=param.var1[21];
    int minLen1=param.var1[22];//len
    int maxLen1=param.var1[23];
    int dif=param.var1[24];//圆心偏差
    int thresh=param.var1[25];//二值
    int maxLen2=param.var1[26];//外圆len

    double ratio=param.var2[0];//像素比
    double minDiameter=param.var2[1];//最小直径
    double maxDiameter=param.var2[2];//最大直径
    double minSideDis=param.var2[3];//贴边最小距离

    if(ratio==0) ratio=1;
    Point cen1,cen2;
    Mat m1,m2,m3;

    int len1,len2,x1,y1,circleSideNum,index1,index2;


    double diameter1=0,diameter2=0;

    Mat rgb=g_dstImage2(roi);

    medianBlur(src(roi), m3, 3);

    for(int i=thresh;i>100;i-=5)
    {
        //out
        param.var1[27]=-1;//是否合格
        param.var1[28]=0;//找到小圆

        cen1.x=cen1.y=cen2.x=cen2.y=0;
        threshold(m3,m1,i,255,THRESH_BINARY);
        threshold(m3,m2,i,255,THRESH_BINARY);
        imshow("m1",m2);
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<minSize1||contours[k].size()>maxSize1)
                continue;
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
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


            if(len1<minLen1||len2>maxLen1)
                continue;



            int num= common::circleFeature(contours[k],End_Rage2D1.center,30,30,30);
            //qDebug()<<"circleFeature"<<num<<"i"<<i;
            if(num<3)
                continue;



            qDebug()<<"len1"<<len1<<maxLen2<<"i"<<i<<"size"<<contours[k].size();
            if(len1>maxLen2&&m2.at<uchar>(contours[k][0].y-1,contours[k][0].x)==0)//外圆
            {


                index1=k;
                qDebug()<<"外圆cen"<<x1+roi.x<<y1+roi.y<<"size"<<contours[k].size()<<"len"<<len1<<len2;
                cen1=End_Rage2D1.center;


            }

            if(m2.at<uchar>(contours[k][0].y-1,contours[k][0].x)==255)//内圆
            {

                for(int i = 0;i< 4;i++)
                {

                    line(rgb,cvPointFrom32f(rectpoint[i]),cvPointFrom32f(rectpoint[(i+1)%4]),Scalar(0,255,0),1);
                }

                index2=k;
                Point2f center;
                float radius;
                minEnclosingCircle(contours[k],center,radius);


                cen2=End_Rage2D1.center;
                diameter1=len2;
                diameter2=radius*2;

                //qDebug()<<"内圆len"<<len1*ratio<<len2*ratio<<"len 3"<<radius*2*ratio<<"cen"<<x1+roi.x<<y1+roi.y;

            }


            if(cen1.x&&cen2.x)
            {
                bool NG=false;

                // param.var1[27]返回值 合格0  直径4  贴边5  裂缝6 未找到铆头-1
                param.var1[28]=1;//找到两个圆
                param.var2[19]=diameter1*ratio;//直径
                param.var2[18]=diameter2*ratio;
                param.var2[17]=common::getLineLen(cen2,cen1)*ratio;//圆心距
                //qDebug()<<"偏差"<<common::getLineLen(cen2,cen1);
                if(common::getLineLen(cen2,cen1)>dif)
                {
                   qDebug()<<"偏差";
                }

                if( param.var2[19]<minDiameter||param.var2[18]>maxDiameter)
                {
                    //直径不合格
                    qDebug()<<"直径不合格";
                    NG=true;
                    param.var1[27]=4;
                }

                int calcMinSide1;
                circleSideNum=calcCircleSide(contours[index1],contours[index2],minSideDis/ratio,calcMinSide1);
                param.var2[16]=calcMinSide1*ratio;
                if(circleSideNum>2)
                {
                    qDebug()<<"贴边";
                    NG=true;
                    param.var1[27]=5;

                }
                //如果超出最小距离且数量不多界面显示时为合格
                if(circleSideNum<=2&&param.var2[16]<minSideDis)
                {
                     param.var2[16]=minSideDis+0.02;
                }
                int num1=common::Smoothlevel(contours[k],10,14,1);


                qDebug()<<"i:"<<i<<"边缘平整"<<num1<<"贴边num"<<circleSideNum<<contours[index1].size()<<contours[index2].size();

                qDebug()<<"************";
                if(!NG)
                {
                    param.var1[27]=0;
                    return;
                }

                break;
            }


        }


    }
    //loop is end


}
void project2::zt_ZDK(Mat &src, Mat &rgb, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    int minLen=param.var1[2];//字体到中心点距离
    int maxLen=param.var1[3];
    unsigned int objMinSize= param.var1[4];//检测字体最小值
    unsigned int objMaxSize=param.var1[5];//检测字体最大值

    int thresh=param.var1[6];//二值
    int triangleLen=param.var1[7];//max len
    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    medianBlur(m5, m5, 3);
    int offset =10;//偏移量
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m5, m5, MORPH_TOPHAT , element);
    else
        morphologyEx(m5, m5, MORPH_BLACKHAT, element);

    for(int i=254;i>thresh;i-=5)
    {

        if(i>200)
            i-=20;
        if(i>160&&i<180)
            i-=10;

        int a1=0,a2=0;
        threshold(m5,m1,i,255,THRESH_BINARY);
        imshow("rrrr",m1);
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

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            circle(rgb,Point(x1+roi.x,y1+roi.y),5,Scalar(255,0,0),2);

            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=common::getLineLen(rectpoint[0],rectpoint[1]);
            len2=common::getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len2<triangleLen)
                continue;

            angle1=common::calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            //字体在中心点相连角度

            if(abs(angle1)<30)
            {
                qDebug()<<"hello"<<angle1<<len1*1.0/len2;
                ++a1;
                for(int i = 0;i< 4;i++)
                {
                    line(rgb,Point(cvPointFrom32f(rectpoint[i]).x+roi.x,cvPointFrom32f(rectpoint[i]).y+roi.y),Point(cvPointFrom32f(rectpoint[(i+1)%4]).x+roi.x,cvPointFrom32f(rectpoint[(i+1)%4]).y+roi.y),Scalar(0,255,0),1);
                }
                if(len2>triangleLen+10)
                    ++a1;
            }
            if(abs(angle1)>150&&abs(angle1)<=180)
            {
                ++a2;
                for(int i = 0;i< 4;i++)
                {
                    line(rgb,Point(cvPointFrom32f(rectpoint[i]).x+roi.x,cvPointFrom32f(rectpoint[i]).y+roi.y),Point(cvPointFrom32f(rectpoint[(i+1)%4]).x+roi.x,cvPointFrom32f(rectpoint[(i+1)%4]).y+roi.y),Scalar(255,0,0),1);
                }
                if(len2>triangleLen+10)
                    ++a2;
            }
            if(a1>1||a2>1)
            {
                param.var1[28]=x1;
                param.var1[29]=y1;
                param.var2[19]=angle1;

                qDebug()<<"i:"<<i<<"size"<<contours[k].size();
                return;
            }
        }


    }
}
void project2::testUdp()
{
    QByteArray byte="startCapture";
    byte.append(254);

}

void project2::armPic()
{


    if(!sharememory->isAttached())
        sharememory->attach();

    if(sharememory->size()>=640*480*3)
    {

        sharememory->lock();           //锁定共享内存
        armImg->imageData=(char*)sharememory->data();
        //cvCvtColor(armImg,armImg2,CV_BGR2RGB);
        //memcpy((void*)armImg->imageData,sharememory->data(),921600);
        cvShowImage("aaa",armImg);
        sharememory->unlock();



    }

}





