#include"visiondll.h"
#include "VisionRivoet.h"
#include "common.h"
#include <algorithm>

using namespace cv;
static double  getAngale(myPoint p1, myPoint crossp, myPoint p3)
{
   double angle;
   const double PI = 3.14159;
   float a=sqrt(float(p1.x-p3.x)*(p1.x-p3.x)+(p1.y-p3.y)*(p1.y-p3.y));
   float b=sqrt(float(p1.x-crossp.x)*(p1.x-crossp.x)+(p1.y-crossp.y)*(p1.y-crossp.y));
   float c=sqrt(float(p3.x-crossp.x)*(p3.x-crossp.x)+(p3.y-crossp.y)*(p3.y-crossp.y));
   angle=(double)acos((b*b+c*c-a*a)/(2*b*c))*180/PI;
   if(p3.y > p1.y && p3.x <crossp.x)
	   angle = 180 -angle;
/*   if(p3.y <p1.y && p3.x <crossp.x)
	   angle = 180 -angle;*/
  // printf("fangle = %f",angle);
   return angle;
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



int calcWork(Mat img,Point &p,float &angle,int c1=603,int c2=623)
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

    if(circles.size()<=2)
        return -1;

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
	if(-1==j1 || -1 == j2 || -1==j3 || -1==j4 )
		return -1;
	
	
    p1.x=circles[j1][0];
    p1.y=circles[j1][1];
    p2.x=circles[j2][0];
    p2.y=circles[j2][1];

    angle=calcAngle(p1,p2);

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
#ifdef DEBUG 
    circle( cimg, Point(p.x, p.y), 3, Scalar(255,0,0), 3, CV_AA);
    namedWindow("detected circles",0);
    resizeWindow("detected circles",800,600);
    imshow("detected circles", cimg);
#endif
	
		return 0;
    //end
}







DETECTRESULT  visulInspection1(IplImage *dectImage,CvRect dectArea, CvRect leftArea,int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY,float anagle)
{
    DETECTRESULT result;
    return result;
}


const int MINDIFF = 600;
const int MAXDIFF = 630;
float visulAngale(IplImage *img){
	  CvPoint pt[4];
	  pt[2] =cvPoint(0,0);
	  pt[3] =cvPoint(0,0);
	  CvMemStorage* storage = cvCreateMemStorage(0);  	
	  CvSeq* results = cvHoughCircles(img,  storage, CV_HOUGH_GRADIENT, 1, 50,100, 30,14, 18 );
	  if(results->total>=2 && results->total <=4){
		  for( int i = 0; i < results->total; i++ ){
			float *p = ( float* )cvGetSeqElem(results, i);
			pt[i] = cvPoint( cvRound( p[0] ), cvRound( p[1] ) );  
		/*	cvCircle(  
            img,  
            pt[i],  //确定圆心  
            cvRound( p[2] ),  //确定半径  
            CV_RGB( 0xff, 0, 0 )  
			);*/  
		//	cvSaveImage("1.png",img);
		}  
	  }
	//	printf("totle %d ",results->total);
		int re =0;
		if(results->total ==2){
			int d = sqrt(float(pt[0].x -pt[1].x) * (pt[0].x -pt[1].x) + (pt[0].y -pt[1].y) *(pt[0].y -pt[1].y));
			if(d >MINDIFF && d <MAXDIFF){
				re =1;

			}

		}else if(results->total ==3){
			int d = sqrt(float(pt[0].x -pt[1].x) * (pt[0].x -pt[1].x) + (pt[0].y -pt[1].y) *(pt[0].y -pt[1].y));
			int d1 = sqrt(float(pt[0].x -pt[2].x) * (pt[0].x -pt[2].x) + (pt[0].y -pt[2].y) *(pt[0].y -pt[2].y));
			int d2 = sqrt(float(pt[2].x -pt[1].x) * (pt[2].x -pt[1].x) + (pt[2].y -pt[1].y) *(pt[2].y -pt[1].y));
			if(d >MINDIFF && d <MAXDIFF){
				re =1;
			}else if(d1 >MINDIFF && d1 <MAXDIFF){
				re =1;
				pt[1].x = pt[2].x;
				pt[1].y = pt[2].y;
			}else if(d2 >MINDIFF && d2 <MAXDIFF){
				re =1;
				pt[0].x = pt[2].x;
				pt[0].y = pt[2].y;
			
			}
			
		} else if(results->total ==4){
			for(int i=1;i<4;i++){
				int d = sqrt(float(pt[0].x -pt[i].x) * (pt[0].x -pt[i].x) + (pt[0].y -pt[i].y) *(pt[0].y -pt[i].y));
				if(d >MINDIFF && d <MAXDIFF){
					re = 1;
					pt[1].x = pt[i].x;
					pt[1].y = pt[i].y;
					break;
				}
			}
		}
		cvReleaseMemStorage(&storage);
		if(re ==0){
			return -100.0; 
		}else{
			myPoint p1,p2,pc;
			p1.x = pt[0].x;
			p1.y = pt[0].y;
			p2.x = pt[1].x;
			p2.y = pt[1].y;
			pc.x = p1.x +10;
			pc.y = p1.y;
			double angale = getAngale(p1,pc,p2);
			return angale;

		}
}


int zt_calcWork1(Mat &m5,Point &p,float &angle)
{

   
    Mat m1,m3;
    int len,len1,len2;

    Point p1[4];
    std::vector<Point> cen(50);
    int minLen=590;
    int maxLen=635;
    
    medianBlur(m5, m3,9);
  
    int thresh=254;
    bool found=false;
	int ret=-1;
    std::vector<std::vector<Point>> contours;


    for(int i=thresh;i>70;i-=5)
    {
        threshold(m3,m1,i,255,THRESH_BINARY);

       // imshow("Threshold",m1);

		cen.clear();

        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        

       
        for (int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<600||contours[k].size()>1200)
                continue;


            //contourFeature(contours[k]);


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            
            cvBoxPoints(End_Rage2D1,rectpoint);

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);
            if(abs(len1-len2)>90)
                continue;


            if(m3.at<uchar>(contours[k][0].y-1,contours[k][0].x)>i)
                continue;

            cen.push_back(End_Rage2D1.center);
            


        }


        int j1=0;


        if(cen.size()>2)//排除错误的区域
        {
            for(int i2=0;i2<cen.size();i2++)
            {
                for(int n=i2+1;n<cen.size();n++)
                {
                    len=getLineLen(cen[i2],cen[n]);
                   
                    if(len>minLen&&len<maxLen)
                    {
                        p1[j1]=cen[i2];
                        p1[j1+1]=cen[n];
                        j1+=2;
                        found=true;

                     
                    }
                }
            }
            if(j1==4)
            {

                int b1=0,b2=0,b3=0;
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

            len=getLineLen(cen[0],cen[1]);
            if(len>minLen&&len<maxLen)
            {
                p1[0]=cen[0];
                p1[1]=cen[1];
                found=true;
              
            }



        }

        if(found)
        {
            //qDebug()<<"i:"<<i;
			ret=0;
            break;
        }
    }
   angle=calcAngle(p1[0],p1[1]);
     p.x=(p1[0].x+p1[1].x)/2;
     p.y=(p1[0].y+p1[1].y)/2;
	 return ret;

}


//检测接口
DETECTRESULT   zt_visulLocation(IplImage *dectImage,CvRect dectArea, CvRect leftArea, int binThreshold,int offset,int tcenterX,int tcenterY,double angleCoord,double pixelRatio,double offsetX, double offsetY){
	DETECTRESULT result1;
	result1.result = -1;
	cvSetImageROI(dectImage,dectArea);
	
	Mat img(dectImage);
    float angle1=360;
    Point pCenter;
    int ret= zt_calcWork1(img,pCenter,angle1);
	cvResetImageROI(dectImage);
	if(ret==0){
	   result1.result = 1;
	   result1.offsetX = (pCenter.x - tcenterX);
	   result1.offsetY =  (pCenter.y - tcenterY);
	   result1.angle = angle1;
 
	   int len = sqrt(result1.offsetX * result1.offsetX + result1.offsetY *result1.offsetY);
       int x = tcenterX +5;
       int y;
       y  = tcenterY -(-5 * tan(angleCoord*3.14/180));
	   myPoint p3;
	   p3.x = x;p3.y=y;
	   myPoint p4;
	   p4.x = pCenter.x; p4.y = pCenter.y;
	   myPoint p1;
	   p1.x = tcenterX; p1.y = tcenterY;
       int angle1 = getAngale(p3,p1,p4);
	  // printf("%d %d %d %d %d %d\n",p3.x,p3.y,p1.x,p1.y,p4.x,p4.y);
	   result1.offsetY = -sin(angle1*3.14/180)*len *pixelRatio + offsetY;
       result1.offsetX = -cos(angle1*3.14/180)*len * pixelRatio + offsetX;
	   //printf("len=%d angle1=%d %f %f %f %f %f\n",len,angle1,result1.offsetX, result1.offsetY,-cos(angle1*3.14/180)*len * pixelRatio,-cos(angle1*3.14/180), offsetX);
	   if(result1.angle >180)
		   result1.angle=180.0;
       if(result1.angle>=0 && result1.angle <= 90+angleCoord){
           result1.angle -= angleCoord;
       }else if(result1.angle > 90+ angleCoord && result1.angle<=180.0){
           result1.angle = -180+result1.angle-angleCoord;
       }
	  
	   std::cout<<"result:"<<result1.result<<" myAngle:"<<result1.angle<<" myX:    "<<result1.offsetX<<" myY:    "<<result1.offsetY<<std::endl;
	  
	   
	 
	   
	}
	return result1;
	cvResetImageROI(dectImage);

    IplImage *dectImage1 = cvCreateImage(cvSize(cvGetSize(dectImage).width,cvGetSize(dectImage).height),IPL_DEPTH_8U, 1);
    cvCopy(dectImage,dectImage1);
	//float angale = visulAngale(dectImage);
	//printf("angale=%f",angale);   
    int t = 10;
    while(result1.result<=0){
		
        if(t + binThreshold >255){
			
			break;
		}
		cvCopy(dectImage1,dectImage);
        result1 = visulInspection1(dectImage,dectArea, leftArea, binThreshold + t, offset,tcenterX,tcenterY,angleCoord,pixelRatio,offsetX, offsetY, -100.0);
	
        t+=10;
    }
    cvReleaseImage(&dectImage1);
	std::cout<<"result "<<result1.result<<" angle:"<<result1.angle<<" offsetX:"<<result1.offsetX<<" offsetY:"<<result1.offsetY<<std::endl;
	std::cout<<""<<std::endl;
	return result1;


}




