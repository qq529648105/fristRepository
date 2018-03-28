
#include "VisionDetection.h"
#include "highgui.h"
#include <QDebug>
#include <math.h>
#include <QTime>

#define ISIMAGESHOW 0

float rivetDia;  //工件内部铆钉直径
float rivetDceDiff; //铆钉圆心位置和圆盘圆心位置距离差值
float diskDia;  //圆盘直径
/*************update12.09*********/
int armDirection;  //手臂方向   0无  2 方向右  1 方向左
bool isStationNull;  //0空 1为有零件

/**********************update12.09***********/

/*************用到的参数***************/
float pixelScale = 0.0226;  //像素与实际长度比例 1像素约0.0226mm
int threshold = 150;       //图像二值化阈值




/********************/


CICLEPAR outCicle;  //圆盘黑框外圆
CICLEPAR inCicle;   //圆盘中间白色区域圆
CICLEPAR rivetCicle;  //铆钉圆

CvRect centerArea;


IplImage *binaryImage = NULL;
IplImage *contourImage = NULL;
IplImage *viewImage1= NULL;


//得到的轮廓有波折 获取外接圆后 再求内接圆，取品均值作为铆钉圆的半径 返回值为铆钉圆的半径
int getRivetR(unsigned char* data,int dcex, int dcey, int r, int widthStep)
{
    int minx,miny,midr;
    int minx1,miny1;
    int rx,ry;
    int angle;
    int maxr =0;
    int minr = r;
    for(angle=0;angle<360;angle++){
        rx = dcex + cos(angle*3.14/180)*r;
        ry = dcey + sin(angle*3.14/180)*r;
        if(ry>dcey && (ry-dcey)>abs(rx-dcex)){
            float k = float(dcey - ry)/(dcex - rx);
            int j;
            for(j=ry;j>dcey; j--){
                int x = (j-ry)/k+rx;

                if(((unsigned char*)(data+j*widthStep))[x]==0)
                {
                    float r = sqrt(float((x-dcex)*(x-dcex)+(j-dcey)*(j-dcey)));
                    if((int)r>maxr)
                        maxr = r;
                    if((int)r<minr){
                        minx1 = x;
                        miny1 = j;
                        if(rx >dcex)
                            minx =  2*dcex - rx;
                        else
                            minx = 2*dcex -rx;
                        miny =  2*dcey -ry;
                        minr =r;
                    }
                    break;
                }

            }
        }else if(ry<dcey && (dcey-ry)>abs(rx-dcex)){
            float k = float(dcey - ry)/(dcex - rx);
            int j;
            for(j=ry;j<dcey; j++){
                int x = (j-ry)/k+rx;

                if(((unsigned char*)(data+j*widthStep))[x]==0)
                {
                    float r = sqrt(float((x-dcex)*(x-dcex)+(j-dcey)*(j-dcey)));
                    if((int)r>maxr)
                        maxr = r;
                    if((int)r<minr){
                        minx1 = x;
                        miny1 = j;
                        if(rx >dcex)
                            minx = 2*dcex-rx;
                        else
                            minx = 2*dcex -rx;
                        miny = 2*dcey -ry;
                        minr =r;
                    }
                    break;
                }

            }

        }
        else if(rx<dcex && (dcex-rx)>abs(ry-dcey)){
            float k = float(dcey - ry)/(dcex - rx);
            int j;
            for(j=rx;j<dcex; j++){
                int y = (j - rx)*k +ry;

                if(((unsigned char*)(data+y*widthStep))[j]==0)
                {
                    float r = sqrt(float((j-dcex)*(j-dcex)+(y-dcey)*(y-dcey)));
                    if((int)r>maxr)
                        maxr = r;
                    if((int)r<minr){
                        minx1 = j;
                        miny1 = y;
                        minr =r;
                        if(ry >dcey)
                            miny =  2*dcey-ry;
                        else
                            miny = 2*dcey-ry;
                        minx = 2*dcex - rx;
                    }
                    break;
                }

            }

        }
        else if(rx>dcex && (rx-dcex)>abs(ry-dcey)){
            float k = float(dcey - ry)/(dcex - rx);
            int j;
            for(j=rx;j>dcex; j--){
                int y = (j - rx)*k +ry;

                if(((unsigned char*)(data+y*widthStep))[j]==0)
                {
                    float r = sqrt(float((j-dcex)*(j-dcex)+(y-dcey)*(y-dcey)));
                    if((int)r>maxr)
                        maxr = r;
                    if((int)r<minr){
                        minx1 = j;
                        miny1 = y;
                        minr = r;
                        if(ry >dcey)
                            miny =  2*dcey -ry;
                        else
                            miny = 2*dcey -ry;
                        minx = 2*dcex - rx;

                    }
                    break;
                }

            }

        }

    }

    if(miny>dcey && (miny-dcey)>abs(minx-dcex)){
        float k = float(dcey - miny)/(dcex - minx);
        int j;
        for(j=miny;j>dcey; j--){
            int x = (j-miny)/k+minx;

            if(((unsigned char*)(data+j*widthStep))[x]==0)
            {
                float r = sqrt(float((x-dcex)*(x-dcex)+(j-dcey)*(j-dcey)));
                midr = r;
                break;
            }

        }
    }else if(miny<dcey && (dcey-miny)>abs(minx-dcex)){
        float k = float(dcey - miny)/(dcex - minx);
        int j;
        for(j=miny;j<dcey; j++){
            int x = (j-miny)/k+minx;

            if(((unsigned char*)(data+j*widthStep))[x]==0)
            {
                float r = sqrt(float((x-dcex)*(x-dcex)+(j-dcey)*(j-dcey)));
                midr = r;
                break;
            }

        }

    }
    else if(minx<dcex && (dcex-minx)>abs(miny-dcey)){
        float k = float(dcey - miny)/(dcex - minx);
        int j;
        for(j=minx;j<dcex; j++){
            int y = (j - minx)*k +miny;

            if(((unsigned char*)(data+y*widthStep))[j]==0)
            {
                float r = sqrt(float((j-dcex)*(j-dcex)+(y-dcey)*(y-dcey)));
                midr = r;
                break;
            }

        }

    }
    else if(minx>dcex && (minx-dcex)>abs(miny-dcey)){
        float k = float(dcey - miny)/(dcex - minx);
        int j;
        for(j=minx;j>dcex; j--){
            int y = (j - minx)*k +miny;

            if(((unsigned char*)(data+y*widthStep))[j]==0)
            {
                float r = sqrt(float((j-dcex)*(j-dcex)+(y-dcey)*(y-dcey)));
                midr = r;
                break;
            }

        }

    }


     minr = (minr+midr)/2;

    // qDebug()<<minr<<"  "<<maxr;
     //  cvCircle( viewImage1, cvPoint(dcex,dcey) ,minr , CV_RGB(0,0,0),3, 8, 3 );
      // cvCircle( viewImage1, cvPoint(dcex,dcey) ,maxr , CV_RGB(0,0,0),1, 8, 3 );
     //  cvCircle(binaryImage,cvPoint(dcex,dcey) ,minr , CV_RGB(255,255,255),1, 8, 3 );
       //cvCircle(binaryImage,cvPoint(dcex,dcey) ,maxr , CV_RGB(0,0,0),1, 8, 3 );
    return (minr+maxr)/2;
   // return maxr;
}

