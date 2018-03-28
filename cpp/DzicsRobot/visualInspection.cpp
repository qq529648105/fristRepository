#include "visualinspection.h"
#include <QPoint>
#include <QDebug>

static double  getAngale(QPoint p1, QPoint crossp, QPoint p3)
{
   double angle;
   const double PI = 3.14159;
   float a=sqrt(float(p1.x()-p3.x())*(p1.x()-p3.x())+(p1.y()-p3.y())*(p1.y()-p3.y()));
   float b=sqrt(float(p1.x()-crossp.x())*(p1.x()-crossp.x())+(p1.y()-crossp.y())*(p1.y()-crossp.y()));
   float c=sqrt(float(p3.x()-crossp.x())*(p3.x()-crossp.x())+(p3.y()-crossp.y())*(p3.y()-crossp.y()));
   angle=(double)acos((b*b+c*c-a*a)/(2*b*c))*180/PI;
   return angle;
}


/****************************
 *dectImage 是保存的检测区域模板
 *binThreshold 2值图阈值同下 一个阈值
 **************************/

QPoint getTemplateCenter(IplImage *dectImage,int binThreshold)
{
    QPoint center;
    //cvSetImageROI(dectImage,dectArea);
    IplImage *bakImage = cvCreateImage(cvSize(cvGetSize(dectImage).width,cvGetSize(dectImage).height),IPL_DEPTH_8U, 1);
    cvThreshold(dectImage, dectImage, binThreshold, 255, CV_THRESH_BINARY);
   // cvCanny(dectImage,dectImage,200,400,3);
    cvCopy(dectImage,bakImage);
    CvMemStorage *storage = cvCreateMemStorage(0);

    CvSeq *seq = NULL;
    int rectLx=5000,rectLy=5000,rectRx=0,rectRy= 0;
    int cnt = cvFindContours(dectImage,storage,&seq);
    for(;seq != 0; seq = seq->h_next ){
        CvRect rect = cvBoundingRect(seq,0);
        if(rect.width * rect.height <80*80)
            continue;
        if(rect.x <rectLx)
            rectLx = rect.x;
        if(rect.x +rect.width > rectRx)
            rectRx = rect.x + rect.width;
        if(rect.y < rectLy)
             rectLy = rect.y;
        if(rect.y + rect.height >rectRy)
            rectRy = rect.y + rect.height;

    }
    center.setX((rectRx-rectLx)/2 +rectLx);
    center.setY((rectRy-rectLy)/2 +rectLy);
    return center;
}

DETECTRESULT  visulInspection(IplImage *dectImage,CvRect dectArea, CvRect leftArea,int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY){
    DETECTRESULT result;
    qDebug()<<"eeeeeeeeeeeeeeeeeeeee";
    IplImage *dectImage1 = cvCreateImage(cvSize(cvGetSize(dectImage).width,cvGetSize(dectImage).height),IPL_DEPTH_8U, 1);
    qDebug()<<"eeeeeeeeeee";
    cvCopy(dectImage,dectImage1);
    qDebug()<<"222222222222222";
    result.result = 0;
    int t = 10;
    while(!result.result){
        if(t + binThreshold >255)
            break;
        qDebug()<<t;
        cvCopy(dectImage1,dectImage);
        result = visulInspection1(dectImage,dectArea, leftArea, binThreshold + t, offset,tcenterX,tcenterY,angleCoord,pixelRatio,offsetX, offsetY);
        t+=10;
    }
    cvReleaseImage(&dectImage1);
    return result;

}



