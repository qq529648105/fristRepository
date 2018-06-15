#include"visiondll.h"
#include"VisionRivoet.h"
#include "common.h"
using namespace cv;
bool comp(const int &a,const int &b)
{
	return a>b;
}

void reverseImage(IplImage *img) {   
	unsigned char* data=(uchar *)img->imageData;  
	int step = img->widthStep/sizeof(uchar);  
	uchar* tmp;  
	for(int i=0;i<img->height;i++)  
		for(int j=0;j<img->width;j++)  
		{
			data[i*step+j] = 255 - data[i*step+j]; 
		}
			
}
//参数：Gray_img --输入的灰度图像  
//      hist 计算后输出的直方图  
void myCal_Hist(IplImage *img,int max_value, int &threshold)
{  
	unsigned char* data=(uchar *)img->imageData;  
	int step = img->widthStep/sizeof(uchar);  
	uchar* tmp;int a =0;
	int histogram[256]={0};
	float histogram_norm[256]={0};
	int i=0, j =0;
	int pixelSum = 0;
	for(i=0;i<img->height;i++)  
	{
		for(int j=0;j<img->width;j++)  
		{
			histogram[data[i*step+j]]++;
			if (data[i*step+j]<max_value)
			{
				pixelSum++;
			}
		}
	}


	//归一化
	for(i = 0; i < max_value; i++)  
	{  
		histogram_norm[i] = (float)(histogram[i]) / (float)(pixelSum);  
	}  

	//经典ostu算法,得到前景和背景的分割  
	//遍历灰度级[0,255],计算出方差最大的灰度值,为最佳阈值  
	float w0, w1, u0tmp, u1tmp, u0, u1, u,deltaTmp, deltaMax = 0;  
	for(i = 0; i < max_value; i++)  
	{  
		w0 = w1 = u0tmp = u1tmp = u0 = u1 = u = deltaTmp = 0;  

		for(j = 0; j < max_value; j++)  
		{  
			if(j <= i) //背景部分  
			{  
				//以i为阈值分类，第一类总的概率  
				w0 += histogram_norm[j];        
				u0tmp += j * histogram_norm[j];  
			}  
			else       //前景部分  
			{  
				//以i为阈值分类，第二类总的概率  
				w1 += histogram_norm[j];        
				u1tmp += j * histogram_norm[j];  
			}  
		}  

		u0 = u0tmp / w0;        //第一类的平均灰度  
		u1 = u1tmp / w1;        //第二类的平均灰度  
		u = u0tmp + u1tmp;      //整幅图像的平均灰度  
		//计算类间方差  
		deltaTmp = w0 * (u0 - u)*(u0 - u) + w1 * (u1 - u)*(u1 - u);  
		//找出最大类间方差以及对应的阈值  
		if(deltaTmp > deltaMax)  
		{     
			deltaMax = deltaTmp;   
			threshold = i;  
		}  
	}  
	//返回最佳阈值;  

	//std::sort(histogram, histogram+256, comp);
}  


int getCircleCenterByThreePoint(Point p1, Point p2, Point p3, Point *circle_center)
{
	int nRet = -1;
	if (circle_center == NULL)
	{
		return nRet;
	}

	if(p1.x == p2.x)
		p2.x++;
	if(p1.x == p3.x)
		p3.x++;

	double u=(p1.x*p1.x-p2.x*p2.x+ p1.y*p1.y-p2.y*p2.y) / (2*p1.x-2*p2.x);
	double v=(p1.x*p1.x-p3.x*p3.x+ p1.y*p1.y-p3.y*p3.y) / (2*p1.x-2*p3.x);
	double k1=(p1.y-p2.y)*1.0 / (p1.x-p2.x);
	double k2=(p1.y-p3.y)*1.0 / (p1.x-p3.x);
	if (fabs(k1 - k2) < EPSINON)
	{
		return nRet;
	}
	circle_center->y = (u-v) / (k1-k2);
	circle_center->x = v-(u-v)*k2 / (k1-k2);
	nRet = 1;
	return nRet;
}

//计算矩不变特征
int anlysisImprintFeature(IplImage *src, CvRect rectROI, int index, int &result)
{
	int Img_W = src->width;  
	int Img_H = src->height;
	int i=0 ,j=0;  
	unsigned char temp = 0;  
	double m_00 =0 ,m_01 =0 ,m_10 =0;  
	double X_c =0 ,Y_c =0 ,Center_x =0 ,Center_y =0;  
	double H_00=0,H_02=0,H_03=0,H_11=0,H_12=0,H_20=0,H_21=0,H_30=0;  
	float eta_02=0,eta_03=0,eta_11=0,eta_12=0,eta_20=0,eta_21=0,eta_30=0;  
	uchar* data = (uchar*)src->imageData;
	//阶矩  
	for (j = 0; j < Img_H;j++)  
	{  
		for (i = 0;i < Img_W;i++)  
		{  
			temp = data[j*src->widthStep + i]/255;  

			m_00 = m_00 + (powf(i,0)*powf(j,0)*temp);  //面积
			m_01 = m_01 + (powf(i,0)*powf(j,1)*temp);  
			m_10 = m_10 + (powf(i,1)*powf(j,0)*temp);  
		}  
	}
	if (fabs(m_00)< EPSINON)
	{
		m_00 = 1;
	}
	X_c = m_10/m_00;  //质心 X
	Y_c = m_01/m_00;  //质心 Y
	//中心矩  
	int max_distance_from_center = 0;
	int long_distance_x = 0;
	int long_distance_y = 0;

	int max_distance_first_quadrant = 0;
	int max_distance_second_quadrant = 0;
	int max_distance_third_quadrant = 0;
	int max_distance_fourth_quadrant = 0;
	CvPoint point_first_quadrant;
	CvPoint point_second_quadrant;
	CvPoint point_third_quadrant;
	CvPoint point_fourth_quadrant;
	point_first_quadrant.x = INFINITE_NUM;
	point_first_quadrant.y = INFINITE_NUM;
	point_second_quadrant.x = INFINITE_NUM;
	point_second_quadrant.y = INFINITE_NUM;
	point_third_quadrant.x = INFINITE_NUM;
	point_third_quadrant.y = INFINITE_NUM;
	point_fourth_quadrant.x = INFINITE_NUM;
	point_fourth_quadrant.y = INFINITE_NUM;

	for (j = 0; j < Img_H; j++)  
	{  
		for (i = 0; i < Img_W; i++)  
		{  
			temp = data[j*src->widthStep + i]/255; 
			Center_x = i - X_c;  
			Center_y = j - Y_c;  
			H_00 = H_00 + pow(Center_x,0)*pow(Center_y,0)*temp;  
			H_02 = H_02 + pow(Center_x,0)*pow(Center_y,2)*temp;  
			H_03 = H_03 + pow(Center_x,0)*pow(Center_y,3)*temp;  
			H_11 = H_11 + pow(Center_x,1)*pow(Center_y,1)*temp;  
			H_12 = H_12 + pow(Center_x,1)*pow(Center_y,2)*temp;  
			H_20 = H_20 + pow(Center_x,2)*pow(Center_y,0)*temp;  
			H_21 = H_21 + pow(Center_x,2)*pow(Center_y,1)*temp;  
			H_30 = H_30 + pow(Center_x,3)*pow(Center_y,0)*temp; 

			if (temp > 0)
			{
				if (max_distance_from_center < pow(Center_x,2)+pow(Center_y,2))
				{
					//求距质心最远点
					max_distance_from_center = pow(Center_x,2)+pow(Center_y,2);
					long_distance_x = i;
					long_distance_y = j;
				}
				//以质心为中心，分为四个像限
				if (Center_x < 0 && Center_y < 0 && max_distance_first_quadrant < pow(Center_x,2)+pow(Center_y,2))
				{
					//求第一像限内距质心最远点
					max_distance_first_quadrant = pow(Center_x,2)+pow(Center_y,2);
					point_first_quadrant.x = i;
					point_first_quadrant.y = j;
				}
				if (Center_x > 0 && Center_y < 0 && max_distance_second_quadrant < pow(Center_x,2)+pow(Center_y,2))
				{
					//求第二像限内距质心最远点
					max_distance_second_quadrant = pow(Center_x,2)+pow(Center_y,2);
					point_second_quadrant.x = i;
					point_second_quadrant.y = j;
				}
				if (Center_x < 0 && Center_y > 0 && max_distance_third_quadrant < pow(Center_x,2)+pow(Center_y,2))
				{
					//求第三像限内距质心最远点
					max_distance_third_quadrant = pow(Center_x,2)+pow(Center_y,2);
					point_third_quadrant.x = i;
					point_third_quadrant.y = j;
				}
				if (Center_x > 0 && Center_y>0 && max_distance_fourth_quadrant < pow(Center_x,2)+pow(Center_y,2))
				{
					//求第四像限内距质心最远点
					max_distance_fourth_quadrant = pow(Center_x,2)+pow(Center_y,2);
					point_fourth_quadrant.x = i;
					point_fourth_quadrant.y = j;
				}
			}

		}  
	}
	////矩的归一化  
	//eta_02 = H_02/pow(H_00 ,1);  
	////eta_03 = H_03/pow(H_00 ,2.5);  
	//eta_11 = H_11/pow(H_00 ,1);  
	////eta_12 = H_12/pow(H_00 ,2.5);  
	//eta_20 = H_20/pow(H_00 ,1);  
	////eta_21 = H_21/pow(H_00 ,2.5);  
	////eta_30 = H_30/pow(H_00 ,2.5);

	//float m_mo_2 = pow(eta_20 - eta_02,2) + 4*pow(eta_11,2);

	//float r1 = sqrt(2*(eta_20 + eta_02 + sqrt(m_mo_2)));
	//float r2 = sqrt(2*(eta_20 + eta_02 - sqrt(m_mo_2)));
	//double radians = 0;
	//if (fabs(eta_02 - eta_20) < EPSINON)
	//{
	//	radians = 
	//}
	//radians = 0.5*atan(2*eta_11/(eta_02-eta_20));

	return 1;
	//矩不变特征  
} 

int findObjectContours(IplImage* pROI, vector<vector<Point> > &contours)
{
	if (pROI == NULL)
	{
		return -1;
	}
	//cvSaveImage("findObjectContours.png",pROI);
	cv::Mat src(pROI,0);
	vector<Vec4i> hierarchy;  
	//vector<Vec4i>hierarchy(10000);
	//vector<Mat>contours(10000);//手动分配内存空间大小
	findContours(src, contours, hierarchy, CV_RETR_CCOMP, 1, Point(0, 0)); 
	//double dConArea;     
	// CvSeq *pContour = NULL;     
	// CvSeq *pConInner = NULL;     
	//CvMemStorage *pStorage = NULL;     
	// 查找所有轮廓     
	//pStorage = cvCreateMemStorage(0);     
	//cvFindContours(pROI, pStorage, &pContour, sizeof(CvContour), CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE);     

};

void getRingROI(IplImage* pIplsrc, int shim_outer_boundary, int shim_inner_boundary, Point circle_outer_centre, Point circle_inner_centre, Mat 

&img_roi)
{
	IplImage* shim_circle_img_mask = NULL;
	shim_circle_img_mask = cvCreateImage(cvGetSize(pIplsrc), 8, 1);
	cvZero(shim_circle_img_mask);

	cvCircle(
		shim_circle_img_mask,
		cvPoint(circle_outer_centre.x,circle_outer_centre.y),
		shim_outer_boundary,
		CV_RGB(255, 255, 255),
		-1, 8, 0
		);

	cvCircle(
		shim_circle_img_mask,
		cvPoint(circle_inner_centre.x,circle_inner_centre.y),
		shim_inner_boundary,
		CV_RGB(0, 0, 0),
		-1, 8, 0
		);
	cv::Mat img_src(pIplsrc,0);
	cv::Mat img_mask(shim_circle_img_mask,0);

	img_src.copyTo(img_roi, img_mask);

	if (shim_circle_img_mask != NULL)
	{
		cvReleaseImage(&shim_circle_img_mask);
	};
}

