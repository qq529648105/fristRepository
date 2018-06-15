#include"visiondll.h"
#include "VisionRivoet.h"
#include "common.h"
#include <algorithm>

using namespace cv;


extern void zt_no3rdLine(Mat &src,Rect roi,params &param);
extern void zt_3rdLineQC(Mat &src,Rect roi,params &param,bool blackHat);
extern void zt_3rdLine(Mat &src,Rect roi,params &param,bool blackHat);
extern Point shimCentre2(Mat &src,Rect roi,params &param);
extern void shimDetection2(Mat &src,Rect roi,params &param,bool blackHat);
extern void HD_circleCenter(Mat &src,Rect roi,params &param);
extern void calcWork1Angle(Mat &src,Rect roi,params &param);
extern void adjustWork1Angle(Mat &src,Rect roi,params &param);
extern void workIsEmpty2(Mat &src,Rect roi,params &param);
extern void hd_maxDiameter2(Mat &src,Rect roi,params &param);
extern void zt_ZT01_2(Mat &src,Rect roi,params &param);
extern void zt_zt01Match(Mat &src,Rect roi,params &param);
extern void shimDetect(Mat &src,Rect roi,params &param);
extern void zt_5thLine(Mat &src,Rect roi,params &param);
extern void wx_direction0(Mat &src,params &param);
extern void wx_direction1(Mat &src,Rect roi,params &param);
extern void wx_defect(Mat &src,Rect roi,params &param);
extern void zt_6thLine(Mat &src,Rect roi,params &param);
extern void zt_GLineGN5(Mat &src,Mat &src2,Rect roi,params &param);
extern void zt_GLineNI(Mat &src,Rect roi,params &param);
extern void zt_GLineGN5_2(Mat &src,Rect roi,params &param);
extern void zt_oneTrangle(Mat &src,Rect roi,params &param);
extern void detectGrayVal(Mat &src,Rect roi,params &param);
extern void xl_blackObj(Mat &src,Rect roi,params &param);
extern void zt_2ndLine(Mat &src,Rect roi,params &param);
extern void hd_smallWork(Mat &src,Rect roi,params &param);
extern void hd_smallWork_other(Mat &src,Rect roi,params &param);
extern void hd_maxDiameter(Mat &src,Rect roi,params &param);
extern void calcWork1Angle_2(Mat &src,Rect roi,params &param);
extern void adjustWork1Angle_2(Mat &src,Rect roi,params &param);
extern void zt_Iline(Mat &src,Rect roi,params &param);
extern void hd_vision3_angle(Mat &src,Rect roi,params &param);
extern void hd_maxDiameter2(Mat &src,Rect roi,params &param);
extern void zt_ZDK(Mat &src,Rect roi,params &param);
extern void zt_JLine(Mat &src,Rect roi,params &param);
extern void zt_JLineQC(Mat &src,Rect roi,params &param);
extern void zt_JLineMatch(Mat &src,Rect roi,params &param);
extern int g_nElementShape;