DETECTRESULT  visulInspection1(IplImage *dectImage,CvRect dectArea, CvRect leftArea,int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY)
{
    DETECTRESULT result;
    int centerX,centerY;
    qDebug()<<"222222222222222";
    cvSetImageROI(dectImage,dectArea);
    qDebug()<<"eeeeeeeeeeeeeeeeeeeeeeeee";
    IplImage *bakImage = cvCreateImage(cvSize(cvGetSize(dectImage).width,cvGetSize(dectImage).height),IPL_DEPTH_8U, 1);
    IplImage *bakImage1 = cvCreateImage(cvSize(cvGetSize(dectImage).width,cvGetSize(dectImage).height),IPL_DEPTH_8U, 1);
    cvThreshold(dectImage, dectImage, binThreshold, 255, CV_THRESH_BINARY);
   // cvCanny(dectImage,dectImage,200,400,3);
    cvCopy(dectImage,bakImage);
    cvCopy(dectImage,bakImage1);
    CvMemStorage *storage = cvCreateMemStorage(0);
    qDebug()<<"11111111";
    CvSeq *seq = NULL;
    int rectLx=5000,rectLy=5000,rectRx=0,rectRy= 0;
    int cnt = cvFindContours(dectImage,storage,&seq);
    for(;seq != 0; seq = seq->h_next ){
        CvRect rect = cvBoundingRect(seq,0);
        if(rect.width * rect.height <40*40)
            continue;
        int rectcX = rect.x + rect.width/2;
        int rectcY = rect.y + rect.height/2;
        if(rectcX <200 && rectcY <200)
            continue;
        if(rectcX <200 && rectcY+rect.height >cvGetSize(dectImage).height-200)
            continue;
        if(rectcX > cvGetSize(dectImage).width -200 &&rectcY <200)
            continue;
        if(rectcX > cvGetSize(dectImage).width-200 && rectcY > cvGetSize(dectImage).height-200)
            continue;
        float area = abs(cvContourArea(seq));
        if(area <800){
            continue;
        }
        if(rect.x <rectLx)
            rectLx = rect.x;
        if(rect.x +rect.width > rectRx)
            rectRx = rect.x + rect.width;
        if(rect.y < rectLy)
             rectLy = rect.y;
        if(rect.y + rect.height >rectRy)
            rectRy = rect.y + rect.height;

    }
    centerX = (rectRx-rectLx)/2 +rectLx;
    centerY = (rectRy-rectLy)/2 +rectLy;
    int r = (rectRx-rectLx + rectRy-rectLy)/4;
    qDebug()<<r<<"fffff";
    if(r>0){
    cvCircle(bakImage,cvPoint(centerX,centerY),r,cvScalar(0,0,0),200);
    cvCircle(bakImage1,cvPoint(centerX,centerY),2,cvScalar(0,0,0),10);
   // cvCircle(bakImage1,cvPoint(418,416),2,cvScalar(0,0,0),10);
    qDebug()<<"eeeeeeee";
    cvRectangle(bakImage1, cvPoint(rectLx, rectLy), cvPoint(rectRx, rectRy),CV_RGB(255, 255, 255), 2, 8);
    }
    qDebug()<<centerX<<centerY<<"ssss";
//    cvNamedWindow("11", 0);
//    cvResizeWindow("11",800,600);

//    cvShowImage("11",bakImage1);
//    cvSaveImage("33.png",bakImage1);
    cvReleaseMemStorage(&storage);
    if(r<=0){
        qDebug()<<"wwwwwwwwwwww";
        result.result = 0;
        cvResetImageROI(dectImage);
        return result;
    }
    qDebug()<<"eeeeeeeeeeeeeee";
    //return result;
    int lx,ly,rx,ry;
    long diffLeft, diffRight;//centerX,centerY;
    CvMemStorage *storage1 = cvCreateMemStorage(0);
    CvSeq *seq1 = NULL;
    int cnt1 = cvFindContours(bakImage,storage1,&seq1);
    int num =0;
    for(;seq1 != 0; seq1 = seq1->h_next ){
        CvBox2D box = cvMinAreaRect2(seq1,NULL);
        CvPoint2D32f pt[4];
        cvBoxPoints(box,pt);
        long len = (pt[1].y - pt[0].y)*(pt[1].y - pt[0].y) + (pt[1].x - pt[0].x)*(pt[1].x - pt[0].x);
        long len1 = (pt[2].y - pt[1].y)*(pt[2].y - pt[1].y) + (pt[2].x - pt[1].x)*(pt[2].x - pt[1].x);
        if(len >len1){
            long temp = len;
            len = len1;
            len1 = temp;
        }
        if(len > ((leftArea.width +offset)*(leftArea.width +offset)) || len < ((leftArea.width -offset)*(leftArea.width-offset)))
        {
         //qDebug()<<"wwwww"<<len;
         //  continue;
        }
        if(len1 > ((leftArea.height +offset)*(leftArea.height +offset)) || len1 < ((leftArea.height -offset)*(leftArea.height -offset))){
           // qDebug()<<"sssss"<<len1;
            continue;
       }
        if(sqrt((long double)len)*sqrt((long double)len1) <(leftArea.width*leftArea.height/2)){

            continue;
        }
        int cx = (pt[0].x + pt[2].x)/2;
        int cy = (pt[0].y + pt[2].y)/2;
        if((cx-centerX)*(cx-centerX)+(cy-centerY)*(cy-centerY)>dectArea.width*dectArea.width/4){
            qDebug()<<"eee";
           // continue;
        }
        if((cx-centerX)*(cx-centerX)+(cy-centerY)*(cy-centerY)<150*150){
            qDebug()<<"eee1";
            continue;
        }
        num++;
        if(num==1){
            lx = (pt[0].x + pt[2].x)/2;
            ly = (pt[0].y + pt[2].y)/2;
            diffLeft = (lx - centerX)*(lx -centerX) + (ly-centerY)*(ly-centerY);
        }else if(num == 2){
            rx = (pt[0].x + pt[2].x)/2;
            ry = (pt[0].y + pt[2].y)/2;
            diffRight = (rx - centerX)*(rx - centerX)+(ry - centerY)*(ry-centerY);
//            centerX = (lx + rx)/2;
//            centerY = (ly + ry)/2;

        }
        if(num ==3){
            int x = (pt[0].x + pt[2].x)/2;
            int y = (pt[0].y + pt[2].y)/2;
            long diff = (x - centerX)*(x -centerX) + (y-centerY)*(y-centerY);
            if(diff > diffLeft && diff > diffRight){
                if(diffLeft >= diffRight){
                    rx = x;
                    ry = y;
                    diffRight = diff;
                }else{
                    lx =x;
                    ly = y;
                    diffLeft = diff;
                }
            }else if(diff > diffLeft){
                lx =x;
                ly = y;
                diffLeft = diff;
            }else if(diff >diffRight){
                rx = x;
                ry = y;
                diffRight = diff;
            }
            num =2;
        }

        for(int i = 0;i<4;++i){
           cvLine(bakImage1,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(255,255,255));
        }
    }

    if(num == 2){
       cvLine(bakImage1,cvPoint(lx,ly),cvPoint(rx,ry),CV_RGB(255,255,255));
       result.result = 1;
       result.offsetX = (centerX - tcenterX);
       result.offsetY =  (centerY - tcenterY);
       rx -= result.offsetX;
       ry -= result.offsetY;
       QPoint p,p1,p2;
       p.setX(tcenterX-50);
       p.setY(tcenterY);
       p1.setX(tcenterX);
       p1.setY(tcenterY);
       p2.setX(rx);
       p2.setY(ry);
       result.angle = getAngale(p,p1,p2);
       qDebug()<<result.angle<<result.offsetX<<result.offsetY;

       int len = sqrt(result.offsetX * result.offsetX + result.offsetY *result.offsetY);
       int x = tcenterX +5;
       int y;
       y  = tcenterY -(-5 * tan(angleCoord*3.14/180));
       int angle1 = getAngale(QPoint(x,y),QPoint(tcenterX,tcenterY),QPoint(centerX,centerY));
       qDebug()<<"angle"<<angle1<<x<<y<<tcenterX<<tcenterY<<centerX<<centerY<<-sin(angle1*3.14/180)*len *pixelRatio;
       result.offsetY = -sin(angle1*3.14/180)*len *pixelRatio + offsetY;
       result.offsetX = -cos(angle1*3.14/180)*len*pixelRatio + offsetX;
       if(result.angle>=0 && result.angle <= 90+angleCoord){
           result.angle -= angleCoord;
       }else if(result.angle > 90+ angleCoord && result.angle<=180){
           result.angle = -180+result.angle-angleCoord;
       }
       qDebug()<<"shuchujieguo"<<result.angle<<result.offsetX<<result.offsetY<<result.offsetX/pixelRatio<<result.offsetY/pixelRatio;
    }else{
       result.result = 0;
    }
    qDebug()<<"num="<<num;


    cvReleaseMemStorage(&storage1);
    if(num == 2){
        cvNamedWindow("11", 0);
        cvResizeWindow("11",800,600);
        cvShowImage("11",bakImage1);
        cvSaveImage("back.png.png",bakImage1);
    }
    cvResetImageROI(dectImage);
    cvReleaseImage(&bakImage);
    cvReleaseImage(&bakImage1);
    qDebug()<<"vvvvvvvvvvvvvvvvvvvvvvv";
    return result;
}