//检测圆心接口
myPoint zt_getTemplateCenter(IplImage *dectImage,int binThreshold)
{



	myPoint center;
    //cvSetImageROI(dectImage,dectArea);
   // IplImage *bakImage = cvCreateImage(cvSize(cvGetSize(dectImage).width,cvGetSize(dectImage).height),IPL_DEPTH_8U, 1);
    cvThreshold(dectImage, dectImage, binThreshold, 255, CV_THRESH_BINARY);
   // cvCanny(dectImage,dectImage,200,400,3);
    //cvCopy(dectImage,bakImage);
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
    center.x = ((rectRx-rectLx)/2 +rectLx);
	center.y = ((rectRy-rectLy)/2 +rectLy);
   // cvCircle(dectImage,cvPoint(center.x,center.y),2,cvScalar(0,0,0),10);
    //cvShowImage("11.png",dectImage);
    return center;

}


//模板匹配接口
long int  zt_matchTemplate(IplImage *dectImage1,IplImage *tp,CvRect dectArea, int method)
{
		
  

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
	result = cvCreateImage( cvSize(res_width, res_height ), IPL_DEPTH_32F , 1 );
    cvMatchTemplate(dectImage1,tp,result,CV_TM_SQDIFF );
    cvMinMaxLoc( result, &minval, &maxval, &minloc, &maxloc, 0 );
    //cvRectangle( dectImage1, minloc, cvPoint( minloc.x + tp->width , minloc.y + tp->height ), cvScalar(0,0,0), 2, 8, 0 );
	cvReleaseImage(&result);
#ifdef DEBUG
	cvShowImage("test",dectImage1);
#endif
    cvResetImageROI(dectImage1);

    long int value = long int(minval/10000);
	return value;

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


void sortCircle(xy_size circleList[],int num,int cenX,int cenY)
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







int circleFeature2(std::vector<Point> &contours,int step,int angleVal)
{
    int size=contours.size();
    int num=0;
    double angle1,angle2,angleDif;
    for(int i=step;i+step<size;i+=step)
    {
        angle1=calcAngle(contours[i-step],contours[i]);
        angle2=calcAngle(contours[i+step],contours[i]);
        angleDif=angle1-angle2;
        if(angleDif<-90)
            angleDif=180+angleDif;
        if(angleDif>90)
            angleDif=180-angleDif;
        if(angleDif>angleVal)
            ++num;

    }
    return num;
}


int trangleCalc(std::vector<Point> &contours,int sideLen,int index,Rect &roi)
{
    int size=contours.size();
    int i=index,j=index;
    int i2,j2,i3,j3;
    int k=0,num=0;
    int times=5;
    int step=5;
    if(step>size/4)
        return 0;
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

        if(abs(calcAngle(contours[i],contours[i2])-calcAngle(contours[i2],contours[i3]))<12)
            ++num;
        if(abs(calcAngle(contours[j],contours[j2])-calcAngle(contours[j2],contours[j3]))<12)
            ++num;

        i+=step;
        j-=step;
        k++;
    }
    return num;
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
    
    num+=trangleCalc(contours,sideLen,0,roi);
    num+=trangleCalc(contours,sideLen,down1,roi);
    num+=trangleCalc(contours,sideLen,index,roi);
    return num;
    
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
        len[i-1]= getLineLen(p1,p2);
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

        if(abs(getLineLen(p1,p2)-len1)>dVal)//排除污点
            continue;

         circleList[++index]=t1[i];

    }
    num1=index+1;

}

//dll代码测试
void xl_circle_old(Mat& img1,Rect roi,int &num,int &num2,xy_size_1 xy_arr[], int &otherNum,params &param)
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

    int s_num=0,b_num=0;//排除污点后大圆，小圆孔数量

    //GaussianBlur( img, img, Size(9, 9), 2, 2 );
    //cvtColor(img, cimg, COLOR_GRAY2BGR);
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

     for (int k=0 ; k<contours.size(); k++)
     {
         if(contours[k].size()>c_size3-100&&contours[k].size()<c_size3+150)
         {
             End_Rage2D1 = minAreaRect(Mat(contours[k]));
             cvBoxPoints(End_Rage2D1,rectpoint);
             cenX=End_Rage2D1.center.x;
             cenY=End_Rage2D1.center.y;
          //   //qDebug()<<"cen"<<cenX<<cenY;
         }
     }

     for (int k=0 ; k<contours.size(); k++)
     {
         if(contours[k].size()<c_size1-dVal||contours[k].size()>c_size2+500)
             continue;
         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;


        distance=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));
        //std::cout<<"distance:"<<distance<<"x:"<<x1<<"y:"<<y1<<std::endl;;
        if(distance<min_radius||distance>max_radius)
            continue;

        if(abs(cen_distance1-distance)>limitVal&&contours[k].size()<=c_size2-dVal)
            continue;
        xy_size curObj;
        curObj.calcCen=false;
        curObj.x=x1;
        curObj.y=y1;
        curObj.size=contours[k].size();
        curObj.p1=contours[k];

        double line1= getLineLen(Point(rectpoint[0].x,rectpoint[0].y),Point(rectpoint[1].x,rectpoint[1].y));
        double line2= getLineLen(Point(rectpoint[2].x,rectpoint[2].y),Point(rectpoint[1].x,rectpoint[1].y));

        if(abs(line1/line2-1)>rectDifVal)
        {
            curObj.calcCen=true;
           // //qDebug()<<"line1 2"<<abs(line1/line2-1)<<x1<<y1;
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
             ////qDebug()<<"p1"<<detectedCircleList[k].p1[0].x<<detectedCircleList[k].p1[0].y;
         }
         else
         {
             smallCircleList[smallNum++]=detectedCircleList[k];
         }
     }
	  sortCircle(detectedCircleList,num,cenX,cenY);
     //sortCircle(bigCircleList,bigNum,cenX,cenY);

     //sortCircle(smallCircleList,smallNum,cenX,cenY);
	
    // if(num>22)
    // {


    //     if(bigNum>10)
    //     {

    //         detectOther2(bigCircleList,bigNum,b_num,len1[0],dVal);
    //         otherNum+=bigNum-b_num;


    //     }

    //     if(smallNum>12)
    //     {

    //         detectOther2(smallCircleList,smallNum,s_num,len1[1],dVal);
    //         otherNum+=smallNum-s_num;

    //     }

    //     num=22;

    // }
    ////圆环上有6个小孔
    // if(num>6&&num<22)
    // {

    //     detectOther2(smallCircleList,smallNum,s_num,len1[2],dVal);
    //     otherNum+=smallNum-s_num;
    //     ////qDebug()<<"otherNum"<<otherNum<<smallNum<<s_num;
    //    num=6;
    // }

 /*  if(b_num==0) b_num=bigNum;
     for(int i=0;i<b_num;i++)
     {
         detectedCircleList[i]=bigCircleList[i];

     }
     if(b_num==0) s_num=smallNum;
     for(int i=0;i<s_num;i++)
     {
         detectedCircleList[i+b_num]=smallCircleList[i];
     }*/




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
            // xy_arr[j1].p1=detectedCircleList[k].p1;
			 
             xy_arr[j1].calcCen=detectedCircleList[k].calcCen;

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

					 if(p1.x==p2.x||p1.x==p3.x||p2.x==p3.x)
                          continue;
                     success=getCircleCenter(p1,p2,p3,pCen);
                    if(abs(pCen.x-pPre.x)<4&&abs(pCen.y-pPre.y)<4&&success)
                     {
                         // //qDebug()<<"pCen1111"<<xy_arr[j1].x<<xy_arr[j1].y<<pCen.x<<pCen.y;
                          xy_arr[j1].x=pCen.x;
                          xy_arr[j1].y=pCen.y;
                          break;
                     }
                     pPre=pCen;
                 }
             }
//              xy_arr[j1].moveX=(x1-offsetX)*pixelRatio;
//              xy_arr[j1].moveY=(y1-offsetY)*pixelRatio;

             calibratePoint=xyCalibration(angle2,mark,Point(x1,y1));
             xy_arr[j1].moveX=(calibratePoint.x-mark.x)*pixelRatio+offsetX;
             xy_arr[j1].moveY=(calibratePoint.y-mark.y)*pixelRatio+offsetY;
             j1++;
         }
     }


   num2=j1;
	



}









void xl_circle(Mat& img1,Rect roi,int &num,int &num2,xy_size_1 xy_arr[],int &otherNum,params &param)
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
	
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
		 //std::cout<<"contours[k].size() "<<contours[k].size()<<" min "<<minSize<<" max "<<maxSize<<std::endl;
         if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;
         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         cvBoxPoints(End_Rage2D1,rectpoint);

         x1=End_Rage2D1.center.x;
         y1=End_Rage2D1.center.y;



        distance=getLineLen(Point(x1,y1),Point(cenX,cenY));

        if(distance<minDistance||distance>maxDistance)
            continue;


        xy_size curObj;
        curObj.calcCen=false;
        curObj.x=x1;
        curObj.y=y1;
        curObj.size=contours[k].size();
        curObj.p1=contours[k];


        len1=getLineLen(rectpoint[0],rectpoint[1]);
        len2=getLineLen(rectpoint[1],rectpoint[2]);

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




                     success=getCircleCenter(p1,p2,p3,pCen);

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


             calibratePoint=xyCalibration(angle2,robotCen,Point(x1,y1));


             xy_arr[j1].moveX=(calibratePoint.x-robotCen.x)*pixelRatio+offsetX;
             xy_arr[j1].moveY=(calibratePoint.y-robotCen.y)*pixelRatio+offsetY;

             j1++;
         }
     }


   num2=j1;


}

int g_nElementShape = MORPH_RECT;

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

    int val=0,val2=0,grayVal;

    int t1=m.at<uchar>(p.y,p.x);
    int t2=m.at<uchar>(p.y-len,p.x);
    int t3=m.at<uchar>(p.y+len,p.x);
    int t4=m.at<uchar>(p2.y,p2.x);
    int t5=m.at<uchar>(p2.y-len,p2.x);
    int t6=m.at<uchar>(p2.y+len,p2.x);
    val=t2+t3-2*t1;
    val2=t5+t6-2*t4;

    return val>val2?p:p2;
}