void commonMatch(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//���ĵ�X
    int cenY=param.var1[1]-roi.y;//���ĵ�Y
    unsigned int objMinSize= param.var1[2];//���������Сֵ
    unsigned int objMaxSize=param.var1[3];//����������ֵ
    int minLen=param.var1[4];//���嵽���ĵ����
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//��ֵ
    int thresh2=param.var1[7];//��ֵ
    int rLen1=param.var1[8];//��Ӿ���len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//��Ӿ��νǶ�Ĭ��360   <45���� or >45����
    int angleRange=param.var1[11];//�Ƕȷ�Χ20  ��ʶ0��������  ��ʶ90��������
    int angleRangeFlag=param.var1[12];//��Χ��ʶ0 90
    int step=param.var1[13];//i+=step;
    bool isBlack=param.var1[14]==1?true:false;//1��ɫ  0��ɫ
	int noisy=param.var1[15];//���
	int bestVal=param.var1[16];//���ƶ�
	int fg_1=param.var1[17];
	int fg_method=(param.var1[18]>200&&param.var1[18]<210)?param.var1[18]:2;

    double lenRatio=param.var2[0];//����  Ĭ��-1    <100 <����  or >100  >����   ��/��
    double areaRatio=param.var2[1];//���  Ĭ��-1   <100 <����  or >100  >����   area/rectArea
    double angleOffset=param.var2[18];//�Ƕ�ƫ��
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1,a3;

    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

	param.var1[27]=0;
    param.var2[19]=360;
    bool loop=true;


    int error=0;
    int matchVal;
    int i=thresh;
    int sign=1;
    if(thresh>thresh2)
        sign=-1;

    while(1)
    {
        if(!loop)
            break;
        i+=sign*step;
        
        if(abs(i-thresh2)<step)
            break;
        arr.clear();
        error=0;


		a3=0;
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
			if(contours[k].size()<20)
                continue;
			
            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                {}
                else
                {
                    ++a3;
                }

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                {}
                else
                {
                    ++a3;
                }

            }

            if(a3>noisy)
            {
                loop=false;
                break;
            }
//if(contours[k].size()==75) std::cout<<"thisSize__:"<<contours[k].size()<<"max min"<<objMinSize<<" "<<objMaxSize*2<<std::endl;
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize)
                continue;

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));


            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                    continue;

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                    continue;

            }

         
            if(distance1<minLen||distance1>maxLen)
                continue;

           
            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                r1Ang=calcAngle(rectpoint[1],rectpoint[2]);
            }

            if(rAngle1>0&&rAngle1<90)
            {
                if(rAngle1<45)
                {
                    if(abs(r1Ang)>rAngle1)
                        continue;
                }
                else
                {
                    if(abs(r1Ang)<rAngle1)
                        continue;
                }
            }

			
        
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

      
			if(len1<rLen1||len2>rLen2)
                continue;

			

            if(contourType(m5,contours[k],i,isBlack))
                continue;

       


            if(len2*1.0/len1<lenRatio)
                continue;
			
     
			area=contourArea(contours[k]);

            if(areaRatio<100&&area/(len1*len2)<areaRatio)
                continue;


      
            if(areaRatio>100)
            {
                if(area/(len1*len2)>areaRatio-100)
                    continue;
            }
			
            matchVal=matchShapes(contours[k],contours[k],fg_method,fg_1);
      
			//std::cout<<"bbb"<<matchVal<<" i:"<<i<<" fg_method:"<<fg_method<<" size:"<<contours[k].size()<<std::endl;
			if(matchVal<bestVal)
                continue;

            param.var2[19]=anlgeOffset(angle1,angleOffset);;
            param.var1[28]=x1;
            param.var1[29]=y1;
			param.var1[27]=matchVal;
			param.var1[26]=i;

            loop=false;
            break;


        }
      
    }


}