/**********************************
 *function: 位置检测
 *binThreshold :图像二值化阈ht值 254
 *leftArea : 零件左边特征区域范围 230 615  190 240
 *rightArea:零件右边特征区域范围 836 630 194 225
 *pixelRatio：像素比
 *offset :偏差
 *返回:
 *
 *********************************/

DETECTRESULT  visulInspectionBak(IplImage *dectImage,int binThreshold,CvRect dectArea, CvRect leftArea, CvRect rightArea,double pixelRatio,int offset)
{
    DETECTRESULT result;
    cvSetImageROI(dectImage,dectArea);

    int tLx,tLy,tRx,tRy,tcenterX,tcenterY;
    int lx,ly,rx,ry,centerX,centerY;
    int num = 0;
    //模板中心点位置
    tLx = (leftArea.x + leftArea.x +leftArea.width)/2 - dectArea.x;
    tLy = (leftArea.y + leftArea.y + leftArea.height)/2 - dectArea.y;
    tRx = (rightArea.x + rightArea.x +rightArea.width)/2 - dectArea.x;
    tRy = (rightArea.y + rightArea.y +rightArea.height)/2 - dectArea.y;
    tcenterX = (tLx + tRx)/2;
    tcenterY = (tRy + tRy)/2;
    qDebug()<<"tecenter"<<tcenterX<<"tencetery"<<tcenterY;
    cvThreshold(dectImage, dectImage, binThreshold, 255, CV_THRESH_BINARY);

//  cvCopy(g_pGrayImage,g_pBackImage);
    CvMemStorage *storage = cvCreateMemStorage(0);
    CvSeq *seq = NULL;
    int cnt = cvFindContours(dectImage,storage,&seq);
    for(;seq != 0; seq = seq->h_next ){
//          if(area >maxArea || area <minArea )
//            continue;
        if(seq->total <5)
            continue;
        CvBox2D ellipse = cvFitEllipse2(seq);//最小二乘法的椭圆拟合
        if(abs(ellipse.size.width -ellipse.size.height)<20 && ellipse.size.width>700 &&  ellipse.size.width <900){
            qDebug()<<ellipse.size.width<<ellipse.size.width<<ellipse.center.x<<ellipse.center.y;
            cvEllipseBox(dectImage,ellipse,CV_RGB(255,255,255));
        }

        CvBox2D box = cvMinAreaRect2(seq,NULL);
        CvPoint2D32f pt[4];
        cvBoxPoints(box,pt);
          long len = (pt[1].y - pt[0].y)*(pt[1].y - pt[0].y) + (pt[1].x - pt[0].x)*(pt[1].x - pt[0].x);
          long len1 = (pt[2].y - pt[1].y)*(pt[2].y - pt[1].y) + (pt[2].x - pt[1].x)*(pt[2].x - pt[1].x);
          if(len >len1){
              long temp = len;
              len = len1;
              len1 = temp;
          }
          if(len > ((leftArea.width +offset)*(leftArea.width +offset)) || len < ((leftArea.width -offset)*(leftArea.width-offset)))
          {
           //   qDebug()<<"wwwww"<<len;
              continue;
          }
          if(len1 > ((leftArea.height +offset-20)*(leftArea.height +offset-20)) || len1 < ((leftArea.height -offset-20)*(leftArea.height -offset-20))){
             // qDebug()<<"sssss"<<len1;
              continue;
         }
          if(len*len1 <leftArea.width*leftArea.height/2){
              continue;
          }
          qDebug()<<"len"<<len<<len1;
          int cx = (pt[0].x + pt[2].x)/2;
          int cy = (pt[0].y + pt[2].y)/2;
          if((cx-tcenterX)*(cx-tcenterX)+(cy-tcenterY)*(cy-tcenterY)>dectArea.width*dectArea.width/4){
              qDebug()<<"eee";
              continue;
          }
          if((cx-tcenterX)*(cx-tcenterX)+(cy-tcenterY)*(cy-tcenterY)<150*150){
              qDebug()<<"eee1";
              continue;
          }
          num++;
          if(num==1){
              lx = (pt[0].x + pt[2].x)/2;
              ly = (pt[0].y + pt[2].y)/2;

          }else if(num == 2){
              rx = (pt[0].x + pt[2].x)/2;
              ry = (pt[0].y + pt[2].y)/2;
              centerX = (lx + rx)/2;
              centerY = (ly + ry)/2;
              cvLine(dectImage,cvPoint(lx,ly),cvPoint(rx,ry),CV_RGB(255,255,255));
          }

          for(int i = 0;i<4;++i){
             cvLine(dectImage,cvPointFrom32f(pt[i]),cvPointFrom32f(pt[((i+1)%4)?(i+1):0]),CV_RGB(255,255,255));
          }

      }
//    qDebug()<<"num="<<num;
//    cvNamedWindow("11", 0);
//    cvResizeWindow("11",800,600);
//    cvShowImage("11",dectImage);
//   cvSaveImage("111.png",dectImage);
      if(num == 2){
         result.result = 1;
         result.offsetX = (centerX - tcenterX)*pixelRatio;
         result.offsetY =  (centerY - tcenterY)*pixelRatio;
         rx -= result.offsetX;
         ry -= result.offsetY;
         QPoint p,p1,p2;
         p.setX(tLx);
         p.setY(tLy);
         p1.setX(tcenterX);
         p1.setY(tcenterY);
         p2.setX(rx);
         p2.setY(ry);
         result.angle = getAngale(p,p1,p2);
         qDebug()<<result.angle<<result.offsetX<<result.offsetY;
      }else{
         result.result = 0;
      }
    cvResetImageROI(dectImage);
    return result;
}