void zt_calcWork2(Mat &m3,Rect roi,params& param,double &angle)
{
    int cenX=param.var1[0]-roi.x;
    int cenY=param.var1[1]-roi.y;
    

    int distance=param.var1[2];
    int minSize=param.var1[3];
    int maxSize=param.var1[4];
    int thresh=param.var1[5];
    int square=param.var1[6];
    int rectLen=param.var1[7];
    int rectLen2=param.var1[8];//大半圆矩形长度
    double angleOffset=param.var2[0];
    Mat m1,m2,m5,m6;
    Mat m4=m3(roi).clone();
    bool foundBigArea=false;
    bool foundSmallArea=false;

    bool calcObjFeature;

    int x1,y1,distance1,x2,y2,x3,y3,len1,len2,index1,index2;

    int step=15,step2=8;
    double d;
    int sizeArr[2]={0};
    Point p1[2];

    medianBlur(m4, m5, 9);

    int i;
    for(i=thresh;i>30;i-=5)
    {
       

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

            if(abs(distance-distance1)>40)
                continue;

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<len2)
                len1=len2;
            if(len1<rectLen)
                continue;



            //qDebug()<<"Len"<<len1<<"rectlen"<<rectLen;


            d=abs(distance1*1.0/distance-1);



            if(len1>rectLen2)
            {
                
                   if(m6.at<uchar>(contours[k][0].y-1,contours[k][0].x))
                   {
                       continue;
                   }
              

                calcObjFeature=false;
                for(int j2=0;j2<contours[k].size();j2++)
                {
                    if(abs(contours[k][j2].y-y1)<30)
                        continue;
                    if(calcObjFeature)
                        break;
                    //qDebug()<<"contours point x y"<<contours[k][j2].x+roi.x<<contours[k][j2].y+roi.y<<"front";
                    index2=j2+step2;
                    if(index2>=contours[k].size())
                        index2-=contours[k].size();
                    if(index2>=contours[k].size())
                        break;
                    
                    if(contours[k][index2].y-y1>55)
                    {
                        if(contours[k][index2].x-contours[k][j2].x<-5)
                        {
                            calcObjFeature=true;
                            break;
                        }
                    }
                    if(contours[k][j2].y-y1<-45)
                    {
                        if(contours[k][index2].x-contours[k][j2].x>5)
                        {
                            calcObjFeature=true;
                            break;
                        }
                    }




                    for(int j3=0;j3<40;j3++)
                    {

                        index1=j3+j2+step;
                        if(index1>=contours[k].size())
                           index1-=contours[k].size();

                        if(index1>=contours[k].size())
                            break;


                        if(abs(contours[k][j2].x-contours[k][index1].x)<5&&abs(contours[k][j2].y-contours[k][index1].y)<5)
                        {
                            calcObjFeature=true;
							break;
                        }
                    }
                }
                if(calcObjFeature)
                    continue;
                p1[1].x=x1;
                p1[1].y=y1;

                sizeArr[1]=contours[k].size();
                foundBigArea=true;
                break;

            }



        }
        if(foundBigArea)
            break;
    }
     if(!foundBigArea)
     {
         angle=360;
         return;
     }


     angle=calcAngle2(Point(cenX,cenY),p1[1]);
     //角度偏移
     if(angleOffset+angle>180)
         angle=angleOffset+angle-360;
     else if(angleOffset+angle<-180)
         angle=angleOffset+angle+360;
     else
         angle+=angleOffset;
 


}




bool zt_2ndFeature(vector<Point> &contours,Rect &roi,Point cen,int step,int step2,int len1,int objAngle,int offset,int curToCen)
{
    int index1,index2;
    int x1=cen.x;
    int y1=cen.y;

    for(int j2=0;j2<contours.size()-step2;j2++)
    {
       
        index2=j2+step2;
        if(index2>=contours.size())
            index2%=contours.size();

        if(objAngle==90)
        {
            if(contours[j2].y-y1>curToCen)
            {
                if(contours[index2].x-contours[j2].x<-offset)
                {

                    return false;
                }
            }
            if(contours[j2].y-y1<-curToCen)
            {
                if(contours[index2].x-contours[j2].x>offset)
                {

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

                    

                    return false;
                }
            }

            if(contours[j2].x-x1<-curToCen)
            {

                if(contours[index2].y-contours[j2].y<-offset)
                {

                    
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
                    
                        return false;
                }
            }

        }

    }

    return true;

}

void zt_2ndLine(Mat &m3, Rect roi, params &param)
{

    int cenX=param.var1[0]-roi.x;//中心
    int cenY=param.var1[1]-roi.y;
    int minDistance=param.var1[2];//中心与字距离
    int maxDistance=param.var1[3];//中心与字距离
    int minSize=param.var1[4];//obj size
    int maxSize=param.var1[5];
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<len2)//取最大长度
                len1=len2;

            if(len1<rectLen||len1>rectLen2)
                continue;


            if(m6.at<uchar>(contours[k][0].y-1,contours[k][0].x))
            {
                continue;
            }
			
			
             if(!zt_2ndFeature(contours[k],roi,Point(x1,y1),step,step2,rectLen,90,5,30)&&calcShapeFlag!=82000)
                continue;


           
            double angle=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            //角度偏移
            if(angleOffset+angle>180)
                angle=angleOffset+angle-360;
            else if(angleOffset+angle<-180)
                angle=angleOffset+angle+360;
            else
                angle+=angleOffset;
            //end

            param.var2[19]=angle;

          
            loop=false;
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

v1=src.at<uchar>(contours[i].y,contours[i].x);
v2=src.at<uchar>(contours[i].y+2,contours[i].x);
v3=src.at<uchar>(contours[i].y-2,contours[i].x);
v4=v1>v2?v1:v2;
v5=v4>v3?v4:v3;
m_gray+=v5;

    }

   
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

    }

  
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





//华东
int getIndex(int allNum,int j,int step)
{
    int index1=j+step;
    if(index1>=allNum)
    {
        index1-=allNum;
    }
    if(index1<0)
    {
        index1+=allNum;
    }
    return index1;
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
    
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
        if(contours[k].size()<minSize||contours[k].size()>maxSize)
             continue;

         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);


         if(len1>len2)
         {
             len1^=len2;
             len2^=len1;
             len1^=len2;
         }
         if(len1<minLen||len2>maxLen)
             continue;


         if(abs(setCen.x-End_Rage2D1.center.x)<offset1&&abs(setCen.y-End_Rage2D1.center.y)<offset1)
         {
             p=End_Rage2D1.center;
            
             loop=false;
             break;
         }

     }


    }

     return p;
}


int minDistanceNum(std::vector<Point> &contours,int step,int len)
{
    int num=0;

    for(int i=step;i<contours.size();i+=step)
    {
        if(getLineLen1(contours[i-step],contours[i])<len)
            ++num;

    }
    return num;
}

bool delegateAngle(int angle1,int angle2,int angleDif2)
{
    return angleDif(angle1,angle2,90,1)>angleDif2;
}
typedef bool (*compare1)(int angle1,int angle2,int angleDif);
compare1 callback1=delegateAngle;
int compareAngleNum(std::vector<Point> &contours,int step,int angle,int angleDif)
{
    int num=0,temp;



    for(int j=step;j<contours.size();j+=step)
    {
        temp= calcAngle1(contours[j-step],contours[j]);

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

    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
 
    if( offset < 0 )
        morphologyEx(m1, m1, MORPH_TOPHAT , element);
    else
        morphologyEx(m1, m1, MORPH_BLACKHAT, element);



    for( int i=thresh;i>thresh2;i--)
    {
        if(i>60) i-=9;
        if(i>40&&i<60) i-=5;
        allnum1=0;
    
        if(!loop)
        {
            break;
        }
        threshold(m1,m3,i,255,THRESH_BINARY);

        m4=m3.clone();
  
        std::vector<std::vector<Point>> contours;

        findContours(m3,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        

        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        double area;
        
        param.var1[28]=0;
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
            if(contours[k].size()<size3)
                continue;

            step=(contours[k].size()-5)/4;
       


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            area=contourArea(contours[k]);
            cvBoxPoints(End_Rage2D1,rectpoint);

            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;

            len3=getLineLen(cen1,Point(x1,y1));

            if(len3<cirLen1-50||len3>cirLen2)
                continue;



                allnum1++;

                if(allnum1>allnum)
                {
                    
                    return;

                }

            if(contours[k].size()<size1||contours[k].size()>size2)
            {

                continue;
            }

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);


            if(len1>len2)
                angle10=calcAngle(rectpoint[0],rectpoint[1]);
            else
                angle10=calcAngle(rectpoint[1],rectpoint[2]);
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }
            if(len1<18)
            {
              
                continue;
            }


            nearLen=1000;
            for(int j1=0;j1<contours[k].size();j1++)
            {
                temp1=getLineLen(Point(x1,y1),contours[k][j1]);
                if(temp1<nearLen)
                {
                    nearLen=temp1;
                    j=j1;

                }
            }
           

            j=getIndex(contours[k].size(),j,contours[k].size()/2);

            index1=getIndex(contours[k].size(),j,step);
            index2=getIndex(contours[k].size(),j,-step);

            p1.x=contours[k][j].x;
            p1.y=contours[k][j].y;
            p2.x=contours[k][index1].x;
            p2.y=contours[k][index1].y;
            p3.x=contours[k][index2].x;
            p3.y=contours[k][index2].y;




           
            bool success=getCircleCenter(p1,p2,p3,cen);

            if(!success)
            {
                
                continue;
            }


          
            p1toCen=getLineLen(cen1,p1);
            p2toCen=getLineLen(cen1,p2);
            p3toCen=getLineLen(cen1,p3);



            if(p1toCen<cirLen1||p1toCen>cirLen2)
            {
                
                continue;
            }


            //小圆半径
            minRLen=getLineLen(p1,cen);
            len=getLineLen(cen1,cen);
     
            if(len<rLen1||len>rLen2)
            {
                
                continue;
            }


            if(minRLen<minR1||minRLen>minR2)
            {
               
                continue;
            }

           

            if(p2toCen>cirLen2-5&&p3toCen>cirLen2-5||p1toCen>cirLen2-5&&p3toCen>cirLen2-5||p1toCen>cirLen2-5&&p2toCen>cirLen2-5)
            {
          
                continue;
            }



           
            cirFeaVal=circleFeature(contours[k],cen,step,5,11);
            if(cirFeaVal<cirFeatureNum)
            {
                
                continue;
            }

            int minDisNum=minDistanceNum(contours[k],20,17);

            s_level=Smoothlevel(contours[k],15,30,0);
            int angleNum=compareAngleNum(contours[k],10,angle10,45);


            if(len1<40)
            {
                if(angleNum>10&&minDisNum>10)
                {
                
                    continue;
                }

                if(s_level>4&&minDisNum>4)
                {
                    
                    continue;
                }
            }

            if(angleNum>20)
            {
                
                continue;
            }

  

            if(loop)
            {
                foundCen=cen;
                foundCirContour=p1;
                param.var1[27]=cirFeaVal;
                param.var1[28]=s_level;

                loop=false;
               
            }



        }
        if(!loop)
        {
       
             if(param.var1[28]>5&&param.var1[27]<2)
             {
                 
                 return ;
             }
             else if(param.var1[28]>40&&param.var1[27]<2)
             {
                
                 return;
             }
             else if(allnum1>90&&param.var1[27]<4)
             {
                 
                 return;
             }
             else if(allnum1>60&&param.var1[27]<3)
             {
                
                 return;
             }
             else if(allnum1>40&&param.var1[27]<2)
             {
                 
                 return;
             }


            param.var1[29]=1;


            

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
         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);

         if(abs(len1-len2)>20)
             continue;


         if(len1>minRectLen&&len1<maxRectLen&&len2>minRectLen&&len2<maxRectLen)
         {
             param.var1[28]=x1;
             param.var1[29]=y1;
            

             loop=false;
             
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



    for(i=thresh;i<60;i++)
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
         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);

         len3=len1>len2?len1:len2;

         len4=getLineLen(Point(cenX,cenY),Point(x1,y1));
         if(len4<minLen||len4>maxLen)
             continue;

		 if(len1>len2)
         {
             angle2=calcAngle(rectpoint[1],rectpoint[2]);
         }
         else
         {
             angle2=calcAngle(rectpoint[0],rectpoint[1]);
         }
         if(abs(angle2-calcAngle(Point(cenX,cenY),Point(x1,y1)))>10)
             continue;
         if(len3>minRectLen&&len3<maxRectLen)
         {
           


             angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));

             loop=false;
             param.var2[19]=angle1;
             
             break;
         }



     }

     // draw black contours on white image


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
         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);

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

         double angleA,angleB;


             if(len1>len2)
             {
                 angleA=calcAngle2(Point((rectpoint[0].x+rectpoint[1].x)/2,(rectpoint[0].y+rectpoint[1].y)/2),rectpoint[0]);
                 angleB=calcAngle2(Point((rectpoint[0].x+rectpoint[1].x)/2,(rectpoint[0].y+rectpoint[1].y)/2),rectpoint[1]);
                 param.var1[24]=rectpoint[0].x;
                 param.var1[25]=rectpoint[0].y;
                 param.var1[26]=rectpoint[1].x;
                 param.var1[27]=rectpoint[1].y;

             }
             else
             {
                 angleA=calcAngle2(Point((rectpoint[1].x+rectpoint[2].x)/2,(rectpoint[1].y+rectpoint[2].y)/2),rectpoint[1]);
                 angleB=calcAngle2(Point((rectpoint[1].x+rectpoint[2].x)/2,(rectpoint[1].y+rectpoint[2].y)/2),rectpoint[2]);

                 param.var1[24]=rectpoint[0].x;
                 param.var1[25]=rectpoint[0].y;
                 param.var1[26]=rectpoint[1].x;
                 param.var1[27]=rectpoint[1].y;
             }

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


             
            
             break;
         }





     // draw black contours on white image


    }


}