int findShimCircle(IplImage *shim_circle_img_copy, Point &shim_big_circle_center, double &shim_big_circle_radius, Point &shim_small_circle_center, 

double &shim_small_circle_radius)
{
	vector<vector<Point> > contours; 
	findObjectContours(shim_circle_img_copy, contours);

	double max_contour_area = 0;
	vector<vector<Point> >::iterator iter;

	Rect shim_outer_rcRect;
	int i = 0;
	for(i = 0, iter = contours.begin(); iter != contours.end(); i++)
	{
		double tmparea = fabs(contourArea(contours[iter - contours.begin()]));
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);  

#ifdef _DEBUG
		cvRectangle(shim_circle_img_copy, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(255, 

128, 255), 1, 8, 0);  
#endif

		if (rcRect.width < SHIM_REGION_SIZE || (rcRect.height) < SHIM_REGION_SIZE)  
			iter = contours.erase(iter); 
		else
		{
			if(max_contour_area < tmparea)
			{
				//先找外圆圆心
				max_contour_area = tmparea;
				shim_big_circle_center.x = rcRect.x + rcRect.width/2;
				shim_big_circle_center.y = rcRect.y + rcRect.height/2;
				shim_big_circle_radius = max(rcRect.width/2,rcRect.height/2);
				shim_outer_rcRect.width = rcRect.width;
				shim_outer_rcRect.height = rcRect.height;
			}
			iter++;

		}
	}

	Point shim_circle_centre;        //圆环中心

	int max_radius;
	int circle_centre_distance = INFINITE_NUM;
	int max_contours_width = 0;

	if (contours.size()<2)
	{
		return -1;
	}
	for(iter = contours.begin(); iter != contours.end(); iter++)
	{
		double tmparea = fabs(contourArea(contours[iter - contours.begin()]));
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
		if(abs(max_contour_area - tmparea) > EPSINON)
		{
			//找内圆圆心
			int temp_x = rcRect.x + rcRect.width/2;
			int temp_y = rcRect.y + rcRect.height/2;
			int temp_distance = abs(temp_x - shim_big_circle_center.x) + abs(temp_y - shim_big_circle_center.y);
			if (temp_distance < circle_centre_distance)
			{
				circle_centre_distance = temp_distance;
				shim_small_circle_center.x = temp_x;
				shim_small_circle_center.y = temp_y;
				shim_small_circle_radius = max(rcRect.width/2,rcRect.height/2);
			}
		}else
		{
			max_contours_width = rcRect.width/2;
		}
		shim_circle_centre.x = (shim_small_circle_center.x + shim_big_circle_center.x)/2;
		shim_circle_centre.y = (shim_small_circle_center.y + shim_big_circle_center.y)/2;
	}

	if (abs(shim_outer_rcRect.height - shim_outer_rcRect.width)<5)
	{

	}else
	{
		//如果在Y偏离较大，用宽度和内圆圆心作为外圆半径和圆心
		shim_big_circle_center.x = shim_small_circle_center.x;
		shim_big_circle_center.y = shim_small_circle_center.y;
		shim_circle_centre.x =  shim_small_circle_center.x;
		shim_circle_centre.y  = shim_small_circle_center.y;
		shim_big_circle_radius = max_contours_width;
	}

	int index = 0;

	Point p1, p2, p3;
	Point estimate_circle_center;
	bool p1_find = 0, p2_find = 0, p3_find = 0;
	for(iter = contours.begin(); iter != contours.end(); iter++)
	{
		double tmparea = fabs(contourArea(contours[iter - contours.begin()]));
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形

		if(abs(max_contour_area - tmparea) > EPSINON)
		{

		}else
		{
			for (int j = 0; j < iter->size(); j++)
			{
				double delta_x = shim_big_circle_center.x - contours[index][j].x;
				double delta_y = shim_big_circle_center.y - contours[index][j].y;

				if (abs(delta_y) > 0.9*abs(delta_x) && abs(delta_y) < 1.1*abs(delta_x))
				{
					if (delta_y>0 && delta_x>0 && !p1_find)
					{
						p1.x = contours[index][j].x;
						p1.y = contours[index][j].y;
						p1_find = 1;
						cvCircle(shim_circle_img_copy, cvPoint(p1.x, p1.y), 2,CV_RGB(255, 255, 255), 1, 8, 0);
					}

					if (delta_y<0 && delta_x<0 && !p2_find)
					{
						p2.x = contours[index][j].x;
						p2.y = contours[index][j].y;
						p2_find = 1;
						cvCircle(shim_circle_img_copy, cvPoint(p2.x, p2.y),5,CV_RGB(255, 255, 255), 1, 8, 0);
					}
					if (delta_y<0 && delta_x>0 && !p3_find)
					{
						p3.x = contours[index][j].x;
						p3.y = contours[index][j].y;
						p3_find = 1;
						cvCircle(shim_circle_img_copy, cvPoint(p3.x, p3.y), 8,CV_RGB(255, 255, 255), 1, 8, 0);
					}
				}
				contours[index][j].x;
				contours[index][j].y;
			}
			index++; 
		}
	}
	if (p1_find && p2_find && p3_find)
	{
		getCircleCenterByThreePoint(p1, p2, p3, &estimate_circle_center);
#ifdef _DEBUG
		cvSaveImage( "shim_circle_3p.bmp", shim_circle_img_copy);
#endif
		//if (abs(shim_big_circle_center.x - estimate_circle_center.x)<30 || abs(shim_big_circle_center.y - estimate_circle_center.y)<30)
		{
			shim_big_circle_center.x = estimate_circle_center.x;
			shim_big_circle_center.y = estimate_circle_center.y;
			shim_big_circle_radius = sqrt(pow((p1.x - estimate_circle_center.x),2.0)+pow((p1.y - estimate_circle_center.y),2.0));
		}

	}
	return 1;
};

int getImageByROI(IplImage* src, IplImage* dst, CvRect rectROI)
{
	//区域有效，将img中的矩形区域复制给roi
	if (src == NULL || dst == NULL)
	{
		return -1;
	}
	cvSetImageROI(src,cvRect(rectROI.x,rectROI.y,rectROI.width, rectROI.height));//设置源图像ROI
	cvCopy(src, dst); //复制图像
	cvResetImageROI(src);//源图像用完后，清空ROI
}

int getImprintFromCollection(IplImage* imgClr, vector<vector<Point> > &imprint_contours, vector<CircleParam> &local_imprint_collection, double 

estimate_imprint_radius, vector<CircleParam> & imprint_collection)
{
    CircleParam imprint_circle;

	//如果符合的印痕个数大于2，根据方差最小的点为印痕的圆心。如果符合的印痕个数等于2，选择一个与预估的印痕距离最近的点作为印痕的圆
	/*	if (local_imprint_collection.size() == 2)
	{
	double distance = pow((local_imprint_collection[1].x - local_imprint_collection[0].x),2.0)+pow((local_imprint_collection[1].y - 

local_imprint_collection[0].y),2.0);
	if (distance < 400)
	{
	if (abs(local_imprint_collection[1].radius - estimate_imprint_radius)>abs(local_imprint_collection[0].radius - estimate_imprint_radius))
	{
	imprint_circle.x = local_imprint_collection[0].x;
	imprint_circle.y = local_imprint_collection[0].y;
	imprint_circle.radius = local_imprint_collection[0].radius;

	}else
	{
	imprint_circle.x = local_imprint_collection[1].x;
	imprint_circle.y = local_imprint_collection[1].y;
	imprint_circle.radius = local_imprint_collection[1].radius;
	}
	imprint_collection.push_back(imprint_circle);
	}
	}else */
	//if (local_imprint_collection.size() > 0)
	//{
	//	double min_distance = INFINITE_NUM;
	//	for (int i = 0; i < local_imprint_collection.size(); i++)
	//	{
	//		double temp_distance = 0;
	//		for (int j = 0; j < local_imprint_collection.size(); j++)
	//		{
	//			temp_distance+= pow((local_imprint_collection[j].x - local_imprint_collection[i].x),2.0)+pow((local_imprint_collection[j].y - local_imprint_collection[i].y),2.0);
	//		}
	//		if (min_distance > temp_distance)
	//		{
	//			min_distance = temp_distance;
	//			imprint_circle.x = local_imprint_collection[i].x;
	//			imprint_circle.y = local_imprint_collection[i].y;
	//			imprint_circle.radius = local_imprint_collection[i].radius;
	//		}
	//	}
	//	imprint_collection.push_back(imprint_circle);
	//	//cvCircle(imgClr, cvPoint(imprint_circle.x, imprint_circle.y), 8,CV_RGB(155, 255, 210), 1, 8, 0);
	//}
	vector<vector<Point> > imprint_contours_copy;
	for(vector<vector<Point> >::iterator iter = imprint_contours.begin(); iter != imprint_contours.end();iter++)
	{
		imprint_contours_copy.push_back(imprint_contours[iter - imprint_contours.begin()]);
	}
	//去除轮廓内部的轮廓
	for(vector<vector<Point> >::iterator iter = imprint_contours_copy.begin(); iter != imprint_contours_copy.end();)
	{
		Point p1, p2, p3;
		Rect rcRect = boundingRect(imprint_contours_copy[iter - imprint_contours_copy.begin()]);			//圆的外接矩形
		vector<CircleParam> imprint_center_collection;
		bool in_side = 0;
		for(vector<vector<Point> >::iterator iter2 = imprint_contours_copy.begin(); iter2 != imprint_contours_copy.end();iter2++ )
		{

			Rect rcRect2 = boundingRect(imprint_contours_copy[iter2 - imprint_contours_copy.begin()]);			//圆的外接矩形
			if (rcRect2.x<rcRect.x&&rcRect2.y<rcRect.y&&rcRect2.x+rcRect2.width>rcRect.x+rcRect.width/2&&rcRect2.y +rcRect2.height>rcRect.y+rcRect.height/2)
			{
				//iter2++;
				in_side = 1;
				break;
			}
		}
		if (in_side)
		{
			iter=imprint_contours_copy.erase(iter);
		}else
		{
			iter++;
		}
	}

	//统计在印痕上的点
	vector<vector<Point> >::iterator iter;

	for (int i = 0; i < local_imprint_collection.size(); i++)
	{	
		int total_size = 0;
		int imprint_size = 0;
		int non_imprint_size = 0;
		Point center_point;
		center_point.x = local_imprint_collection[i].x;
		center_point.y = local_imprint_collection[i].y;
		int m=0;

		for(iter = imprint_contours_copy.begin(); iter != imprint_contours_copy.end(); iter++)
		{
			m++;
			if (m==6)
			{
				int stop = 0;
			}
			Point p1, p2, p3;
			Rect rcRect = boundingRect(imprint_contours_copy[iter - imprint_contours_copy.begin()]);			//圆的外接矩形
			cvRectangle(imgClr, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(255, 255, 255), 1, 8, 0);
			int total_points = iter->size();
			int near_imprint_pixels = 0;
			int far_imprint_pixels = 0;
			int total_pixels = 0;
			vector<CircleParam> imprint_center_collection;
			for (int j = 0; j < total_points; j += 3)
			{
				double temp_distance = sqrt(pow((imprint_contours_copy[iter - imprint_contours_copy.begin()][j].x -  center_point.x),2.0)+pow((imprint_contours_copy[iter - imprint_contours_copy.begin()][j].y - center_point.y),2.0));

				if (abs(temp_distance - local_imprint_collection[i].radius) < 15)
				{
					near_imprint_pixels++;
				}
				if (abs(temp_distance - local_imprint_collection[i].radius) < 100 && abs(temp_distance - local_imprint_collection[i].radius) > 30)
				{
					far_imprint_pixels++;
				}
				total_pixels++;
			}
			if (near_imprint_pixels>2 && near_imprint_pixels>0.9*total_pixels)
			{
				imprint_size += max(rcRect.width , rcRect.height);
			}
			if (far_imprint_pixels>0.1*total_pixels)
			{
				non_imprint_size += max(rcRect.width , rcRect.height);
			}
		}	
		if (imprint_size > 50)
		{
			if (0.5*imprint_size > non_imprint_size)
			{
				imprint_collection.push_back(local_imprint_collection[i]);
			}

		}
	}
	return 1;
}