/**********************************
 *function: 位置反转检测
 *binThreshold :图像二值化阈ht值
 *leftArea : 零件左边特征区域范围
 *rightArea:零件右边特征区域范围
 *pixelRatio：像素比
 *threshold : 黑点个数 （200）
 *isUp：1
 *返回:true 不需要反转
 *    false 需要反转
 *********************************/
long int isWorkPieceRollback(IplImage *dectImage1,IplImage *tp,CvRect dectArea, int time)
{
//    static long double firstValue = 0.0;
//    static long double secondValue = 0.0;

    cvSetImageROI(dectImage1,dectArea);
    IplImage *result;
    CvPoint  minloc, maxloc;
    double   minval, maxval;
    int img_width, img_height;
    int tpl_width, tpl_height;
    int res_width, res_height;
    img_width  = cvGetSize(dectImage1).width;//dectImage->width;
    img_height = cvGetSize(dectImage1).height;//dectImage->height;

    tpl_width  = cvGetSize(tp).width;
    tpl_height = cvGetSize(tp).height;
    res_width  = img_width - tpl_width + 1;
    res_height = img_height - tpl_height + 1;
   // qDebug()<<res_width<<res_height<<img_width<<img_height<<tpl_width<<tpl_height;
    result = cvCreateImage( cvSize(res_width, res_height ), IPL_DEPTH_32F , 1 );
    cvMatchTemplate(tp,dectImage1,result,CV_TM_SQDIFF );
    cvMinMaxLoc( result, &minval, &maxval, &minloc, &maxloc, 0 );
    cvRectangle( dectImage1, minloc, cvPoint( minloc.x + tp->width , minloc.y + tp->height ), cvScalar(0,0,0), 2, 8, 0 );
    cvResetImageROI(dectImage1);
    cvReleaseImage(&result);
    long int value = long int(minval/10000);
    qDebug()<<value<<"ddddddddd";
    return value;
   // return
//    if(time == 1){
//        firstValue = minval;
//    }else if(time ==2){
//        secondValue = minval;
//        if(firstValue < secondValue)
//            return false;
//        else
//            return true;
//    }
//    return true;
}


 double  matchTemplate(IplImage *dectImage,IplImage *tempImage,int method)
{

    IplImage *result;
    CvPoint  minloc, maxloc;
    double   minval, maxval;
    int img_width, img_height;
    int tpl_width, tpl_height;
    int res_width, res_height;
    img_width  = cvGetSize(dectImage).width;//dectImage->width;
    img_height = cvGetSize(dectImage).height;//dectImage->height;

    tpl_width  = cvGetSize(tempImage).width;
    tpl_height = cvGetSize(tempImage).height;
    res_width  = img_width - tpl_width + 1;
    res_height = img_height - tpl_height + 1;
    qDebug()<<res_width<<res_height<<img_width<<img_height<<tpl_width<<tpl_height;
    result = cvCreateImage( cvSize(res_width, res_height ), IPL_DEPTH_32F , 1 );
    cvMatchTemplate(tempImage,dectImage,result,CV_TM_SQDIFF );
    cvMinMaxLoc( result, &minval, &maxval, &minloc, &maxloc, 0 );
    cvRectangle( dectImage, minloc, cvPoint( minloc.x + tempImage->width , minloc.y + tempImage->height ), cvScalar(0,0,0), 2, 8, 0 );
//    cvNamedWindow("11", 0);
//    cvResizeWindow("11",800,600);
//    cvShowImage("11",dectImage);
    qDebug()<<minval;
    return minval;

}