void countMatch(Mat &src, Rect roi, params &param)
{
    int cenX=param.var1[0]-roi.x;//���ĵ�X
    int cenY=param.var1[1]-roi.y;//���ĵ�Y
    unsigned int objMinSize= param.var1[2];//���������Сֵ
    unsigned int objMaxSize=param.var1[3];//����������ֵ
    int minLen=param.var1[4];//���嵽���ĵ����
    int maxLen=param.var1[5];
    int thresh=param.var1[6];//��ֵ
    int thresh2=param.var1[7];//��ֵ
    int rLen1=param.var1[8];//��Ӿ���len
    int rLen2=param.var1[9];
    int rAngle1=param.var1[10];//��Ӿ��νǶ�Ĭ��360   <45���� or >45����
    int angleRange=param.var1[11];//�Ƕȷ�Χ20  ��ʶ0��������  ��ʶ90��������
    int angleRangeFlag=param.var1[12];//��Χ��ʶ0 90
    int step=param.var1[13];//i+=step;
    bool isBlack=param.var1[14]==1?true:false;//1��ɫ  0��ɫ
	int noisy=param.var1[15];//���
	int bestVal=param.var1[16];//���ƶ�
	int fg_1=param.var1[17];
	int fg_method=(param.var1[18]>200&&param.var1[18]<210)?param.var1[18]:2;

    double lenRatio=param.var2[0];//����  Ĭ��-1    <100 <����  or >100  >����   ��/��
    double areaRatio=param.var2[1];//���  Ĭ��-1   <100 <����  or >100  >����   area/rectArea
    double angleOffset=param.var2[18];//�Ƕ�ƫ��
    double angle1,area,r1Ang;
    int x1,y1,len1,len2,distance1,a1,a2,a3;

    Mat m5=src.clone();
    Mat m1;

    std::vector<Point> arr;

	param.var1[27]=0;
    param.var2[19]=360;
    bool loop=true,found=false;


    int error=0;
    int matchVal;
    int i=thresh;
    int sign=1;
    if(thresh>thresh2)
        sign=-1;

    while(1)
    {
        if(!loop)
            break;
        i+=sign*step;
        
        if(abs(i-thresh2)<step)
            break;
        arr.clear();
        error=0;


		a1=a2=a3=0;
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
			if(contours[k].size()<20)
                continue;
			
            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                {}
                else
                {
                    ++a3;
                }

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                {}
                else
                {
                    ++a3;
                }

            }

            if(a3>noisy)
            {
                loop=false;
                break;
            }
//if(contours[k].size()==75) std::cout<<"thisSize__:"<<contours[k].size()<<"max min"<<objMinSize<<" "<<objMaxSize*2<<std::endl;
            if(contours[k].size()<objMinSize||contours[k].size()>objMaxSize*2)
                continue;

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;
            distance1=sqrt((x1-cenX*1.0)*(x1-cenX)+(y1-cenY)*(y1-cenY));

            angle1=calcAngle2(Point(cenX,cenY),Point(x1,y1));


            if(angleRangeFlag==0)
            {
                if(abs(angle1)>angleRange&&abs(angle1)<180-angleRange)
                    continue;

            }
            else if(angleRangeFlag==90)
            {
                if(abs(angle1)<90-angleRange||abs(angle1)>90+angleRange)
                    continue;

            }

         
            if(distance1<minLen||distance1>maxLen)
                continue;

           
            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                r1Ang=calcAngle(rectpoint[0],rectpoint[1]);
            }
            else
            {
                r1Ang=calcAngle(rectpoint[1],rectpoint[2]);
            }

            if(rAngle1>0&&rAngle1<90)
            {
                if(rAngle1<45)
                {
                    if(abs(r1Ang)>rAngle1)
                        continue;
                }
                else
                {
                    if(abs(r1Ang)<rAngle1)
                        continue;
                }
            }

			
        
            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

      
			if(len1<rLen1||len2>rLen2)
                continue;

			

            if(contourType(m5,contours[k],i,isBlack))
                continue;

       


            if(len2*1.0/len1<lenRatio)
                continue;
			
     
			area=contourArea(contours[k]);

            if(areaRatio<100&&area/(len1*len2)<areaRatio)
                continue;


      
            if(areaRatio>100)
            {
                if(area/(len1*len2)>areaRatio-100)
                    continue;
            }
			
           
			 if(!found)
            {
                found=true;
                break;
            }

            double angle3=anlgeOffset(angle1,angleOffset);
            if(angleRangeFlag==90)
            {
                if(angle3<0)
                    a1++;
                else
                    a2++;

            }
            else
            {
                if(abs(angle3)<90)
                    a1++;
                else
                    a2++;
            }

            param.var2[19]=anlgeOffset(angle1,angleOffset);;
            param.var1[28]=x1;
            param.var1[29]=y1;
			param.var1[26]=i;

            loop=false;
            


        }
      
    }

	if(loop==false)
    {
        if(a1>0&&a2>0)
            param.var2[19]=360;

    }


}





int calcDefect(vector<Point> &contours,int step,int pos,int xyDif,int xEnd,int yEnd)
{
   int num=0;
   bool b1,b2,b3,b4;
   b1=b2=b3=b4=false;
   for(int i=0;i<contours.size()-pos;i+=step)
   {
        if(!b1&&abs(contours[i].x-contours[i+pos].x)>xyDif)
        {
            ++num;
            b1=true;
            
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
            
        }
        if(abs(contours[i].y-contours[i+pos].y)<yEnd)
        {

            b3=false;
        }



   }
   return num;
}

void defect(Mat &src, Rect roi, params &param)
{

    unsigned int objMinSize=param.var1[0];
    unsigned int objMaxSize=param.var1[1];
    unsigned int thresh=param.var1[2];//��ֵ
    int step=param.var1[3];
    int pos=param.var1[4];//��һλ��
    int angleDif=param.var1[5];//�ǶȲ�
    int xyDif=param.var1[6];//ƫ��
    int xEnd=param.var1[7];//x����
    int yEnd=param.var1[8];//y����
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

        if(contourType(src,contours[k],thresh,true))
            continue;

        End_Rage2D1 = minAreaRect(Mat(contours[k]));
        cvBoxPoints(End_Rage2D1,rectpoint);
        x1=End_Rage2D1.center.x;
        y1=End_Rage2D1.center.y;

       
        int num=calcDefect(contours[k],step,pos,xyDif,xEnd,yEnd);
        
        param.var1[29]=num;
        
        break;



    }

}