int getImprintCollection(IplImage* imgClr, vector<vector<Point> > &imprint_contours, double shim_big_circle_radius, Point shim_big_circle_center, 

vector<CircleParam> &imprint_collection)
{
	vector<vector<Point> >::iterator iter;
	double estimate_imprint_radius = shim_big_circle_radius*0.16;
	imprint_collection.clear();
	for(iter = imprint_contours.begin(); iter != imprint_contours.end(); iter++)
	{
		Point p1, p2, p3;
		Point pre_imprint_circle_center;
		Point imprint_circle_center;
		imprint_circle_center.x = 0;
		imprint_circle_center.y = 0;
		Rect rcRect = boundingRect(imprint_contours[iter - imprint_contours.begin()]);
		
		int total_points = iter->size();
		vector<CircleParam> local_imprint_collection;
		if (rcRect.width > 30 || rcRect.height > 30)
		{
			for (int i = 0; (i+20) < total_points; i+=3)
			{

				p1.x = imprint_contours[iter - imprint_contours.begin()][i].x;
				p1.y = imprint_contours[iter - imprint_contours.begin()][i].y;

				p2.x = imprint_contours[iter - imprint_contours.begin()][i+10].x;
				p2.y = imprint_contours[iter - imprint_contours.begin()][i+10].y;

				p3.x = imprint_contours[iter - imprint_contours.begin()][i+20].x;
				p3.y = imprint_contours[iter - imprint_contours.begin()][i+20].y;

				pre_imprint_circle_center.x = imprint_circle_center.x;
				pre_imprint_circle_center.y = imprint_circle_center.y;
				getCircleCenterByThreePoint(p1, p2, p3, &imprint_circle_center);
#ifdef _DEBUG
				cvCircle(imgClr, p1, 2, CV_RGB(255, 122, 201), 1, 8, 0);
				cvCircle(imgClr, p2, 2, CV_RGB(255, 122, 201), 1, 8, 0);
				cvCircle(imgClr, p3, 2, CV_RGB(255, 122, 201), 1, 8, 0);
				cvCircle(imgClr, imprint_circle_center, 2, CV_RGB(255, 122, 201), 1, 8, 0);
#endif

				double shim_imprint_radius = sqrt(pow((p1.x - imprint_circle_center.x),2.0)+pow((p1.y - imprint_circle_center.y),2.0));
				double imprint_circle_center_distance = sqrt(pow((shim_big_circle_center.x - imprint_circle_center.x),2.0)+pow((shim_big_circle_center.y - imprint_circle_center.y),2.0));
				double imprint_delta_distance = sqrt(pow((pre_imprint_circle_center.x - imprint_circle_center.x),2.0)+pow((pre_imprint_circle_center.y - imprint_circle_center.y),2.0));

				if (abs(shim_imprint_radius - estimate_imprint_radius) < 20 && abs(imprint_circle_center_distance - 0.8*shim_big_circle_radius) < 20)
				{
					//if (imprint_delta_distance < 5)
					//如果印痕半径和印痕圆心距离垫片圆心的距离符合，记录印痕
					CircleParam temp_imprint_circle;
					temp_imprint_circle.radius = shim_imprint_radius;
					temp_imprint_circle.x = imprint_circle_center.x;
					temp_imprint_circle.y = imprint_circle_center.y;
					local_imprint_collection.push_back(temp_imprint_circle);

#ifdef _DEBUG
					cvCircle(imgClr, imprint_circle_center, 8, CV_RGB(255, 122, 201), 1, 8, 0);
#endif
				}
			}
		}
		if (local_imprint_collection.size()>0)
		{
			getImprintFromCollection(imgClr, imprint_contours, local_imprint_collection, estimate_imprint_radius, imprint_collection);
		}
	}
	return 1;
}
/** 
** method to remove sharp the raw image with unsharp mask 
* @param gray input grayscale binary array  
* @param smooth output data for smooth result, the memory need to be allocated outside of the function 
* @param width width of the input grayscale image 
* @param height height of the input grayscale image 
*/  
/*void sharpenImage  (unsigned char* gray, unsigned char* smooth, int width, int height)  
{  

int templates[25] = { -1, -4, -7, -4, -1,   
-4, -16, -26, -16, -4,   
-7, -26, 505, -26, -7,  
-4, -16, -26, -16, -4,   
-1, -4, -7, -4, -1 };         
memcpy ( smooth, gray, width*height*sizeof(unsigned char) );  
for (int j=2;j<height-2;j++)  
{  
for (int i=2;i<width-2;i++)  
{  
int sum = 0;  
int index = 0;  
for ( int m=j-2; m<j+3; m++)  
{  
for (int n=i-2; n<i+3; n++)  
{  
sum += gray [ m*width + n] * templates[index++] ;  
}  
}  
sum /= 273;  
if (sum > 255)  
sum = 255;  
if (sum <0)  
sum = 0;  
smooth [ j*width+i ] = sum;  
}  
}  
}*/  
int shimDetect(IplImage* pIplsrc, CvRect rectROI, params& param)
{
	//输出到日志测试
	char file_path[FILE_PATH_LENTH];
	FILE *fp = NULL; 
	fp = fopen("log_shim_detect.txt","a+");
	if (fp != NULL)
	{
		fprintf(fp,"start shimDetect\n");
	}

	if (rectROI.width>0 && rectROI.height>0 && rectROI.width < pIplsrc->width && rectROI.height < pIplsrc->height)
	{

	}else
	{
		rectROI.x = 0;
		rectROI.y = 0;
		rectROI.width = pIplsrc->width;
		rectROI.height = pIplsrc->height;
	}

	IplImage* pShimROI = NULL;
	IplImage* shim_circle_img = NULL;
	IplImage* shim_circle_img_copy = NULL;
	shim_circle_img = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	shim_circle_img_copy = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	pShimROI = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);//创建目标图像
	param.var1[0] = 0;

	getImageByROI(pIplsrc, pShimROI, rectROI);

	//定义一个3通道图象
#ifdef _DEBUG
	IplImage* imgClr = cvCreateImage(cvSize(pShimROI->width, pShimROI->height), IPL_DEPTH_8U, 3);
	cvCvtColor(pShimROI, imgClr, CV_GRAY2BGR);
#endif

	cvSmooth(pShimROI, pShimROI,CV_BLUR,3,3,0,0);         //邻域平均滤波
	//cvSmooth(pShimROI, pShimROI,CV_BLUR,5,5,0,0);         //邻域平均滤波
	cvCopy(pShimROI, shim_circle_img_copy, NULL);
	//cvThreshold(shim_circle_img_copy, shim_circle_img_copy, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvThreshold(shim_circle_img_copy, shim_circle_img_copy, 50, 255, CV_THRESH_BINARY/* | CV_THRESH_OTSU*/);
	//cvCanny( shim_circle_img_copy, shim_circle_img_copy, 50, 150, 3 );//边缘检测 

	//#ifdef _DEBUG
	//	sprintf(file_path, "Binary_src.bmp");
	//	cvSaveImage(file_path, shim_circle_img_copy, 0);
	//#endif
	Point shim_big_circle_center;
	double shim_big_circle_radius;
	Point shim_small_circle_center;
	double shim_small_circle_radius;
	int circle_exist = -1;
	circle_exist = findShimCircle(shim_circle_img_copy, shim_big_circle_center, shim_big_circle_radius, shim_small_circle_center, shim_small_circle_radius);

	if (circle_exist < 0)
	{
		return -1;
		if (fp != NULL)
		{
			fprintf(fp, "find shim circle fail");
		}
	}

	//#ifdef _DEBUG
	//	sprintf(file_path, "Binary_src.bmp");
	//	cvSaveImage(file_path, shim_circle_img_copy, 0);
	//#endif


	int shim_outer_boundary = shim_big_circle_radius;
	int shim_inner_boundary = shim_big_circle_radius * SHIM_INNER_SHRINK_RATIO;

#ifdef _DEBUG
	if (fp != NULL)
	{
		fprintf(fp, " shim_outer_boundary: %d, shim_inner_boundary: %d\n",  shim_outer_boundary, shim_inner_boundary);
	}
	//测试代码
	cvCircle(imgClr, shim_big_circle_center, 8,CV_RGB(255, 128, 255), 1, 8, 0); 
	cvCircle(imgClr, shim_small_circle_center, 3,CV_RGB(255, 128, 255), 1, 8, 0);
	cvCircle(imgClr, shim_big_circle_center, shim_big_circle_radius, cvScalar(255,162,199,0.5), 1, 8, 0);
	cvCircle(imgClr, shim_small_circle_center, shim_small_circle_radius, cvScalar(255,162,199,0.5), 1, 8, 0);
	cvCircle(imgClr, shim_big_circle_center, 0.8*shim_big_circle_radius, cvScalar(255,162,199,0.5), 1, 8, 0);
#endif
	//根据圆形掩码，获取圆环的圆形区域

	Rect shim_rect(shim_big_circle_center.x - shim_outer_boundary, shim_big_circle_center.y - shim_outer_boundary, 2*shim_outer_boundary, 2*shim_outer_boundary);
	if (shim_rect.x + shim_rect.width >= pIplsrc->width ||
		shim_rect.y + shim_rect.height >= pIplsrc->height ||
		shim_rect.x < 0 ||
		shim_rect.y < 0)
	{
		return -1;
	}


	vector<vector<Point> >::iterator iter;

	int index=0;
	int pos = 0;
	double estimate_imprint_radius = shim_big_circle_radius*0.16;

	//通过canny提取印痕边缘
	//	pos = 130;
	//	while (1)
	//	{
	//		Mat img_roi;
	//		Mat mat_img_shim;
	//		getRingROI(pShimROI, shim_outer_boundary, shim_inner_boundary, shim_big_circle_center, shim_big_circle_center, img_roi);
	//
	//		//shim_circle_img = &IplImage(img_roi);
	//		cvCopy(&IplImage(img_roi), shim_circle_img, NULL);
	//		//#ifdef _DEBUG
	//		//		sprintf_s(file_path, "shim_circle_src_%d.bmp", index);
	//		//		cvSaveImage(file_path, shim_circle_img, 0);
	//		//#endif
	//		cvCanny( shim_circle_img, shim_circle_img, 50, pos, 3 );//边缘检测
	
	//
	//
	//		getRingROI(shim_circle_img, shim_outer_boundary - 20, shim_inner_boundary + 20, shim_big_circle_center, shim_big_circle_center, mat_img_shim);
	//		//shim_circle_img = &IplImage(mat_img_shim);
	//		cvCopy(&IplImage(mat_img_shim), shim_circle_img, NULL);
	//
	//		//#ifdef _DEBUG
	//		//		sprintf_s(file_path, "shim_circle_img_%d.bmp", index);
	//		//		cvSaveImage(file_path, shim_circle_img, 0);
	//		//#endif
	//
	//		cvDilate( shim_circle_img,  shim_circle_img, NULL, 3); //膨胀 
	//		cvErode(shim_circle_img,shim_circle_img, NULL, 3);//腐蚀
	//		//deleteNoiseRegion( shim_circle_img, shim_circle_img_copy, 40);
	//
	//		cvCopy(shim_circle_img, shim_circle_img_copy, NULL);
	//
	//		//印痕区域提取
	//		imprint_contours.clear();
	//		findObjectContours(shim_circle_img_copy, imprint_contours);
	//
	//		if (imprint_contours.size() < 10 || pos > 220)
	//		{
	//			break;
	//		}else
	//		{
	//			pos += 20;
	//		}
	//		index++;
	//	}
	//#ifdef _DEBUG
	//	cvSaveImage("shim_circle_img_by_canny.jpg",  shim_circle_img, 0);
	//#endif
	//	vector<CIRCLE_PARAM> imprint_collection_by_canny;
	//	getImprintCollection( imgClr, imprint_contours, shim_big_circle_radius, shim_big_circle_center, imprint_collection_by_canny);

	cv::Mat matShimROI(pShimROI,0);

	//形态学梯度（MORPH_GRADIENT）
	cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(10, 10));
	cv::morphologyEx(matShimROI, matShimROI, cv::MORPH_TOPHAT, element);
	/*#ifdef _DEBUG
	cvSaveImage("shim_circle_img_src.jpg",  pShimROI, 0);
	#endif
	sharpenImage  ((unsigned char *)pShimROI->imageData, (unsigned char *)shim_circle_img_copy->imageData, pShimROI->width, pShimROI->height);
	#ifdef _DEBUG
	cvSaveImage("shim_circle_img_by_sharpen.jpg",  shim_circle_img_copy, 0);
	#endif*/	
	//通过二值化提取印痕边缘
	pos = 30;
	while (1)
	{
		Mat img_roi;
		Mat mat_img_shim;
		getRingROI(pShimROI, shim_outer_boundary, shim_inner_boundary, shim_big_circle_center, shim_big_circle_center, img_roi);

		//shim_circle_img = &IplImage(img_roi);
		cvCopy(&IplImage(img_roi), shim_circle_img, NULL);

		cvThreshold(shim_circle_img, shim_circle_img, pos, 255, CV_THRESH_BINARY); 

		getRingROI(shim_circle_img, shim_outer_boundary - 20, shim_inner_boundary, shim_big_circle_center, shim_big_circle_center, mat_img_shim);
		//shim_circle_img = &IplImage(mat_img_shim);

		cv::Mat element = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
		cv::morphologyEx(mat_img_shim, mat_img_shim, cv::MORPH_CLOSE, element);

		cvCopy(&IplImage(mat_img_shim), shim_circle_img, NULL);
		cvCopy(shim_circle_img, shim_circle_img_copy, NULL);

		//印痕区域提取
		/*imprint_contours.clear();*/
		vector<vector<Point> > imprint_contours;
		findObjectContours(shim_circle_img_copy, imprint_contours);
		bool stop_iteration = 0;

		if (imprint_contours.size()<50)
		{
			for(iter = imprint_contours.begin(); iter != imprint_contours.end(); iter++)
			{
				Point p1, p2, p3;
				Rect rcRect = boundingRect(imprint_contours[iter - imprint_contours.begin()]);
				if ((rcRect.width > estimate_imprint_radius*2 || rcRect.height > estimate_imprint_radius*2))
				{
					stop_iteration = 0;
					break;
				}else
				{
					stop_iteration = 1;
				}
			}
		}

		if (stop_iteration || pos>255)
		{
			break;
		}else
		{
			pos += 10;
		}

		index++;
	}
#ifdef _DEBUG
	cvSaveImage("shim_circle_img_by_binary.jpg",  shim_circle_img, 0);