void hd_maxDiameter(Mat &src,Rect roi,params &param)
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
             len1=getLineLen(rectpoint[0],rectpoint[1]);
             len2=getLineLen(rectpoint[1],rectpoint[2]);

             if(abs(len1-len2)>20)
                 continue;

             if(contourType(m2,contours[k],thresh,true))
                 continue;

             Point2f center;
             float radius;
             minEnclosingCircle(contours[k],center,radius);

             param.var2[4]=radius*2*ratio;


         }


}


void hd_maxDiameter2(Mat &src,Rect roi,params &param)
{
    
        Mat m1,m8;
        Mat m2=src(roi);

        int x1,y1,len1,len2;

        double ratio=param.var2[2];//比例
        int thresh=param.var2[3];

		int minSize=param.var1[20];//size
        int maxSize=param.var1[21];

		bool isBlack=param.var1[22];//isBlack
		int lenDif=param.var1[23];//len偏差

        Point cen1(param.var1[24]-roi.x,param.var1[25]-roi.y);//中心点

		//std::cout<<"aaa"<<param.var2[2]<<param.var2[3]<<param.var1[20]<<param.var1[21]<<param.var1[22]<<param.var1[23]<<std::endl;
         param.var2[4]=0;//输出外接矩形较小直径
         param.var2[5]=0;//输出外接矩形较大直径
         param.var2[6]=0;//输出外接圆直径

         threshold(m2,m1,thresh,255,THRESH_BINARY);
		 //threshold(m2,m8,thresh,255,THRESH_BINARY);

		 
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
             len1=getLineLen(rectpoint[0],rectpoint[1]);
             len2=getLineLen(rectpoint[1],rectpoint[2]);

			 if(len1>len2)
             {
                 len1^=len2;
                 len2^=len1;
                 len1^=len2;
             }
             if(abs(len1-len2)>lenDif)
                 continue;

             if(contourType(m2,contours[k],thresh,isBlack))
                 continue;

			 if(getLineLen(cen1,Point(x1,y1))>40)
				 continue;
         
             Point2f center;
             float radius;
             minEnclosingCircle(contours[k],center,radius);

             param.var2[4]=len1*ratio;
			 param.var2[5]=len2*ratio;
             param.var2[6]=radius*2*ratio;
	
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

         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);

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

   
         loop=false;
         break;



     }




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

         len3=getLineLen(End_Rage2D1.center,Point(param.var1[24],param.var1[25]));
         if(len3<distance||len3>distance2)
             continue;

		 if(contourType(m2,contours[k],i,false))
            continue;
         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);


         if(len1>len2)
         {
             len1^=len2;
             len2^=len1;
             len1^=len2;
         }

         len4=getLineLen(End_Rage2D1.center,Point(param.var1[20],param.var1[21]));
         len5=getLineLen(End_Rage2D1.center,Point(param.var1[22],param.var1[23]));
         if(len4<disDif)
         {
             
             double angle=calcAngle2(Point(param.var1[24],param.var1[25]),Point(param.var1[20],param.var1[21]));
             param.var2[19]=anlgeOffset(angle,angleDif);
             loop=false;
             break;
         }

         if(len5<disDif)
         {
             
             double angle=calcAngle2(Point(param.var1[24],param.var1[25]),Point(param.var1[22],param.var1[23]));
             param.var2[19]=anlgeOffset(angle,angleDif);
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
    
    medianBlur(src(roi), m1,9);


    int minSize=param.var1[0];//size范围
	int thresh=param.var1[1];//二值
 
    threshold(m1,m1,thresh,255,THRESH_BINARY);

    
     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

     // Print contours' length
    
     std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     
     allNum=0;
     for (unsigned int k=0 ; k<contours.size(); k++)
     {
        if(contours[k].size()<minSize)
             continue;

        ++allNum;
        if(allNum>1)
        {
            param.var1[29]=1;
			//std::cout<<"aaaaa"<<End_Rage2D1.center.x<<"y:"<<End_Rage2D1.center.y<<"size:"<<contours[k].size()<<std::endl;
            break;
        }


        //contourFeature(contours[k]);


         End_Rage2D1 = minAreaRect(Mat(contours[k]));
         area=contourArea(contours[k]);
         cvBoxPoints(End_Rage2D1,rectpoint);

         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);

       }


    
}


void workIsEmpty2(Mat &src,Rect roi,params &param)
{
    param.var1[29]=1;
    Mat m1;
    int len1,len2,allNum;
    Mat m2=src.clone();

    int thresh=150;//二值
    int offsetXY=100;//xy偏移
    int minSize=550;//size
    int maxSize=2000;//size
    //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
    threshold(m2,m1,thresh,255,THRESH_BINARY);

     std::vector<std::vector<Point>> contours;

     findContours(m1,
                  contours, // a vector of contours
                  CV_RETR_LIST, // retrieve the external contours
                  CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


     
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
             
             break;
         }





     }

   

   

}


void zt_5thLine(Mat &src, Rect roi, params &param)
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

    int num1=0;
    for(int i=30;i<thresh;i+=5)
    {
       
        int a1=0,a2=0;
        threshold(m5,m1,i,255,THRESH_BINARY);
       
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

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
            
            if(num1>7)
            {


            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;
			std::cout<<"i:"<<i<<std::endl;
           
            return;

            }
        }


    }
}
void zt_ZDK(Mat &src, Rect roi, params &param)
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len2<triangleLen)
                continue;

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            //字体在中心点相连角度
            if(abs(angle1)<30)
            {
                ++a1;
				if(len2>triangleLen+10)
                    ++a1;
            }
            if(abs(angle1)>150&&abs(angle1)<=180)
            {
                ++a2;
				if(len2>triangleLen+10)
                    ++a2;
            }
            if(a1>1||a2>1)
            {
                param.var1[28]=x1;
                param.var1[29]=y1;
                param.var2[19]=angle1;
                return;
            }
        }

      
    }
}

void zt_ZT01(Mat &src, Rect roi, params &param)
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
    {
        if(i>200)
            i-=20;
        if(i>160&&i<180)
            i-=10;
        int a1=0,a2=0,a3=0,lenA=0,lenB=0,preY1=0,preY2=0;
        threshold(m5,m1,i,255,THRESH_BINARY);
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

			if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }
            if(len1<11&&len2<11)
                continue;

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            //字体在中心点相连角度
            if(abs(angle1)<30)
            {

                if(len2>60)
                    ++a3;

                if(abs(x1+y1-lenA)>12)
                {
                    
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
               
                lenA=x1+y1;
                preY1=y1;
//           
            }
            if(abs(angle1)>150&&abs(angle1)<=180)
            {
                if(len2>60)
                    ++a3;

                if(abs(x1+y1-lenB)>12)
                {
                   
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

            }
            
            if(a1>1||a2>1||a3>0)
            {
                param.var1[28]=x1;
                param.var1[29]=y1;
                param.var2[19]=angle1;
                return;
            }
        }

      
    }
}




void zt_ZT01_2(Mat &src, Rect roi, params &param)
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
    {
        if(i>200)
            i-=20;
        if(i>160&&i<180)
            i-=10;

        threshold(m5,m1,i,255,THRESH_BINARY);
       
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
                objAngle=calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                objAngle=calcAngle(rectpoint[2],rectpoint[1]);
            }

            if(abs(objAngle)>12)
                continue;
            if(len2<18)
                continue;

            if(len2*1.0/len1<2.1)
                continue;

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));



            if(abs(angle1)>30&&abs(angle1)<150)
                continue;


            param.var1[28]=x1;
            param.var1[29]=y1;
            angle1=anlgeOffset(angle1,angleOffset);
            param.var2[19]=angle1;
        
            return;

        }


    }
}