//接口
//areaRect 第一个模板的区域位置
//tp1 tp4 是4个保存下来的模板图像
//method 保留 默认1
bool  isWorkPieceRollback1(IplImage *dectImage,CvRect areaRect,IplImage *tp1,IplImage *tp2,IplImage *tp3,IplImage *tp4, int method){

   cvSetImageROI(dectImage,areaRect);
    double min1 = matchTemplate(dectImage,tp1, method);
    double min2 =  matchTemplate(dectImage,tp2, method);
    double min3 =  matchTemplate(dectImage,tp3, method);
    double min4 =  matchTemplate(dectImage,tp4, method);

    double minR = (min1<min2)?min1:min2;
    double minL = (min3<min4)?min3:min4;
    cvResetImageROI(dectImage);
    if(minR < minL)
        return true;
    else
        return false;

}
//int bSums(Mat src)
//{

//    int counter = 0;
//    //迭代器访问像素点
//    Mat_<uchar>::iterator it = src.begin<uchar>();
//    Mat_<uchar>::iterator itend = src.end<uchar>();
//    for (; it!=itend; ++it)
//    {
//        if((*it)>0) counter+=1;//二值化后，像素点是0或者255
//    }
//    return counter;
//}

int isWorkPieceR2(IplImage *dectImage1, IplImage *tp, CvRect dectArea, int time)
{
    cvSetImageROI(dectImage1,dectArea);

    return -1;

}