#endif
	vector<vector<Point> > imprint_contours;
	cvCopy(shim_circle_img, shim_circle_img_copy, NULL);
	findObjectContours(shim_circle_img_copy, imprint_contours);

	vector<CircleParam> imprint_collection_by_binary;
	getImprintCollection( imgClr, imprint_contours, shim_big_circle_radius, shim_big_circle_center, imprint_collection_by_binary);

	Point imprint_circle_center;

	//for (int i = 0; i < imprint_collection_by_canny.size(); i++)
	//{
	//	imprint_circle_center.x = imprint_collection_by_canny[i].x;
	//	imprint_circle_center.y = imprint_collection_by_canny[i].y;
	//	#ifdef _DEBUG
	//	cvCircle(imgClr, imprint_circle_center, 8, CV_RGB(0, 0, 0), 1, 8, 0);
	//	#endif
	//}

	for (int i = 0; i < imprint_collection_by_binary.size(); i++)
	{

		imprint_circle_center.x = imprint_collection_by_binary[i].x;
		imprint_circle_center.y = imprint_collection_by_binary[i].y;

#ifdef _DEBUG
		cvCircle(imgClr, imprint_circle_center, 8, CV_RGB(255, 255, 255), 1, 8, 0);
#endif
	}
	if (/*imprint_collection_by_canny.size()>0 || */imprint_collection_by_binary.size()>0)
	{
		param.var1[0] = 1;
	}
	//根据印痕圆心和半径，遍历统计在印痕边界的区域，并进行累加
	//for (int i = 0; i < imprint_circle_collection.size(); i++)
	//{	
	//	int total_size = 0;
	//	int imprint_size = 0;
	//	Point center_point;
	//	center_point.x = imprint_circle_collection[i].x;
	//	center_point.y = imprint_circle_collection[i].y;
	//	for(iter = imprint_contours.begin(); iter != imprint_contours.end(); iter++)
	//	{
	//		Point p1, p2, p3;
	//		Rect rcRect = boundingRect(imprint_contours[iter - imprint_contours.begin()]);			//圆的外接矩形
	//		//cvRectangle(shim_circle_img, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(255, 255, 255), 1, 8, 0);
	//		int total_points = iter->size();
	//		int on_times = 0;
	//		int total_times = 0;
	//		vector<CIRCLE_PARAM> imprint_center_collection;
	//		for (int j = 0; j < total_points; j += 5)
	//		{
	//			double temp_distance = sqrt(pow((imprint_contours[iter - imprint_contours.begin()][j].x - center_point.x),2.0)+pow((imprint_contours[iter - imprint_contours.begin()][j].y - center_point.y),2.0));

	//			if (abs(temp_distance - imprint_circle_collection[i].radius)<20)
	//			{
	//				on_times++;
	//			}
	//			if (abs(temp_distance - imprint_circle_collection[i].radius)<shim_inner_boundary)
	//			{
	//				total_times++;
	//			}
	//			
	//		}
	//		if (on_times>0.8*total_times)
	//		{
	//			imprint_size += max(rcRect.width , rcRect.height);
	//		}
	//		if (total_times>0)
	//		{
	//			total_size += max(rcRect.width , rcRect.height);
	//		}
	//	}
	//	if (/*imprint_size>0.8*total_size && */imprint_size>50)
	//	{
	//		param.var1[0] = 1;
	//	}/*else if (imprint_size > 0.8*total_size && imprint_size>30)
	//	{
	//		param.var1[0] = 1;
	//	}*/
	//}


	//#ifdef _DEBUG
	//cvSaveImage("img_dilate.jpg",  shim_circle_img, 0);
	//#endif

#ifdef _DEBUG
	if (imgClr != NULL)
	{
		cvSaveImage("color.jpg",imgClr,0);
		cvReleaseImage(&imgClr);
	}	
#endif
	if (pShimROI!=NULL)
	{
		cvReleaseImage(&pShimROI);
	}
	if (shim_circle_img!=NULL)
	{
		cvReleaseImage(&shim_circle_img);
	}
	if (shim_circle_img_copy!=NULL)
	{
		cvReleaseImage(&shim_circle_img_copy);
	}
	if (fp != NULL)
	{
		fclose(fp);
		fp = NULL;
	}
	return 1;
}

int countPixels(IplImage *src, Rect ringRect, int threshold, int &num)  
{  
	int start_x = ringRect.x;
	int start_y = ringRect.y;

	int end_x = ringRect.x + ringRect.width;
	int end_y = ringRect.y + ringRect.height;

	int i, j;  
	uchar* data = (uchar*)src->imageData; 
	if (src == NULL)
	{
		return -1;
	}
	if (start_x<0 || start_y<0 || end_x>src->width || end_y>src->height)
	{
		return -1;
	}
	//统计灰度级中每个像素在整幅图像中的个数  
	num = 0;
	for(j = start_y; j < end_y; j++)  
	{  
		for(i = start_x;i < end_x; i++)  
		{  
			//不统计像素为零的点
			if ( data[j * src->widthStep + i] > threshold)
			{
				num++;
			}
		}  
	}  

}