void zt_zt01Match(Mat &src, Rect roi, params &param)
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
    int sideLen=param.var1[9];//三角形最小边长

    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;
    medianBlur(m5, m5, 3);


    int num1=0;
    int errorFlag;
    for(int i=30;i<thresh;i+=10)
    {
        if(!loop)
            break;
        int a1=0,a2=0;

        threshold(m5,m1,i,255,THRESH_BINARY);

        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];

        for (unsigned int k=0 ; k<contours.size(); k++)
        {



            errorFlag=-1;


            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
            {

                errorFlag=0;
                continue;
            }
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(distance1<minLen||distance1>maxLen)
            {

                errorFlag=1;
                continue;
            }

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<rLen1||len2>rLen2)
            {
                errorFlag=2;
                continue;
            }


            if(contourType(m5,contours[k],i,true))
                continue;


            int matchVal=matchShapes(contours[k],contours[k],1,31);
            if(matchVal<80)
                continue;

            
            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;



            loop=false;
            break;




        }



    }

}


int calcLineGray(Mat &src,Point &pos,bool horizontal,int times)
{
    int ret=0;
    if(horizontal)
    {
        if(pos.x+times>=src.cols)
            return -1;

        for(int i=0;i<times;i++)
        {
            ret+=src.at<uchar>(pos.y,i+pos.x);
           
        }
    }
    else
    {
        if(pos.y+times>=src.rows)
            return -1;
        
        for(int i=0;i<times;i++)
        {
            ret+=src.at<uchar>(i+pos.y,pos.x);
           
        }
    }

    return ret/times;
}

void wx_direction0(Mat &src, params &param)
{
    Mat m1=src;
    
    Point pos1(param.var1[0],param.var1[1]);
    Point pos2(param.var1[2],param.var1[3]);
    int times=param.var1[4];
    int grayVal1=param.var1[5];
    int result1= calcLineGray(m1,pos1,true,times);
    int result2= calcLineGray(m1,pos2,true,times);

   
    if(result1<grayVal1)
        param.var1[29]=1;//有料方向下
    else if(result2<grayVal1)
        param.var1[29]=2;//无料方向右
    else
        param.var1[29]=360;//无料方向下

}

void wx_direction1(Mat &src, Rect roi, params &param)
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


            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);
            if(len1<minLen||len2>maxLen)
                continue;

            if(abs(x1-pos1.x)<20&&abs(y1-pos1.y)<20)
            {
                param.var1[29]=1;
               
                return;
            }

            if(abs(x1-pos2.x)<20&&abs(y1-pos2.y)<20)
            {
                param.var1[29]=2;
               
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
            len=getLineLen(a[i],b[j]);
            if(len<num1)
                num1=len;
            if(len<minSideDis)
                ++num;
        }
    }
    getMinVal=num1;
    return num;
}

void wx_defect(Mat &src, Rect roi, params &param)
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


    medianBlur(src(roi), m3, 3);

    for(int i=thresh;i>100;i-=5)
    {
        //out
        param.var1[27]=-1;//是否合格
        param.var1[28]=0;//找到小圆
        cen1.x=cen1.y=cen2.x=cen2.y=0;

        threshold(m3,m1,i,255,THRESH_BINARY);
        threshold(m3,m2,i,255,THRESH_BINARY);
       
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


            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<minLen1||len2>maxLen1)
                continue;



            int num= circleFeature(contours[k],End_Rage2D1.center,30,30,30);
            
            if(num<3)
                continue;



            if(len1>maxLen2&&m2.at<uchar>(contours[k][0].y-1,contours[k][0].x)==0)//外圆
            {


                index1=k;
               
                cen1=End_Rage2D1.center;


            }

            if(m2.at<uchar>(contours[k][0].y-1,contours[k][0].x)==255)//内圆
            {

                index2=k;
                Point2f center;
                float radius;
                minEnclosingCircle(contours[k],center,radius);


                cen2=End_Rage2D1.center;
                diameter1=len2;
                diameter2=radius*2;

                
            }


            if(cen1.x&&cen2.x)
            {
                bool NG=false;

                // param.var1[27]返回值 合格0  直径4  贴边5  裂缝6 未找到铆头-1
                param.var1[28]=1;//找到两个圆
                param.var2[19]=diameter1*ratio;//直径
                param.var2[18]=diameter2*ratio;
                param.var2[17]=getLineLen(cen2,cen1)*ratio;//圆心距
   
                if(getLineLen(cen2,cen1)>dif)
                {
                  
                }

                if( param.var2[19]<minDiameter||param.var2[18]>maxDiameter)
                {
                    //直径不合格
                  
                    NG=true;
                    param.var1[27]=4;
                }

                int calcMinSide1;
                circleSideNum=calcCircleSide(contours[index1],contours[index2],minSideDis/ratio,calcMinSide1);
                param.var2[16]=calcMinSide1*ratio;
				if(circleSideNum>2)
                {
                   
                    NG=true;
                    param.var1[27]=5;
                }
				//如果超出最小距离且数量不多界面显示时为合格
                if(circleSideNum<=2&&param.var2[16]<minSideDis)
                {
                     param.var2[16]=minSideDis+0.02;
                }
                int num1=circleFeature2(contours[k],10,14);

               
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

bool getTrangleP2(Mat &src,std::vector<Point> &contours,Point cen,int r,int sideLen,int pIndex[])
{
    pIndex[0]=pIndex[1]=0;
    bool flag=false;
    if(cen.x+r>=src.cols||cen.y+r>=src.rows)
        return flag;
    int indexTmp;
    int index[3];
    int times=0,j=0;

    for(int i=0;i<contours.size();i++)
    {
        if(abs(getLineLen(contours[i],cen)-r)<2)
        {

            if(times>0)
            {
              
               if(getLineLen(contours[i],contours[indexTmp])>sideLen)
               {

                   index[0]=indexTmp;
                   index[1]=i;

                   ++j;
                   break;

               }
            }

            ++times;
            indexTmp=i;
        }
    }
    if(j==1)
    {
        pIndex[0]=index[0];
        pIndex[1]=index[1];
        flag=true;

    }

    return flag;
}

int trangleFeature2(std::vector<Point> &contours,int sideLen,int pIndex[],Rect &roi)
{
    int num=0;
    num+=trangleCalc(contours,sideLen,pIndex[0],roi);
    num+=trangleCalc(contours,sideLen,pIndex[1],roi);
    
    return num;

}

Point line6thArr[100];
int line6Feature[100];
bool findTrangle(int num)
{
    bool flag=false;
    for(int i=0;i<num;i++)
    {
        for(int j=i+1;j<num;j++)
        {
           if(abs(line6thArr[i].x-line6thArr[j].x)<9&&abs(line6thArr[i].y-line6thArr[j].y)<9)
            {
                if(line6Feature[i]>3||line6Feature[j]>3)
                flag=true;
            }
        }
    }

    return flag;
}
void zt_6thLine(Mat &src, Rect roi, params &param)
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
    int sideLen=param.var1[9];//三角形最小边长
    double angleOffset=param.var2[0];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;
    medianBlur(m5, m5, 3);

    int num1=0;
    for(int i=30;i<thresh;i+=5)
    {
        if(!loop)
            break;
        int a1=0,a2=0;
        threshold(m5,m1,i,255,THRESH_BINARY);
        
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;



            Point2f center;
            float radius;
            minEnclosingCircle(contours[k],center,radius);

            int pIndex[2];
            getTrangleP2(src,contours[k],center,radius,sideLen,pIndex);
            int num= trangleFeature2(contours[k],sideLen,pIndex,roi);

            line6thArr[a1]=End_Rage2D1.center;
            line6Feature[a1]=num;
            ++a1;
           
            if(a1>1&&a1<100)
            {
                if(findTrangle(a1))
                {
                   
                  
					angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
                    if(angleOffset+angle1>180)
                        angle1=angleOffset+angle1-360;
                    else if(angleOffset+angle1<-180)
                        angle1=angleOffset+angle1+360;
                    else
                        angle1+=angleOffset;

                    param.var2[19]=angle1;
                    param.var1[28]=x1;
                    param.var1[29]=y1;
                  

                    loop=false;
                    break;
                }
            }

        }

    }
}

bool Comp(const Point &a,const Point &b)
{
    return a.y<b.y;
}

bool zt_gn5(Point p[],int maxAngleDif,int num1)
{
    std::vector<Point> arr;
    for(int i=0;i<num1;i++)
    {
        arr.push_back(p[i]);
    }

    std::sort(arr.begin(),arr.end(),Comp);


   // qDebug()<<"angleDif(d1,d2,90)"<<angleDif(d1,d2,90);
    if(abs(calcAngle(arr[0],arr[num1-1]))<70)
        return false;

    return true;
}

void zt_GLine(Mat &src, Rect roi, params &param)
{
   int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int minRectLen=param.var1[8];//最小边长
    int maxRectLen=param.var1[9];//最小边长
    int maxFontDistance=param.var1[10];//字体最大间距
    double angleOffset=param.var2[0];//角度偏移
    double ratioWH=param.var2[1];//长宽比例
    double angle1,angleTmp;

    int x1,y1,len1,len2,distance1;

    Point p1[3],p2[3],p3;
    Mat m5=src.clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;


    int num1,num2;
    for(int i=thresh;i<thresh2;i+=2)
    //for(int i=90;i<91;i+=2)
    {
        num1=0;
        num2=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY_INV);

       
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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<minRectLen||len2>maxRectLen)
                continue;




            if(m5.at<uchar>(contours[k][0].y-1,contours[k][0].x)<i)
                continue;


            if(len2*1.0/len1>ratioWH)
                continue;

            angleTmp=calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
            if(abs(angleTmp)<45)
            {
                p1[num1++]=End_Rage2D1.center;
               
            }

            if(abs(angleTmp)>135&&abs(angleTmp)<=180)
            {
                p2[num2++]=End_Rage2D1.center;
               
            }
          
            if(num1>2||num2>2)
            {
                int fontWidth1,fontWidth2,fontNum=0;

                Point upPos(1000,1000),downPos(0,0);
                for(int i=0;i<num1;i++)
                {
                    if(p1[i].y<upPos.y)
                        upPos=p1[i];
                    if(p1[i].y>downPos.y)
                        downPos=p1[i];
                }
                fontWidth1=getLineLen(upPos,downPos);
                if(fontWidth1<maxFontDistance&&num1>2)
                {
                    p3.x=(p1[0].x+p1[1].x+p1[2].x)/3;
                    p3.y=(p1[0].y+p1[1].y+p1[2].y)/3;

                    ++fontNum;
                }
                upPos.y=1000;
                downPos.y=0;
                for(int i=0;i<num2;i++)
                {
                    if(p2[i].y<upPos.y)
                        upPos=p2[i];
                    if(p2[i].y>downPos.y)
                        downPos=p2[i];
                }

                fontWidth2=getLineLen(upPos,downPos);
                if(fontWidth2<maxFontDistance&&num2>2)
                {
                    p3.x=(p2[0].x+p2[1].x+p2[2].x)/3;
                    p3.y=(p2[0].y+p2[1].y+p2[2].y)/3;
                    ++fontNum;
                }

                if(fontNum>1)
                    continue;


                angle1=calcAngle2(Point(cenX,cenY),p3);
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                param.var2[19]=angle1;
                param.var1[28]=p3.x;
                param.var1[29]=p3.y;
               
                loop=false;
                break;
            }

        }

    }
}