void zt_LLine(Mat &src, Rect roi, params &param)
{

   int cenX=param.var1[0]-roi.x;//���ĵ�X
    int cenY=param.var1[1]-roi.y;//���ĵ�Y
    unsigned int objMinSize=param.var1[2];
    unsigned int objMaxSize=param.var1[3];
    int thresh=param.var1[4];//��ֵ
    int rMinLen=param.var1[5];//��
    int rMaxLen=param.var1[6];//��
    int redius=param.var1[7];//�뾶
    int pos1=param.var1[8];//
    int pos2=param.var1[9];//
    int pos3=param.var1[10];//
    int pos4=param.var1[11];//
    double angleOffset=param.var2[18];//�Ƕ�ƫ��
    Mat m1,m2;

    double angle1;

    int x1,y1,len1,len2;
    Mat m5=src(roi).clone();

    medianBlur(m5, m5, 3);

    circle(m5,Point(cenX,cenY),redius,Scalar(0,0,0),3);
    line(m5,Point(cenX+pos1,cenY+pos2),Point(cenX+pos3,cenY+pos4),Scalar(0,0,0),6);


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

            if(contourType(src,contours[k],thresh,false))
                continue;

            End_Rage2D1 = minAreaRect(Mat(contours[k]));
            cvBoxPoints(End_Rage2D1,rectpoint);
            x1=End_Rage2D1.center.x;
            y1=End_Rage2D1.center.y;

            if(getLineLen(Point(x1,y1),Point(cenX,cenY))>80)
                continue;

            len1=getLineLen(rectpoint[0],rectpoint[1]);
            len2=getLineLen(rectpoint[1],rectpoint[2]);

            if(len1>len2)
            {
                len1^=len2;
                len2^=len1;
                len1^=len2;
            }

            if(len1<rMinLen||len2>rMaxLen)
                continue;

            int posLen=getLineLen(contours[k][0],contours[k][300]);

            angle1=posLen<rMinLen/2?90:-90;

            param.var2[19]=anlgeOffset(angle1,angleOffset);

            return;



        }

    }

}