//寻找圆盘直径 由于圆盘轮廓直接和手柄连在一起 无法直接求外接圆 采用内圈圆往外扩展方式求得 返回圆盘直径
int findDiskDia(unsigned char *data,int dcex, int dcey, int r, int widthStep){
    int diskR;
    int r1,r2,r3;
    for(int x = dcex+r+100; x< binaryImage->width; x++){
        unsigned char value = ((unsigned char*)(data+dcey*widthStep))[x];
        if(value==0){
            r1 = x- dcex;
            break;
        }
    }
    for(int x = dcex-r-100; x> 0; x--){
        unsigned char value = ((unsigned char*)(data+dcey*widthStep))[x];
        if(value==0){
            r2 = dcex -x;
            break;
        }
    }
    for(int y = dcey-r-100; y>0; y-- ){
        unsigned char value = ((unsigned char*)(data+dcey*widthStep))[dcex];
        if(value ==0){
            r3 = dcey -y;
            break;
        }
    }
    if(abs(r1-r2)>15)
        return 0;
    else
        return (r1+r2);
}


//在一定区域内找圆 大小依次为 上面3个圆
int findCircle(IplImage *binaryImage,float scale,int minDia){


    CICLEPAR swapCicle1,swapCicle2,swapCicle3,swapCicle4, tmpCicle;


    cvCopy(binaryImage, contourImage, NULL);
    CvMemStorage *storage=cvCreateMemStorage(0);
    CvSeq *contour =0;
    cvFindContours( contourImage, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    int num = 0;

    for(contour ;contour!= NULL; contour= contour->h_next)
    {
        if(contour->total <5)
            continue;
        CvBox2D ellipse = cvFitEllipse2(contour);//最小二乘法的椭圆拟合

        int centerx = ellipse.center.x;
        int centery = ellipse.center.y;
        int width = ellipse.size.width;
        int height = ellipse.size.height;
        int d = (width+height)/2;
        //cvCircle(binaryImage,cvPointFrom32f(center),cvRound(radius),CV_RGB(0,0,0));
        if(width > 1.5 *height || height >1.5*width)
            continue;
        if(centerx <centerArea.x || centerx  >centerArea.x + centerArea.width ||centery<centerArea.y ||centery >centerArea.y+centerArea.height || d*scale <(6-0.3) || d*scale >18)
            continue;
        cvEllipseBox(viewImage1,ellipse,CV_RGB(0,0,0)); //在图上画椭圆
         cvEllipseBox(binaryImage,ellipse,CV_RGB(255,255,255)); //在图上画椭圆
      // qDebug()<<ellipse.center.x<<ellipse.center.y<<width<<height<<" ..";
       //qDebug()<<(width+height)/2*scale<<"   >>>";

        num++;
        if(num==1){
            swapCicle1.discx = centerx;
            swapCicle1.discy = centery;
            swapCicle1.r     = d/2;

        }
        else if(num==2){
            swapCicle2.discx = centerx;
            swapCicle2.discy = centery;
            swapCicle2.r     = d/2;

        }
        else if(num==3){
            swapCicle3.discx = centerx;
            swapCicle3.discy = centery;
            swapCicle3.r     = d/2;

        }
    }

    if(num ==3){
        for(int i=0;i<3;i++)
        {
            if (swapCicle1.r >swapCicle2.r )
            { tmpCicle.r=swapCicle1.r;
                tmpCicle.discx = swapCicle1.discx;
                tmpCicle.discy = swapCicle1.discy;
                swapCicle1.r = swapCicle2.r;
                swapCicle1.discx = swapCicle2.discx;
                swapCicle1.discy = swapCicle2.discy;
                swapCicle2.r  = tmpCicle.r;
                swapCicle2.discx  = tmpCicle.discx;
                swapCicle2.discy  = tmpCicle.discy;
            }
            if (swapCicle2.r >swapCicle3.r )
            {
                tmpCicle.r=swapCicle2.r;
                tmpCicle.discx = swapCicle2.discx;
                tmpCicle.discy = swapCicle2.discy;
                swapCicle2.r = swapCicle3.r;
                swapCicle2.discx = swapCicle3.discx;
                swapCicle2.discy = swapCicle3.discy;
                swapCicle3.r  = tmpCicle.r;
                swapCicle3.discx  = tmpCicle.discx;
                swapCicle3.discy  = tmpCicle.discy;
            }
        }
        rivetCicle.r = swapCicle1.r;
        rivetCicle.discx = swapCicle1.discx;
        rivetCicle.discy = swapCicle1.discy;
        inCicle.r = swapCicle2.r;
        inCicle.discx = swapCicle2.discx;
        inCicle.discy = swapCicle2.discy;
        outCicle.r = swapCicle3.r;
        outCicle.discx = swapCicle3.discx;
        outCicle.discy = swapCicle3.discy;


    }else if(num == 2){
        if(swapCicle1.r >swapCicle2.r){
            outCicle.r = swapCicle1.r;
            outCicle.discx = swapCicle1.discx;
            outCicle.discy = swapCicle1.discy;
            inCicle.r = swapCicle2.r;
            inCicle.discx = swapCicle2.discx;
            inCicle.discy = swapCicle2.discy;

        }else{
            outCicle.r = swapCicle2.r;
            outCicle.discx = swapCicle2.discx;
            outCicle.discy = swapCicle2.discy;
            inCicle.r = swapCicle1.r;
            inCicle.discx = swapCicle1.discx;
            inCicle.discy = swapCicle1.discy;

        }
     }

    cvReleaseMemStorage(&storage);
    return num;

}

//有可能铆钉粘着边导致轮廓圆一部分没有 铆钉圆计算不出，自己补下2值图后再进行寻找，此情况圆心偏离太多 已经属于不合格产品固圆寻找标准放宽
int isCompleteCircle(IplImage *binaryImage, int dcex, int dcey, int r, int minDia)
{

    int r1;
    for(r1=r;r1>r-10;r1--){
        cvCircle(binaryImage, cvPoint(dcex,dcey) ,r1 , CV_RGB(255,255,255),1, 8, 3 );
    }
    int num = findCircle(binaryImage,pixelScale,minDia);
    return num;
}


/****************update12.09**************************/

int detectArmDirection(unsigned char *data, int widthStep)
{

    int pos = 0;
    int th = 0;
    int threshold = 10;
    float k = -1.514; //工位平台斜率
    int lx = 1948;
    int ly = 1648;  //工位台左边基准点
    int rx = 2268;
    int ry = 1188;  //工位台右边基准
    int y,x;
    //cvCircle(binaryImage,cvPoint(1560,920),450,CV_RGB(0,0,0),4);
    for(y = ly; y <binaryImage->height; y++){
        //k = (ly-y)/(lx-x);
        x = lx -(ly-y)/k;
      //  cvCircle(binaryImage,cvPoint(x,y),2,CV_RGB(0,0,0),1,8);
        unsigned char value = ((unsigned char*)(data+y*widthStep))[x];
        if(value ==0){
            th++;
            if(th >threshold)
            {
                pos = 2;
                isStationNull = 1;
                return pos;
            }
        }
        else
            th=0;

    }
//    if(th >threshold)
//    {
//        pos = 2;
//        return pos;
//    }
    th =0;
    for(x = rx; x <binaryImage->width; x++){
        y=(x-rx)*k+ry;
       cvCircle(binaryImage,cvPoint(x,y),2,CV_RGB(0,0,0),1,8);
        unsigned char value = ((unsigned char*)(data+y*widthStep))[x];
        if(value ==0){
            th++;
            if(th >threshold){
                pos =1;
                isStationNull = 1;
                return pos;
            }


        }
        else
            th=0;
    }

    if(pos == 0 ){

         isStationNull = 0;
         pos = 2;
         CvRect rect;
         rect.x = 735;
         rect.y = 1115;
         rect.width = 185;
         rect.height = 173;
         cvCopy(binaryImage, contourImage, NULL);
         cvSetImageROI(contourImage,rect);
         CvMemStorage *storage=cvCreateMemStorage(0);
         CvSeq *contour =0;
         cvFindContours( contourImage, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
         //int num = 0;
         for(contour ;contour!= NULL; contour= contour->h_next){
             if(contour->total <5){
                continue;
             }
             CvBox2D ellipse = cvFitEllipse2(contour);//最小二乘法的椭圆拟合
             int width = ellipse.size.width;
             int height = ellipse.size.height;
             if(abs(width - height)<5 && width >80 && height >80){
               //  cvCircle(contourImage,cvPointFrom32f(center),cvRound(radius),CV_RGB(0,0,0));
                 pos = 1;
                 break;
             }
         }
          cvReleaseMemStorage(&storage);

        /* int angle;
         int rx,ry;
         int dcex = 1560;
         int dcey = 920;
         int r = 450;
         int th = 0;
         for(angle=60;angle<120;angle++){
             rx = dcex + cos(angle*3.14/180)*r;
             ry = dcey + sin(angle*3.14/180)*r;
            // cvCircle(binaryImage,cvPoint(rx,ry),2,CV_RGB(0,0,0),4);
            unsigned char value = ((unsigned char*)(data+ry*widthStep))[rx];
            if(value == 0)
                th++;

         }
         if(th >4){
             pos = 2;
         }else{
             pos =1;
         }*/


    }
    cvResetImageROI(contourImage);

    return pos;
}


/**************update12.25****************/

bool detectTrac(IplImage* viewImage, int dcex, int dcey, int inR , int outR,int threshold)
{
      cvThreshold(viewImage,binaryImage,threshold,255,CV_THRESH_BINARY);
      unsigned char *data = (unsigned char*)binaryImage->imageData;
      int widthStep = binaryImage->widthStep;
      int angle;
      int r = outR -10;
      int len = outR - inR -25;
      int blackCount =0;
      int rx,ry;
      for(angle=0;angle<360;angle++){
          rx = dcex + cos(angle*3.14/180)*r;
          ry = dcey + sin(angle*3.14/180)*r;
          if(ry>dcey && (ry-dcey)>abs(rx-dcex)){
              float k = float(dcey - ry)/(dcex - rx);
              int j;
              for(j=ry;j>dcey; j--){
                  int x = (j-ry)/k+rx;
                  float r = sqrt(float((x-dcex)*(x-dcex)+(j-dcey)*(j-dcey)));
                  if(r < inR+10){
                      blackCount = 0;
                      break;
                  }
                  if(((unsigned char*)(data+j*widthStep))[x]==0)
                  {
                      blackCount++;
                      if(blackCount >= 5)
                      {
                        return false;
                      }

                  }else{
                      blackCount = 0;
                  }

              }
          }else if(ry<dcey && (dcey-ry)>abs(rx-dcex)){
              float k = float(dcey - ry)/(dcex - rx);
              int j;
              for(j=ry;j<dcey; j++){
                  int x = (j-ry)/k+rx;
                  float r = sqrt(float((x-dcex)*(x-dcex)+(j-dcey)*(j-dcey)));
                  if(r < inR+10){
                      blackCount = 0;
                      break;
                  }
                  if(((unsigned char*)(data+j*widthStep))[x]==0)
                  {
                      blackCount++;
                      if(blackCount >= 5)
                      {
                        return false;
                      }

                  }else{
                      blackCount = 0;
                  }

              }

          }
          else if(rx<dcex && (dcex-rx)>abs(ry-dcey)){
              float k = float(dcey - ry)/(dcex - rx);
              int j;
              for(j=rx;j<dcex; j++){
                  int y = (j - rx)*k +ry;
                  float r = sqrt(float((j-dcex)*(j-dcex)+(y-dcey)*(y-dcey)));
                  if(r < inR+10){
                      blackCount = 0;
                      break;
                  }
                  if(((unsigned char*)(data+y*widthStep))[j]==0)
                  {

                      blackCount++;
                      if(blackCount >= 5)
                      {
                        return false;
                      }

                  }else{
                      blackCount = 0;
                  }

              }

          }
          else if(rx>dcex && (rx-dcex)>abs(ry-dcey)){
              float k = float(dcey - ry)/(dcex - rx);
              int j;
              for(j=rx;j>dcex; j--){
                  int y = (j - rx)*k +ry;
                  float r = sqrt(float((j-dcex)*(j-dcex)+(y-dcey)*(y-dcey)));
                  if(r < inR+10){
                      blackCount = 0;
                      break;
                  }
                  if(((unsigned char*)(data+y*widthStep))[j]==0)
                  {

                      blackCount++;
                      if(blackCount >= 5)
                      {
                        return false;
                      }
                  }else{
                      blackCount = 0;
                  }


              }

          }

      }
    return true;
}


/********************/



/*********************update12.09********************/

bool detecitonWorkPiece(IplImage* viewImage, CvRect areaRect,float rivetminDia, float rivetmaxDia,float dcediff,int threshold, float pixelScale)
{

   cvResetImageROI(viewImage);
   cvSmooth(viewImage,viewImage,CV_GAUSSIAN,3,3,0,0);
   viewImage1 = viewImage;
    rivetDia =0.0;
    rivetDceDiff = 0.0;
    diskDia = 0.0;
    centerArea = areaRect;
    binaryImage = cvCreateImage(cvGetSize(viewImage),viewImage->depth,viewImage->nChannels);
    contourImage = cvCreateImage(cvGetSize(binaryImage),binaryImage->depth,binaryImage->nChannels);
    cvThreshold(viewImage,binaryImage,threshold,255,CV_THRESH_BINARY);
    unsigned char *binData = (unsigned char*)binaryImage->imageData;
    armDirection = detectArmDirection(binData, binaryImage->widthStep);

    if(!isStationNull)
        return false;
    int num = findCircle(binaryImage,pixelScale,rivetminDia);

  // qDebug()<<num<<"sssss";
    bool isQualified  = true;
    unsigned char *data = (unsigned char*)contourImage->imageData;
    if(num !=2 && num !=3 ){
        /*******update12.09*********/
        isQualified = false;
        /*********update12.09*******/
    }else if(num == 2){
      //  num =  isCompleteCircle(binaryImage, inCicle.discx, inCicle.discy, inCicle.r,rivetminDia);
        /**************update12.09***********/
        isQualified = false;
        /************update12.09**********/
    }
    if(num ==3 ){
        // 求铆钉圆直径

        float inDia = rivetCicle.r*2*pixelScale;
        rivetDia = inDia;
        //求圆盘直径
        int diskImageDia = findDiskDia(data,outCicle.discx, outCicle.discy, outCicle.r,contourImage->widthStep);
        diskDia = (float)(diskImageDia*pixelScale);
        //求圆心距
        float diff = sqrt((float)(inCicle.discx - rivetCicle.discx)*(inCicle.discx - rivetCicle.discx) + (inCicle.discy - rivetCicle.discy)*(inCicle.discy - rivetCicle.discy));
        (float)diff*=pixelScale;
        rivetDceDiff = diff;

        //判断是否合格
        if(rivetDia > rivetmaxDia || rivetDia <rivetminDia || rivetDceDiff > dcediff)
            isQualified = false;

       //判断痕迹
       bool isHas = detectTrac(viewImage, outCicle.discx, outCicle.discy, outCicle.r , diskDia/2,90);
       if(!isHas)
           isQualified = false;
        cvCircle(viewImage,cvPoint(rivetCicle.discx,rivetCicle.discy),rivetCicle.r,cvScalar(0,0,0),1);
     //  cvCircle(viewImage,cvPoint(outCicle.discx,outCicle.discy), diskImageDia/2,cvScalar(0,0,0),4);

    }
     /******************update12.09**************/

      /********************update12.09***************/

   if(ISIMAGESHOW){
        cvNamedWindow("rect_image",0);
        cvNamedWindow("binary_image",0);
       // cvNamedWindow("contour_image",1);
        cvResizeWindow("rect_image",648,488);
        cvResizeWindow("binary_image",648,488);
      //  cvResizeWindow("contour_image",648,488);
          //cvResizeWindow("",648,488);
       // cvSetImageROI(viewImage,cvRect(800,350,1300,1100));
         //cvSetImageROI(viewImage1,cvRect(800,350,1300,1100));
       //  cvSetImageROI(binaryImage,cvRect(800,350,1300,1100));
       //cvShowImage("rect_image",viewImage);
        cvShowImage("binary_image",binaryImage);//鏄剧ず浜屽€煎浘
     //   cvSaveImage("bin.png",viewImage1);
        cvShowImage("rect_image",viewImage1);
       // cvResetImageROI(viewImage);
        //cvResetImageROI(viewImage1);
       // cvResetImageROI(binaryImage);
        //cvResetImageROI(binaryImage);

    }

    cvReleaseImage(&binaryImage);
    cvReleaseImage(&contourImage);
    return isQualified;

}



//更新11.16 求圆心   最后2个参数是为了求实际圆的直径好排除不是所求的圆 这个参数要更具实际设置
CICLEPAR  getCicularDce(IplImage* viewImage, CvRect areaRect,int threshold, float pixelScale,float Dia )
{
    cvResetImageROI(viewImage);
    CICLEPAR cicle;
    //CICLEPARDIFF cicleDiff;
    binaryImage = cvCreateImage(cvGetSize(viewImage),viewImage->depth,viewImage->nChannels);
    cvThreshold(viewImage,binaryImage,threshold,255,CV_THRESH_BINARY);
    contourImage = cvCreateImage(cvGetSize(binaryImage),binaryImage->depth,binaryImage->nChannels);
    cvCopy(binaryImage, contourImage, NULL);
    CvMemStorage *storage=cvCreateMemStorage(0);
    CvSeq *contour =0;
    cvFindContours( contourImage, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    for(contour ;contour!= NULL; contour= contour->h_next)
    {
        CvPoint2D32f center;
        float radius;
        cvMinEnclosingCircle(contour,&center,&radius);
        if(center.x +radius > areaRect.width +areaRect.x  || center.x -radius<areaRect.x ||center.y-radius <areaRect.y || center.y +radius >areaRect.height+areaRect.y)
            continue;
        if(abs(Dia-radius*pixelScale*2)>0.4)
            continue;

        cicle.discx = center.x;
        cicle.discy = center.y;
        cicle.r = radius;
       // cvCircle(viewImage,cvPointFrom32f(center),cvRound(radius),CV_RGB(0,0,0),4);
      //  qDebug()<<cicle.discx <<cicle.discy <<cicle.r;

   }
////    if(ISIMAGESHOW){
////        cvNamedWindow("rect_image",0);
////        cvNamedWindow("binary_image",0);
////        cvNamedWindow("contour_image",0);
////        cvResizeWindow("rect_image",648,488);
////        cvResizeWindow("binary_image",648,488);
////        cvResizeWindow("contour_image",648,488);
////        cvShowImage("rect_image",viewImage);
////        cvShowImage("binary_image",binaryImage);//鏄剧ず浜屽€煎浘
////        cvShowImage("contour_image",contourImage);
//    }
     cvReleaseMemStorage(&storage);
     cvReleaseImage(&binaryImage);
     cvReleaseImage(&contourImage);
    return cicle;
}




CICLEPAR  getBaseCicularDce(IplImage* viewImage, CvRect areaRect,int threshold, float pixelScale,float Dia )
{
    cvResetImageROI(viewImage);
    CICLEPAR cicle;
   // CICLEPARDIFF cicleDiff;
    binaryImage = cvCreateImage(cvGetSize(viewImage),viewImage->depth,viewImage->nChannels);
    cvThreshold(viewImage,binaryImage,threshold,255,CV_THRESH_BINARY);
    contourImage = cvCreateImage(cvGetSize(binaryImage),binaryImage->depth,binaryImage->nChannels);
    cvCopy(binaryImage, contourImage, NULL);
    CvMemStorage *storage=cvCreateMemStorage(0);
    CvSeq *contour =0;
    cvFindContours( contourImage, storage, &contour, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_NONE);
    //int num = 0;
    for(contour ;contour!= NULL; contour= contour->h_next)
    {
        CvPoint2D32f center;
        float radius;
        cvMinEnclosingCircle(contour,&center,&radius);
        if(center.x +radius > areaRect.width +areaRect.x  || center.x -radius<areaRect.x ||center.y-radius <areaRect.y || center.y +radius >areaRect.height+areaRect.y)
            continue;

        if(abs(radius*pixelScale*2-Dia)>2)
            continue;
        cicle.discx = center.x;
        cicle.discy = center.y;
        cicle.r = radius;
//        cicleDiff.discx = center.x*pixelScale;
//        cicleDiff.discy = center.y*pixelScale;
//        cicleDiff.r = cicle.r*pixelScale;
      //  cvCircle(viewImage,cvPointFrom32f(center),cvRound(radius),CV_RGB(0,0,0),4);
       // qDebug()<<cicle.discx <<cicle.discy <<cicle.r;
      //  qDebug()<<cicleDiff.discx<<cicleDiff.discy<<cicleDiff.r;

      // return cicle;
    }
//    if(ISIMAGESHOW){
//        cvNamedWindow("rect_image",0);
//        cvNamedWindow("binary_image",0);
//        cvNamedWindow("contour_image",0);
//        cvResizeWindow("rect_image",648,488);
//        cvResizeWindow("binary_image",648,488);
//        cvResizeWindow("contour_image",648,488);
//        cvShowImage("rect_image",viewImage);
//        cvShowImage("binary_image",binaryImage);//鏄剧ず浜屽€煎浘
//        cvShowImage("contour_image",contourImage);
//    }
    cvReleaseMemStorage(&storage);
    cvReleaseImage(&binaryImage);
    cvReleaseImage(&contourImage);
    return cicle;
}


CICLEDCEOFFSET getDceOffset(CICLEPAR rivetCicle, CICLEPAR baseCicle, float pixelScale)
{
      CICLEDCEOFFSET offset;
      offset.x = (rivetCicle.discx - baseCicle.discx)*pixelScale;
      offset.y = (rivetCicle.discy - baseCicle.discy)*pixelScale;
      return  offset;

}



/****************update12.16***************************/
bool detectGap(IplImage* viewImage, CvRect areaRect, int threshold, int threshold1)
{
    int x,y;
    int count =0;
    int count1 =0;
    bool isCount = 0;
    int widthStep = viewImage->widthStep;

    unsigned char* data = (unsigned char*)viewImage->imageData;
    int gap =100;
    for(x = areaRect.x; x< areaRect.width-1 + areaRect.x; x++){
        for(y = areaRect.y; y< areaRect.height-1 + areaRect.y;y++){
            unsigned char value = ((unsigned char*)(data+y*widthStep))[x];
           // qDebug()<<value<<"sss";
            if(value>threshold ){
                if(!isCount)
                    isCount = true;
                count ++;

            }else{
                if(isCount && x != areaRect.x){
                    isCount = false;
                    if(count>1 && gap-count>8){
                        count1++;
                       // qDebug()<<"count="<<count;
                    }
                    else
                        count1= 0;
                    count =0;
                }

            }
        }
       // qDebug()<<gap<<count;
        if(x == areaRect.x ){
            gap = count;
        }else if(count1 >  threshold1){
            cvRectangle(viewImage,cvPoint(areaRect.x, areaRect.y),cvPoint(areaRect.x + areaRect.width,areaRect.y+ areaRect.height),cvScalar(0,0,0));
            cvNamedWindow("contour_image",0);
            cvvResizeWindow("contour_image",640,480);
            cvShowImage("contour_image",viewImage);
                return false;
            }
       // qDebug()<<gap;

}
    cvRectangle(viewImage,cvPoint(areaRect.x, areaRect.y),cvPoint(areaRect.x + areaRect.width,areaRect.y+ areaRect.height),cvScalar(0,0,0));
    cvNamedWindow("contour_image",0);
    cvvResizeWindow("contour_image",640,480);
    cvShowImage("contour_image",viewImage);
    return true;

}
/*********************update12.16****************/


//update by zhugz
void rotateRoi(Mat &src,Rect roi)
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

void cppTemplateMatch(Mat &src,Mat &temp,Point &p,Rect &roi,int method)
{
   int  nMatchMethod=method;
    //【1】给局部变量初始化
    Mat resultImage;

    Mat srcImage=src(roi).clone();

    //【2】初始化用于结果输出的矩阵
    int resultImage_cols =  srcImage.cols - temp.cols + 1;
    int resultImage_rows = srcImage.rows - temp.rows + 1;
   //  qDebug()<<"resultImage_cols"<<srcImage.rows<<srcImage.cols<<temp.cols <<temp.rows;
    resultImage.create( resultImage_cols, resultImage_rows, CV_32FC1 );

    //【3】进行匹配和标准化

    matchTemplate( srcImage, temp, resultImage, nMatchMethod );

    normalize( resultImage, resultImage, 0, 1, NORM_MINMAX, -1, Mat() );

    //【4】通过函数 minMaxLoc 定位最匹配的位置
    double minValue; double maxValue; Point minLocation; Point maxLocation;
    Point matchLocation;

    minMaxLoc( resultImage, &minValue, &maxValue, &minLocation, &maxLocation, Mat() );

    //【5】对于方法 SQDIFF 和 SQDIFF_NORMED, 越小的数值有着更高的匹配结果. 而其余的方法, 数值越大匹配效果越好
    if( nMatchMethod  == CV_TM_SQDIFF || nMatchMethod == CV_TM_SQDIFF_NORMED )
    { matchLocation = minLocation; }
    else
    { matchLocation = maxLocation; }
    p.x=matchLocation.x;
    p.y=matchLocation.y;

    //【6】绘制出矩形，并显示最终结果

   // rectangle( resultImage, matchLocation, Point( matchLocation.x + temp.cols , matchLocation.y + temp.rows ), Scalar(0,0,255), 2, 8, 0 );

    //        CV_TM_SQDIFF        =0,
    //        CV_TM_SQDIFF_NORMED =1,
    //        CV_TM_CCORR         =2,
    //        CV_TM_CCORR_NORMED  =3,
    //        CV_TM_CCOEFF        =4,
    //        CV_TM_CCOEFF_NORMED =5
    switch(nMatchMethod)
    {
    case CV_TM_SQDIFF:
        minValue / (temp.cols * temp.cols);
        break;
    case CV_TM_CCOEFF:
        maxValue / (temp.cols * temp.cols);
        break;
    }

   // qDebug()<<"method"<<nMatchMethod<<"min max"<<minValue<<maxValue;
    //qDebug()<<"min max"<<minValue/(temp.cols*temp.cols)<<maxValue/(temp.cols*temp.cols);

   // imshow( "84_2", resultImage );
}

int gap1(Mat &src,Rect roi,int minWidth,int difVal)
{
    if(roi.width>src.cols-1||roi.height>src.rows-1||src.rows==0||src.cols==0)
        return 0;
    int threshVal,tmp1,tmp2,leftx=0,rightx=0;
    threshVal=src.at<uchar>(roi.y+roi.height/2,roi.x+roi.width/2);

    for(int i=roi.x+roi.width/2;i>roi.x;i--)
    {
        for(int j=roi.y;j<roi.y+roi.height;j++)
        {
            tmp1=src.at<uchar>(j,i);
            tmp2=src.at<uchar>(j,roi.x*2+roi.width-i);
            if(leftx==0&&tmp1-threshVal>40)
                leftx=i;
            if(rightx==0&&tmp2-threshVal>40)
                rightx=roi.x*2+roi.width-i;

            if(leftx&&rightx)
                goto END;

        }
    }
END:
    return rightx-leftx+difVal<minWidth?0:1;

}