void zt_GLineGN5(Mat &src,Mat &src2, Rect roi, params &param)
{
	
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize=param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int minRectLen=param.var1[8];//最小边长
    int maxRectLen=param.var1[9];//最小边长
    int maxFontDistance=param.var1[10];//字体最大间距
    int minFontDistance=91;
    int allNum=37;
    double angleOffset=param.var2[0];//角度偏移
    double ratioWH=param.var2[1];//长宽比例
    double angle1,angleTmp;

    int x1,y1,len1,len2,distance1;

    int maxAngleDif=25;
    Point p1[10],p2[10],p3;
    Mat m5=src.clone();
    Mat m1,m6;


    param.var2[19]=360;
    bool loop=true;


    int num1,num2,num3;

    for(int i=100;i>35;i--)
    {
		
        num1=0;
        num2=0;
        num3=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY_INV);

        
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {
			
			
            if(contours[k].size()>50&&contours[k].size()<objMaxSize)
            {
                ++num3;
            }

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;
			
	
            if(num3>allNum)
                return;
		

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

			
            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<minRectLen||len2>maxRectLen)
                continue;

            if(src2.at<uchar>(contours[k][0].y-1,contours[k][0].x)<i)
                continue;


            if(len2*1.0/len1>ratioWH)
                continue;

            angleTmp=calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
            if(abs(angleTmp)<45)
            {
                p1[num1++]=End_Rage2D1.center;
				if(num1>9)
                    num1=9;
                
            }

            if(abs(angleTmp)>135&&abs(angleTmp)<=180)
            {
                p2[num2++]=End_Rage2D1.center;
				if(num2>9)
                    num2=9;
                
            }
			
            if(num1>2||num2>2)
            {
                int fontWidth1,fontWidth2,fontNum=0;

                Point upPos(1000,1000),downPos(0,0);
                for(int i1=0;i1<num1;i1++)
                {
                    if(p1[i1].y<upPos.y)
                        upPos=p1[i1];
                    if(p1[i1].y>downPos.y)
                        downPos=p1[i1];
                }
                fontWidth1=getLineLen(upPos,downPos);
                if(fontWidth1>minFontDistance&&fontWidth1<maxFontDistance&&num1>2)
                {
                    p3.x=(p1[0].x+p1[1].x+p1[2].x)/3;
                    p3.y=(p1[0].y+p1[1].y+p1[2].y)/3;
                   
                    if(zt_gn5(p1,maxAngleDif,num1))
                        ++fontNum;
				}
                upPos.y=1000;
                downPos.y=0;
                for(int i1=0;i1<num2;i1++)
                {
                    if(p2[i1].y<upPos.y)
                        upPos=p2[i1];
                    if(p2[i1].y>downPos.y)
                        downPos=p2[i1];
                }

                fontWidth2=getLineLen(upPos,downPos);
                if(fontWidth2>minFontDistance&&fontWidth2<maxFontDistance&&num2>2)
                {
                    p3.x=(p2[0].x+p2[1].x+p2[2].x)/3;
                    p3.y=(p2[0].y+p2[1].y+p2[2].y)/3;

                    
                    if(zt_gn5(p2,maxAngleDif,num2))
					{
                       ++fontNum;
					}
					
                }

                if(fontNum!=1)
                    continue;



                if(!loop)
                    continue;


                angle1=calcAngle2(Point(cenX,cenY),p3);
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                param.var2[19]=angle1;
                param.var1[28]=p3.x;
                param.var1[29]=p3.y;
               
				
                loop=false;

            }

        }

    }
}



void zt_GLineGN5_2(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    volatile unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值

    double angleOffset=param.var2[0];//角度偏移

    double angle1,angleTmp;

    int x1,y1,len1,len2,distance1;

   
    Mat m5=src.clone();
    Mat m1;


    param.var2[19]=360;
    bool loop=true;


    int num1,num2,num3;

    for(int i=50;i<110;i+=2)
    //for(int i=50;i<51;i+=2)
    {
        num1=0;
        num2=0;
        num3=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY_INV);

        
        std::vector<std::vector<Point>> contours;
        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours


        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {


            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            if(distance1<minLen||distance1>maxLen)
                continue;



            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

            if(contourType(m5,contours[k],i,true))
                continue;

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }





            angleTmp=calcAngle2(Point(cenX,cenY),End_Rage2D1.center);

            if(abs(angleTmp)>45&&abs(angleTmp)<135)
                continue;



            int matchVal=matchShapes(contours[k],contours[k],1,31);

            if(matchVal<80)
                continue;


            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

                param.var2[19]=angle1;

                
               
                loop=false;
                break;



        }

    }
}


bool calcI(Mat &src, Point &cen, Point &font, int area, int len, Rect &rect, int difVal)
{
    int y,num=0;
    if(font.x>cen.x)
        y=rect.y-len;
    else
        y=rect.y;

    Rect roi(rect.x,y,rect.width,rect.height+len);
    Mat m=src(roi);
    num=countNonZero(m);

    
    if(num-area<difVal)
        return true;
    else
        return false;


}

void zt_GLineNI(Mat &src, Rect roi, params &param)
{

int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[15];//检测字体最小值
    unsigned int objMaxSize= param.var1[16];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int minRectLen=param.var1[17];//最小边长
    int maxRectLen=param.var1[18];//最大边长
    double angleOffset=180;//角度偏移
    double ratioWH=param.var2[2];//长宽比例
    double ratioArea=param.var2[3];//面积比例
    double angle1,angleTmp,fontAngle,area;

    int x1,y1,len1,len2,distance1;


    Mat m5=src.clone();
    Mat m1,m6,m7;

    param.var2[19]=360;
    bool loop=true;


    int num1,num2;

    for(int i=100;i>21;i-=2)
    {
        num1=0;
        num2=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        threshold(m5,m7,i,255,THRESH_BINARY);



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

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
                fontAngle=calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                fontAngle=calcAngle(rectpoint[1],rectpoint[2]);
            }


            if(len1<minRectLen||len1>26||len2>maxRectLen||len2<40)
                continue;



            if(len2*1.0/len1<ratioWH)
                continue;

            area=contourArea(contours[k]);
            Rect rectpoint2=boundingRect(contours[k]);
            if(!calcI(m7,Point(cenX,cenY),Point(x1,y1),area,50,rectpoint2,100))
                continue;




            angleTmp=calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
          
            if(abs(fontAngle)<10)
            {

                angle1=calcAngle2(Point(cenX,cenY),End_Rage2D1.center);
                if(angleOffset+angle1>180)
                    angle1=angleOffset+angle1-360;
                else if(angleOffset+angle1<-180)
                    angle1=angleOffset+angle1+360;
                else
                    angle1+=angleOffset;

				if(abs(angle1)<20||abs(angle1)>160)
                {
                param.var2[19]=angle1;
                param.var1[28]=x1;
                param.var1[29]=y1;

				std::cout<<x1+roi.x<<" "<<y1+roi.y<<std::endl;
         
                loop=false;
                break;
				}
            }

        }

    }
}




void detectGrayVal(Mat &src, Rect roi, params &param)
{
	int setVal=param.var1[0];//灰度值
	Mat m1=src(roi);
	Scalar meanValue=mean(m1);
	if(meanValue[0]<setVal)
		param.var1[29]=1;
	else
		param.var1[29]=0;
}

void zt_Iline(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[20];//检测最小值
    unsigned int objMaxSize=param.var1[21];//检测最大值
    int rLen1=param.var1[22];//矩形len
    int rLen2=param.var1[23];
    int step=param.var1[24];//step
    int offsetX=param.var1[25];//x距离
    int startCalcDis=param.var1[26];//开始位置
    int thresh=param.var1[27];//二值
    Mat m5=src(roi).clone();

    Mat m1;
    std::vector<std::vector<Point>> contours;
    int x1,y1,len1,len2;
    param.var2[19]=360;
    medianBlur(m5, m5, 5);
	int n1,j2;
    for(int i=thresh;i>80;i-=10)
    {
		n1=0;
        threshold(m5,m1,i,255,THRESH_BINARY);

        int errorFlag=0;


        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
        for (unsigned int k=0 ; k<contours.size(); k++)
        {


            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
            {
                
                continue;
            }
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;


            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }
           
            if(len1<rLen1||len2>rLen2)
                continue;
           
            if(contourType(m5,contours[k],i,true))
                continue;

            for(int j=0;j<contours[k].size()-step;j++)
            {
                if(abs(calcAngle2(Point(x1,y1),contours[k][j]))<25||abs(calcAngle2(Point(x1,y1),contours[k][j]))>155)
                {
                    if(abs(contours[k][j].x-contours[k][j+step].x)>offsetX)
                    {
                        ++n1;
                        j2=j;
                        j+=150;
                       
                    }
                }
            }
			if(n1==1)
			{
			 param.var2[19]=calcAngle2(Point(cenX,cenY),contours[k][j2]);
             return;
			}

        }

        
    }

}