void detectWork(Mat &src,Rect roi,params& param,int method)
{
		 if(method==1)//ƽ��QC  ��ͦC��
        {
			param.var1[25]=1;
			zt_no3rdLine(src,roi,param);
			if(param.var2[10]==82000)
				return;
			param.var1[25]=2;
			zt_3rdLineQC(src,roi,param,false);
			if(param.var2[19]==360)
			{
				param.var1[25]=3;
				zt_3rdLine(src,roi,param,false);
			}
            if(param.var2[19]==360)
            {
				param.var1[25]=4;
                zt_3rdLine(src,roi,param,true);
            }
		 }
		 else if(method==2)//�������Ƭ
		 {
			Mat m;
			medianBlur(src(roi).clone(), m,3);
			Point pCen= shimCentre2(m,roi,param);
			param.var1[25]=pCen.x;//����õ�Բ������
			param.var1[26]=pCen.y;
			shimDetection2(m,roi,param,false);
		}
		 else if(method==3)//�����Ƕ�
		 {
			 Mat m;
			medianBlur(src, m,3);
			HD_circleCenter(m,roi,param);
			calcWork1Angle(m,roi,param);
			adjustWork1Angle(m,roi,param);
		
		}
		 else if(method==4)//��������
		 {
			 workIsEmpty2(src,roi,param);
		} 
		 else if(method==5)//�����ѷ�
		 {
			 IplImage *pIplsrc = NULL;
			 pIplsrc = &IplImage(src);

			 CvRect rectROI;
			 rectROI.x = roi.x;
			 rectROI.y = roi.y;
			 rectROI.width = roi.width;
			 rectROI.height = roi.height;
			 if(param.var1[0]==1000)
				detectDiameterAndCrackPoint(pIplsrc, rectROI, param);
			 else if(param.var1[0]==2000)
				 hd_maxDiameter2(src,roi,param);	
			 else
				getCrackRegion(pIplsrc, rectROI, param);
		 }else if(method==6){//zt01 ��ͦD��
			 zt_ZT01_2(src,roi,param); 
				if(param.var2[19]==360)
					{
         params param2;
         param2.var1[0]=param.var1[0];
         param2.var1[1]=param.var1[1];
         param2.var1[2]=40;// param.var1[2];//���������Сֵ
         param2.var1[3]=200;//param.var1[3];//����������ֵ
         param2.var1[4]=param.var1[4];//���嵽���ĵ����
         param2.var1[5]=param.var1[4]+40;
         param2.var1[6]=230;//��ֵ
         param2.var1[7]=15;//��������Ӿ���len
         param2.var1[8]=40;
         param2.var1[9]=15;//��������С�߳�
         param2.var2[0]=anlgeOffset(param.var2[0],180);//�Ƕ�ƫ��
         zt_6thLine(src,roi,param2);
		 
         param.var2[19]=param2.var2[19];

         param.var1[28]= param2.var1[28];
         param.var1[29]= param2.var1[29];
		 if(param.var2[19]==360)
		 {
               zt_zt01Match(src,roi,param2);
			   param.var2[19]=param2.var2[19];

				param.var1[28]= param2.var1[28];
				param.var1[29]= param2.var1[29];

		 }


     }
		 }else if(method==7){//������Ƭ���ι�
			CvRect rectROI;
			rectROI.x = roi.x;
			rectROI.y = roi.y;
			rectROI.width = roi.width;
			rectROI.height = roi.height;
			shimDetect(&IplImage(src),rectROI,param);
		 }else if(method==8){//��ͦE��
		 zt_5thLine(src,roi,param);
		 }else if(method==9){//��������
			  IplImage *pIplsrc = NULL;
			 pIplsrc = &IplImage(src);

			 CvRect rectROI;
			 rectROI.x = roi.x;
			 rectROI.y = roi.y;
			 rectROI.width = roi.width;
			 rectROI.height = roi.height;

			 rivoetDirectionDetect(pIplsrc, rectROI, param);


		 }else if(method==10){//����ȱ��test  using

			 wx_direction0(src,param);
			 if(param.var1[29]==360)
			 {
				 wx_direction1(src,roi,param);
			 }
			 

		 }else if(method==11){//using
			 //����ȱ�� param.var1[27]����ֵ �ϸ�0  ֱ��4  ����5  �ѷ�6 δ�ҵ�íͷ-1
			Rect roi(470,220,400,400);
			wx_defect(src,roi,param);
		 }else if(method==12){//��ͦF��
				zt_6thLine(src,roi,param);
		 }else if(method==13){//��ͦG��
		    Mat m5,m6;
			medianBlur(src(roi), m5, 3);
			int offset = 10;
			int Absolute_offset = offset > 0 ? offset : -offset;
			 Mat element = getStructuringElement(g_nElementShape, Size(Absolute_offset*2+1, Absolute_offset*2+1), Point(Absolute_offset, Absolute_offset) );
			 if( offset < 0 )
			   morphologyEx(m5, m6, MORPH_TOPHAT , element);
			  else
			    morphologyEx(m5, m6, MORPH_BLACKHAT, element);

			zt_GLineGN5(m6,m5,roi,param);
			if(param.var2[19]==360)
			{
				std::cout<<"calc NI"<<std::endl;
			    zt_GLineNI(m6,roi,param);
			}
			if(param.var2[19]==360)
			{
				
			    zt_GLineGN5_2(m5,roi,param);
			}
		 }else if(method==14){//������
			zt_oneTrangle(src,roi,param);
			
		 }else if(method==15){//�Ҷ�ֵ���
			 detectGrayVal(src,roi,param);
		 }else if(method==16){//ѩ������
			 xl_blackObj(src,roi,param);
		 }else if(method==17){//��ͦB��
			 
			 zt_2ndLine(src,roi,param);
		 }else if(method==18){//����С��Ƭ
			 hd_smallWork_other(src,roi,param);
			 if(param.var1[26]==82000)
				hd_smallWork(src,roi,param);
		 }else if(method==19){//����С��Ƭ�Ƕ�
			 Mat m;
			medianBlur(src, m,3);

			Rect roi2(527,378,250,250);
			hd_maxDiameter(m,roi2,param);
			calcWork1Angle_2(m,roi,param);
			adjustWork1Angle_2(m,roi,param);
		 }else if(method==20){//��ͦI��
			
		    zt_oneTrangle(src,roi,param);
            if(param.var2[19]==360)
				zt_Iline(src,roi,param);

		 }else if(method==21){//������3�׽Ƕ�
			Mat m5;
			medianBlur(src, m5, 3);
			hd_vision3_angle(m5,roi,param);
			hd_maxDiameter2(m5,roi,param);	

		 }else if(method==100){//D��ZDK
				zt_ZDK(src,roi,param);

		 }else if(method==101){//J��
			 Mat m5;
			medianBlur(src(roi), m5, 3);
				zt_JLine(m5,roi,param);
				if(param.var2[19]==360)
					zt_JLineQC(m5,roi,param);
				if(param.var2[19]==360)
					zt_JLineMatch(m5,roi,param);

		 }else if(method==102){//E��NI  C��������  i��
			Mat m5;
			medianBlur(src(roi), m5, 3);
			commonMatch(m5,roi,param);
			
		 }else if(method==103){//H��
			param.var1[29]=82000;//����һ����
			zt_2ndLine(src,roi,param);
		 }else if(method==104){//L��
			 zt_LLine(src,roi,param);
		 }else if(method==200){//ë�̼��
			 defect(src,roi,param);
		 }else if(method==300){
		 
		 }else if(method==301){
		 
		 }else if(method==1000){
			 //������
			 Mat m5;
			medianBlur(src(roi), m5, 3);
			param.var1[18]=2;
			commonMatch(m5,roi,param);

			if(param.var2[19]==360)
			{
				std::cout<<"second"<<std::endl;
				params param_2=param;
				param_2.var1[2]=60;
				param_2.var1[3]=150;
				param_2.var1[8]=14;
				param_2.var1[9]=50;
				param_2.var1[14]=0;
				param_2.var1[16]=75;
				param_2.var1[18]=201;
				
				commonMatch(m5,roi,param_2);
				param.var2[19]=param_2.var2[19];
				param.var1[27]=param_2.var1[27];
			}

		 }else if(method==1001){
			 //��ͦI��CG12-02
			Mat m5;
			medianBlur(src(roi), m5, 3);

			params param_2;
            param_2.var1[0]=param.var1[0];//���ĵ�X
            param_2.var1[1]=param.var1[1];//���ĵ�Y
            param_2.var1[2]=340;//���������Сֵ
            param_2.var1[3]=410;//����������ֵ
            param_2.var1[4]=160;//���嵽���ĵ����
            param_2.var1[5]=230;//���嵽���ĵ����
            param_2.var1[6]=180;//��ֵ
            param_2.var1[7]=80;//��ֵ
            param_2.var1[8]=38;//��Ӿ���len
            param_2.var1[9]=190;//��Ӿ���len
            param_2.var1[10]=360;//��Ӿ��νǶ�Ĭ��360   <45���� or >45����
            param_2.var1[11]=60;//�Ƕȷ�Χ20  ��ʶ0��������  ��ʶ90��������
            param_2.var1[12]=0;//��Χ��ʶ0 90
            param_2.var1[13]=5;//i+=step;
            param_2.var1[14]=1;//1��ɫ  0��ɫ
            param_2.var1[15]=1000;//���
            param_2.var1[16]=80;//���ƶ�
            param_2.var1[17]=31;//fg
            param_2.var1[18]=209;//method

            param_2.var2[0]=2;//����Ĭ��-1    <100 <����  or >100  >����  ��/��
            param_2.var2[1]=-1;//���Ĭ��-1     <100 <����  or >100  >����  area/rectArea

            param_2.var2[18]=0;//�Ƕ�ƫ��


            param_2.var2[19]=360;//�Ƕ�

			commonMatch(m5,roi,param_2);

			if(param_2.var2[19]!=360)
            {
				param.var1[26]=param_2.var1[26];
                param.var1[27]=param_2.var1[27];
				param.var1[28]=param_2.var1[28];
                param.var1[29]=param_2.var1[29];
                if(abs(param_2.var2[19])<90)
                    param.var2[19]=90;
                else
                    param.var2[19]=-90;
            }

			if(param.var2[19]==360)
			{
				commonMatch(m5,roi,param);
			}

		 }


}