int ringDetect(IplImage* pIplsrc, CvRect rectROI, params& param)
{
	if (rectROI.width>0 && rectROI.height>0 && rectROI.width < pIplsrc->width && rectROI.height < pIplsrc->height)
	{

	}else
	{
		rectROI.x = 0;
		rectROI.y = 0;
		rectROI.width = pIplsrc->width;
		rectROI.height = pIplsrc->height;
	}
	IplImage* pROI = NULL;
	pROI = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	IplImage* pROICopy = NULL;
	pROICopy = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	getImageByROI(pIplsrc, pROI, rectROI);

	vector<vector<Point> >::iterator iter;
	vector<vector<Point> > contours;
	param.var1[0] = -1;

	cvThreshold(pROI, pROICopy, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	//#ifdef _DEBUG
	//	cvSaveImage("pROICopy.jpg",  pROICopy, 0);
	//#endif

	findObjectContours(pROICopy, contours);

	int max_radius = 0;
	Point ring_center;
	Rect  ringRect;
	for(vector<vector<Point> >::iterator iter = contours.begin(); iter != contours.end(); iter++)
	{
		Point p1, p2, p3;
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
		if (max(rcRect.width, rcRect.height)>max_radius)
		{
			max_radius = max(rcRect.width, rcRect.height)/2;
			ring_center.x = rcRect.x + rcRect.width/2;
			ring_center.y = rcRect.y + rcRect.height/2;
			ringRect.x = rcRect.x;
			ringRect.y = rcRect.y;
			ringRect.width = rcRect.width;
			ringRect.height = rcRect.height;
		}
	}
	int num = 0;
	countPixels(pROI, ringRect, 200, num);
	if (num > 3000)
	{
		param.var1[0] = 1;
	}else
	{
		param.var1[0] = 0;
	}

	//#ifdef _DEBUG
	//	cvSaveImage("pROI.jpg",  pROI, 0);
	//#endif

	if (pROI!=NULL)
	{
		cvReleaseImage(&pROI);
	}
	if (pROICopy!=NULL)
	{
		cvReleaseImage(&pROICopy);
	}
	return 1;
}
int findCircleByRadius(IplImage* pIplsrc, int radius_threshold,int radius_delta, CvPoint center_point, CircleParam& one_circle)
{
	int nRet = -1;
	vector<vector<Point> >::iterator iter;
	vector<vector<Point> > contours;
	findObjectContours(pIplsrc, contours);

	int pos_delta = 99999;
	for(vector<vector<Point> >::iterator iter = contours.begin(); iter != contours.end(); iter++)
	{
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
		if ((abs(rcRect.width - radius_threshold) < radius_delta) && (abs(rcRect.height - radius_threshold) < radius_delta))
		{
			int temp_distance = getLineLen(cvPoint(rcRect.x + rcRect.width/2, rcRect.y + rcRect.height/2), center_point);
			if (temp_distance < pos_delta)
			{
				one_circle.radius = max(rcRect.width, rcRect.height)/2;
				one_circle.x = rcRect.x + rcRect.width/2;
				one_circle.y = rcRect.y + rcRect.height/2;
				nRet = 1;
				pos_delta = temp_distance;
			}
		}else
		{
		
		}
	}
	return nRet;
}

int findCircleByRadius2(IplImage* pIplsrc, int radius_threshold,int radius_delta, CircleParam dark_circle[], int& circle_num)
{
	int nRet = -1;
	vector<vector<Point> >::iterator iter;
	vector<vector<Point> > contours;
	findObjectContours(pIplsrc, contours);
	
	for(vector<vector<Point> >::iterator iter = contours.begin(); iter != contours.end(); iter++)
	{
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
		if ((abs(rcRect.width - radius_threshold) < radius_delta) && (abs(rcRect.height - radius_threshold) < radius_delta))
		{
			//int temp_distance = getLineLen(cvPoint(dark_circle[circle_num].x, dark_circle[circle_num].y),center_point);
			if (circle_num < 100)
			{
				dark_circle[circle_num].radius = max(rcRect.width, rcRect.height)/2;
				dark_circle[circle_num].x = rcRect.x + rcRect.width/2;
				dark_circle[circle_num].y = rcRect.y + rcRect.height/2;
				
				nRet = 1;
				++circle_num;
				//pos_delta = temp_distance;
			}else
			{
				//存储最多100个圆
			}
		}else
		{
			//cvRectangle(pIplsrc, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(125, 125, 125), 1, 8, 0);
		}
		
	}
	return nRet;
}



int piplineDetect(IplImage* pIplsrc, CvRect pipline_roi, int& black_pixel_num)
{
	//先找有料无料标记点（上面一个，左面一个）
	int nRet = -1;
	int num;
	countPixels(pIplsrc, pipline_roi, 130, num);
	if (num < 1)
	{
		return nRet;
	}
	black_pixel_num = pipline_roi.width*pipline_roi.height - num;
	if (black_pixel_num > 17000)
	{
		nRet = 1;
	}

	return nRet;
}
int discFlagDetect(IplImage* pIplsrc, CvRect disc_flag_roi, CvRect &rect_result)
{
	//先找有料无料标记点（上面一个，左面一个）
	int nRet = -1;
	IplImage* disc_flag_img;
	IplImage* disc_flag_img_copy;

	disc_flag_img = cvCreateImage(cvSize(disc_flag_roi.width, disc_flag_roi.height), pIplsrc->depth, pIplsrc->nChannels);
	disc_flag_img_copy = cvCreateImage(cvSize(disc_flag_roi.width, disc_flag_roi.height), pIplsrc->depth, pIplsrc->nChannels);
	getImageByROI(pIplsrc, disc_flag_img, disc_flag_roi);

	for (int i = 60; i < 160; i += 10)
	{
		cvCopy(disc_flag_img, disc_flag_img_copy, NULL);
		cvThreshold(disc_flag_img_copy, disc_flag_img_copy, i, 255, CV_THRESH_BINARY);

		//有些图断断续续会产生很多细小区域
		cvDilate( disc_flag_img_copy,  disc_flag_img_copy, NULL, 1); //膨胀 
		cvErode(disc_flag_img_copy, disc_flag_img_copy, NULL, 1);//腐蚀
		
		vector<vector<Point> >::iterator iter;
		vector<vector<Point> > contours;
		findObjectContours(disc_flag_img_copy, contours);
		int disc_flag_size = 70;
		for(vector<vector<Point> >::iterator iter = contours.begin(); iter != contours.end(); iter++)
		{
			Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
			if (abs(rcRect.width - disc_flag_size) < 20 && abs(rcRect.height - disc_flag_size) < 20)
			{
				rect_result.x = rcRect.x;
				rect_result.y = rcRect.y;
				rect_result.width = rcRect.width;
				rect_result.height = rcRect.height;
				nRet = 1;
				break;
			}
		}
		if (nRet == 1)
		{
			break;
		}
	}
	

	if (disc_flag_img != NULL)
	{
		cvReleaseImage(&disc_flag_img);
	}
	if (disc_flag_img_copy != NULL)
	{
		cvReleaseImage(&disc_flag_img_copy);
	}

	return nRet;
}

int rivoetDirectionDetect(IplImage* pIplsrc, CvRect rectROI, params& param)
{
	int nRet = -1;
	if (rectROI.width>0 && rectROI.height>0 && rectROI.width < pIplsrc->width && rectROI.height < pIplsrc->height)
	{

	}else
	{
		rectROI.x = 0;
		rectROI.y = 0;
		rectROI.width = pIplsrc->width;
		rectROI.height = pIplsrc->height;
	}

	CvPoint rectROI_Center = cvPoint(rectROI.x+rectROI.width/2, rectROI.y+rectROI.height/2);
	IplImage* pROI = NULL;
	pROI = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	IplImage* pROICopy = NULL;
	pROICopy = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	getImageByROI(pIplsrc, pROI, rectROI);


	//cvThreshold(pROI, pROI, 0, 255, CV_THRESH_BINARY | CV_THRESH_OTSU);
	cvThreshold(pROI, pROI, 100, 255, CV_THRESH_BINARY);
	cvCopy(pROI, pROICopy, NULL);
	CvRect rect_result;

	//CvRect disc_up_flag_roi;
	//disc_up_flag_roi.x = 517;
	//disc_up_flag_roi.y = 50;
	//disc_up_flag_roi.width = 272;
	//disc_up_flag_roi.height = 189;
	//CvRect disc_left_flag_roi;
	//disc_left_flag_roi.x = 316;
	//disc_left_flag_roi.y = 408;
	//disc_left_flag_roi.width = 181;
	//disc_left_flag_roi.height = 231;

	CvRect disc_up_flag_roi;
	disc_up_flag_roi.x = 581;
	disc_up_flag_roi.y = 72;
	disc_up_flag_roi.width = 135;
	disc_up_flag_roi.height = 129;
	CvRect disc_left_flag_roi;
	disc_left_flag_roi.x = 372;
	disc_left_flag_roi.y = 481;
	disc_left_flag_roi.width = 94;
	disc_left_flag_roi.height = 117;


	int disc_up_flag_result = discFlagDetect(pIplsrc, disc_up_flag_roi, rect_result);
	if (disc_up_flag_result>0)
	{
		cvRectangle(pIplsrc, cvPoint(disc_up_flag_roi.x + rect_result.x, disc_up_flag_roi.y + rect_result.y), 
			cvPoint(disc_up_flag_roi.x + rect_result.x+rect_result.width, disc_up_flag_roi.y + rect_result.y+rect_result.height),
			CV_RGB(125, 125, 125), 1, 8, 0);
	}

	int disc_left_flag_result = discFlagDetect(pIplsrc, disc_left_flag_roi, rect_result);
	if (disc_left_flag_result>0)
	{
		cvRectangle(pIplsrc, cvPoint(disc_left_flag_roi.x + rect_result.x, disc_left_flag_roi.y + rect_result.y), 
			cvPoint(disc_left_flag_roi.x + rect_result.x+rect_result.width, disc_left_flag_roi.y + rect_result.y+rect_result.height),
			CV_RGB(125, 125, 125), 1, 8, 0);
	}
	
	CvRect right_pipline_roi;
	right_pipline_roi.x = 880;
	right_pipline_roi.y = 150;
	right_pipline_roi.width = 260;
	right_pipline_roi.height = 210;

	CvRect down_pipline_roi;
	down_pipline_roi.x = 595;
	down_pipline_roi.y = 690;
	down_pipline_roi.width = 210;
	down_pipline_roi.height = 260;

	int black_pixel_num_right = 0;
	int black_pixel_num_down = 0;
	int right_pipline_result = piplineDetect(pIplsrc, right_pipline_roi, black_pixel_num_right);

	int down_pipline_result = piplineDetect(pIplsrc, down_pipline_roi, black_pixel_num_down);


	//if (disc_left_flag_result < 1 && disc_up_flag_result < 1)
	{
		//需要满足管子区域黑色像素大于两倍的非管子区域，检测的管子区域才有效
		if (black_pixel_num_right > 2*black_pixel_num_down)
		{
			if (right_pipline_result > 0)
			{
				cvRectangle(pIplsrc, cvPoint(right_pipline_roi.x, right_pipline_roi.y), 
					cvPoint(right_pipline_roi.x+right_pipline_roi.width, right_pipline_roi.y + right_pipline_roi.height),
					CV_RGB(125, 125, 125), 1, 8, 0);
			}else
			{
				right_pipline_result = 0;
			}
			

		}else if (black_pixel_num_down > 2*black_pixel_num_right)
		{
			if (down_pipline_result > 0)
			{
				cvRectangle(pIplsrc, cvPoint(down_pipline_roi.x, down_pipline_roi.y), 
					cvPoint(down_pipline_roi.x+down_pipline_roi.width, down_pipline_roi.y + down_pipline_roi.height),
					CV_RGB(125, 125, 125), 1, 8, 0);
			}else
			{
				down_pipline_result = 0;
			}
		}
	}
	//params.var1[0](=1向下，=2向右，=360无法计算),params.var1[1](0表示无料，1表示有料)
	if(disc_up_flag_result>0 && disc_left_flag_result<1)
	{
		param.var1[0] = 1;
		param.var1[1] = 0;
	}else if(disc_up_flag_result<1 && disc_left_flag_result>0)
	{
		param.var1[0] = 2;
		param.var1[1] = 0;
	}else
	{
		param.var1[0] = 360;
	}

	if (param.var1[0] == 360)
	{
		if (right_pipline_result>0)
		{
			param.var1[1] = 1;
			param.var1[0] = 2;
		}else if (down_pipline_result>0)
		{
			param.var1[1] = 1;
			param.var1[0] = 1;
		}else
		{
			param.var1[1] = 360;
		}
	}


		//		double angle_value = arc_value*i;
		//		if (angle_value>PI_VALUE/2 && angle_value<5*PI_VALUE/6)
		//		{
		//			//对于向右的角度进行判断
		//			window_roi.x = dark_circle.x + (inner_boundary+outer_boundary)/2 * sin(angle_value) - 70;
		//			window_roi.y = dark_circle.y + (inner_boundary+outer_boundary)/2 * cos(angle_value) - 10;
		//			window_roi.width = 140;
		//			window_roi.height = 20;
		//			param.var1[0] = 2;
		//			////		}else if (angle_value<PI_VALUE/4 || angle_value>11*PI_VALUE/6)
		//		{
		//			//对于向下的角度进行判断
		//			window_roi.x = dark_circle.x + (inner_boundary+outer_boundary)/2 * sin(angle_value) - 10;
		//			window_roi.y = dark_circle.y + (inner_boundary+outer_boundary)/2 * cos(angle_value) - 60;
		//			window_roi.width = 20;
		//			window_roi.height = 120;
		//			param.var1[0] = 1;
		//		}
		//		if (param.var1[0] == 1||param.var1[0] == 2)
		//		{
		//			bright_pixels_num = 0;
		//			countPixels(pROI, window_roi, 200, bright_pixels_num);

		//			if (bright_pixels_num < 0.3*window_roi.width*window_roi.height)
		//			{	
		//				//找到缺口，跳出
		//				direction_find = 1;
		//				break;
		//			}
		//		}
		//		else
		//		{
		//			//没找到到缺口，继续找
		//			param.var1[0] = 360;
		//		}
		//	}
		//	//cvSaveImage("pROI_youliao.jpg",  pROI, 0);
		//	if (direction_find != 1)
		//	{
		//		param.var1[0] = 360;
		//	}
		//}


	//if (param.var1[1] < 0)
	//{
	//	//找没料时，200+-50的亮圆
	//	cvCopy(pROI, pROICopy, NULL);
	//	CIRCLE_PARAM bright_circle;
	//	radius_delta = 50;
	//	radius_threshold = 200;
	//	nRet =-1;
	//	//int circle_num = 0;
	//	nRet = findCircleByRadius(pROICopy, radius_threshold, radius_delta, rectROI_Center, bright_circle);
	//	if (nRet > 0)
	//	{
	//		param.var1[1] = 0;
	//		//cvSaveImage("pROIOTSU.jpg",  pROICopy, 0);
	//		Mat img_roi;
	//		Mat mat_img_shim;
	//		int outer_boundary = 3*bright_circle.radius+20;
	//		int inner_boundary = 3*bright_circle.radius-100;
	//		getRingROI(pROI, outer_boundary, inner_boundary, cvPoint(bright_circle.x, bright_circle.y), cvPoint(bright_circle.x, bright_circle.y), img_roi);
	//		cvCopy(&IplImage(img_roi), pROICopy, NULL);
	//		//cvSaveImage("pRing.jpg",  pROICopy, 0);

	//		//检测窗口的中心轨迹
	//		int num_windows = 30;
	//		//double each_segment = ((360/num_windows)/double(180))*PI_VALUE;
	//		double arc_value = 2*PI_VALUE/30;

	//		param.var1[0] = 360;
	//		int right_case = -1;
	//		int down_case = -1;
	//		for (int i = 0; i < num_windows; i++)
	//		{
	//			double angle_value = arc_value*i;
	//			
	//			if (angle_value>PI_VALUE/2 && angle_value<5*PI_VALUE/6)
	//			{
	//				//对于向右的角度进行判断
	//				window_roi.x = bright_circle.x + (inner_boundary+outer_boundary)/2 * sin(angle_value) - 40;
	//				window_roi.y = bright_circle.y + (inner_boundary+outer_boundary)/2 * cos(angle_value) - 40;
	//				window_roi.width = 80;
	//				window_roi.height = 80;
	//				param.var1[0] = 2;
	
	//			}else if (angle_value<PI_VALUE/4 || angle_value>11*PI_VALUE/6)
	//			{
	//				//对于向下的角度进行判断
	//				window_roi.x = bright_circle.x + (inner_boundary+outer_boundary)/2 * sin(angle_value) - 40;
	//				window_roi.y = bright_circle.y + (inner_boundary+outer_boundary)/2 * cos(angle_value) - 40;
	//				window_roi.width = 80;
	//				window_roi.height = 80;
	//				param.var1[0] = 1;
	

	//			}
	//			if (param.var1[0] == 1)
	//			{
	//				int bright_pixels_num = 0;
	//				countPixels(pROI, window_roi, 200, bright_pixels_num);

	//				if (bright_pixels_num > 0.9*window_roi.width*window_roi.height)
	//				{	
	//					//找到缺口，跳出
	//					down_case = 1;

	//					//break;
	//				}
	//			}
	//			else if(param.var1[0] == 2)
	//			{
	//				int bright_pixels_num = 0;
	//				countPixels(pROI, window_roi, 200, bright_pixels_num);

	//				if (bright_pixels_num > 0.9*window_roi.width*window_roi.height)
	//				{	
	//					//找到缺口，跳出
	//					right_case = 1;
	//					//break;

	//				}
	//			}
	//			else
	//			{
	//				//没找到到缺口，继续找
	//				param.var1[0] = 360;
	//			}
	//		}

	//		if (right_case==1&&down_case!=1)
	//		{
	//			param.var1[0] = 2;
	//		}else if(right_case!=1&&down_case==1)
	//		{
	//			param.var1[0] = 1;
	//		}else //if (right_case==1&&down_case==1)
	//		{
	//			

	//		}
	//	}else
	//	{
	//		param.var1[0] = 360; 
	//	}
	//}

	//cvSaveImage("pROIOTSU.jpg",  pROI, 0);

	if (pROI!=NULL)
	{
		cvReleaseImage(&pROI);
	}
	if (pROICopy!=NULL)
	{
		cvReleaseImage(&pROICopy);
	}
	return nRet;
}

int rivoetDefectDetect(IplImage* pIplsrc, CvRect rectROI, params& param)
{
	
	return 1;
}

int detectCrackRegion(IplImage* pIplsrc, CvRect rectROI, params& param)
{
	return 1;
}

bool getRivoetPosition(IplImage* pROI, IplImage* pROICopy,  int total_segments, Point &circle_centre, Point candidate_crack_points[128], Point candidate_long_points[128], int &binary_threshold_value, int &crack_num)  
{
	cvCopy(pROI, pROICopy); //复制图像
	//通过循环二值化提取印痕边缘
	int bright_pos = 210;   //图像偏亮的情况，默认图像偏亮
	bool rivoet_exist = 0;
	int change_point = 0;
	char file_path[256];
	while (1)
	{
		cvCopy(pROI, pROICopy); //复制图像
		//cvSaveImage( "Threshold0.bmp", pROICopy);

		cvThreshold(pROICopy, pROICopy, bright_pos, 255, CV_THRESH_BINARY); 
		//cvSaveImage( "Threshold1.bmp", pROICopy);
		//cvCopy(pROI, pROICopy); //复制图像
		//cvAdaptiveThreshold(pROICopy, pROICopy, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 20, 10);  
		//cvSaveImage( "AdaptiveThreshold.bmp", pROICopy);

		vector<vector<Point> > contours;
		//vector<Vec4i>hierarchy(10000);
		//vector<Mat>contours(10000);//手动分配内存空间大小
		findObjectContours(pROICopy, contours);
		cvCopy(pROI, pROICopy); //复制图像

		int threshold = 250;
		int num_rivoet = 0;
		int delta = 50;
		int min_area = INFINITE_NUM;

		//定位
		int rivoet_num = 0;
		int contour_index = 0;
		int rivoet_index = 0;
		Rect rivoetRect;
		rivoetRect.height = pROI->height;
		rivoetRect.width = pROI->width;
		for(vector<vector<Point> >::iterator iter = contours.begin(); iter != contours.end(); iter++)
		{
			Rect rcRect = boundingRect(contours[iter - contours.begin()]);
			//cvRectangle(pROICopy, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(128, 128, 128), 1, 8, 0);
			if ((abs(rcRect.width - threshold) < delta) && (abs(rcRect.height - threshold) < delta)) 
			{

				if (rivoetRect.height > rcRect.height && rivoetRect.width > rcRect.width)
				{
					rivoet_num = 1;
					rivoetRect.height = rcRect.height;
					rivoetRect.width = rcRect.width;
					rivoet_index = contour_index;
				}

			}
			contour_index++;
		}


		//sprintf_s(file_path, "show//liefeng_%d.bmp", bright_pos);
		//cvSaveImage( file_path, pROICopy);
		if (rivoet_num == 1)
		{
			//cvRectangle(pROICopy, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(128, 128, 128), 1, 8, 0);
			Rect rcRect = boundingRect(contours[rivoet_index]);
			circle_centre.x = rcRect.x + rcRect.width/2;
			circle_centre.y = rcRect.y + rcRect.height/2;

			int  short_distance_points[CRACK_POINT_NUM] = {0};			//记录每一段上距离圆心的最近的点
			int  long_distance_points[CRACK_POINT_NUM] = {0};			//记录每一段上距离圆心的最近的点
			//for(iter = contours.begin(); iter != contours.end(); iter++)
			//{
			int segment_index;
			int num_points_on_segment = contours[rivoet_index].size() / total_segments;			//每个区域的点数

			int current_direction = 0;
			int pre_index = -999999;//记录上次变化的点
			int pre_direction = 0;//记录之前方向

			crack_num = 0;
			change_point = 0;
			for (int i = 0; i < contours[rivoet_index].size() - 5; i++)
			{
				//cvCircle(pROICopy, (*iter)[i], 3, cvScalar(255,255,255,0.5), 1, 8, 0);
				int distance_from_centre1 = getLineLen(contours[rivoet_index][i], circle_centre);
				int distance_from_centre2 = getLineLen(contours[rivoet_index][i+5], circle_centre);

				//int distance_from_centre3 = getLineLen(contours[rivoet_index][i], circle_centre);
				//int distance_from_centre4 = getLineLen(contours[rivoet_index][i+3], circle_centre);//裂缝很细，取3个像素间隔
				if (abs(distance_from_centre1 - distance_from_centre2) > 2 /*|| abs(distance_from_centre3 - distance_from_centre4) > 2*/)
				{
					change_point++;
					current_direction = distance_from_centre1 - distance_from_centre2;
					if (abs(distance_from_centre1 - distance_from_centre2) > 6)
					{
						if (crack_num == 128)//最多存储128个点
						{
							break;
						}
						cvCircle(pROICopy, contours[rivoet_index][i], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
						cvCircle(pROICopy, contours[rivoet_index][pre_index], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
						candidate_crack_points[crack_num].x = contours[rivoet_index][pre_index].x;
						candidate_crack_points[crack_num].y = contours[rivoet_index][pre_index].y;
						crack_num++ ;

					}else if ((i - pre_index) < 15)//15个像素内出现反方向变化，才是裂缝
					{
						if (pre_direction*current_direction < 0)
						{
							if (crack_num == 128)
							{
								break;
							}
							cvCircle(pROICopy, contours[rivoet_index][i], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
							cvCircle(pROICopy, contours[rivoet_index][pre_index], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
							candidate_crack_points[crack_num].x = contours[rivoet_index][pre_index].x;
							candidate_crack_points[crack_num].y = contours[rivoet_index][pre_index].y;
							crack_num++ ;
						}else
						{

						}

					}else
					{

					}
					pre_index = i;
					pre_direction = distance_from_centre1 - distance_from_centre2;
				}

			}
			//i = 0;

			for (segment_index = 0; segment_index < total_segments; segment_index++)
			{
				double short_distance = INFINITE_NUM;
				double long_distance = 0;
				bool segment_start_flag = true;
				short_distance_points[segment_index] = num_points_on_segment * segment_index;//初始为第一个点
				//cvCircle(pROICopy, (*iter)[num_points_on_segment * segment_index], 3, cvScalar(255,255,255,0.5), 1, 8, 0);

				for (int i = num_points_on_segment * segment_index; i < num_points_on_segment * (segment_index+1) - 1; i++)
				{
					//找距离最近的点和最远点
					if (segment_start_flag)
					{
						segment_start_flag = false;
					}
					Point current_point = contours[rivoet_index][i];

					double delta_x = current_point.x - circle_centre.x;
					double delta_y = current_point.y - circle_centre.y;
					double distance_form_circle_centre = delta_x*delta_x + delta_y*delta_y;
					if (short_distance > distance_form_circle_centre)
					{
						short_distance = distance_form_circle_centre;
						short_distance_points[segment_index] = i;
					}
					if (long_distance < distance_form_circle_centre)
					{
						long_distance = distance_form_circle_centre;
						long_distance_points[segment_index] = i;
					}
				}

				candidate_long_points[segment_index].x = contours[rivoet_index][long_distance_points[segment_index]].x;
				candidate_long_points[segment_index].y = contours[rivoet_index][long_distance_points[segment_index]].y;
			}
			rivoet_exist = true;
			binary_threshold_value = bright_pos;
		}else
		{
			bright_pos -= 10;
		}

		if (rivoet_exist || bright_pos < 10)
		{
			break;
		}
	}

	if (crack_num > 8)
	{
		//如果存在很多裂缝点，可能图像偏暗，在次验证
		int dark_pos = 100;		//图像偏暗的情况
		bool rivoet_exist = 0;
		while (1)
		{
			cvCopy(pROI, pROICopy); //复制图像
			//cvSaveImage( "Threshold0.bmp", pROICopy);
			cvThreshold(pROICopy, pROICopy, dark_pos, 255, CV_THRESH_BINARY); 
			//cvSaveImage( "Threshold1.bmp", pROICopy);
			//cvCopy(pROI, pROICopy); //复制图像
			//cvAdaptiveThreshold(pROICopy, pROICopy, 255, CV_ADAPTIVE_THRESH_MEAN_C, CV_THRESH_BINARY, 20, 10);  
			//cvSaveImage( "AdaptiveThreshold.bmp", pROICopy);

			vector<vector<Point> > contours;
			findObjectContours(pROICopy, contours);
			cvCopy(pROI, pROICopy); //复制图像

			int threshold = 250;
			int num_rivoet = 0;
			int delta = 50;
			int min_area = INFINITE_NUM;

			//定位
			int rivoet_num = 0;
			int contour_index = 0;
			int rivoet_index = 0;
			Rect rivoetRect;
			rivoetRect.height = pROI->height;
			rivoetRect.width = pROI->width;

			for(vector<vector<Point> >::iterator iter = contours.begin(); iter != contours.end(); iter++)
			{
				Rect rcRect = boundingRect(contours[iter - contours.begin()]);
				if ((abs(rcRect.width - threshold) < delta) && (abs(rcRect.height - threshold) < delta)) 
				{
					if (rivoetRect.height > rcRect.height && rivoetRect.width > rcRect.width)
					{
						rivoet_num = 1;
						rivoetRect.height = rcRect.height;
						rivoetRect.width = rcRect.width;
						rivoet_index = contour_index;
					}
				}


				contour_index++;
			}

			if (rivoet_num == 1)
			{
				//cvRectangle(pROICopy, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(128, 128, 128), 1, 8, 0);
				Rect rcRect = boundingRect(contours[rivoet_index]);
				circle_centre.x = rcRect.x + rcRect.width/2;
				circle_centre.y = rcRect.y + rcRect.height/2;

				int  short_distance_points[CRACK_POINT_NUM] = {0};			//记录每一段上距离圆心的最近的点
				int  long_distance_points[CRACK_POINT_NUM] = {0};			//记录每一段上距离圆心的最近的点
				//for(iter = contours.begin(); iter != contours.end(); iter++)
				//{
				int segment_index;
				int num_points_on_segment = contours[rivoet_index].size() / total_segments;			//每个区域的点数

				int current_direction = 0;
				int pre_index = -999999;//记录上次变化的点
				int pre_direction = 0;//记录之前方向

				crack_num = 0;
				for (int i = 0; i < contours[rivoet_index].size() - 5; i++)
				{
					//cvCircle(pROICopy, (*iter)[i], 3, cvScalar(255,255,255,0.5), 1, 8, 0);
					int distance_from_centre1 = getLineLen(contours[rivoet_index][i], circle_centre);
					int distance_from_centre2 = getLineLen(contours[rivoet_index][i+5], circle_centre);

					int distance_from_centre3 = getLineLen(contours[rivoet_index][i], circle_centre);
					int distance_from_centre4 = getLineLen(contours[rivoet_index][i+3], circle_centre);//裂缝很细，取3个像素间隔
					if (abs(distance_from_centre1 - distance_from_centre2) > 2 /*|| abs(distance_from_centre3 - distance_from_centre4) > 2*/)
					{
						current_direction = distance_from_centre1 - distance_from_centre2;
						if (abs(distance_from_centre1 - distance_from_centre2) > 6)
						{
							if (crack_num == 128)//最多存储128个点
							{
								break;
							}
							cvCircle(pROICopy, contours[rivoet_index][i], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
							cvCircle(pROICopy, contours[rivoet_index][pre_index], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
							candidate_crack_points[crack_num].x = contours[rivoet_index][pre_index].x;
							candidate_crack_points[crack_num].y = contours[rivoet_index][pre_index].y;
							crack_num++ ;

						}else if ((i - pre_index) < 15)//15个像素内出现反方向变化，才是裂缝
						{
							if (pre_direction*current_direction < 0)
							{
								if (crack_num == 128)
								{
									break;
								}
								cvCircle(pROICopy, contours[rivoet_index][i], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
								cvCircle(pROICopy, contours[rivoet_index][pre_index], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
								candidate_crack_points[crack_num].x = contours[rivoet_index][pre_index].x;
								candidate_crack_points[crack_num].y = contours[rivoet_index][pre_index].y;
								crack_num++ ;
							}else
							{

							}

						}else
						{

						}
						pre_index = i;
						pre_direction = distance_from_centre1 - distance_from_centre2;
					}

				}
				//i = 0;

				for (segment_index = 0; segment_index < total_segments; segment_index++)
				{
					double short_distance = INFINITE_NUM;
					double long_distance = 0;
					bool segment_start_flag = true;
					short_distance_points[segment_index] = num_points_on_segment * segment_index;//初始为第一个点
					//cvCircle(pROICopy, (*iter)[num_points_on_segment * segment_index], 3, cvScalar(255,255,255,0.5), 1, 8, 0);

					for (int i = num_points_on_segment * segment_index; i < num_points_on_segment * (segment_index+1) - 1; i++)
					{
						//找距离最近的点和最远点
						if (segment_start_flag)
						{
							segment_start_flag = false;
						}
						Point current_point = contours[rivoet_index][i];

						double delta_x = current_point.x - circle_centre.x;
						double delta_y = current_point.y - circle_centre.y;
						double distance_form_circle_centre = delta_x*delta_x + delta_y*delta_y;
						if (short_distance > distance_form_circle_centre)
						{
							short_distance = distance_form_circle_centre;
							short_distance_points[segment_index] = i;
						}
						if (long_distance < distance_form_circle_centre)
						{
							long_distance = distance_form_circle_centre;
							long_distance_points[segment_index] = i;
						}
					}

					candidate_long_points[segment_index].x = contours[rivoet_index][long_distance_points[segment_index]].x;
					candidate_long_points[segment_index].y = contours[rivoet_index][long_distance_points[segment_index]].y;
				}
				rivoet_exist = true;
				binary_threshold_value = dark_pos;
			}
			else
			{
				dark_pos += 10;
			}

			if (rivoet_exist/*pos < 30||*/|| dark_pos > 240)
			{
				break;
			}

			//index++;
		}
	}

	cvCircle(pROICopy, cvPoint(circle_centre.x, circle_centre.y), 5,CV_RGB(0, 0, 0), 1, 8, 0);
	//cvSaveImage( "show//liefeng.bmp", pROICopy);
	return rivoet_exist;
}

int getCrackRegion(IplImage* pIplsrc, CvRect rectROI, params& param)
{  
	//区域有效，将img中的矩形区域复制给roi
	cvSetImageROI(pIplsrc,cvRect(rectROI.x,rectROI.y,rectROI.width, rectROI.height));//设置源图像ROI
	IplImage* pROI = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);//创建目标图像
	IplImage* pROICopy = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);//创建目标图像
	cvCopy(pIplsrc, pROI); //复制图像
	cvResetImageROI(pIplsrc);//源图像用完后，清空ROI

	Point crack_point[CRACK_POINT_NUM];
	memset(crack_point, 0, CRACK_POINT_NUM*sizeof(Point));

	int max_radius = 0;
	//第一步确定铆钉的位置
	int crack_region_radius = CRACK_REGION_RADIUS;

	//输出到日志测试
	FILE *fp = NULL; 
	fp = fopen("log_crack_detect.txt","w+"); 
	if(fp == NULL)  
	{  
		return -1;  
	}
	int segment_index = 0;
	int total_segments = 16;
	Point candidate_crack_points[CRACK_POINT_NUM]; //候选裂缝点
	Point candidate_long_points[CRACK_POINT_NUM]; //候选距离圆心最远点

	Point  circle_centre;
	circle_centre.x = 0;
	circle_centre.y = 0;

	int binary_threshold_value = 0;
	int crack_num = 0;
	//找裂缝的候选区域和铆钉圆心,确定裂隙 
	bool rivoet_exist = getRivoetPosition(pROI, pROICopy, total_segments, circle_centre, candidate_crack_points, candidate_long_points, binary_threshold_value, crack_num);
	double min_distance_from_centre = INFINITE_NUM;

	if (rivoet_exist)
	{
		double longest_distance = 0;
		int inner_boundary = 0;
		int outer_boundary = 0;
		//double array_distance[16] = {0};
		double avrage_distance = 0;
		for (int i = 0; i < total_segments; i++)
		{
			double delta_x = (candidate_long_points[i].x - circle_centre.x);
			double delta_y = (candidate_long_points[i].y  - circle_centre.y);
			double distance_form_circle_centre = (delta_x*delta_x + delta_y*delta_y);
			if(longest_distance < distance_form_circle_centre)
			{
				longest_distance = distance_form_circle_centre;
			}
			//array_distance[i] = distance_form_circle_centre;
			avrage_distance += distance_form_circle_centre;
			//cvCircle(imgClr,cvPoint(candidate_long_points[i].x,candidate_long_points[i].y), 10, cvScalar(200,162,199,0.5), 1, 8, 0);
		}
		avrage_distance = avrage_distance/16;
		if (longest_distance > 1.1*avrage_distance)//1.1倍，说明铆钉白色部分变形较多。
		{
			max_radius = sqrt(0.8*longest_distance);
			outer_boundary = max_radius+30-5;
			inner_boundary = max_radius+1-5;

		}else
		{
			max_radius = sqrt(longest_distance);
			outer_boundary = max_radius+20-5;
			inner_boundary = max_radius+1-5;
		}

		double inner_radius[256] = {0};
		int inner_diameter[128] = {0};
		double outer_radius[256] = {0};
		int outer_diameter[128] = {0};
		double avg_delta = 0;
		int valid_segment_num = 0;

		//int segment_point_num = 0; //每段间隔的点数
		int num_segments = 60; //分割的段数,设为偶数段

		CvRect rect_segment;
		rect_segment.width = (outer_boundary - inner_boundary)*2;
		rect_segment.height = (outer_boundary - inner_boundary)*2;
		IplImage* pSegmentImg = cvCreateImage(cvSize(rect_segment.width, rect_segment.height), pROI->depth, pROI->nChannels);//创建目标图像
		IplImage* pSegmentImgCopy = cvCreateImage(cvSize(rect_segment.width, rect_segment.height), pROI->depth, pROI->nChannels);//创建目标图像
		for (int i = 0; i < num_segments; i++)
		{	

			double angle = i*(2*PI_VALUE/60);

			rect_segment.x = circle_centre.x + ((inner_boundary+outer_boundary)/2)*cos(angle) - rect_segment.width/2;
			rect_segment.y = circle_centre.y + ((inner_boundary+outer_boundary)/2)*sin(angle) - rect_segment.height/2;

			//cvRectangle(pROI, cvPoint(rect_segment.x, rect_segment.y), cvPoint(rect_segment.x + rect_segment.width, rect_segment.y + rect_segment.height),CV_RGB(125, 125, 125), 1, 8, 0);

			cvSetImageROI(pROI, rect_segment);//设置源图像ROI

			cvCopy(pROI, pSegmentImg); //复制图像

			char imgPath[512];
			sprintf_s(imgPath, "show//pSegment%d.bmp", i);


			cvResetImageROI(pROI);//源图像用完后，清空ROI


			//cvSmooth(pSegmentImg, pSegmentImg, CV_GAUSSIAN, 3, 3, 0, 0);
			cvSaveImage(imgPath, pSegmentImg);

			//bool bounary_exist = 1;
			vector<vector<Point> >::iterator iter;
			vector<vector<Point> > rivoet_contours;

			cvCopy(pSegmentImg, pSegmentImgCopy, NULL);

			//计算最佳阈值，提取边界
			int dark_threshold = 40;
			myCal_Hist(pSegmentImg, 100, dark_threshold);

			double avg_radius1 = 0;
			double avg_radius2 = 0;

			{
				vector<int> v;   //存储排序的长宽

				cvCopy(pSegmentImgCopy, pSegmentImg, NULL);
				cvThreshold(pSegmentImg, pSegmentImg, dark_threshold, 255, CV_THRESH_BINARY);
				cvDilate( pSegmentImg,  pSegmentImg, NULL, 1); //膨胀 
				cvErode(pSegmentImg, pSegmentImg, NULL, 1);//腐蚀
				//sprintf_s(imgPath, "show//pSegmentTheCRC%d.bmp", i);
				//cvSaveImage(imgPath, pSegmentImg);
				//找出黑色部分，取反后是白色部分
				reverseImage(pSegmentImg);
				cvCopy(pSegmentImg, pSegmentImgCopy, NULL);
				findObjectContours(pSegmentImg, rivoet_contours);
				cvCopy(pSegmentImgCopy, pSegmentImg, NULL);
				for(iter = rivoet_contours.begin() ; iter != rivoet_contours.end();iter++)
				{
					double tmparea = fabs(contourArea(rivoet_contours[iter - rivoet_contours.begin()]));
					Rect rcRect = boundingRect(rivoet_contours[iter - rivoet_contours.begin()]);
					v.push_back(MAX(rcRect.width,rcRect.height));
				}
				sort(v.begin(),v.end() ,comp);

				//删除细小的区域
				iter = rivoet_contours.begin();
				for(int j = 0; j<rivoet_contours.size();j++,iter++)
				{
					double tmparea = fabs(contourArea(rivoet_contours[iter - rivoet_contours.begin()]));
					Rect rcRect = boundingRect(rivoet_contours[iter - rivoet_contours.begin()]);
					//cvRectangle(pSegmentImg, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(200, 200, 200), 1, 8, 0);

					if (MAX(rcRect.width, rcRect.height) < v[0])
					{
						unsigned char* data=(uchar *)pSegmentImg->imageData;  
						int step = pSegmentImg->widthStep/sizeof(uchar);  
						//uchar* tmp;  
						for(int n=rcRect.y;n<rcRect.y + rcRect.height;n++)  
							for(int m=rcRect.x;m<rcRect.x + rcRect.width;m++)  
							{
								data[n*step+m] = 0; 
							}
					}
				}

				reverseImage(pSegmentImg);
				//找边界
				cvCopy(pSegmentImg, pSegmentImgCopy, NULL);
				rivoet_contours.clear();
				findObjectContours(pSegmentImg, rivoet_contours);
				cvCopy(pSegmentImgCopy, pSegmentImg, NULL);

				v.clear();
				for(iter = rivoet_contours.begin() ; iter != rivoet_contours.end();iter++)
				{
					double tmparea = fabs(contourArea(rivoet_contours[iter - rivoet_contours.begin()]));
					Rect rcRect = boundingRect(rivoet_contours[iter - rivoet_contours.begin()]);
					v.push_back(MAX(rcRect.width,rcRect.height));
				}
				sort(v.begin(),v.end() ,comp);

				iter = rivoet_contours.begin();
				int counter =0;
				if (rivoet_contours.size()>1)
				{

					double sum_radius1 = 0;
					double sum_radius2 = 0;
					//double avg_radius = 0;
					int point_num1 = 0;
					int point_num2 = 0;
					int distance1[512] = {0};
					int distance2[512] = {0};
					for(int j = 0; j<rivoet_contours.size();j++,iter++)
					{
						double tmparea = fabs(contourArea(rivoet_contours[iter - rivoet_contours.begin()]));
						Rect rcRect = boundingRect(rivoet_contours[iter - rivoet_contours.begin()]);
						cvRectangle(pSegmentImg, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(200, 200, 200), 1, 8, 0);

						if (MAX(rcRect.width, rcRect.height) >= v[1])
						{
							counter++;
							Point p1, p2, p3;
							int total_points = rivoet_contours[iter - rivoet_contours.begin()].size();

							//区分内边界还是外边界（倒角的边界）
							for (int k = 0; k < total_points; k++)
							{
								p1.x = rivoet_contours[iter - rivoet_contours.begin()][k].x;
								p1.y = rivoet_contours[iter - rivoet_contours.begin()][k].y;

								p2.x = rect_segment.x + rivoet_contours[iter - rivoet_contours.begin()][k].x;
								p2.y = rect_segment.y + rivoet_contours[iter - rivoet_contours.begin()][k].y;

								if(p1.x>2 && p1.y>2 && p1.x<rect_segment.width-2 && p1.y<rect_segment.height-2)
								{
									if (counter == 1)
									{
										distance1[point_num1] = getLineLen(p2, circle_centre);
										sum_radius1 += distance1[point_num1];
										point_num1++;
									}else
									{
										distance2[point_num2] = getLineLen(p2, circle_centre);
										sum_radius2 += distance2[point_num2];
										point_num2++;
									}
									cvCircle(pSegmentImg, p1, 2, CV_RGB((counter)*80, (counter)*80, (counter)*80), 1, 8, 0);
								}
							}
						}
					}

					if (point_num1>0 && point_num2>0)
					{
						double min_distance1 = 99999 ,max_distance1 = 0;						//倒角的部分直径(外径)
						double min_distance2 = 99999 ,max_distance2 = 0;						//白色亮的部分直径(内径)

						avg_radius1 = sum_radius1/point_num1;
						avg_radius2 = sum_radius2/point_num2;

						for (int k = 0; k < point_num1; k++)
						{
							if (min_distance1 > distance1[k])
							{
								min_distance1 = distance1[k];
							}

							if (max_distance1 < distance1[k])
							{
								max_distance1 = distance1[k];
							}
						}

						for (int k = 0; k < point_num2; k++)
						{
							if (min_distance2 > distance2[k])
							{
								min_distance2 = distance2[k];
							}

							if (max_distance2 < distance2[k])
							{
								max_distance2 = distance2[k];
							}
						}


						if ((max_distance1 - min_distance1)>8)    //距离变化大，无效;
						{
							avg_radius1 = 0;
						}
						if ((max_distance2 - min_distance2)>8)    //距离变化大，无效;
						{
							avg_radius2 = 0;
						}

						//Point temp_p;
						//temp_p.x = 8;
						//temp_p.y = 8;
						//if (avg_radius1 != 0)
						//{
						//	
						//	cvCircle(pSegmentImg, temp_p, 5, CV_RGB(100, 100, 100), 3, 8, 0);
						//}
						//if (avg_radius2 != 0)
						//{
						//	cvCircle(pSegmentImg, temp_p, 2, CV_RGB(100, 100, 100), 3, 8, 0);
						//}
					}else
					{
						avg_radius1 = 0;
						avg_radius2 = 0;
					}
				}
			}

			//if (i == 30 || i == 5)
			//{
			//	int stop_cmd = 0;
			//}
			if (avg_radius1 > avg_radius2)
			{
				inner_radius[i] = avg_radius2;
				outer_radius[i] = avg_radius1;
			}else
			{
				inner_radius[i] = avg_radius1;
				outer_radius[i] = avg_radius2;
			}

			sprintf_s(imgPath, "show//pSegmentImgThreshold%d.bmp", i);
			cvSaveImage(imgPath, pSegmentImg);

			//sprintf_s(imgPath, "show//pSegmentImgCopy%d.bmp", i);
			//cvSaveImage(imgPath, pSegmentImgCopy);
		}
		if (pSegmentImg != NULL)
		{
			cvReleaseImage(&pSegmentImg);
		}
		if (pSegmentImgCopy != NULL)
		{
			cvReleaseImage(&pSegmentImgCopy);
		}


		for (int i = 0; i < num_segments/2; i++)
		{
			//记录直径
			if (inner_radius[i] != 0 && inner_radius[num_segments/2+i] != 0)
			{
				inner_diameter[i] = inner_radius[i] + inner_radius[num_segments/2+i];
			}else
			{
				inner_diameter[i] = 0;
			}

			//记录直径
			if (outer_radius[i] != 0 && outer_radius[num_segments/2+i] != 0)
			{
				outer_diameter[i] = outer_radius[i] + outer_radius[num_segments/2+i];
			}else
			{
				outer_diameter[i] = 0;
			}
			if (inner_diameter[i] != 0 && outer_diameter[i] != 0)
			{
				avg_delta +=  outer_diameter[i] - inner_diameter[i];
				valid_segment_num++;
			}
		}
		avg_delta = avg_delta / valid_segment_num;
		valid_segment_num = 0;
		double max_diameter_inner = 0;
		double min_diameter_inner = INFINITE_NUM;
		double max_diameter_outer = 0;
		double min_diameter_outer = INFINITE_NUM;

		int long_inner_segment1 = 0;
		int long_inner_segment2 = 0;
		int long_outer_segment1 = 0;
		int long_outer_segment2 = 0;

		int short_inner_segment1 = 0;
		int short_inner_segment2 = 0;
		int short_outer_segment1 = 0;
		int short_outer_segment2 = 0;

		//最大最小直径
		for (int i = 0; i < num_segments/2; i++)
		{
			if (inner_diameter[i] > 0)
			{
				if (inner_diameter[i] < min_diameter_inner)
				{
					min_diameter_inner = inner_diameter[i];
					short_inner_segment1 = i;
					short_inner_segment2 = num_segments/2+i;

				}
				if (inner_diameter[i] > max_diameter_inner)
				{
					max_diameter_inner = inner_diameter[i];
					long_inner_segment1 = i;
					long_inner_segment2 = num_segments/2+i;
				}
			}

			if (outer_diameter[i] > 0)
			{
				if (outer_diameter[i] < min_diameter_outer)
				{
					min_diameter_outer = outer_diameter[i];
					short_outer_segment1 = i;
					short_outer_segment2 = num_segments/2+i;

				}
				if (outer_diameter[i] > max_diameter_outer)
				{
					max_diameter_outer = outer_diameter[i];
					long_outer_segment1 = i;
					long_outer_segment2 = num_segments/2+i;
				}
			}
		}

		double calibrateRatio = (10.0/280.0);
		param.var2[4] = 14;
		max_diameter_inner = (max_diameter_inner + param.var2[4])*calibrateRatio;	//param.var2[4]倒角部分补偿值
		min_diameter_inner = (min_diameter_inner + param.var2[4])*calibrateRatio;	//param.var2[4]倒角部分补偿值

		max_diameter_outer = max_diameter_outer*calibrateRatio;
		min_diameter_outer = min_diameter_outer*calibrateRatio;

		if (/*max_diameter_inner > 10.5|| min_diameter_inner < 9||*/crack_num > 0)
		{
			param.var1[0] = 0;
		}else
		{
			param.var1[0] = 1;
		}
		//cvCircle(imgClr,cvPoint(circle_centre.x,circle_centre.y), max_radius, cvScalar(200,162,199,0.5), 1, 8, 0);
		//cvSaveImage("color.jpg",imgClr,0);
		if (fp != NULL)
		{
			fprintf(fp,"max_diameter_inner:%0.3f, min_diameter_inner:%0.3f, crack_num:%d, short_segment:%d,%d, long_segment:%d,%d\n", max_diameter_inner, min_diameter_inner, crack_num, short_inner_segment1,short_inner_segment2,long_inner_segment1, long_inner_segment2);
			fprintf(fp,"max_diameter_outer:%0.3f, min_diameter_outer:%0.3f, crack_num:%d, short_segment:%d,%d, long_segment:%d,%d\n", max_diameter_outer, min_diameter_outer, crack_num, short_outer_segment1,short_outer_segment2,long_outer_segment1, long_outer_segment2);
			fprintf(fp,"-----------------------------------------------------------\n"); 

			fclose(fp);
			fp = NULL;
		}
		param.var1[1] = crack_num;
		param.var2[0] = max_diameter_inner;
		param.var2[1] = min_diameter_inner;
		param.var2[2] = max_diameter_outer;
		param.var2[3] = min_diameter_outer;


		//画上最大最小直径的位置
		double short_angle = short_inner_segment1*(2*PI_VALUE/60);
		double long_angle = long_inner_segment1*(2*PI_VALUE/60);
		rect_segment.x = circle_centre.x + ((inner_boundary+outer_boundary)/2)*cos(short_angle) - rect_segment.width/2;
		rect_segment.y = circle_centre.y + ((inner_boundary+outer_boundary)/2)*sin(short_angle) - rect_segment.height/2;
		cvRectangle(pROI, cvPoint(rect_segment.x, rect_segment.y), cvPoint(rect_segment.x + rect_segment.width, rect_segment.y + rect_segment.height),CV_RGB(100, 100, 100), 1, 8, 0);
		rect_segment.x = circle_centre.x + ((inner_boundary+outer_boundary)/2)*cos(long_angle) - rect_segment.width/2;
		rect_segment.y = circle_centre.y + ((inner_boundary+outer_boundary)/2)*sin(long_angle) - rect_segment.height/2;
		cvRectangle(pROI, cvPoint(rect_segment.x, rect_segment.y), cvPoint(rect_segment.x + rect_segment.width, rect_segment.y + rect_segment.height),CV_RGB(200, 200, 200), 1, 8, 0);
		cvSaveImage("show//pROI.bmp", pROI);
		//画上裂缝点
		for (int i = 0; i < crack_num; i++)
		{
			//cvCircle(pIplsrc, candidate_crack_points[i], 3, cvScalar(1,1,1,0.5), 1, 8, 0);
		}

		if (pROI != NULL)
		{
			cvReleaseImage(&pROI);
		}
		if (pROICopy != NULL)
		{
			cvReleaseImage(&pROICopy);
		}
		return 1; 
	}else
	{
		return 0;
	}


}
int detectDiameterAndCrackPoint(IplImage* pIplsrc, CvRect rectROI, params& param)
{  
	//输出到日志测试
	//FILE *fp = NULL; 
	//fp = fopen("log_crack_detect.txt","a+"); 
	//if (fp != NULL)
	//{
	//	fprintf(fp,"maxDiameter:%0.3f, minDiameter:%0.3f, quanlify:%d, crackNum:%d\n", param.var2[0], param.var2[1], param.var1[0], param.var1[1]);
	//	fprintf(fp,"-----------------------------------------------------------\n"); 

	//	fclose(fp);
	//	fp = NULL;
	//}
	//if(fp == NULL)  
	//{  
	//	return 0;  
	//}

    char file_path[256];
	int nRet = -1;
	//rectROI.x = 550;
	//rectROI.y = 380;
	//rectROI.width = 250;
	//rectROI.height = 240;
	if (rectROI.width>0 && rectROI.height>0 && rectROI.width < pIplsrc->width && rectROI.height < pIplsrc->height)
	{

	}else
	{
		rectROI.x = 0;
		rectROI.y = 0;
		rectROI.width = pIplsrc->width;
		rectROI.height = pIplsrc->height;
	}

	cvSetImageROI(pIplsrc,cvRect(rectROI.x,rectROI.y,rectROI.width, rectROI.height));//设置源图像ROI
	IplImage* pROI = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);//创建目标图像
	IplImage* pROICopy = cvCreateImage(cvSize(rectROI.width, rectROI.height), pIplsrc->depth, pIplsrc->nChannels);
	cvCopy(pIplsrc, pROI); //复制图像
	cvResetImageROI(pIplsrc);//源图像用完后，清空ROI

	cvCopy(pROI, pROICopy);


	int bestThreshold = 100;   //图像偏亮的情况，默认图像偏亮
	vector<vector<Point> > contours;
	vector<vector<Point> >::iterator iter;
	int totalPointNumOnCircleBoundary;
	Point circleBoundaryPoint, rectCentre, circleCentre;
	////确定阈值
	//int findThreshold;
	//for (int i=0 ; bestThreshold > 30 && findThreshold>1; i++)
	//{
	//	findThreshold = 0;
	//	cvThreshold(pROICopy, pROICopy, bestThreshold, 255, CV_THRESH_BINARY); 
	//	//sprintf_s(file_path, "my//Threshold_%d.bmp", bestThreshold);
	//	//cvSaveImage(file_path, pROICopy);
	//	//cvCanny(pROICopy, pROICopy, bestThreshold, intialThreshold * 3, 3);  
	//	//cvSaveImage( "my\\Canny.bmp", pROICopy);

	//	findObjectContours(pROICopy, contours);

	//	for(iter = contours.begin(); iter != contours.end(); iter++)
	//	{
	//		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
	//		rectCentre.x = rcRect.x + rcRect.width/2;
	//		rectCentre.y = rcRect.y + rcRect.height/2;
	//		Point point1, point2, point3;

	//		totalPointNumOnCircleBoundary = contours[iter - contours.begin()].size();
	//		if (rcRect.width>100 && rcRect.height>100)
	//		{
	//			for (int j = 0; j < totalPointNumOnCircleBoundary - 20; j+=20)
	//			{
	//				point1.x = contours[iter - contours.begin()][j].x;
	//				point1.y = contours[iter - contours.begin()][j].y;
	//				point2.x = contours[iter - contours.begin()][j+10].x;
	//				point2.y = contours[iter - contours.begin()][j+10].y;
	//				point3.x = contours[iter - contours.begin()][j+20].x;
	//				point3.y = contours[iter - contours.begin()][j+20].y;

	//				getCircleCenter(point1, point2, point3, circleCentre);
	//				int distanceOfRectCentreAndCircleCentre = getLineLen(circleCentre, rectCentre);//得到两点距离
	//				if (distanceOfRectCentreAndCircleCentre < 5)
	//				{
	//					findThreshold++;
	//				}
	//			}
	//		}
	//	}
	//	if (i%2==0)
	//	{
 //           bestThreshold = 120 + 10*i;
	//	}else
	//	{
 //           bestThreshold = 120 - 10*i;
	//	}
	//}
	//
	//if (findThreshold<2)
	//{
	//	return -1;;
	//}
	//cvCopy(pROI, pROICopy);

	//求圆的最大最小直径
	cvThreshold(pROICopy, pROICopy, bestThreshold, 255, CV_THRESH_BINARY); 
	//sprintf_s(file_path, "my//Threshold_%d.bmp", bestThreshold);
	//cvSaveImage(file_path, pROICopy);
	cvCanny(pROICopy, pROICopy, bestThreshold, bestThreshold * 3, 3);  
	//cvSaveImage( "my\\Canny.bmp", pROICopy);

	findObjectContours(pROICopy, contours);
	vector<CircleParam> vecCircleParam;	
	int index = 0;
	for(iter = contours.begin(),index = 0; iter != contours.end(); iter++, index++)
	{
		Rect rcRect = boundingRect(contours[iter - contours.begin()]);			//圆的外接矩形
		rectCentre.x = rcRect.x + rcRect.width/2;
		rectCentre.y = rcRect.y + rcRect.height/2;
		if (rcRect.width>100 && rcRect.height>100)
		{
			CircleParam circleParam;
			cvRectangle(pROICopy, cvPoint(rcRect.x, rcRect.y), cvPoint(rcRect.x + rcRect.width, rcRect.y + rcRect.height),CV_RGB(255, 255, 255), 1, 8, 0);
			int totalPointNumOnCircleBoundary = iter->size();
			int totalPartNum = 16;   //总共1圆边界分为16段（要偶数段）
			int totalPointNumOnPartBoundary = totalPointNumOnCircleBoundary / totalPartNum;			//划分16个边界，每个部分点数

			double distanceOfPartToCentre[256] = {0};
			for (int i = 0; i < totalPartNum; i++)
			{
				//求一段的平均距离
				for (int j = totalPointNumOnPartBoundary * i; j < totalPointNumOnPartBoundary * (i+1) - 1; j++)
				{
					circleBoundaryPoint.x = contours[iter - contours.begin()][j].x;
					circleBoundaryPoint.y = contours[iter - contours.begin()][j].y;

					double deltaX = circleBoundaryPoint.x - rectCentre.x;
					double deltaY = circleBoundaryPoint.y - rectCentre.y;
					double distanceOfPointToCentre = deltaX*deltaX + deltaY*deltaY;
					distanceOfPartToCentre[i] += sqrt(distanceOfPointToCentre);
				}
				distanceOfPartToCentre[i] = distanceOfPartToCentre[i]/totalPointNumOnPartBoundary;
			}

			double maxDiameter = 0;
			double minDiameter = 999999;

			for (int i = 0; i < totalPartNum/2; i++)
			{
				//求直径
				if (maxDiameter < distanceOfPartToCentre[i] + distanceOfPartToCentre[totalPartNum/2+i])
				{
                    maxDiameter = distanceOfPartToCentre[i] + distanceOfPartToCentre[totalPartNum/2+i];
				}
				if (minDiameter > distanceOfPartToCentre[i] + distanceOfPartToCentre[totalPartNum/2+i])
				{
					minDiameter = distanceOfPartToCentre[i] + distanceOfPartToCentre[totalPartNum/2+i];
				}
			}

			circleParam.minDiameter = minDiameter;
			circleParam.maxDiameter = maxDiameter;
			vecCircleParam.push_back(circleParam);
		}
	}
	cvSaveImage( "my\\result.bmp", pROICopy);
	double calibrateRatio = param.var2[2];   //像素与长度的比值(10.0/280.0)
	 

	vector<CircleParam>::iterator ita;
	int i = 0;
	int indexOfInnerCircle = 0;
	double minDiameter = 999999;
	for(ita = vecCircleParam.begin(), i=0;ita != vecCircleParam.end();i++,ita++)//v1.begin()指向v1的第一个元素，v1.end()指向最后元素的下一位置
	{
		if (minDiameter > ita->minDiameter)
		{
			indexOfInnerCircle = i;
			minDiameter = ita->minDiameter;
		}
	}

	if (vecCircleParam.empty())
	{
		return -1;
	}

	CircleParam aCircleParam = vecCircleParam[indexOfInnerCircle];

	param.var2[0] = aCircleParam.maxDiameter*calibrateRatio;
	param.var2[1] = aCircleParam.minDiameter*calibrateRatio;

	//进入检测裂缝
	int crackNum = 0;
	Rect rcRect = boundingRect(contours[indexOfInnerCircle]);			//圆的外接矩形
	rectCentre.x = rcRect.x + rcRect.width/2;
	rectCentre.y = rcRect.y + rcRect.height/2;
	int indexOfSteepPoint = -1;
	totalPointNumOnCircleBoundary = contours[indexOfInnerCircle].size();
	for (int i = 0; i < totalPointNumOnCircleBoundary-3; i++)
	{
		circleBoundaryPoint.x = contours[indexOfInnerCircle][i].x;
		circleBoundaryPoint.y = contours[indexOfInnerCircle][i].y;	
		//int distance_from_centre1 = getLineLen(contours[rivoet_index][i], rectCentre);
		//int distance_from_centre2 = getLineLen(contours[rivoet_index][i+5], rectCentre);

		int distanceOfCurrentPointToCentre = getLineLen(contours[indexOfInnerCircle][i], rectCentre);
		int distanceOfNextPointToCentre = getLineLen(contours[indexOfInnerCircle][i+3], rectCentre);          //取3个像素间隔

		if (abs(distanceOfCurrentPointToCentre - distanceOfNextPointToCentre) > 2)
		{
			if ((i - indexOfSteepPoint) < 15 && indexOfSteepPoint!=-1)
			{
				crackNum++;
				cvCircle(pROICopy, circleBoundaryPoint, 5 ,CV_RGB(128, 128, 128), 1, 8, 0);
			}
			indexOfSteepPoint = i;
		}else
		{

		}
	}

	param.var1[1] = crackNum;

	if (param.var2[0] > 7|| param.var2[1] < 6||param.var1[1] > 0)
	{
		param.var1[0] = 0;
	}else
	{
		param.var1[0] = 1;
	}

	//if (fp != NULL)
	//{
	//	fprintf(fp,"maxDiameter:%0.3f, minDiameter:%0.3f, quanlify:%d, crackNum:%d\n", param.var2[0], param.var2[1], param.var1[0], param.var1[1]);
	//	fprintf(fp,"-----------------------------------------------------------\n"); 

	//	fclose(fp);
	//	fp = NULL;
	//}

	if (pROI != NULL)
	{
		cvReleaseImage(&pROI);
	}
	if (pROICopy != NULL)
	{
		cvReleaseImage(&pROICopy);
	}

	return 1;
}