void zt_oneTrangle(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//三角形外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//三角形外接矩形角度
    int rAngle2=param.var1[11];
    int sideLen=param.var1[12];//三角形最小边长
    int featureNum=param.var1[13];//特征数量
    double angleOffset=param.var2[0];//角度偏移
    double angle1,area;
    int x1,y1,len1,len2,distance1;

    Mat m5=src(roi).clone();
    Mat m1;

    param.var2[19]=360;
    bool loop=true;
    medianBlur(m5, m5, 3);


    for(int i=thresh;i<thresh2;i+=10)
    {
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
   
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

         
            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                if( abs(calcAngle(rectpoint[0],rectpoint[1]))<rAngle2)
                    continue;
            }
            else
            {
                if( abs(calcAngle(rectpoint[1],rectpoint[2]))<rAngle1)
                    continue;
            }
          

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;

 

            //area=contourArea(contours[k]);
            if(contourType(m5,contours[k],i,false))
                continue;

            area=contourArea(contours[k]);
            if(area/(len1*len2)<0.4)
                continue;


            Point2f center;
            float radius;
            minEnclosingCircle(contours[k],center,radius);


            int pIndex[2];

            if(!getTrangleP2(src,contours[k],center,radius,sideLen,pIndex))
                continue;

       

            int num= trangleFeature2(contours[k],sideLen,pIndex,roi);
           
            if(num<featureNum)
                continue;

           
            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;
           

            loop=false;
            break;



        }


    }

   
}


void xl_blackObj(Mat &src,  Rect roi, params &param)
{
    param.var1[29]=0;
    Mat m1;
    int len1,len2,allNum,x1,y1;
    Mat m2=src.clone();

    medianBlur(m2, m2,3);

    int cenX= param.var1[0];//圆心X
    int cenY= param.var1[1];//圆心Y
    int minSize=param.var1[2];//小圆孔size
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


     
     //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
     RotatedRect     End_Rage2D1;
     CvPoint2D32f rectpoint[4];
     double area;
     
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

         len1=getLineLen(rectpoint[0],rectpoint[1]);
         len2=getLineLen(rectpoint[1],rectpoint[2]);

         if(len1>len2)
         {
             len1^=len2;
             len2^=len1;
             len1^=len2;
         }

         if(len1<minLen||len1>maxLen)
             continue;

         if(getLineLen(Point(x1,y1),Point(cenX,cenY))>radius)
             continue;

         ++allNum;
         if(allNum>0)
         {
             param.var1[29]=1;
             
             break;
         }

     }



}

void zt_3rdLine(Mat &src,Rect roi,params& param,bool blackHat)
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

    Mat m7=m5.clone();


    int offset =-10;//偏移量
    if(blackHat) offset*=-1;
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m5, m5, MORPH_TOPHAT , element);
    else
        morphologyEx(m5, m5, MORPH_BLACKHAT, element);



    Mat m1;
    int a1,a2,a3,b1,c1,v1,v2,smoothNum,a4,shadowVal;
    int errorFlag=0;
    if(blackHat)
        thresh=40;

    std::vector<std::vector<Point>> contours;
    for(int i=thresh;i>9;)
    {

      
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
        //threshold(m5,m2,i,255,THRESH_BINARY);



        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];
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


            if(abs(distance-distance1)>35)
                continue;


            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
       

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

                    if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*4)
                        continue;

                    if(abs(angle1)>13&&abs(angle1)<167)
                        continue;

                }
              

                len1=getLineLen(rectpoint[0],rectpoint[1]);
                len2=getLineLen(rectpoint[1],rectpoint[2]);

                if(len1>len2)
                {
                    angle2=calcAngle(rectpoint[0],rectpoint[1]);
                }
                else
                {
                    angle2=calcAngle(rectpoint[1],rectpoint[2]);
                }


                if(len1<len2)
                {
                    len1^=len2;
                    len2^=len1;
                    len1^=len2;

                }

                if(!blackHat)
                {
                    if(abs(angle2)>30)
                    {
                        
                        continue;
                    }
                    if(abs(angle2)>20&&len1>37)//矩形角度
                    {
                        
                        continue;
                    }

                    if(len1>44||len1<19)
                    {
                       
                        continue;
                    }

                    if(len1*1.0/len2<1.3)
                    {
                       
                        continue;
                    }
                }
                else
                {

                    if(len1>70||len1<13)
                    {
                       
                        continue;
                    }

                    if(len1*1.0/len2<1.3&&contours[k].size()<100)
                    {
                        
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

                if(!blackHat)
                {

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
                     
                        continue;
                    }


                }

                a4= Smoothlevel(contours[k],5,20,1);
                smoothNum=a4;

                
                if(a4>9&&len2<15)
                {
              
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
                      
                        continue;
                    }


                    if(contours[k].size()<objMinSize+25)
                        continue;


                    if(angleOffset+angle1>180)
                        angle1=angleOffset+angle1-360;
                    else if(angleOffset+angle1<-180)
                        angle1=angleOffset+angle1+360;
                    else
                        angle1+=angleOffset;
                    px=x1;
                    py=y1;
                    angle=angle1;

					//std::cout<<"I:"<<i<<"size"<<contours[k].size()<<std::endl;

                    loop=false;

                }
//查找两个相同特征
                if(loop&&calcFont_QC(a1,QC_Arr))
                {

                    loop=false;
                }

            }

        }

        param.var1[27]=i;
        param.var1[26]=a1;
        if(!loop)
        {
            loop=true;
            int c1Num;

            Point nP1;
            
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
                           
                            continue;
                        }
                         v1= grayValDif(m7,contours[QC_Index[i]]);
                         v2= grayValDif(m7,contours[QC_Index[j]]);
    
                        angle1=calcAngle2(Point(cenX,cenY),QC_Arr[i]);
                        if(angleOffset+angle1>180)
                            angle1=angleOffset+angle1-360;
                        else if(angleOffset+angle1<-180)
                            angle1=angleOffset+angle1+360;
                        else
                            angle1+=angleOffset;


         

                        c1Num=0;
                        for(int k=0;k<c1;k++)
                        {

                            
                            nP1.x=(QC_Arr[i].x+QC_Arr[j].x)/2;
                            nP1.y=(QC_Arr[i].y+QC_Arr[j].y)/2;
                            if(getLineLen(QC_Noise[k],nP1)<40)
                                ++c1Num;
                        }
                       

                        if(c1Num>6)
                        {
                            if(v1<30&&v2<30)
                            {
                               
                                continue;
                            }

    
                        }

                        if(c1Num>9)
                        {
                            
                            continue;
                        }

                        if(a2>1&&a3>1&&abs(a2-a3)<2)
                        {
                            
                            continue;
                        }

                        
                        if(b1>9||(b1>7&&c1Num>7))
                        {
                            
                            continue;
                        }

						if(QC_Shadow[i]>50&&QC_Shadow[j]>50)
                        {
                            
                            continue;
                        }

                            px=QC_Arr[i].x;
                            py=QC_Arr[i].y;
                            angle=angle1;

                            param.var2[19]=angle;
                            param.var1[29]=py;
                            param.var1[28]=px;
                           
                            goto END;
                    }
               }



            }
            if(blackHat)
            {
                if(a2>0&&a3>0)
                {
                  
                    continue;
                }
                c1Num=0;
                for(int k=0;k<c1;k++)
                {


                    nP1.x=px;
                    nP1.y=py;
                    if(getLineLen(QC_Noise[k],nP1)<40)
                        ++c1Num;
                }




              
                if(a1<4&&c1Num<6&&b1<6&&param.var1[27]>9)
                {
                   
                    if(abs(angle)<15||abs(angle)>165)
                    {


                        param.var2[19]=angle;
                        param.var1[29]=py;
                        param.var1[28]=px;

                        goto END;
                    }
                }

            }

        }


    }


END:

             return;


}


void zt_no3rdLine(Mat &src, Rect roi, params& param)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测NI最小值
    unsigned int objMaxSize=param.var1[3];//检测NI最大值
    int distance=param.var1[4];//字体到中心点距离
    int thresh=param.var1[5];//二值


    param.var2[19]=360;//工件角度
	double fontAngleArr[3];
    param.var2[10]=0;
    int angle1,angle2,x1,y1,len1,len2,distance1,j1;

    Mat m5;
    m5=src(roi).clone();

    Mat m1;



    std::vector<std::vector<Point>> contours;
    for(int i=thresh;i>29;i-=10)
    {

        j1=0;

        //threshold(m1,m1,thresh,255,THRESH_BINARY_INV);
        threshold(m5,m1,i,255,THRESH_BINARY);


        findContours(m1,
                     contours, // a vector of contours
                     CV_RETR_LIST, // retrieve the external contours
                     CV_CHAIN_APPROX_NONE); // retrieve all pixels of each contours

        //std::vector<std::vector<cv::Point>>::const_iterator itContours= contours.begin();
        RotatedRect     End_Rage2D1;
        CvPoint2D32f rectpoint[4];

        for (unsigned int k=0 ; k<contours.size(); k++)
        {

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            if(abs(distance-distance1)>35)
                continue;

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));

            //字体在中心点相连角度
             if(abs(angle1)>35&&abs(angle1)<145)
                continue;

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*4)
                continue;

            if(contourType(m5,contours[k],i,false))
                continue;

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                angle2=calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                angle2=calcAngle(rectpoint[1],rectpoint[2]);
            }


            if(abs(angle2)>30)
                continue;



            if(len1<len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;

            }

            if(len1>44||len1<30)
            {

                continue;
            }
            if(len1*1.0/len2<5)
                continue;


            if(contourArea(contours[k])/len1/len2<0.5)
                continue;

			fontAngleArr[j1];
            ++j1;

            if(j1>1)
            {
            
            double d1=abs(fontAngleArr[0]-fontAngleArr[1]);
            
            if(d1<8)
                 param.var2[10]=82000;
            return;
            }




        }
    }
}


void zt_3rdLineQC(Mat &src,Rect roi,params& param,bool blackHat)
{

    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测NI最小值
    unsigned int objMaxSize=param.var1[3];//检测NI最大值
    int distance=param.var1[4];//字体到中心点距离
    int thresh=param.var1[5];//二值
    double angleOffset=param.var2[0];//角度偏移

    param.var2[19]=360;//工件角度
    int px=0;//字体坐标
    int py=0;
    int matchVal,j1;
    int fontGrayVal,fontGrayDif;
    int angle1,angle2,x1,y1,len1,len2,distance1,min1=255,max1=0;
    bool loop=true;
    double area;

    Mat m5;
    m5=src(roi).clone();

    medianBlur(m5, m5,3);
  

    
    int offset =-10;//偏移量
    if(blackHat) offset*=-1;
    int Absolute_offset = offset > 0 ? offset : -offset;//偏移量绝对值
    //自定义核
    Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
    //进行操作
    if( offset < 0 )
        morphologyEx(m5, m5, MORPH_TOPHAT , element);
    else
        morphologyEx(m5, m5, MORPH_BLACKHAT, element);

    Mat m1,m2;
    int a1,a2,a3,b1,c1,v1,v2,smoothNum,a4;
    int matchCount=0;


    std::vector<std::vector<Point>> contours;
    for(int i=thresh;i>3;)
    {

      
        if(i>70)
        {

            i-=20;
        }
        else if(i>40)
        {
            i-=10;
        }
        else if(i>25)
        {
            i-=5;
        }
        else if(i>15)
        {
            i-=2;
        }
        else
            i--;


        j1=0;
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


            if(abs(distance-distance1)>35)
                continue;


            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));

            //字体在中心点相连角度
            if(abs(angle1)>25&&abs(angle1)<155)
                continue;




            if(contours[k].size()>objMinSize-15)
                ++b1;


            if(abs(angle1)<160&&abs(angle1)>20)
            {
                if(contours[k].size()<55)
                    continue;
            }

            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*4)
                continue;

            if(m2.at<uchar>(contours[k][0].y-1,contours[k][0].x)==255)
                continue;

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                angle2=calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                angle2=calcAngle(rectpoint[1],rectpoint[2]);
            }

            if(abs(angle2)>40&&contours[k].size()<70)
                continue;

            if(abs(angle2)>60)
                continue;



            if(len1<len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;

            }


            if(len1>44||len1<19)
            {
                continue;
            }

            matchVal=matchShapes(contours[k],contours[k],1,31);
           
            ++matchCount;
            param.var1[29]=matchCount;
            if(matchCount>29)
                return;
            if(matchVal>80)
            {
                QC_Arr[j1++]=Point(x1,y1);
             

            }

            if(matchVal>90&&contours[k].size()>70)
            {
                goto END;
            }
            if(j1<2)
                continue;


            if(calcFont_QC(j1,QC_Arr))
            {
END:
                param.var2[19]=angle1;
                
                return;
            }




        }
    }
}

bool semicircleCen(vector<Point> &contours,int x1,int y1,int step,int minR,int maxR,int cirFeaNum,int move,int offset,Point &cen)
{
    int nearLen=10000,temp1,index1,index2,j,j2;
    Point p1,p2,p3;
    for(int j1=0;j1<contours.size();j1++)
    {

        temp1=getLineLen(Point(x1,y1),contours[j1]);
        if(temp1<nearLen)
        {
            nearLen=temp1;
            j=j1;

        }
    }


    j2=getIndex(contours.size(),j,contours.size()/2);

    index1=getIndex(contours.size(),j2,step);
    index2=getIndex(contours.size(),j2,-step);
	
    p1.x=contours[j2].x;
    p1.y=contours[j2].y;
    p2.x=contours[index1].x;
    p2.y=contours[index1].y;
    p3.x=contours[index2].x;
    p3.y=contours[index2].y;


    if(!getCircleCenter(p1,p2,p3,cen))
        return false;

    int r1=getLineLen(p1,cen);

    if(r1<minR||r1>maxR)
        return false;


    int feaNum=circleFeature(contours,cen,step,move,offset);

    if(feaNum<cirFeaNum)
        return false;

    
    return true;
}
void hd_smallWork_other(Mat &src, Rect roi, params &param)
{

    
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




        len1=getLineLen(rectpoint[0],rectpoint[1]);
        len2=getLineLen(rectpoint[1],rectpoint[2]);

        if(len1>len2)
        {
            len1^=len2;
            len2^=len1;
            len1^=len2;
        }


      
        circleFea=circleRediusFeature(contours[k],step,50,rLen1,rLen2);

        if(circleFea<cirFeaVal)
            continue;

        if(area/contours[k].size()<3)
            continue;


        param.var1[26]=90000;
        param.var1[29]=82000;


        break;

    }


}
void hd_smallWork(Mat &src, Rect roi, params &param)
{
    Mat m1=src.clone();
    Mat m3;
    int step,x1,y1,circleFea;
    int len1,len2;
    

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
	std::cout<<"start"<<std::endl;
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




        len1=getLineLen(rectpoint[0],rectpoint[1]);
        len2=getLineLen(rectpoint[1],rectpoint[2]);

        if(len1>len2)
        {
            len1^=len2;
            len2^=len1;
            len1^=len2;
        }


   

        circleFea=circleRediusFeature(contours[k],step,20,rLen1,rLen2);
       
        if(circleFea<cirFeaVal-1)
            continue;

        if(area/contours[k].size()>3)
        {
            break;
        }


        param.var1[27]=x1;
        param.var1[28]=y1;
        param.var1[29]=90000;

        break;

    }

  std::cout<<"end"<<std::endl;

}

void zt_JLine(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//外接矩形角度

    double lenRatio=param.var2[0];//长度
    double areaRatio=param.var2[1];//面积
    double angleOffset=param.var2[2];//角度偏移
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1;

    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

    param.var2[19]=360;
    bool loop=true;
   
    int error=0;
	
    for(int i=thresh;i<thresh2;i+=3)
    {
        int a1=0;
        arr.clear();
        error=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        
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

           
            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=calcAngle(rectpoint[0],rectpoint[1]);
                if(abs(r1Ang)<rAngle1)
                    continue;
            }
            else
            {
                r1Ang=calcAngle(rectpoint[1],rectpoint[2]);
                if(abs(r1Ang)<rAngle1)
                    continue;
            }

          

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;

            area=contourArea(contours[k]);

            if(area/(len1*len2)<areaRatio)
                continue;         

            if(len2*1.0/len1<lenRatio)
                continue;

            arr.push_back(Point(x1,y1));

            if(arr.size()<1)
                continue;
            
            if(nearbyPos(arr,5,1)==false)
                continue;

           

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));
            if(angleOffset+angle1>180)
                angle1=angleOffset+angle1-360;
            else if(angleOffset+angle1<-180)
                angle1=angleOffset+angle1+360;
            else
                angle1+=angleOffset;

            param.var2[19]=angle1;
            param.var1[28]=x1;
            param.var1[29]=y1;
            

            loop=false;
            break;

        }

       
    }

}

int calc_JLineQC(vector<Point> &contours,int x1,int y1,int maxDis,int dif,Rect &roi)
{
    Point cen(x1,y1);
    for(int i=0;i<contours.size()-15;i++)
    {
        if(contours[i].x>x1&&contours[i].y<y1)
        {

            if(getLineLen(cen,contours[i])-maxDis>dif)
            {
                
                return 0;
            }
        }
        if(contours[i].x<x1&&contours[i].y>y1)
        {
            if(getLineLen(cen,contours[i])-maxDis>dif)
                return 180;

        }

    }
    return 360;
}

void zt_JLineQC(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize=500;//检测字体最小值
    unsigned int objMaxSize=1600;//检测字体最大值
    int maxToCen=30;//中心点偏离
    int thresh=73;//二值
    int thresh2=254;//二值
    int rLen1=220;//外接矩形len
    int rLen2=240;
    int rLenDif=50;//矩形差


    double angleOffset=param.var2[2];//角度偏移
    double angle1;
    int x1,y1,len1,len2,distance1;

    Mat m5=src.clone();
    Mat m1;


    param.var2[19]=360;
    bool loop=true;
    

	bool whiteCircle=false;
    int error=0;


    for(int i=thresh;i<thresh2;i+=10)
    //for(int i=thresh2;i>thresh;i-=10)
    {
        
        error=0;
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);

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


   
            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);



          

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;


            if(abs(len1-len2)>rLenDif)
                continue;

            


            if(getLineLen(Point(x1,y1),Point(cenX,cenY))>maxToCen)
                continue;


           
             if(contourType(m5,contours[k],i,true))
            {
                continue;
            }


             angle1=calc_JLineQC(contours[k],x1,y1,len2/2,10,roi);
            if(angle1==360)
                continue;

           
            param.var2[19]=anlgeOffset(angle1,angleOffset);;

            

            loop=false;
            break;

        }

        
    }

}





void zt_JLineMatch(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//中心点X
    int cenY=param.var1[1]-roi.y;//中心点Y
    unsigned int objMinSize= param.var1[2];//检测字体最小值
    unsigned int objMaxSize=param.var1[3];//检测字体最大值
    int minLen=param.var1[4];//字体到中心点距离
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//二值
    int thresh2=param.var1[7];//二值
    int rLen1=param.var1[8];//外接矩形len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//外接矩形角度

    double lenRatio=param.var2[0];//长度
    double areaRatio=param.var2[1];//面积
    double angleOffset=param.var2[2];//角度偏移
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1;

    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

    param.var2[19]=360;
    bool loop=true;


    int error=0;
    int matchVal;

    for(int i=thresh;i<thresh2;i+=15)
    
    {
        int a1=0;
        arr.clear();
        
        if(!loop)
            break;

        threshold(m5,m1,i,255,THRESH_BINARY);
        
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
           
            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));


            //circle(g_dstImage2,Point(x1+roi.x,y1+roi.y),7,Scalar(255,0,0),3);
            if(distance1<minLen||distance1>maxLen)
                continue;

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=calcAngle(rectpoint[0],rectpoint[1]);

                if(abs(r1Ang)<rAngle1)
                    continue;
            }
            else
            {
                r1Ang=calcAngle(rectpoint[1],rectpoint[2]);

                if(abs(r1Ang)<rAngle1)
                    continue;
            }

       

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }


            if(len1<rLen1||len2>rLen2)
                continue;

            


            if(contourType(m5,contours[k],i,true))
                continue;

          
            if(len2*1.0/len1<lenRatio)
                continue;

           


            matchVal=matchShapes(contours[k],contours[k],1,31);
            if(matchVal<80)
                continue;


            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));


            param.var2[19]=anlgeOffset(angle1,angleOffset);;
            param.var1[28]=x1;
            param.var1[29]=y1;
           


            loop=false;
            break;


        }

        
    }

}

void hd_vision3_angle(Mat &src, Rect roi, params &param)
{
    //华东第3套角度
        int cenX=param.var1[0]-roi.x;//中心点
        int cenY=param.var1[1]-roi.y;
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


             if(getLineLen(Point(x1,y1),Point(cenX,cenY))>100)
                 continue;


             len1=getLineLen(rectpoint[0],rectpoint[1]);
             len2=getLineLen(rectpoint[1],rectpoint[2]);


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
                 temp1=getLineLen(contours[k][i1],cen);

                 if(temp1>maxDis)
                 {
                     maxDis=temp1;
                     index=i1;
                 }
                 j++;
             }

             p1=contours[k][index];

             double angle1=calcAngle2(Point(x1,y1),p1);
             param.var2[19]=anlgeOffset(angle1,angleOffset);
             param.var1[29]=i;
       
			 
             loop=false;
             break;



         }


     }

}



